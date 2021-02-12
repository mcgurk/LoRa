#define SENSE    4           //ICP1 (Arduino Pro Micro: pin4, Arduino Uno: pin8)

volatile uint16_t buf[16][29]; //16 slots, 28 bits + flag

void setup() {
  Serial.begin(115200);
  while (!Serial);
  pinMode(SENSE, INPUT_PULLUP);

  TIMSK1 = _BV(ICIE1); // ICIE1: Timer/Counter1, Input Capture Interrupt Enable
  TCCR1A = 0; // TCCR1A WGM10, WGM11 = 0 and TCCR1B WGM12, WGM13 = 0 -> TIMER1: Normal mode
  TCCR1B = _BV(ICNC1) | _BV(ICES1) | _BV(CS11); //CS11: prescaler 8, ICES1: rising edge, ICNC1: Input Capture Noise Canceller
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

void loop() {
  for (int slot = 0; slot < 16; slot++)
  if (buf[slot][28]) {
    buf[slot][28] = 0;
    uint32_t message = 0;
    for (int i = 0; i < 28; i++) {
      bitWrite(message, 27-i, ((buf[slot][i] > 8000) ? 1 : 0) ); // 1 unit is 0,5us
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


ISR(TIMER1_CAPT_vect) { // ICIE1
  static uint32_t delta;
  static uint8_t p;
  static uint8_t receiving = 0; 
  static uint16_t oldCounter;
  static uint8_t slot = 0;
  uint16_t newCounter = ICR1; //ICR1: Input Capture Register
  delta = newCounter - oldCounter; // 1 unit in delta is 0,5us (1s/16000000Hz*8prescaler)
  oldCounter = newCounter;

  if (delta < 4800) {
    receiving = 0;
    return;
  }
  if (delta > 20200) {
    receiving = 0;
    return;
  }
  if ( (delta > 20000) && (delta < 20100) ) {
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

