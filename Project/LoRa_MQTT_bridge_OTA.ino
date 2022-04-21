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

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <TZ.h>

#include <PubSubClient.h> //from library manager (https://pubsubclient.knolleary.net/)
#include <ArduinoJson.h>  //from library manager (https://arduinojson.org/)
// https://arduinojson.org/v6/how-to/configure-the-serialization-of-floats/
// floating point "issue": https://github.com/bblanchon/ArduinoJson/issues/543
#include <rBase64.h> // from library manager (https://github.com/boseji/rBASE64)

#define LORA_SYNCWORD 0x77
#define TRIGGER_PIN D1
#define MYTZ TZ_Europe_Helsinki
#define DEFAULT_KEEPALIVE 30*60 // in seconds

#include <RadioLib.h> // from library manager (https://github.com/jgromes/RadioLib)
SX1276 lora = new Module(D8, D2, D3); // NSS, DIO0, RST, DIO1
void ICACHE_RAM_ATTR setFlag(void); // Mandatory for ESP (without this ESP crashes "ISR not in IRAM!")

WiFiClient espClient;
PubSubClient client(espClient);

#include "credentials.h"

char msg[1024];

unsigned long lastMsg = 0;
unsigned int value = 0;
unsigned int keepalive = DEFAULT_KEEPALIVE;
time_t last_keepalive;

void setup_lora() {
  Serial.print(F("Initializing SX1276..."));
  //int state = lora.begin(868, 125, 9, 7, 0x64); // Freq[MHz], BW[kHz], SF, CR, syncword, preambleLength
  int state = lora.begin(868, 125, 10, 8, LORA_SYNCWORD); 
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("Initialized successfully!"));
  } else {
    Serial.print(F("Initializing failed, code "));
    Serial.println(state);
    while (true);
  }
  lora.setDio0Action(setFlag);
  Serial.print(F("Begin to listen LoRa-packets. "));
  state = lora.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("Listening mode success!"));
  } else {
    Serial.print(F("Listening mode failed, code "));
    Serial.println(state);
    while (true);
  }  
}

void setup_ota() {
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    int result;
    char topic[100+3+4+1];
    sprintf(topic, "%s/0x%02X/state", MQTT_TOPIC, LORA_SYNCWORD);
    if (MQTT_USERNAME) {
      result = client.connect(MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD, topic, 0, true, "offline"); //LWT
    } else {
      result = client.connect(MQTT_CLIENTID, topic, 0, true, "offline");
    }
    if (result) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //char topic[100+3+4+1];
      sprintf(topic, "%s/0x%02X/log", MQTT_TOPIC, LORA_SYNCWORD);
      char msg[200];
      time_t now = time(nullptr);
      char *date = ctime(&now);
      date[strcspn(date, "\r\n")] = 0; //remove \r and/or \n
      sprintf(msg, "ESP8266-LoRa-MQTT-bridge connected to MQTT-broker at %s.", date);  
      //client.publish(topic, "ESP8266-LoRa-MQTT-bridge started!", true); //greetings message
      client.publish(topic, msg, true); //greetings message
      sprintf(topic, "%s/0x%02X/state", MQTT_TOPIC, LORA_SYNCWORD);
      client.publish(topic, "online", true);
      // ... and resubscribe
      sprintf(topic, "%s/0x%02X/set", MQTT_TOPIC, LORA_SYNCWORD);
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

void setup() {  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);  // LED off (active low)
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  while(!Serial);
  //delay(1000); // for Arduino IDE:n monitor
  Serial.println(); Serial.print("--- (compilation date: "); Serial.print(__DATE__); Serial.print(" "); Serial.print(__TIME__); Serial.println(") ---");

  //Serial.setDebugOutput(true);
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  //implement NTP update of timekeeping (with automatic hourly updates)
  configTime(MYTZ, "pool.ntp.org", "time.nist.gov");
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
  client.setCallback(callback);

  setup_lora();
}

volatile bool receivedFlag = false;
volatile bool enableInterrupt = true;

void setFlag(void) {
  if(!enableInterrupt) {
    return;
  }
  receivedFlag = true;
}

void loop() {
  poll_lora();
  ArduinoOTA.handle();
    
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (keepalive != 0) {
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
  }
  //Serial.print(keepalive); Serial.print(" "); Serial.print(last_keepalive); Serial.print(" "); Serial.println(millis());
  //delay(100);
  
}

double round2(double value) {
   return (int)(value * 100 + 0.5) / 100.0;
}

void poll_lora() {
  if(receivedFlag) {
    Serial.println(F("LoRa-packet detected!"));
    // don't react other interrupts while processing this interrupt
    enableInterrupt = false;
    receivedFlag = false;

    int length = lora.getPacketLength();
    Serial.print(F("Packet length:\t\t")); Serial.println(length);
    // you can read received data as an Arduino String
    //String str;
    //int state = lora.readData(str);
    // you can also read received data as byte array
    byte buf[100];
    int state = lora.readData(buf, length);
    Serial.print(F("Receive state:\t\t")); Serial.println(state);

    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("LoRa-packet successfully received!"));
      //Serial.print(F("Packet length:\t\t")); Serial.println(lora.getPacketLength());

      // print RSSI (Received Signal Strength Indicator)
      Serial.print(F("RSSI:\t\t"));
      Serial.print(lora.getRSSI());
      Serial.println(F(" dBm"));

      // print SNR (Signal-to-Noise Ratio)
      Serial.print(F("SNR:\t\t"));
      Serial.print(lora.getSNR());
      Serial.println(F(" dB"));

      // print frequency error
      Serial.print(F("Frequency error:\t"));
      Serial.print(lora.getFrequencyError());
      Serial.println(F(" Hz"));

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      Serial.println(F("CRC-tarkistussumma virhe!"));
    } else { // jokin muu virhe kuin CRC-virhe
      Serial.print(F("Paketin vastaanotto epäonnistui, koodi "));
      Serial.println(state);
    }

    // Set SX1276 back to listening mode
    lora.startReceive();
    // reactivate interrupt
    enableInterrupt = true;

    DynamicJsonDocument doc = createBaseDoc();
    JsonObject doclora = doc.createNestedObject("lora");
    rbase64.encode(buf, length);
    doclora["data"] = rbase64.result();
    byte loraID = buf[0];
    doclora["id"] = loraID;
    doclora["syncword"] = LORA_SYNCWORD;
    doclora["RSSI"] = lora.getRSSI();
    doclora["SNR"] = lora.getSNR();
    doclora["length"] = length;
    doclora["freqError"] = lora.getFrequencyError();
    doclora["error"] = state;

    switch(loraID) {
      case 1:
        { 
        int16_t v = *((int16_t*)&buf[1]);
        int16_t c = *((int16_t*)&buf[3]);
        int16_t t = *((int16_t*)&buf[5]);
        int16_t h = *((int16_t*)&buf[7]);
        double voltage = v/100.0;
        double current = c/100.0;
        double temperature = t/100.0;
        double humidity = h/100.0;
        if (v != 32767) doclora["voltage"] = voltage; else doclora["voltage"] = "err";
        if (c != 32767) doclora["current"] = current; else doclora["current"] = "err";
        if (t != 32767) doclora["temperature"] = temperature; else doclora["temperature"] = "err";
        if (h != 32767) doclora["humidity"] = humidity; else doclora["humidity"] = "err";
        }
        break;
      case 2:
        {
        float t = *((float*)&buf[1]);
        float h = *((float*)&buf[5]);
        /*doclora["temp1"] = String(buf[1], HEX);
        doclora["temp2"] = String(buf[2], HEX);
        doclora["temp3"] = String(buf[3], HEX);
        doclora["temp4"] = String(buf[4], HEX);*/
        doclora["temperature"] = round2(t);
        doclora["humidity"] = round2(h);
        }
        break;
      case 3:
        {
        word k = *((word*)&buf[1]);
        doclora["keepAlive"] = k;
        doclora["online"] = true;
        if (state == RADIOLIB_ERR_NONE) {
          keepalive = k;
          last_keepalive = millis();
          if (keepalive != 0) digitalWrite(BUILTIN_LED, LOW);
        }
        }
        break;
      default:
        break;
    }
    serializeJson(doc, msg);
    sendMQTT(loraID, state);
  }  
}

void sendMQTT(byte loraID, int state) {
  //serializeJson(doc, msg);
  Serial.print("Server: \""); Serial.print(MQTT_SERVER); Serial.print("\", ");
  char topic[100+3+4+1];
  if (state == RADIOLIB_ERR_NONE) {
    sprintf(topic, "%s/0x%02X/%i", MQTT_TOPIC, LORA_SYNCWORD, loraID);
  } else {
    sprintf(topic, "%s/0x%02X/error", MQTT_TOPIC, LORA_SYNCWORD);
  }
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
  doc["counter"] = value++;
  doc["time"] = now;
  doc["date"] = date;
  JsonObject docesp = doc.createNestedObject("ESP");
  docesp["chipId"] = ESP.getChipId();
  docesp["freeHeap"] = ESP.getFreeHeap();
  return doc;
}
