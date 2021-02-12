#define GPIO 5 // D1

volatile uint8_t flag = 0;
volatile uint32_t buf[16][29]; //16 slots, 28 bits + flag

ICACHE_RAM_ATTR void ISR() {
  static uint32_t delta;
  static uint8_t p;
  static uint8_t receiving = 0; 
  static uint32_t last_micros = 0;
  static uint8_t slot = 0;
  uint32_t new_micros = micros();
  delta = new_micros - last_micros;
  last_micros = new_micros;

  if (delta < 2400) {
    receiving = 0;
    return;
  }
  if (delta > 10100) {
    receiving = 0;
    return;
  }
  if ( (delta > 10000) && (delta < 10040) ) {
    p = 0;
    receiving = 1;
    return;
  }
  if (receiving) {
    buf[slot][p++] = delta;
    if (p == 28) {
      buf[slot][28] = 0xff;
      slot++;
      slot &= 0xf;
      receiving = 0;
    }
  }

}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  attachInterrupt(digitalPinToInterrupt(GPIO), ISR, RISING);
}

uint8_t isValid(uint32_t msg) {
  uint32_t sum = 0;
  for (int i = 0; i < 6; i++) {
    sum += msg & 0xf;
    msg >>= 4;
  }
  sum--;
  return ((msg&0xf) == (sum&0xf));
}

uint32_t message = 0;

void loop() {
  for (int slot = 0; slot < 16; slot++)
  if (buf[slot][28]) {
    buf[slot][28] = 0;
    for (int i = 0; i < 28; i++) {
      bitWrite(message, 27-i, ((buf[slot][i] > 4000) ? 1 : 0) );
    }
    if (!isValid(message)) {
      Serial.println("Checksum error!");
      return;
    }
    int16_t t = ((uint16_t)message) >> 4;
    float temperature = ((float)t)/10;
    Serial.print("Slot:"); Serial.print(slot); Serial.print(" millis(): "); Serial.print(millis()); Serial.print(" value: "); Serial.println(temperature);
  }
}
