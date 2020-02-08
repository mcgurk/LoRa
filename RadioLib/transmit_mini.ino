#include <RadioLib.h>

SX1276 lora = new Module(D8, D2, D3, D1); // NSS, DIO0, RST, DIO1

void setup() {
  Serial.begin(115200); while (!Serial);
  Serial.print(F("Alustetaan SX1276..."));
  int state = lora.begin(868, 125, 9, 7, 0x64, 2); // Freq[MHz], BW[kHz], SF, CR, syncword, power[dBm]
  //lora.setCRC(false);
  if (state == ERR_NONE) {
    Serial.println(F("Alustus onnistui!"));
  } else {
    Serial.print(F("Alustus epäonnistui, koodi "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  Serial.print(F("Lähetetään paketti..."));
  int state = lora.transmit("Skrolli!");

  if (state == ERR_NONE) {
    Serial.println(F("Paketin lähetys onnistui!"));

    // print measured data rate
    Serial.print(F("Datanopeus:\t"));
    Serial.print(lora.getDataRate());
    Serial.println(F(" bps"));

  } else {
    Serial.print(F("Virhe paketin lähetyksessä, koodi "));
    Serial.println(state);
  }

  delay(5000);
}
