// mark: 442us...447us...440us
// symbol "0" (mark+space): 2442us...2442us
// symbol "1" (mark+space): 5010us...5005us
// M = startsymbol (mark+space): 10ms

// M------------TTTTTTTTTTTT---- // temperature *10 in celcius
// M------------------------CCbm // channel? (battery, mode)
// MPPPP------------------------ // 4bit parity/checksum?
// M----SSSSSSSS---------------- // 8bit serial?

#define PIN 10

char message[29];
const char test1[] = "1000111110000000111011010111\0"; //23.7C

/*#define MARK 500
#define ZERO digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(2500-MARK);
#define ONE digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(5000-MARK);
#define START digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(10000-MARK);*/

#define MARK 440
#define ZERO digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(2442-MARK);
#define ONE digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(5000-MARK);
#define START digitalWrite(PIN, HIGH); delayMicroseconds(MARK); digitalWrite(PIN, LOW); delayMicroseconds(10000-MARK);

void generate_message(char* buf, float temperature) {
  //checksum
  int8_t serial = 248;
  int16_t t = temperature*10;
  byte channel = 1; // 3 channels: 1(01),2(10),3(11)
  byte battery = 1; // 1=battery ok?
  byte mode = 1; // 1=celcius?
  for (int i = 0; i < 28; i++) buf[i] = '0'; //clear
  for (int i = 0; i < 8; i++) buf[4+i] = bitRead(serial, 7-i) ? '1' : '0'; // serial 8bit
  for (int i = 0; i < 12; i++) buf[12+i] = bitRead(t, 11-i) ? '1' : '0'; // temperature*10 12bit
  for (int i = 0; i < 2; i++) buf[24+i] = bitRead(channel, 1-i) ? '1' : '0'; //channel 2bit
  buf[26] = bitRead(battery, 0) ? '1' : '0'; // battery status 1bit
  buf[27] = bitRead(mode, 0) ? '1' : '0'; // c/f-mode 1bit
  buf[28] = '\0';

  int ptr = 4;
  unsigned int sum = 0;
  for (int i = 0; i < 6; i++) {
    int bit3 = (buf[ptr++] == '1') ? 1 : 0;
    int bit2 = (buf[ptr++] == '1') ? 1 : 0;
    int bit1 = (buf[ptr++] == '1') ? 1 : 0;
    int bit0 = (buf[ptr++] == '1') ? 1 : 0;
    int nibble = (bit3 << 3) | (bit2 << 2) | (bit1 << 1) | bit0;
    sum += nibble;
  }
  int checksum = (sum - 1) & 0b1111;
  for (int i = 0; i < 4; i++) buf[0+i] = bitRead(checksum, 3-i) ? '1' : '0'; //cheksum 4bit

}

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
  Serial.begin(115200);
  while (!Serial);
  pinMode(PIN, OUTPUT);
}

void loop() {
  Serial.println('-');
  Serial.println(test1);
  //generate_message(message, 11.1);
  float temp = random(-500, 500);
  generate_message(message, temp/10);
  //generate_message(message, 23.7);
  Serial.println(message);
  //send(test1);
  send(message);
  delay(65000);
}
