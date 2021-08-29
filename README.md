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
- RadioLib by Jan Gromes (4.0.3)
  - https://github.com/jgromes/RadioLib
  - https://jgromes.github.io/RadioLib/
- Optional: DHT sensor library for ESPx by beegee_tokyo 1.17.0


### Arduino IDE settings
Select right board from board settings (in my case "LOLIN(WEMOS) D1 R2 & mini"). Wrong board might give weird results.

### Wiring

#### LED for free use and I<sup>2</sup>C unaccessible
RFM95 | Direction | ESP8266/Wemos/NodeMCU | ESP32 | ESP-201
--- | --- | --- | --- | ---
MISO/SDO (SPI) (brown ![#brown](https://via.placeholder.com/15/c68c53/000000?text=+)) | &leftarrow; | D6 (GPIO12) (SPI MISO) | GPIO19 (VSPI MISO) | IO12
MOSI/SDI (SPI) (orange ![#orange](https://via.placeholder.com/10/ff8000/000000?text=+)) | &rightarrow; | D7 (GPIO13) (SPI MOSI) | GPIO23 (VSPI MOSI) | IO13
SCK/SCLK (SPI) (yellow ![#yellow](https://via.placeholder.com/10/ffff00/000000?text=+)) | &leftarrow; | D5 (GPIO14) (SPI SCLK) | GPIO18 (VSPI SCLK) | IO14
NSS/SS/CS (SPI) (green ![#green](https://via.placeholder.com/10/00ff00/000000?text=+)) | &leftarrow; | D8 (GPIO15) | GPIO5 | IO15
RESET (blue ![#blue](https://via.placeholder.com/10/0000ff/000000?text=+)) | &leftarrow; | D3 (GPIO0) | GPIO22 | IO0
DIO0 (IRQ) (grey ![#grey](https://via.placeholder.com/10/999999/000000?text=+)) | &rightarrow; | D2 (GPIO4) | GPIO21 | IO4
GND (black ![#black](https://via.placeholder.com/10/000000/000000?text=+)) | | G | GND | GND
3.3V (red ![#red](https://via.placeholder.com/10/ff0000/000000?text=+)) | | 3V3 | 3V3 | 3.3V
ANA/ANT | | Antenna (86mm) |

#### Alternative wiring for ESP8266: LED used as CS and I<sup>2</sup>C useable
RFM95 | Direction | ESP8266/Wemos/NodeMCU
--- | --- | ---
MISO/SDO (SPI) (brown ![#brown](https://via.placeholder.com/10/c68c53/000000?text=+)) | &leftarrow; | D6 (GPIO12) (SPI MISO)
MOSI/SDI (SPI) (orange ![#orange](https://via.placeholder.com/10/ff8000/000000?text=+)) | &rightarrow; | D7 (GPIO13) (SPI MOSI)
SCK/SCLK (SPI) (yellow ![#yellow](https://via.placeholder.com/10/ffff00/000000?text=+)) | &leftarrow; | D5 (GPIO14) (SPI SCLK)
NSS/SS/CS (SPI) (green ![#green](https://via.placeholder.com/10/00ff00/000000?text=+)) | &leftarrow; | D4 (GPIO2)
RESET (blue ![#blue](https://via.placeholder.com/10/0000ff/000000?text=+)) | &leftarrow; | D3 (GPIO0)
DIO0 (IRQ) (grey ![#grey](https://via.placeholder.com/10/999999/000000?text=+)) | &rightarrow; | D8 (GPIO15)
GND (black ![#black](https://via.placeholder.com/10/000000/000000?text=+)) | | G
3.3V (red ![#red](https://via.placeholder.com/10/ff0000/000000?text=+)) | | 3V3
ANA/ANT | | Antenna (86mm)

**Warning:** if DIO0 interrupt signal is triggered, ESP8266 doesn't boot up. This is safe wiring only if interrupt is not used.

#### Deepsleep
ESP8266/Wemos/NodeMCU | Direction | ESP8266/Wemos/NodeMCU | notes
--- | --- | --- | ---
D0 (GPIO16) (violet ![#violet](https://via.placeholder.com/10/cc0066/000000?text=+)) | &rightarrow; | RST (ESP8266) | unconnect when flashing!

#### Reset
High-Z = normal, low (>100us) = reset. Notice that Wemos/NodeMCU has pullup resistor for D3 and D4 and pulldown resistor for D8, so those cannot be in High-Z-state.  Nevertheless it looks like D3 and D4 works for reset.
#### DIO0/IRQ
Normally low, high when active. DIO0 could be connected to D8, but then there is small risk that SX1276 interrupt is triggered when ESP8266 resets itself. Then D8 would be up and ESP8266 would not boot.

### Wemos/NodeMCU pinout

Wemos/NodeMCU | GPIO | use | notes | ESP-201
--- | --- | --- | --- | ---
D0 | GPIO16 | in/out | No interrupts, no pullup feature, no PWM. Used to wake up from deepsleep. User-button is sometimes connected to this. | XPD
D1 | GPIO5 | in/out/SCL | I<sup>2</sup>C (ESP32: GPIO22) | IO5
D2 | GPIO4 | in/out/SDA | I<sup>2</sup>C (ESP32: GPIO21) | IO4
D3 | GPIO0 | out | Flash-mode. Flash-button in some modules. Must be high at boot up (boot mode selection). | IO0
D4 | GPIO2 | out/led | Must be high at boot up (boot mode selection). Blue led in Wemos and NodeMCU. | IO2
D5 | GPIO14 | in/out/SCK | SPI | IO14
D6 | GPIO12 | in/out/MISO | SPI | IO12
D7 | GPIO13 | in/out/MOSI | SPI, if serial port flow control is used, this is CTS. | IO13
D8 | GPIO15 | out | Must be low at boot up (boot mode selection). If serial port flow control is used, this is RTS. | IO15
D9 | GPIO3 | x | Serial RX. | RX
D10 | GPIO1 | x | Serial TX. | TX

#### Using D9/GPIO/RX for your own uses (TX is D10/GPIO1)
https://github.com/esp8266/Arduino/issues/2430
```
Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY, D10);
```

## Misc

RegSyncWord (0x39) 7-0 SyncWord rw 0x12 LoRa Sync Word
(Value 0x34 is reserved for LoRaWAN networks)

