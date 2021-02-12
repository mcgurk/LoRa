//#define TEST
//#define DEBUG

#define REG B //for AVR
#define PIN D2 //for ESP

#define CONCAT2(a, b) a ## b
#define CONCAT(a, b) CONCAT2(a, b)
#define PORT CONCAT(PORT, REG)
#define DDR CONCAT(DDR, REG)

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

uint8_t on[] = "011000111010011000001111 1101100000\0";
uint8_t off[] = "011000111010011000001110 1101100100\0";

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.setTimeout(100);
  
  #ifdef __AVR__
  //pinMode(11, OUTPUT);
  DDR = 0xff;
  #ifdef DEBUG
  Serial.println(STR(PORT));
  Serial.println(STR(DDR));
  #endif
  #else
  pinMode(PIN, OUTPUT);
  #ifdef DEBUG  
  Serial.println(); Serial.println();
  Serial.println(STR(PIN));
  #endif
  #endif
}

#ifdef __AVR__
#define ZERO PORT = 0xff; delayMicroseconds(300); PORT = 0x00; delayMicroseconds(1141-300);
#define ONE PORT = 0xff; delayMicroseconds(815); PORT = 0x00; delayMicroseconds(1141-815);
#else
#define ZERO digitalWrite(PIN, HIGH); delayMicroseconds(300); digitalWrite(PIN, LOW); delayMicroseconds(1141-300);
#define ONE digitalWrite(PIN, HIGH); delayMicroseconds(815); digitalWrite(PIN, LOW); delayMicroseconds(1141-815);
#endif

void send(const char* code) {
  uint8_t buf[34];
  int p = 0; int c = 0;
  do {
    char b = code[c++];
    if (b == '0') buf[p++] = 0x00;
    if (b == '1') buf[p++] = 0xff;
    if (b == '\r' || b == '\n' || b == '\0') break;
  } while (p < 34);
  #ifdef DEBUG
  for (int c = 0; c < 34; c++) Serial.print(buf[c] ? '1' : '0'); Serial.println(); // debug
  #endif
  for (int r = 0; r < 6; r++) {
    noInterrupts();
    for (int c = 0; c < 34; c++) {
      if (buf[c]) {
        ONE
      } else {
        ZERO
      }
    }
    interrupts();
    delay(10);
  }
}

void loop() {
  #ifndef TEST
  if(Serial.available()) {
     String str = Serial.readString();
     #ifdef DEBUG
     Serial.println(str);
     #endif
     send(str.c_str());
     delay(100);
  }
  #else
  Serial.println("loop");
  Serial.println("on");
  send(on);
  delay(5000);
  Serial.println("off");
  send(off);
  delay(5000);
  #endif
}
