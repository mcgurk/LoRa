/*
credentials.h:
#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "passwd"
#define MQTT_SERVER "ipaddress"
#define MQTT_USERNAME "name"
#define MQTT_PASSWORD "passwd"
#define MQTT_TOPIC "topic"
#define MQTT_CLIENTID "LoRa-MQTT-bridge"
#define OTA_HOSTNAME MQTT_CLIENTID
*/
#include "credentials.h"

//#include <ESP8266WiFi.h> //ESP8266
//#include <ESP8266mDNS.h> //ESP8266
#include <WiFi.h> // ESP32
#include <ESPmDNS.h> // ESP32
//#include <TZ.h> // ESP8266
#define TZ_Europe_Helsinki PSTR("EET-2EEST,M3.5.0/3,M10.5.0/4") // ESP32
// https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
#include <ArduinoOTA.h>
#include <PubSubClient.h> //from library manager (https://pubsubclient.knolleary.net/)
#include <ArduinoJson.h>  //from library manager (https://arduinojson.org/)
// https://arduinojson.org/v6/how-to/configure-the-serialization-of-floats/
// floating point "issue": https://github.com/bblanchon/ArduinoJson/issues/543
// https://www.h-schmidt.net/FloatConverter/IEEE754.html
#include <rBase64.h> // from library manager (https://github.com/boseji/rBASE64)

#define MYTZ TZ_Europe_Helsinki
#define DEFAULT_KEEPALIVE 30*60 // in seconds

WiFiClient espClient;
PubSubClient client(espClient);

char msg[1024];

//unsigned long lastMsg = 0;
unsigned int message_counter = 0;
unsigned int keepalive = DEFAULT_KEEPALIVE;
time_t last_keepalive;
time_t last_sendtime;

inline void setup_ota(void);
double round2(double);
DynamicJsonDocument createBaseDoc(void);
void sendMQTT(void);
void mqtt_reconnect(void);
inline void setup_wifi_time_ota_mqtt(void);
inline void loop_ota_mqtt(void);
// -----------------------------------------------

#include <MPU6500_WE.h>
#include <Wire.h>
#define MPU6500_ADDR 0x69
MPU6500_WE myMPU6500 = MPU6500_WE(MPU6500_ADDR);

void setup_mpu6500() {
  Wire.begin();
  if(!myMPU6500.init()){
    Serial.println("MPU6500 does not respond");
  }
  else{
    Serial.println("MPU6500 is connected");
  }
  
  Serial.println("Position you MPU6500 flat and don't move it - calibrating...");
  delay(1000);
  myMPU6500.autoOffsets();
  Serial.println("Done!");
  
  /*  MPU6500_ACC_RANGE_2G      2 g   (default)
   *  MPU6500_ACC_RANGE_4G      4 g
   *  MPU6500_ACC_RANGE_8G      8 g   
   *  MPU6500_ACC_RANGE_16G    16 g
   */
  myMPU6500.setAccRange(MPU6500_ACC_RANGE_2G);

  /*  Enable/disable the digital low pass filter for the accelerometer 
   *  If disabled the bandwidth is 1.13 kHz, delay is 0.75 ms, output rate is 4 kHz
   */
  myMPU6500.enableAccDLPF(false);

  //myMPU6500.enableAccAxes(MPU6500_ENABLE_XYZ);
  myMPU6500.enableGyrAxes(MPU6500_ENABLE_000);
  delay(200);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  /*// Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }*/
}

void setup() {  
//  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
//  digitalWrite(BUILTIN_LED, HIGH);  // LED off (active low)
//  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  while(!Serial);
  //delay(1000); // for Arduino IDE:n monitor
  Serial.println(); Serial.print("--- (compilation date: "); Serial.print(__DATE__); Serial.print(" "); Serial.print(__TIME__); Serial.println(") ---");
  setup_wifi_time_ota_mqtt();
  setup_mpu6500();
}

float table[32];
uint8_t idx;
uint32_t counter = 0;

void loop() {
  loop_ota_mqtt();

  xyzFloat gValue = myMPU6500.getGValues();
  float g = abs(gValue.x)+abs(gValue.y)+abs(gValue.z);
  float sum = 0;
  for(int i = 0; i < 32; i++) sum += table[i];
  float a = sum/32.0;
  float d = abs(g-a);
  //if (d > 0.05)
  if (d > 0.03) {
    Serial.println(d, 8);
    counter++;
  }
  table[(idx++)&31] = g;

  // https://github.com/arduino/Arduino/blob/master/build/shared/ArduinoSerialPlotterProtocol.md

  uint32_t current_millis = millis();
  if ((current_millis - last_sendtime) > 1000) {
    DynamicJsonDocument doc = createBaseDoc();
    JsonObject docmpu = doc.createNestedObject("mpu6500");
    docmpu["counter"] = counter;
    serializeJson(doc, msg);
    sendMQTT();
    counter = 0;
    last_sendtime = current_millis;
    Serial.println("Send");
  }
  
  /*if (keepalive != 0) {
    if ( (millis() - last_keepalive) > (((time_t)keepalive)*1000) ) {
      Serial.println("keepAlive triggered!");
      keepalive = 0;
      //client.publish("lora/0x77/3", "{ \"lora\": { \"online\": false } }", true);
      digitalWrite(BUILTIN_LED, HIGH);
      DynamicJsonDocument doc = createBaseDoc();
      JsonObject doclora = doc.createNestedObject("lora");
      byte loraID = 3;
      doclora["id"] = loraID;
      doclora["online"] = false;
      serializeJson(doc, msg);
      sendMQTT(loraID, RADIOLIB_ERR_NONE);
    }
  }*/
  //Serial.print(keepalive); Serial.print(" "); Serial.print(last_keepalive); Serial.print(" "); Serial.println(millis());
  //delay(100);
  
}

// --------------------------------------
inline void loop_ota_mqtt() {
  ArduinoOTA.handle();  
  if (!client.connected()) {
    mqtt_reconnect();
  }
  client.loop(); 
}

inline void setup_wifi_time_ota_mqtt() {
  //Serial.setDebugOutput(true);
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  //implement NTP update of timekeeping (with automatic hourly updates)
  //configTime(MYTZ, "pool.ntp.org", "time.nist.gov"); //ESP8266
  configTime(0, 0, "pool.ntp.org", "time.nist.gov"); //ESP32
  setenv("TZ", MYTZ, 1); //ESP32
  tzset(); //ESP32
  Serial.println("Waiting to get initial NTP time...");
  while (time(nullptr) < 1000000000) delay(50);
  time_t now = time(nullptr);
  char *date = ctime(&now);
  date[strcspn(date, "\r\n")] = 0; //remove \r and/or \n
  Serial.print("Got date and time: "); Serial.println(date);
  // in this point network must be up because we got network time

  setup_ota();
  
  last_keepalive = millis();

  client.setBufferSize(1024); //default = 128
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(mqtt_callback);
}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    int result;
    char topic[100+3+4+1];
    sprintf(topic, "%s/state", MQTT_TOPIC);
    if (MQTT_USERNAME) {
      result = client.connect(MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD, topic, 0, true, "offline"); //LWT
    } else {
      result = client.connect(MQTT_CLIENTID, topic, 0, true, "offline");
    }
    if (result) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      sprintf(topic, "%s/log", MQTT_TOPIC);
      char msg[200];
      time_t now = time(nullptr);
      char *date = ctime(&now);
      date[strcspn(date, "\r\n")] = 0; //remove \r and/or \n
      sprintf(msg, "%s connected to MQTT-broker at %s.", OTA_HOSTNAME, date);  
      client.publish(topic, msg, true); //greetings message
      sprintf(topic, "%s/state", MQTT_TOPIC);
      client.publish(topic, "online", true);
      // ... and resubscribe
      sprintf(topic, "%s/set", MQTT_TOPIC);
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

inline void setup_ota() {
  ArduinoOTA.setHostname(OTA_HOSTNAME);
    
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}

double round2(double value) {
   return (int)(value * 100 + 0.5) / 100.0;
}

void sendMQTT() {
  //serializeJson(doc, msg);
  Serial.print("Server: \""); Serial.print(MQTT_SERVER); Serial.print("\", ");
  char topic[100+3+4+1];
  sprintf(topic, "%s", MQTT_TOPIC);
  Serial.print("Topic: \""); Serial.print(topic); Serial.println("\", Message:");
  Serial.println(msg);
  if(!client.publish(topic, msg, true)) {
    Serial.println("Error publishing MQTT-message (message too big?)!");
  }
  // maximum message size: https://www.hivemq.com/blog/mqtt-client-library-encyclopedia-arduino-pubsubclient
}

DynamicJsonDocument createBaseDoc() {
  DynamicJsonDocument doc(1000);
  time_t now = time(nullptr);
  char *date = ctime(&now);
  date[strcspn(date, "\r\n")] = 0; //remove \r and/or \n
  doc["client"] = MQTT_CLIENTID;
  doc["counter"] = message_counter++;
  doc["time"] = now;
  doc["date"] = date;
  JsonObject docesp = doc.createNestedObject("ESP");
  //docesp["chipId"] = ESP.getChipId(); //ESP8266
  docesp["chipId"] = ESP.getEfuseMac(); //ESP32
  docesp["freeHeap"] = ESP.getFreeHeap();
  return doc;
}
