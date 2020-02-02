#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");
  LoRa.setPins(D8, D2, D1); // ss (10), reset (9), dio0 (2) (must be interrupt capable via attachInterrupt(...))

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSignalBandwidth(125E3);
  LoRa.setSpreadingFactor(10);
  LoRa.setCodingRate4(8);
  //LoRa.setTxPower(2); // PA_BOOST: 2-20, 0dBm...20dBm, RegPaDac (0x4D) bits 2-0, RegOcp (0x0B) bit 0-5 
  //LoRa.setTxPower(4); //4:lla ei toiminu kovassa sateessa
  //LoRa.setTxPower(10);
  LoRa.enableCrc(); // CRC: RegModemConfig 2 (0x1E) Bit 2 (RxPayloadCrcOn) = 1 (page 30)
  LoRa.setSyncWord(0x77);
  LoRa.dumpRegisters(Serial);
  
  delay(1000);
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
