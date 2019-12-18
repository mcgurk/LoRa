
// gcc -o rf_sender rf_sender.c -O3 -Wall -I/usr/local/include -Winline -pipe -L/usr/local/lib -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt; sudo chown root:root rf_sender; sudo chmod u+s rf_sender

#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#define delayMicroseconds usleep

#define PIN 9 // OrangePi PC2: pin5/PA11

#define CORRECTION1 100 //OrangePi PC2
#define CORRECTION2 25 //OrangePi PC2

#define SHORT_LIDL 600 //LIDL
#define LONG_LIDL 1300 //LIDL
#define BETWEEN_CODES_LIDL 80 //LIDL
#define REPEAT_TIMES_LIDL 4 //LIDL
#define CODE_LENGTH_LIDL 21 //LIDL

#define SHORT_MOTONET 300 //MOTONET
#define LONG_MOTONET 800 //MOTONET
#define BETWEEN_CODES_MOTONET 10 //MOTONET
#define REPEAT_TIMES_MOTONET 6 //MOTONET
#define CODE_LENGTH_MOTONET 34 //LIDL

//space first:
#define ZERO_LIDL delayMicroseconds(LONG_LIDL-CORRECTION1); digitalWrite(PIN, HIGH); delayMicroseconds(SHORT_LIDL-CORRECTION2); digitalWr$
#define ONE_LIDL delayMicroseconds(SHORT_LIDL-CORRECTION1); digitalWrite(PIN, HIGH); delayMicroseconds(LONG_LIDL-CORRECTION2); digitalWri$
//mark first:
#define ZERO_MOTONET digitalWrite(PIN, HIGH); delayMicroseconds(SHORT_MOTONET-CORRECTION1); digitalWrite(PIN, LOW); delayMicroseconds(LON$
#define ONE_MOTONET digitalWrite(PIN, HIGH); delayMicroseconds(LONG_MOTONET-CORRECTION1); digitalWrite(PIN, LOW); delayMicroseconds(SHORT$

void send_code(char mode, char *buf) {
  switch(mode) {
    case 'L':
      for (int r = 0; r < REPEAT_TIMES_LIDL; r++) {
        for (short int c = 0; c < CODE_LENGTH_LIDL; c++) {
          if (buf[c]) {
            ONE_LIDL
          } else {
            ZERO_LIDL
          }
        }
        delay(BETWEEN_CODES_LIDL);
      }
      break;
    case 'M':
      for (int r = 0; r < REPEAT_TIMES_MOTONET; r++) {
        for (short int c = 0; c < CODE_LENGTH_MOTONET; c++) {
          if (buf[c]) {
            ONE_MOTONET
          } else {
            ZERO_MOTONET
          }
        }
        delay(BETWEEN_CODES_MOTONET);
      }
      break;
  }
}

//https://github.com/mcgurk/LoRa-and-RF/blob/master/rf_sender.ino
void send(char mode, char *codestr) {
  char buf[50];
  int p = 0; int c = 0;
  do {
    char b = codestr[c++];
    if (b == '0') buf[p++] = 0x00;
    if (b == '1') buf[p++] = 0xff;
    if (b == '\r' || b == '\n' || b == '\0') break;
  } while (p < 50);
  printf("Send:\n %i symbols: ", p);
  for (int c = 0; c < p; c++) printf("%c", buf[c] ? '1' : '0');
  printf("\n");
  send_code(mode, buf);
}

int main(int argc, char* argv[]) {
  //char codestr[] = "0011011101101 11 1011 10\0"; //21 on
  //char codestr[] = "0011011101101 11 1111 11\0"; //21 off
  //char codestr[] = "011000111010011000001111 1101100000\0"; //on
  //char codestr[] = "011000111010011000001110 1101100100\0"; //off
  if (argc < 3) {
    fprintf(stderr, "Anna argumentit (L/M ja koodi)!\n");
    return 1;
  }
  printf("Arguments:\n");
  printf(" mode:%c, code:", argv[1][0]);
  printf(argv[2]);
  printf("\n");
  wiringPiSetup();
  pinMode(PIN, OUTPUT);
  //send('L', codestr);
  send(argv[1][0], argv[2]);
  return 0;
}
