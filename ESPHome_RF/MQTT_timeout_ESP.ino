#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
// https://pubsubclient.knolleary.net/api
// By default, it is set to 15 seconds - as defined by the MQTT_KEEPALIVE constant in PubSubClient.h.
// mosquitto_sub -u username -P password -h mqttserver -d -t "kellokytkin"

const char* ssid = "xxxxx";
const char* password = "xxxxx";
const char* mqtt_server = "xxxxx";
const char* mqtt_username = "xxxxx";
const char* mqtt_password = "xxxxx";
const char* mqtt_topic = "kellokytkin";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
  /*if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }*/

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    String clientId = "ESP8266Client-";
    clientId += String(ESP.getChipId(), HEX);
    Serial.println(clientId);
    // Attempt to connect
    Serial.print("Attempting MQTT connection...");
    client.setKeepAlive(5);
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password, mqtt_topic, 0, false, "offline")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(mqtt_topic, "online", false);
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 2 seconds before retrying
      digitalWrite(BUILTIN_LED, LOW); // LED on
      delay(100);
      digitalWrite(BUILTIN_LED, HIGH); // LED off
      delay(1000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH); // LED off
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    digitalWrite(BUILTIN_LED, HIGH); // LED off
    reconnect();
  } else digitalWrite(BUILTIN_LED, LOW); // LED on
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 30L*1000L) {
    lastMsg = now;    
    client.publish(mqtt_topic, "online", false);
    digitalWrite(BUILTIN_LED, HIGH); // LED off
    delay(100);
    digitalWrite(BUILTIN_LED, LOW); // LED on
  }
}
