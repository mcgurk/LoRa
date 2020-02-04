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
GPIO.setup(22, GPIO.OUT, GPIO.HIGH) # RST
spi.open(1,0) # use spi.open(0,0) with Orange Pi PC
spi.max_speed_hz = 5000000
spi.xfer([0x42, 0])
#this doesn't allways work. something to do with pinmodes...
```
### Python 3 / pyLoRa
#### Install
```
sudo pip3 install pyLoRa
```
Get pip3 packages install directory:
```
$python3 -c "import site; print(site.getsitepackages()[0])"
/usr/local/lib/python3.7/dist-packages
```
Modify settings by editing /usr/local/lib/python3.7/dist-packages/SX127x/board_config.py:
```
# ZERO:
sudo sed -i 's/RPi.GPIO/OPi.GPIO/g' /usr/local/lib/python3.7/dist-packages/SX127x/board_config.py
sudo sed -i 's/GPIO.BCM/GPIO.BOARD/g' /usr/local/lib/python3.7/dist-packages/SX127x/board_config.py
# PC: (edit board_config.py line 59):
import orangepi.zero
GPIO.setmode(orangepi.pc.BOARD)
# pyLoRa needs spidev0.0 and spidev0.1. Let's do some very ugly hack and just make link to only spidev:
# Orange Pi Zero: Very ugly hack (after this both spi-devices points to same SPI_BUS=1 and same SPI_CS=0):
sudo ln -s /dev/spidev1.0 /dev/spidev0.0
sudo ln -s /dev/spidev1.0 /dev/spidev0.0
# Orange Pi PC: Very ugly hack (after this both spi-devices points to same SPI_BUS=0 and same SPI_CS=0):
sudo ln -s /dev/spidev0.0 /dev/spidev0.1
```
/usr/local/lib/python3.7/dist-packages/SX127x/board_config.py (physical pin numbers):
```
    DIO0 = 22
    DIO1 = 13
    DIO2 = 15
    DIO3 = 10
    RST  = 18
    LED  = 3
```

