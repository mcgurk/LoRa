#include "esphome.h"
using namespace esphome;

#define PIN D2

const char on[] = "011000111010011000001111 1101100000\0";
const char off[] = "011000111010011000001110 1101100100\0";

#define ZERO digitalWrite(PIN, HIGH); delayMicroseconds(300); digitalWrite(PIN, LOW); delayMicroseconds(1141-300);
#define ONE digitalWrite(PIN, HIGH); delayMicroseconds(815); digitalWrite(PIN, LOW); delayMicroseconds(1141-815);

void send(const char* code) {
  uint8_t buf[34];
  int p = 0; int c = 0;
  do {
    char b = code[c++];
    if (b == '0') buf[p++] = 0x00;
    if (b == '1') buf[p++] = 0xff;
    if (b == '\r' || b == '\n' || b == '\0') break;
  } while (p < 34);
  for ( int t = 0; t < 3; t++) {
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
    delay(1000);      
  }
}

class RfRemoteMotonet : public Component, public Switch {
 public:
  void setup() override {
    // This will be called by App.setup()
    pinMode(PIN, OUTPUT);
  }

  void write_state(bool state) override {
    if (state) send(on); else send(off);
    //digitalWrite(D4, state);
    publish_state(state);
  }
};
