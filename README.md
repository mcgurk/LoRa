# LoRa

## Python / Raspberry Pi 2
https://pypi.org/project/pyLoRa/

https://github.com/rpsreal/pySX127x

https://www.mouser.com/datasheet/2/761/down-767039.pdf

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

| RF95 / "BOARD_DRAGINO_PIHAT" | Raspi 2 |
| --- | --- |
| MISO (SPI) (ruskea) | 21 (*GPIO9) |
| MOSI (SPI) (oranssi) | 19 (*GPIO10) |
| SCK/SCLK (SPI) (keltainen) | 23 (*GPIO11) |
| RESET (sininen) | 15 (*GPIO22) |
| NSS/SS/CS (SPI) (vihreä) | 24 (*GPIO8) |
| DIO0 (IRQ) (harmaa) | 7 (*GPIO4) |
| GND (musta) | 6 |
| 3.3V (punainen) | 1 |
| Optional: |
| DIO1 (purppura) | 11 (*GPIO17) |
| DIO2 (valkoinen) | 12 (*GPIO18) |
| DIO3 (valko/sini) | 13 (*GPIO27) |
| LED | 33 (*GPIO13) |


## Arduino IDE / ESP8266 / Wemos

### ESP8266 support for Aduino IDE

Start Arduino and open the Preferences window. Enter https://arduino.esp8266.com/stable/package_esp8266com_index.json into the Additional Board Manager URLs field (you can add multiple URLs, separating them with commas).
Open Boards Manager from Tools > Board menu and install esp8266 platform.

### Libraries
- LoRa by Sandeep Mistry (0.7.0)
-- https://github.com/sandeepmistry/arduino-LoRa
-- https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
- Optional: DHT sensor library for ESPx by beegee_tokyo 1.17.0


### Arduino IDE settings
Select right board from board settings (in my case "LOLIN(WEMOS) D1 R2 & mini"). Wrong board might give weird results.

RFM95 | Wemos D1 R2 & mini
--- | ---
MISO (SPI) | D6 (GPIO12)
MOSI (SPI) | D7 (GPIO13)
SCK/SCLK (SPI) | D5 (GPIO14)
NSS/SS/CS (SPI) | D8 (GPIO15)
RESET | D2 (GPIO4)
DIO0 (IRQ) | D1 (GPIO5)
GND | G
3.3V | 3V3


# Misc

address byte
node id
nodeaddress
page 77

sync word
page 74

spi
page 80

RegSyncWord (0x39) 7-0 SyncWord rw 0x12 LoRa Sync Word
(Value 0x34 is reserved for LoRaWAN networks)
