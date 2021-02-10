#define PIN 10

const char test1[] = "1000111110000000111011010111\0"; //23.7C

#define MARK 500
#define ZERO digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(2500-MARK);
#define ONE digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(5000-MARK);
#define START digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(10000-MARK);

/*#define MARK 440
#define ZERO digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(2442-MARK);
#define ONE digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(5000-MARK);
#define START digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(10000-MARK);*/

void send(const char* code) {
  uint8_t buf[28];
  int p = 0; int c = 0;
  do {
    char b = code[c++];
    if (b == '0') buf[p++] = 0x00;
    if (b == '1') buf[p++] = 0xff;
    if (b == '\r' || b == '\n' || b == '\0') break;
  } while (p < 28);
  for (int r = 0; r < 6; r++) {
    noInterrupts();
    START
    for (int c = 0; c < 28; c++) {
      if (buf[c]) {
        ONE
      } else {
        ZERO
      }
    }
    interrupts();
    yield();
    //delay(10);
  }
}
void setup() {
  pinMode(PIN, OUTPUT);

}

void loop() {
  send(test1);
  delay(2000);

}
