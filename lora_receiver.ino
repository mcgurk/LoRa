/*
LoRa.h:
  uint8_t CrcOnPayload();
LoRa.cpp:
uint8_t LoRaClass::CrcOnPayload()
{
  return (readRegister(0x1C) >> 6) & 1; //RegHopChannel, bit 6 = CrcOnPayload
}
*/

#include <SPI.h>
#include <LoRa.h>
#include <Ticker.h>
#include <ArduinoJson.h>

Ticker mwg;

StaticJsonDocument<1024> doc;

#define TURN_LED_ON digitalWrite(LED_BUILTIN, LOW)
#define TURN_LED_OFF digitalWrite(LED_BUILTIN, HIGH)

void message_watchdog() {
  TURN_LED_OFF;
  doc["info"] = "Message watchdog triggered";
  serializeJson(doc, Serial); Serial.println(); doc.clear();
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

  doc["info"] = "LoRa started";
  serializeJson(doc, Serial); Serial.println(); doc.clear();
  delay(1000);
}

float temp, humi;
uint8_t packetcnt;

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //Serial.println(packetSize);
    // received a packet
    TURN_LED_ON;
    mwg.attach(62, message_watchdog); //in seconds
    uint8_t error = 0;
    uint8_t buf[100];
    uint16_t bytes=0;
    // read packet
    while (LoRa.available()) {
      buf[bytes++] = LoRa.read();
      if(bytes > 99) break;
    }
    doc["size"] = bytes;
    doc["RSSI"] = LoRa.packetRssi();
    doc["SNR"] = LoRa.packetSnr();
    doc["CRC"] = LoRa.CrcOnPayload();
    char chipidstr[10];
    sprintf(chipidstr, "%08X", ESP.getChipId());
    doc["chipid"] = chipidstr;
    char hexbuf[512];
    char *ptr = hexbuf;
    for (uint16_t i = 0; i < bytes; i++) {
      if (i > 0) ptr += sprintf(ptr, " ");
      ptr += sprintf(ptr, "0x%02X", buf[i]);
    }
    doc["message"] = hexbuf;

    if (bytes == 5) {
      packetcnt = buf[0];
      if (buf[1] == 0xff && buf[2] == 0xff) error = 1;
      temp = *((int16_t*)&buf[1]) / 10.0;
      if (buf[3] == 0xff && buf[4] == 0xff) error = 1;
      humi = *((int16_t*)&buf[3]) / 10.0;
      doc["packetcnt"] = packetcnt;
      doc["temp"] = temp;
      doc["humi"] = humi;
      if (!LoRa.CrcOnPayload()) doc["error"] = "CRC MISSING!!!";
      if (error) doc["error"] = "Sensor value error";
    } else {
      doc["error"] = "Packet size error";
    }
    serializeJson(doc, Serial); Serial.println(); doc.clear();
 
  }
}
