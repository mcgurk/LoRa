#include <RadioLib.h>

SX1276 lora = new Module(D8, D2, D3, D1); //NSS, DIO0, RST, DIO1

void setup() {
  Serial.begin(115200); while (!Serial);
  Serial.print(F("[SX1276] Initializing ... "));
  int state = lora.begin(868, 125, 10, 8, 0x77, 2); // tajuus, kaista, spreading factor, coding rate, sync word, power
  lora.setCRC(false);
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  Serial.print(F("[SX1276] Paketin lähetys ... "));
  int state = lora.transmit("Skrolli!");

  if (state == ERR_NONE) {
    Serial.println(F("Onnistui!"));

    // print measured data rate
    Serial.print(F("[SX1276] Datarate:\t"));
    Serial.print(lora.getDataRate());
    Serial.println(F(" bps"));

  } else {
    Serial.print(F("Virhe, "));
    Serial.println(state);
  }

  delay(5000);
}
