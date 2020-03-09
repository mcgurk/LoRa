// RadioLib by Jan Gromes
// Adafruit INA219

#include <SPI.h>
#include <RadioLib.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

//#define DEBUG
#define SLEEP 30

//SX1276 lora = new Module(D8, D2, D3, D1); // NSS, DIO0, RST, DIO1
SX1276 lora = new Module(D4, D8, D3); // NSS, DIO0, RST, DIO1

Adafruit_INA219 ina219;
  
uint8_t counter = 0;

void setup() {
  #ifdef DEBUG
  Serial.begin(115200); while (!Serial);
  Serial.println("\n\n\n\n");
  Serial.print(F("Initializing SX1276..."));
  #endif
  int state = lora.begin(868, 125, 10, 8, 0x77, 14); // Freq[MHz], BW[kHz], SF, CR, syncword, power[dBm]
  #ifdef DEBUG
  if (state == ERR_NONE) {
    Serial.println(F("Initializing success!"));
  } else {
    Serial.print(F("Initializing failed, code "));
    Serial.println(state);
    while (true);
  }
  #endif

  ina219.begin();
  
}

void loop() {
  float voltage = ina219.getBusVoltage_V();
  float current = ina219.getCurrent_mA();
  int16_t v = 32767;
  int16_t c = 32767;

  #ifdef DEBUG
  Serial.print("Counter: "); Serial.println(counter);
  Serial.print("Voltage[V]: "); Serial.println(voltage);
  Serial.print("Current[mA]: "); Serial.println(current);
  #endif

  if (!isnan(voltage) && !isnan(current)) {
    v = voltage * 10.0;
    c = current * 10.0;
  }
  #ifdef DEBUG
  Serial.println("Scaled values:");
  Serial.println(v);
  Serial.println(c);
  #endif
  
  uint8_t packet[5];
  packet[0] = counter;
  *((int16_t*)&packet[1]) = v;
  *((int16_t*)&packet[3]) = c;
  
  #ifdef DEBUG
  Serial.print(F("Sending packet..."));
  for(int i=0; i<5; i++) Serial.print(packet[i], HEX), Serial.println();
  #endif
  int state = lora.transmit(packet, 5);

  #ifdef DEBUG
  if (state == ERR_NONE) {
    Serial.println(F("Packet send succeeded!"));
    Serial.print(F("Speed:\t"));
    Serial.print(lora.getDataRate());
    Serial.println(F(" bps"));
  } else {
    Serial.print(F("Packet send failed, code "));
    Serial.println(state);
  }
  #endif
  
  counter++;

  //delay(30000);
  //delay(5000);
  ESP.deepSleep(SLEEP*1000*1000);
}
