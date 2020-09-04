#include <RadioLib.h>

//SX1276 lora = new Module(D8, D2, D3, D1); // NSS, DIO0, RST, DIO1
SX1276 lora = new Module(D8, D2, D3); // NSS, DIO0, RST, DIO1

void ICACHE_RAM_ATTR setFlag(void); // Arduinollä jätä tämä rivi pois

void setup() {
  Serial.begin(115200);
  Serial.print(F("Alustetaan SX1276..."));
  int state = lora.begin(868, 125, 9, 7, 0x64); // Freq[MHz], BW[kHz], SF, CR, syncword
  if (state == ERR_NONE) {
    Serial.println(F("Alustus onnistui!"));
  } else {
    Serial.print(F("Alustus epäonnistui, koodi "));
    Serial.println(state);
    while (true);
  }
  lora.setDio0Action(setFlag);
  Serial.print(F("Aloita LoRa-pakettien kuuntelu."));
  state = lora.startReceive();
  if (state == ERR_NONE) {
    Serial.println(F("Kuuntelutilan asetus onnistui!"));
  } else {
    Serial.print(F("Kuuntelutilan asetus epäonnistui, koodi "));
    Serial.println(state);
    while (true);
  }
}

volatile bool receivedFlag = false;
volatile bool enableInterrupt = true;

void setFlag(void) {
  if(!enableInterrupt) {
    return;
  }
  receivedFlag = true;
}

void loop() {
  // check if the flag is set
  if(receivedFlag) {
    // jätetään reagoimatta mahdollisiin jatkokeskeytyksiin
    enableInterrupt = false;
    // nollataan lippu
    receivedFlag = false;

    // you can read received data as an Arduino String
    String str;
    int state = lora.readData(str);

    // you can also read received data as byte array
    /*
      byte byteArr[8];
      int state = lora.receive(byteArr, 8);
    */

    if (state == ERR_NONE) {
      Serial.println(F("Vastaanotettiin paketti!"));
      Serial.print(F("Data:\t\t"));
      Serial.println(str);

      // print RSSI (Received Signal Strength Indicator)
      Serial.print(F("RSSI:\t\t"));
      Serial.print(lora.getRSSI());
      Serial.println(F(" dBm"));

      // print SNR (Signal-to-Noise Ratio)
      Serial.print(F("SNR:\t\t"));
      Serial.print(lora.getSNR());
      Serial.println(F(" dB"));

      // print frequency error
      Serial.print(F("Frequency error:\t"));
      Serial.print(lora.getFrequencyError());
      Serial.println(F(" Hz"));

    } else if (state == ERR_CRC_MISMATCH) {
      Serial.println(F("CRC-tarkistussumma virhe!"));
    } else { // jokin muu virhe kuin CRC-virhe
      Serial.print(F("Paketin vastaanotto epäonnistui, koodi "));
      Serial.println(state);
    }

    // asetetaan SX1276 takaisin kuuntelutilaan
    lora.startReceive();

    // aktivoidaan keskeytysten käsittely uudelleen
    enableInterrupt = true;
  }

}
