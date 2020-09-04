// RadioLib by Jan Gromes
// U8g2 by oliver

#include <RadioLib.h>

#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//SX1276 lora = new Module(D8, D2, D3, D1); // NSS, DIO0, RST, DIO1
//SX1276 lora = new Module(D8, D2, D3); // NSS, DIO0, RST
SX1276 lora = new Module(D4, D8, D3); // NSS, DIO0, RST

void ICACHE_RAM_ATTR setFlag(void); // Arduinollä jätä tämä rivi pois

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,10,"Start");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  
  Serial.print(F("Alustetaan SX1276... "));
  int state = lora.begin(868, 125, 10, 8, 0x77); // Freq[MHz], BW[kHz], SF, CR, syncword
  if (state == ERR_NONE) {
    Serial.println(F("Alustus onnistui!"));
  } else {
    Serial.print(F("Alustus epäonnistui, koodi "));
    Serial.println(state);
    while (true);
  }
  lora.setDio0Action(setFlag);
  Serial.print(F("Aloita LoRa-pakettien kuuntelu... "));
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
uint32_t counter = 0;

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
    char buf[6];
    str.toCharArray(buf, 6);

    // you can also read received data as byte array
    //byte buf[5];
    //int state = lora.receive(buf, 5);
    
    if (state == ERR_NONE) {
      Serial.println(F("Vastaanotettiin paketti!"));
      Serial.print(F("Data:\t\t"));
      Serial.println(str);
      int16_t temp1 = *((int16_t*)&buf[1]);
      int16_t temp2 = *((int16_t*)&buf[3]);
      float voltage = temp1/1000.0;
      float current = temp2/100.0;
      Serial.println(voltage);
      Serial.println(current);

      // print RSSI (Received Signal Strength Indicator)
      Serial.print(F("RSSI:\t\t"));
      Serial.print(lora.getRSSI());
      Serial.println(F(" dBm"));

      u8g2.clearBuffer();          // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      //u8g2.drawStr(0,10,"Hello World!");  // write something to the internal memory
      u8g2.setCursor(10,10);
      u8g2.print(voltage); u8g2.print("V");
      u8g2.setCursor(10,20);
      u8g2.print(current); u8g2.print("mA");
      u8g2.setCursor(10,60);
      u8g2.print(counter++);

      u8g2.setCursor(60,50);
      u8g2.print(lora.getRSSI()); u8g2.print("dBm");
      u8g2.setCursor(60,60);
      u8g2.print(lora.getSNR()); u8g2.print("dB");

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
      u8g2.setCursor(10,30);
      u8g2.print("CRC-virhe!");
    } else { // jokin muu virhe kuin CRC-virhe
      Serial.print(F("Paketin vastaanotto epäonnistui, koodi "));
      Serial.println(state);
      u8g2.setCursor(10,30);
      u8g2.print("Joku virhe!");
    }

    u8g2.sendBuffer();          // transfer internal memory to the display 
      
    // asetetaan SX1276 takaisin kuuntelutilaan
    lora.startReceive();

    // aktivoidaan keskeytysten käsittely uudelleen
    enableInterrupt = true;
  }

}
