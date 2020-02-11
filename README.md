# LoRa
- https://www.mouser.com/datasheet/2/761/down-767039.pdf

## LoRa Python links
- https://pypi.org/project/pyLoRa/
- https://github.com/rpsreal/pySX127x
- https://github.com/rpsreal/pySX127x/blob/master/LORA_CLIENT.py

## Raspberry Pi
[Raspberry Pi](RASPBERRYPI.md)
## Orange Pi
[Orange Pi](ORANGEPI.md)



## Arduino IDE / ESP8266 / Wemos

https://wiki.wemos.cc/products:retired:d1_mini_v2.2.0

### ESP8266 support for Aduino IDE

Start Arduino and open the Preferences window. Enter https://arduino.esp8266.com/stable/package_esp8266com_index.json into the Additional Board Manager URLs field (you can add multiple URLs, separating them with commas).
Open Boards Manager from Tools > Board menu and install esp8266 platform.

### Libraries
- LoRa by Sandeep Mistry (0.7.0)
  - https://github.com/sandeepmistry/arduino-LoRa
  - https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
- Optional: DHT sensor library for ESPx by beegee_tokyo 1.17.0


### Arduino IDE settings
Select right board from board settings (in my case "LOLIN(WEMOS) D1 R2 & mini"). Wrong board might give weird results.

### Wiring
RFM95 | Direction | ESP8266/Wemos/NodeMCU | ESP32
--- | --- | --- | ---
MISO/SDO (SPI) (brown![#c68c53](https://placehold.it/15/f03c15/000000?text=+)) | &leftarrow; | D6 (GPIO12) | GPIO19
MOSI/SDI (SPI) (orange![#ff8000](https://placehold.it/15/f03c15/000000?text=+)) | &rightarrow; | D7 (GPIO13) | GPIO23
SCK/SCLK (SPI) (yellow) | &leftarrow; | D5 (GPIO14) | GPIO18
NSS/SS/CS (SPI) (green) | &leftarrow; | D8 (GPIO15) | GPIO5
RESET (blue) | &leftarrow; | D3 (GPIO0) | GPIO22
DIO0 (IRQ) (grey) | &rightarrow; | D2 (GPIO4) | GPIO21
GND (black) | | G | GND
3.3V (red){: style="color: red"} | | 3V3 | 3V3

### Wemos/NodeMCU pinout

Wemos/NodeMCU | GPIO | notes
--- | --- | ---
D0 | GPIO16 | No interrupts, no pullup feature, no PWM. Used to wake up from deepsleep. User-button is sometimes connected to this.
D1 | GPIO5 | SCL
D2 | GPIO4 | SDA
D3 | GPIO0 | Flash-mode. Flash-button in some modules. Must be high at boot up (boot mode selection).
D4 | GPIO2 | Must be high at boot up (boot mode selection). Blue led in Wemos and NodeMCU.
D5 | GPIO14 | SCK (SPI)
D6 | GPIO12 | MISO (SPI)
D7 | GPIO13 | MOSI (SPI), if serial port flow control is used, this is CTS.
D8 | GPIO15 | Must be low at boot up (boot mode selection). If serial port flow control is used, this is RTS.
D9 | GPIO3 | Serial RX.
D10 | GPIO1 | Serial TX.


# Misc

RegSyncWord (0x39) 7-0 SyncWord rw 0x12 LoRa Sync Word
(Value 0x34 is reserved for LoRaWAN networks)

# RF 433MHz

### Motonet
Motonet Kauko-ohjattava ulkopistorasia IP44 3600W (38-4663) / EMAX 6867 ja 68671

- 230V AC, 50Hz
- 16A, 3600W
- Frequency: 433,92MHz
- Protection: IP44

button | code (0 = short, 1 = long)
--- | ---
A on |  01100011101001100000 1111 1101 1000 00
A off | 01100011101001100000 1110 1101 1001 00
B on |  01100011101001100000 1101 1101 1011 00
B off | 01100011101001100000 1100 1101 1010 00
C on |  01100011101001100000 1011 1101 1111 00
C off | 01100011101001100000 1010 1101 1110 00
D on |  01100011101001100000 0111 1101 0000 00
D off | 01100011101001100000 0110 1101 0001 00
All on | 01100011101001100000 0100 1101 0010 00
All off | 01100011101001100000 1000 1101 1100 00

- Code is 34 bits
- 34 bits take 38.8ms
- One bit takes 1.141ms
- 0 (short): mark 300µs + space 841µs
- 1 (long): mark 815µs + space 326µs
- Remote sends code 6 times. About 10ms between codes.

### LIDL
TR-502MSV
- Code is 21 bits
- 0 (short): space 1300µs + mark 600µs
- 1 (long): space 600µs + mark 1300µs
- Remote sends code 4 times. About 80ms between codes.
- 21 bits (13+2+4+2)
- 13 bits identification: randomly selected when reset button is pressed from remot
- 2 bit address: 11 = 1, 01 = 2, 10 = 3, 00 = 4 (dim sends last pressed BtnID and if "all" is pressed BtnID is 00)
- 4 bit opcode: 1011 = on, 1111 = off, 1101 = dim+, 1001 = dim-, 0011 = all on, 0111 = all off
- 2 bit checksum: last 3 bitpairs xorred with each other
Examples:

Button | RemoteID | BtnID | OpCode | Chksum
--- | --- | --- | --- | ---
1 on |  0011011101101 | 11 | 1011 | 10
1 off | 0011011101101 | 11 | 1111 | 11
2 on |  0011011101101 | 01 | 1011 | 00
2 off | 0011011101101 | 01 | 1111 | 01

## Python

sudo apt install python3-serial

```
#!/usr/bin/env python3

import serial, time, sys

#print('Number of arguments:', len(sys.argv), 'arguments.')
#print('Argument List:', str(sys.argv))

ser = serial.Serial('/dev/ttyUSB0', 115200)
#print(ser)
if sys.argv[1] == "on":
  print("on")
  ser.write(b'011000111010011000001111 1101100000')
if sys.argv[1] == "off":
  print("off")
  ser.write(b'011000111010011000001110 1101100100')

#ser.write(b'011000111010011000001111 1101100000')
#time.sleep(5)
#ser.write(b'011000111010011000001110 1101100100')
ser.close()

```
