#define SENSE    4           //ICP1 (Arduino Pro Micro: pin4, Arduino Uno: pin8)

volatile uint16_t buf[16][29]; //16 slots, 28 bits + flag

void setup() {
  Serial.begin(115200);
  while (!Serial);
  pinMode(SENSE, INPUT_PULLUP);

  TIMSK1 = _BV(ICIE1); // ICIE1: Timer/Counter1, Input Capture Interrupt Enable
  TCCR1A = 0; // TCCR1A WGM10, WGM11 = 0 and TCCR1B WGM12, WGM13 = 0 -> TIMER1: Normal mode
  //TCCR1B = _BV(ICNC1) | _BV(CS10); //CS10: No prescaler, ICNC1: Input Capture Noise Canceller
  //TCCR1B = _BV(ICNC1) | _BV(ICES1) | _BV(CS10) | _BV(CS12); //CS1x: prescaler 1024, ICES1: rising edge, ICNC1: Input Capture Noise Canceller
  TCCR1B = _BV(ICNC1) | _BV(ICES1) | _BV(CS11); //CS11: prescaler 8, ICES1: rising edge, ICNC1: Input Capture Noise Canceller
}


void loop() {
  if (buf[0][28]) {
    buf[0][28] = 0;
    Serial.print(" value: "); Serial.println(buf[0][0]);
    //Serial.print(" value: "); Serial.println(temp);
  }
}


ISR(TIMER1_CAPT_vect) { // ICIE1
  static uint16_t oldCounter;
  uint16_t newCounter = ICR1; //ICR1: Input Capture Register
  uint16_t delta = newCounter - oldCounter;
  oldCounter = newCounter;
  if (delta > 20000 && delta < 20050) {
    buf[0][0] = delta;
    buf[0][28] = 0xff;
  }
  //temp = ICR1;
  //temp = TCNT1;
  //uint16_t counter = ICR1; //ICR1: Input Capture Register
  //counter = ICR1; //ICR1: Input Capture Register
  //TIFR1 = 0xff; // Clear all pending TIMER1 interrupt flags
}

