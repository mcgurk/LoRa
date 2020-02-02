#include <SPI.h>
#include <LoRa.h>
#include <Ticker.h>

Ticker mwg;

#define TURN_LED_ON digitalWrite(LED_BUILTIN, LOW)
#define TURN_LED_OFF digitalWrite(LED_BUILTIN, HIGH)

void message_watchdog() {
  TURN_LED_OFF;
  Serial.println("{ \"message_watchdog_triggered\": 1, \"error\": 1 }");
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");
  LoRa.setPins(D8, D2, D1); // ss, reset, dio0

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSignalBandwidth(125E3);
  LoRa.setSpreadingFactor(10);
  LoRa.setCodingRate4(8);
  LoRa.enableCrc(); // CRC: RegModemConfig 2 (0x1E) Bit 2 (RxPayloadCrcOn) = 1 (page 30)
  LoRa.setSyncWord(0x77);
  LoRa.dumpRegisters(Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  TURN_LED_OFF;
  mwg.attach(62, message_watchdog); //in seconds

  delay(1000);
}

float temp, humi;
uint8_t packetcnt;

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    TURN_LED_ON;
    mwg.attach(62, message_watchdog); //in seconds
    uint8_t error = 0;
    // read packet
    while (LoRa.available()) {
      packetcnt = LoRa.read();
      uint8_t c1 = LoRa.read(); uint8_t c2 = LoRa.read();
      if(c1 == 0xff && c2 == 0xff) error = 1;
      temp = (c1 + (c2 << 8)) / 10.0;
      c1 = LoRa.read(); c2 = LoRa.read();
      if(c1 == 0xff && c2 == 0xff) error = 1;
      humi = (c1 + (c2 << 8)) / 10.0;
    }
    Serial.print("{ \"packetcnt\": ");
    Serial.print(packetcnt);
    Serial.print(", \"temp\": ");
    Serial.print(temp, 1);
    Serial.print(", \"humi\": ");
    Serial.print(humi, 1);
    Serial.print(", \"RSSI\": ");
    Serial.print(LoRa.packetRssi());
    Serial.printf(", \"chipid\": \"%08X\"", ESP.getChipId());
    Serial.printf(", \"error\": %i", error);
    Serial.println(" }");
 
  }
}
