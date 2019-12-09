#include <SPI.h>
#include <LoRa.h>
#include "DHTesp.h"

DHTesp dht;

//uint16_t counter = 0;
uint8_t counter = 0;

void setup() {
  dht.setup(D0, DHTesp::DHT22);

  pinMode(D4, OUTPUT); //GPIO2, blue led
  digitalWrite(D4, LOW);
  
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
  LoRa.setTxPower(10);
  // https://www.mouser.com/datasheet/2/761/down-767039.pdf
  // register table summary: page 90
  // lora mode registers: 108 ->
  // PacketMode: RegPacketConfig 1 (0x30) bit 7 (PacketFormat) = 1 (Variable length packet)
  LoRa.enableCrc(); // CRC: RegModemConfig 2 (0x1E) Bit 2 (RxPayloadCrcOn) = 1 (page 30)

  LoRa.setSyncWord(0x77);
  
  LoRa.dumpRegisters(Serial);
  
  delay(1000);
}

void loop() {
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  int16_t t = 32767;
  int16_t h = 32767;

  Serial.print("Sending packet: ");
  Serial.println(counter);
  Serial.println(temperature);
  Serial.println(humidity);

  if (!isnan(temperature) && !isnan(humidity)) {
    t = temperature * 10.0;
    h = humidity * 10.0;
  }
  Serial.println(t);
  Serial.println(h);
  
  // send packet
  digitalWrite(D4, HIGH);
  LoRa.beginPacket(); //0 = ExplicitHeaderModeOn, 1 = ImplicitHeaderModeOn
  // ExplicitHeaderModeOn: RegModemConfig 1 (0x1D) Bit 0 (ImplicitHeaderModeOn) = 0 //header includes coding rate and crc (page 29)
  //LoRa.print("McGurk - ");
  LoRa.write(counter);
  LoRa.write((uint8_t*)&t, 2);
  LoRa.write((uint8_t*)&h, 2);
  /*LoRa.write(temperature>>8);
  LoRa.write(temperature&0xff);
  LoRa.write(humidity>>8);
  LoRa.write(humidity&0xff);*/
  //LoRa.print("hello ");
  //LoRa.print(counter);
  LoRa.endPacket();
  digitalWrite(D4, LOW);

  counter++;

  delay(30000);
}
