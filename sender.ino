#include <SPI.h>
#include <LoRa.h>

//uint16_t counter = 0;
uint8_t counter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(D8, D2, D1);

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  LoRa.setSignalBandwidth(125E3);
  LoRa.setSpreadingFactor(7);
  LoRa.setCodingRate4(8);
  //LoRa.setTxPower(14);
  LoRa.setTxPower(2);
  //LoRa.setTxPower(0,0);
  // https://www.mouser.com/datasheet/2/761/down-767039.pdf
  // register table summary: page 90
  // registers: page 93 ->
  // lora mode registers: 108 ->
  // PacketMode: RegPacketConfig 1 (0x30) bit 7 (PacketFormat) = 1 (Variable length packet)
  LoRa.enableCrc(); // CRC: RegModemConfig 2 (0x1E) Bit 2 (RxPayloadCrcOn) = 1 (page 30)
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
  
  LoRa.dumpRegisters(Serial);

  // send packet
  LoRa.beginPacket(); //0 = ExplicitHeaderModeOn, 1 = ImplicitHeaderModeOn
  // ExplicitHeaderModeOn: RegModemConfig 1 (0x1D) Bit 0 (ImplicitHeaderModeOn) = 0 //header includes coding rate and crc
  //LoRa.print("McGurk - ");
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(10000);
}
