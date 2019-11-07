#include <SPI.h>
#include <LoRa.h>
#include "DHTesp.h"

DHTesp dht;

uint16_t counter = 0;
//uint8_t counter = 0;

void setup() {
  dht.setup(D0, DHTesp::DHT22);

  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(D8, D2, D1); // ss (10), reset (9), dio0 (2) (must be interrupt capable via attachInterrupt(...))

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  LoRa.setSignalBandwidth(125E3);
  //LoRa.setSpreadingFactor(7); // 7-12 (6 is special case) (page 27)
  LoRa.setSpreadingFactor(10);
  LoRa.setCodingRate4(8);
  //LoRa.setTxPower(2); // PA_BOOST: 2-20, 0dBm...20dBm, RegPaDac (0x4D) bits 2-0, RegOcp (0x0B) bit 0-5 
  LoRa.setTxPower(4);
  //LoRa.setTxPower(0,0);
  // https://www.mouser.com/datasheet/2/761/down-767039.pdf
  // register table summary: page 90
  // lora mode registers: 108 ->
  // PacketMode: RegPacketConfig 1 (0x30) bit 7 (PacketFormat) = 1 (Variable length packet)
  LoRa.enableCrc(); // CRC: RegModemConfig 2 (0x1E) Bit 2 (RxPayloadCrcOn) = 1 (page 30)

  LoRa.setSyncWord(0x77);
  
  LoRa.dumpRegisters(Serial);
}

void loop() {
  int16_t temperature = dht.getTemperature()*10.0;
  int16_t humidity = dht.getHumidity()*10.0;

  Serial.print("Sending packet: ");
  Serial.println(counter);
  Serial.println(temperature);
  Serial.println(humidity);
    
  // send packet
  LoRa.beginPacket(); //0 = ExplicitHeaderModeOn, 1 = ImplicitHeaderModeOn
  // ExplicitHeaderModeOn: RegModemConfig 1 (0x1D) Bit 0 (ImplicitHeaderModeOn) = 0 //header includes coding rate and crc (page 29)
  //LoRa.print("McGurk - ");
  //LoRa.write((uint8_t*)&temperature, 2);
  //LoRa.write((uint8_t*)&humidity, 2);
  LoRa.write(counter);
  LoRa.write(temperature>>8);
  LoRa.write(temperature&0xff);
  LoRa.write(humidity>>8);
  LoRa.write(humidity&0xff);
  //LoRa.print("hello ");
  //LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(30000);
}
