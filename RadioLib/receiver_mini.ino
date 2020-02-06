#include <RadioLib.h>

SX1276 lora = new Module(D8, D2, D3, D1); //NSS, DIO0, RST, DIO1

void ICACHE_RAM_ATTR setFlag(void);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.print(F("[SX1276] Initializing ... "));
  // carrier frequency:           434.0 MHz
  // bandwidth:                   125.0 kHz
  // spreading factor:            9
  // coding rate:                 7
  // sync word:                   0x12
  // output power:                17 dBm
  // current limit:               100 mA
  // preamble length:             8 symbols
  // amplifier gain:              0 (automatic gain control)
  //int state = lora.begin();
  int state = lora.begin(868, 125, 10, 8, 0x77, 2);
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the function that will be called
  // when new packet is received
  lora.setDio0Action(setFlag);

  // start listening for LoRa packets
  Serial.print(F("[SX1276] Starting to listen ... "));
  state = lora.startReceive();
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // if needed, 'listen' mode can be disabled by calling
  // any of the following methods:
  //
  // lora.standby()
  // lora.sleep()
  // lora.transmit();
  // lora.receive();
  // lora.readData();
  // lora.scanChannel();
}

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void) {
  // check if the interrupt is enabled
  if(!enableInterrupt) {
    return;
  }

  // we got a packet, set the flag
  receivedFlag = true;
}

void loop() {
  // check if the flag is set
  if(receivedFlag) {
    // disable the interrupt service routine while
    // processing the data
    enableInterrupt = false;

    // reset flag
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
      // packet was successfully received
      Serial.println(F("[SX1278] Received packet!"));

      // print data of the packet
      Serial.print(F("[SX1276] Data:\t\t"));
      Serial.println(str);

      // print RSSI (Received Signal Strength Indicator)
      Serial.print(F("[SX1276] RSSI:\t\t"));
      Serial.print(lora.getRSSI());
      Serial.println(F(" dBm"));

      // print SNR (Signal-to-Noise Ratio)
      Serial.print(F("[SX1276] SNR:\t\t"));
      Serial.print(lora.getSNR());
      Serial.println(F(" dB"));

      // print frequency error
      Serial.print(F("[SX1276] Frequency error:\t"));
      Serial.print(lora.getFrequencyError());
      Serial.println(F(" Hz"));

    } else if (state == ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      Serial.println(F("[SX1276] CRC error!"));

    } else {
      // some other error occurred
      Serial.print(F("[SX1276] Failed, code "));
      Serial.println(state);

    }

    // put module back to listen mode
    lora.startReceive();

    // we're ready to receive more packets,
    // enable interrupt service routine
    enableInterrupt = true;
  }
  
  delay(500);
}
