# Raspberry Pi 2

- https://cdn.sparkfun.com/assets/learn_tutorials/4/2/4/header_pinout.jpg

## PROBLEM

Using C library for Broadcom BCM 2835 as used in Raspberry Pi (https://www.airspayce.com/mikem/bcm2835/) to access SPI, it doesn't revert pins to previous state. After "bcm2835_spi_end()" MOSI, MISO, SCLK, CE0, CE1 are in "IN"-mode. So things said here doesn't happen: https://raspberrypi.stackexchange.com/questions/58864/pigpio-python-module-doesnt-always-set-gpio-pin-modes-for-spi.

After "bcm2835_spi_begin()" MOSI, MISO, SCLK, CE0 and CE1 are in "ALT0"-mode as should be.

At bootup, MOSI, MISO and SCLK is in state "ALT0" and CE0 and CE1 in state "OUT"

Python spidev-module relies that pins are at bootup-state. It uses CE0 and CE1 with softare and doesn't use hardware CE0 and CE1 usage. Even worse is that you cannot change MOSI, MISO and SCLK state to ALT0 with RPi.GPIO-python module.

### "Workaround"
Set pin states same as after bootup with Wiringpi (MOSI, MISO and SCLK = "ALT0", CE0 and CE1 = "OUT"/1):
```
gpio mode 12 alt0; gpio mode 13 alt0; gpio mode 14 alt0; 
gpio mode 10 out; gpio write 10 1; gpio mode 11 out; gpio write 11 1;
```
Set RST to "OUT"/1 and DIO0 to "IN":
```
gpio mode 5 out; gpio write 5 1; gpio mode 6 in;
```
## Pins after bootup
```
 $ gpio readall
+-----+-----+---------+------+---+---Pi 2---+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 |   IN | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | ALT0 | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | ALT0 | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 1 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI | ALT0 | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO | ALT0 | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
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
**Notice! Wiringpi "Name"-column "GPIO"-numbers are not same as GPIO-numbers in Python or pinout-charts. Use Wiringpi "BCM"-column as GPIO numbers.**

## Connections
| RFM95 | Raspi 2, physical pin number in gpio header |
| --- | --- |
| MOSI/SDI (SPI) (orange) | 19 (GPIO10) |
| MISO/SDO (SPI) (brown) | 21 (GPIO9) |
| SCK/SCLK (SPI) (yellow) | 23 (GPIO11) |
| NSS/SS/CS/SEL (SPI) (green) | 24 (GPIO8) |
| RESET/RST (blue) | 18 (GPIO24) |
| DIO0/IO0 (IRQ) (grey) | 22 (GPIO25) |
| GND (black) | 6 |
| 3.3V (red) | 1 |
| ANA/ANT | Antenna (86mm) |
| Optional: |
| DIO1 (purple) | 11 (GPIO17) |
| DIO2 (white) | 12 (GPIO18) |
| DIO3 (white/blue) | 13 (GPIO27) |
| LED | 3 (GPIO2) |

## Add spi-device and test / scan (without Python)
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
**Notice! spi_scan doesn't test RST ja DIO0 pins! You can only be sure that all SPI-pins worked.**

## Test with Python3 spidev
```
gpio mode 12 alt0; gpio mode 13 alt0; gpio mode 14 alt0; 
gpio mode 10 out; gpio write 10 1; gpio mode 11 out; gpio write 11 1; 
gpio mode 5 out; gpio write 5 1; gpio mode 6 in;
```
```
import spidev
spi = spidev.SpiDev()
spi.open(0,0)
spi.max_speed_hz = 5000000 # SX127x can go up to 10MHz, pick half that to be safe
ver=spi.xfer([0x42, 0])[1]
print(hex(ver))
spi.close()
```
**Notice! Spidev doesn't set right pin-modes. Set them manually with Wiringpi before starting python!** 

## Print all registers with pyLoRa/pySX127x
```
sudo apt install git python3-spidev
cd ~
git clone https://github.com/rpsreal/pySX127x
```
pySX127x/SX127x/board_config.py:
```
    DIO0 = 25   # RaspPi GPIO 25
    DIO1 = 17   # RaspPi GPIO 17
    DIO2 = 18   # RaspPi GPIO 18
    DIO3 = 27   # RaspPi GPIO 27
    RST  = 24   # RaspPi GPIO 24
    LED  = 2   # RaspPi GPIO 2 connects to the LED and a resistor (1kohm or 330ohm)
```
```
gpio mode 12 alt0; gpio mode 13 alt0; gpio mode 14 alt0; 
gpio mode 10 out; gpio write 10 1; gpio mode 11 out; gpio write 11 1; 
gpio mode 5 out; gpio write 5 1; gpio mode 6 in;
python3 pySX127x/lora_util.py # without "python3"-command lora_util.py tries to use python2
```

## Install pip3, spidev and pyLoRa and start receiver
```
sudo apt install python3-rpi.gpio python3-pip python3-spidev wiringpi screen
sudo pip3 install pyLoRa
```
Get pip3 packages install directory:
```
$python3 -c "import site; print(site.getsitepackages()[0])"
/usr/local/lib/python3.7/dist-packages
```
/usr/local/lib/python3.7/dist-packages/SX127x/board_config.py:
```
    DIO0 = 25   # RaspPi GPIO 25
    DIO1 = 17   # RaspPi GPIO 17
    DIO2 = 18   # RaspPi GPIO 18
    DIO3 = 27   # RaspPi GPIO 27
    RST  = 24   # RaspPi GPIO 24
    LED  = 2   # RaspPi GPIO 2 connects to the LED and a resistor (1kohm or 330ohm)
```
```
cd ~
wget https://raw.githubusercontent.com/mcgurk/LoRa-and-RF/master/lora_receiver.py
chmod 777 lora_receiver.py
gpio mode 12 alt0; gpio mode 13 alt0; gpio mode 14 alt0; 
gpio mode 10 out; gpio write 10 1; gpio mode 11 out; gpio write 11 1; 
gpio mode 5 out; gpio write 5 1; gpio mode 6 in;
screen ./lora_receiver.py
```

