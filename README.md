# LoRa

## Python / Raspberry Pi 2
https://pypi.org/project/pyLoRa/

https://github.com/rpsreal/pySX127x

https://www.mouser.com/datasheet/2/761/down-767039.pdf

https://github.com/rpsreal/pySX127x/blob/master/LORA_CLIENT.py

https://cdn.sparkfun.com/assets/learn_tutorials/4/2/4/header_pinout.jpg

```
 $ gpio readall
 +-----+-----+---------+------+---+---Pi 2---+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 |   IN | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 0 |  7 || 8  | 1 | ALT0 | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | ALT0 | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |  OUT | 1 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI | ALT0 | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO | ALT0 | 0 | 21 || 22 | 1 | OUT  | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK | ALT0 | 0 | 23 || 24 | 1 | OUT  | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | OUT  | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 0 | IN   | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 2---+---+------+---------+-----+-----+
```

| RFM95 | Raspi 2, physical pin number in gpio header |
| --- | --- |
| MOSI/SDI (SPI) (oranssi) | 19 (GPIO10) |
| MISO/SDO (SPI) (ruskea) | 21 (GPIO9) |
| SCK/SCLK (SPI) (keltainen) | 23 (GPIO11) |
| NSS/SS/CS/SEL (SPI) (vihreä) | 24 (GPIO8) |
| RESET/RST (sininen) | 18 (GPIO24) |
| DIO0/IO0 (IRQ) (harmaa) | 22 (GPIO25) |
| GND (musta) | 6 |
| 3.3V (punainen) | 1 |
| ANA/ANT | Antenna (86mm) |
| Optional: |
| DIO1 (purppura) | 11 (GPIO17) |
| DIO2 (valkoinen) | 12 (GPIO18) |
| DIO3 (valko/sini) | 13 (GPIO27) |
| LED | 3 (GPIO2) |

**Notice! Wiringpi "Name"-column "GPIO"-numbers are not same as GPIO-numbers in Python or pinout-charts. Use Wiringpi "BCM" numbers as GPIO numbers.**

### test / scan
Reset and DIO0 can be unconnected when using spi_scan?
```
SPI on:
sudo raspi-config
5 Interfacing Options -> SPI -> On
/dev/spidev0.0 /dev/spidev0.1
(or: sudo raspi-config nonint do_spi 0 # enable spi)

cd ~
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.62.tar.gz
tar xvfz bcm2835-1.62.tar.gz
cd bcm2835-1.62
./configure
make
sudo make check
sudo make install

sudo apt update
sudo apt install git
git clone https://github.com/idreamsi/RadioHead
cd ~/RadioHead/examples/raspi/spi_scan
make
sudo ./spi_scan
sudo ~/RadioHead/examples/raspi/spi_scan/spi_scan
Checking register(0x42) with CS=GPIO08 => SX1276 RF95/96 (V=0x12)
```

### Install pip3, spidev and pyLoRa and start receiver
```
sudo apt install python3-rpi.gpio python3-pip python3-spidev wiringpi
sudo pip3 install pyLoRa
```
/usr/local/lib/python3.7/dist-packages/SX127x/board_config.py:
```
    DIO0 = 25   # RaspPi GPIO 4
    DIO1 = 17   # RaspPi GPIO 17
    DIO2 = 18   # RaspPi GPIO 18
    DIO3 = 27   # RaspPi GPIO 27
    RST  = 24   # RaspPi GPIO 22
    LED  = 2   # RaspPi GPIO 13 connects to the LED and a resistor (1kohm or 330ohm)
```
```
cd ~
wget https://raw.githubusercontent.com/mcgurk/LoRa-and-RF/master/lora_receiver.py
chmod 777 lora_receiver.py
./lora_receiver.py
```

## Orange Pi PC
```
$ gpio readall
 +-----+-----+----------+------+---+-Orange Pi+---+---+------+---------+-----+--+
 | BCM | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | BCM |
 +-----+-----+----------+------+---+----++----+---+------+----------+-----+-----+
 |     |     |     3.3v |      |   |  1 || 2  |   |      | 5v       |     |     |
 |  12 |   8 |    SDA.0 | ALT5 | 0 |  3 || 4  |   |      | 5V       |     |     |
 |  11 |   9 |    SCL.0 | ALT5 | 0 |  5 || 6  |   |      | 0v       |     |     |
 |   6 |   7 |   GPIO.7 | ALT3 | 0 |  7 || 8  | 0 | ALT3 | TxD3     | 15  | 13  |
 |     |     |       0v |      |   |  9 || 10 | 0 | ALT3 | RxD3     | 16  | 14  |
 |   1 |   0 |     RxD2 | ALT3 | 0 | 11 || 12 | 0 | ALT3 | GPIO.1   | 1   | 110 |
 |   0 |   2 |     TxD2 | ALT3 | 1 | 13 || 14 |   |      | 0v       |     |     |
 |   3 |   3 |     CTS2 | ALT3 | 0 | 15 || 16 | 0 | ALT3 | GPIO.4   | 4   | 68  |
 |     |     |     3.3v |      |   | 17 || 18 | 0 | ALT3 | GPIO.5   | 5   | 71  |
 |  64 |  12 |     MOSI | ALT4 | 0 | 19 || 20 |   |      | 0v       |     |     |
 |  65 |  13 |     MISO | ALT4 | 0 | 21 || 22 | 0 | ALT3 | RTS2     | 6   | 2   |
 |  66 |  14 |     SCLK | ALT4 | 0 | 23 || 24 | 0 | ALT4 | CE0      | 10  | 67  |
 |     |     |       0v |      |   | 25 || 26 | 0 | ALT3 | GPIO.11  | 11  | 21  |
 |  19 |  30 |    SDA.1 | ALT4 | 0 | 27 || 28 | 0 | ALT4 | SCL.1    | 31  | 18  |
 |   7 |  21 |  GPIO.21 | ALT3 | 0 | 29 || 30 |   |      | 0v       |     |     |
 |   8 |  22 |  GPIO.22 | ALT3 | 0 | 31 || 32 | 0 | ALT3 | RTS1     | 26  | 200 |
 |   9 |  23 |  GPIO.23 | ALT3 | 0 | 33 || 34 |   |      | 0v       |     |     |
 |  10 |  24 |  GPIO.24 | ALT3 | 0 | 35 || 36 | 0 | ALT3 | CTS1     | 27  | 201 |
 |  20 |  25 |  GPIO.25 | ALT3 | 0 | 37 || 38 | 0 | ALT3 | TxD1     | 28  | 198 |
 |     |     |       0v |      |   | 39 || 40 | 0 | ALT3 | RxD1     | 29  | 199 |
 +-----+-----+----------+------+---+----++----+---+------+----------+-----+-----+
 | BCM | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | BCM |
 +-----+-----+----------+------+---+-Orange Pi+---+------+----------+-----+-----+
```
| RFM95 | Orange Pi PC, physical pin number in gpio header |
| --- | --- |
| MISO (SPI) (brown) | 21 (GPIO65/PC1/SPI0_MISO) |
| MOSI (SPI) (orange) | 19 (GPIO64/PC0/SPI0_MOSI) |
| SCK/SCLK (SPI) (yellow) | 23 (GPIO66/PC2/SPI0_CLK) |
| RESET (blue) | 18 (GPIO71/PC7) |
| NSS/SS/CS (SPI) (green) | 24 (GPIO67/PC3/SPI0_CS) |
| DIO0 (IRQ) (grey) | 22 (GPIO2/PA2) |
| GND (black) | 20 |
| 3.3V (red) | 17 |
| ANA | Antenna (86mm) |
| Optional: |
| DIO1 (purple) | 13 (GPIO0/PA0) |
| DIO2 (white) | 15 (GPIO3/PA3) |
| DIO3 (white/blue) | 16 (GPIO68/PC4) |
| LED | 3 (GPIO12/PA12) |

### Python 3 / OPi.GPIO
#### Permissions
/etc/udev/rules.d/99-gpio.rules:
```
SUBSYSTEM=="gpio", KERNEL=="gpiochip*", ACTION=="add", PROGRAM="/bin/sh -c 'chown root:gpio /sys/class/gpio/export /sys/class/gpio/unexport ; chmod 220 /sys/class/gpio/export /sys/class/gpio/unexport'" 
SUBSYSTEM=="gpio", KERNEL=="gpio*", ACTION=="add", PROGRAM="/bin/sh -c 'chown root:gpio /sys%p/active_low /sys%p/direction /sys%p/edge /sys%p/value ; chmod 660 /sys%p/active_low /sys%p/direction /sys%p/edge /sys%p/value'"
```
```
sudo groupadd gpio
sudo adduser "$USER" gpio
sudo udevadm control --reload-rules
# reboot
```
#### Install
```
sudo pip3 install OPi.GPIO
```

#### Continue
Go to Python 3 / spidev

## Orange Pi Zero

http://linux-sunxi.org/Xunlong_Orange_Pi_Zero

https://linux-sunxi.org/images/e/e0/Orange-Pi-Zero-Schanetics-v1_11.pdf

```
$ gpio readall
+-----+-----+----------+------+--Orange Pi Zero--+------+----------+-----+-----+
| H2+ | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | H2+ |
+-----+-----+----------+------+---+----++----+---+------+----------+-----+-----+
|     |     |     3.3v |      |   |  1 || 2  |   |      | 5v       |     |     |
|  12 |   8 |    SDA.0 |  OUT | 1 |  3 || 4  |   |      | 5V       |     |     |
|  11 |   9 |    SCL.0 | ALT3 | 0 |  5 || 6  |   |      | 0v       |     |     |
|   6 |   7 |   GPIO.7 | ALT3 | 0 |  7 || 8  | 0 | ALT3 | TxD3     | 15  | 198 |
|     |     |       0v |      |   |  9 || 10 | 0 | ALT3 | RxD3     | 16  | 199 |
|   1 |   0 |     RxD2 | ALT3 | 0 | 11 || 12 | 0 | ALT3 | GPIO.1   | 1   | 7   |
|   0 |   2 |     TxD2 | ALT3 | 0 | 13 || 14 |   |      | 0v       |     |     |
|   3 |   3 |     CTS2 | ALT3 | 0 | 15 || 16 | 0 | ALT3 | GPIO.4   | 4   | 19  |
|     |     |     3.3v |      |   | 17 || 18 | 0 | ALT3 | GPIO.5   | 5   | 18  |
|  15 |  12 |     MOSI | ALT5 | 0 | 19 || 20 |   |      | 0v       |     |     |
|  16 |  13 |     MISO | ALT5 | 0 | 21 || 22 | 0 | ALT3 | RTS2     | 6   | 2   |
|  14 |  14 |     SCLK | ALT5 | 0 | 23 || 24 | 0 | ALT5 | CE0      | 10  | 13  |
|     |     |       0v |      |   | 25 || 26 | 0 | ALT3 | GPIO.11  | 11  | 10  |
+-----+-----+----------+------+---+---LEDs---+---+------+----------+-----+-----+
|  17 |  30 | STAT-LED |  OUT | 0 | 27 || 28 |   |      | PWR-LED  |     |     |
+-----+-----+----------+------+---+-----+----+---+------+----------+-----+-----+
| H2+ | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | H2+ |
+-----+-----+----------+------+--Orange Pi Zero--+---+------+---------+-----+--+
```

| RFM95 | Orange Pi Zero, physical pin number in gpio header |
| --- | --- |
| MISO (SPI) (brown) | 21 (GPIO16/PA16/SPI1_MISO) |
| MOSI (SPI) (orange) | 19 (GPIO15/PA15/SPI1_MOSI) |
| SCK/SCLK (SPI) (yellow) | 23 (GPIO14/PA14/SPI1_CLK) |
| RESET (blue) | 18 (GPIO18/PA18) |
| NSS/SS/CS (SPI) (green) | 24 (GPIO13/PA13/SPI1_CS) |
| DIO0 (IRQ) (grey) | 22 (GPIO2/PA02) |
| GND (black) | 6 |
| 3.3V (red) | 1 |
| ANA | Antenna (86mm) |
| Optional: |
| DIO1 (purple) | 13 (GPIO0/PA00) |
| DIO2 (white) | 15 (GPIO3/PA03) |
| DIO3 (white/blue) | 16 (GPIO19/PA19) |
| LED | 3 (GPIO12/PA12) |

### WiringPi to Orange Pi Zero (do we even need this?)
#### Install
```
git clone https://github.com/xpertsavenue/WiringOP-Zero.git
cd WiringOP-Zero
sudo ./build
```
- https://github.com/orangepi-xunlong/wiringOP doesn't compile with Orange Pi Zero (20.12.2019)

#### Test:
```
gpio readall
gpio write 30 1 # red led on
gpio write 30 0 # red led off
```
### Python 3 / OPi.GPIO
#### Install
```
sudo pip3 install OPi.GPIO
sudo adduser "$USER" kmem
sudo chmod g+rw /dev/kmem
# make permanent:
sudo sed -i /kmem/s/0640/0660/ /lib/udev/rules.d/50-udev-default.rules
```
#### Test
```
import OPi.GPIO as GPIO
from time import sleep
GPIO.setmode(GPIO.BOARD)
GPIO.setup(3, GPIO.OUT) # pin3 (GPIO12/PA12/wPi8)

while True:
  GPIO.output(3, 1)
  sleep(0.5)
  GPIO.output(3, 0)
  sleep(0.5)
```
### Python 3 / spidev
#### /dev/spi
/boot/armbianEnv.txt:
```
overlays=spi-spidev usbhost2 usbhost3
param_spidev_spi_bus=1 # use 0 with Orange Pi PC
```
/etc/udev/rules.d/50-spi.rules:
```
SUBSYSTEM=="spidev", GROUP="spiuser", MODE="0660"
```
```
sudo groupadd spiuser
sudo adduser "$USER" spiuser
sudo udevadm control --reload-rules
sudo modprobe -r spidev; sudo modprobe spidev
# logout out and login to update user group
```

#### Install
```
sudo apt install python3-pip python3-setuptools python3-dev python3-wheel python3-numpy
sudo pip3 install spidev
```
#### Test
```
#!/usr/bin/env python3
import spidev
spi = spidev.SpiDev()
import OPi.GPIO as GPIO
GPIO.cleanup()
GPIO.setmode(GPIO.BOARD)
GPIO.setup(24, GPIO.OUT, GPIO.LOW) # CE0
GPIO.setup(22, GPIO.IN, GPIO.HIGH) # RST
spi.open(1,0) # use spi.open(0,0) with Orange Pi PC
spi.max_speed_hz = 5000000
spi.xfer([0x42, 0])
(miksi tää testi ei toimi?)
```
### Python 3 / pyLoRa
#### Install
```
sudo pip3 install pyLoRa

# notice: python3.7 might be python3.5
sudo sed -i 's/RPi.GPIO/OPi.GPIO/g' /usr/local/lib/python3.7/dist-packages/SX127x/board_config.py
sudo sed -i 's/GPIO.BCM/GPIO.BOARD/g' /usr/local/lib/python3.7/dist-packages/SX127x/board_config.py
# Orange Pi Zero: Very ugly hack (after this both spi-devices points to same SPI_BUS=1 and same SPI_CS=0):
sudo sed -i 's/SPI_BUS=0/SPI_BUS=1/g' /usr/local/lib/python3.7/dist-packages/SX127x/board_config.py
# Orange Pi PC: Very ugly hack (after this both spi-devices points to same SPI_BUS=0 and same SPI_CS=0):
sudo sed -i 's/SPI_CS=1/SPI_CS=0/g' /usr/local/lib/python3.7/dist-packages/SX127x/board_config.py
```
/usr/local/lib/python3.7/dist-packages/SX127x/board_config.py:
(or /usr/local/lib/python3.5/dist-packages/SX127x/board_config.py)
```
    DIO0 = 18
    DIO1 = 13
    DIO2 = 15
    DIO3 = 16
    RST  = 22
    LED  = 3
```

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
RFM95 | Wemos D1 R2 & mini
--- | ---
MISO/SDO (SPI) | D6 (GPIO12)
MOSI/SDI (SPI) | D7 (GPIO13)
SCK/SCLK (SPI) | D5 (GPIO14)
NSS/SS/CS (SPI) | D8 (GPIO15)
RESET | D2 (GPIO4)
DIO0 (IRQ) | D1 (GPIO5)
GND | G
3.3V | 3V3

### Wemos pinout

NodeMCU | GPIO | notes
--- | --- | ---
D0 | GPIO16 | Ei tue keskeytyksiä. Ei ylösvetovastusominaisuutta. Ei PWM:ää. Käytetään syväunesta heräämiseen kytkemällä tämä RST:hen. Joissain moduuleissa user-nappi.
D1 | GPIO5 |
D2 | GPIO4 |
D3 | GPIO0 | Flash-moodi. Flash-nappi NodeMCU-moduulissa. Pitää olla ylhäällä bootatessa normaalisti.
D4 | GPIO2 | Pitää olla ylhäällä bootatessa. Sininen LED NodeMCU:ssa.
D5 | GPIO14 | SCK
D6 | GPIO12 | MISO
D7 | GPIO13 | MOSI, sarjaportin vuonohjausta käytettäessä CTS.
D8 | GPIO15 | SS, boottilähteen valinta. Pitää olla alhaalla bootatessa normaalisti. Sarjaportin vuonohjausta käytettäessä RTS.
D9 | GPIO3 | Sarjaportin RX.
D10 | GPIO1 | Sarjaportin TX.


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
