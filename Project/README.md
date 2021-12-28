
### Parts
- WeMos D1 mini R2 (ESP8266 module)
- OBD male connector
- AMS1117-3.3 regulator "Power module" 
  - 3.3V, max input voltage 15V, dropout max 1.3V
  - http://www.advanced-monolithic.com/pdf/ds1117.pdf
  - quiescent current when led-removed: 1-2mA
- GY-INA219 module (INA219, voltage and current measure, I2C)
  - 0.1ohm current resistor
  - https://www.ti.com/lit/ds/symlink/ina219.pdf
- AM2320(B) sensor (temperature and humidity measure, I2C) 
  - Red=VDD, Yellow=SDA, Black=GND, White=SCL
- LoRa SX1276 module

### Wiring
- OBD[+12V] -> INA219[VIN+]
- INA219[VIN-] -> AMS1117[IN+]
- AMS1117[OUT+] -> INA219[VCC]
- OBD[GND] -> AMS1117[IN-GND]
- AMS1117[OUT-GND] -> INA219[GND]
- INA219[VCC/GND/SDA/SCL] -> AM2320[VCC/GND/SDA/SCL] + ESP[VCC/GND/SDA/SCL]
- ESP8266[D0/GPIO16] -> ESP8266[RST]
- optional: for deepsleep, connect D0 -> RST in ESP8266

#### Wiring to ESP8266 NodeMCU devboard
INA219/AM2320 | ESP8266 NodeMCU devboard
--- | --- 
SCL (I<sup>2</sup>C) (orange ![#orange](https://via.placeholder.com/10/ff8000/000000?text=+)) | D1
SDA (I<sup>2</sup>C) (brown ![#brown](https://via.placeholder.com/10/c68c53/000000?text=+)) | D2
GND (black ![#black](https://via.placeholder.com/10/000000/000000?text=+)) | G
VCC (3.3V) (red ![#red](https://via.placeholder.com/10/ff0000/000000?text=+)) | 3V
--- | optional: for deepsleep, connect D0 -> RST
 
### CRC check in receiver
```
#define RADIOLIB_GODMODE
#define CRCONPAYLOAD() _mod->SPIgetRegValue(SX127X_REG_HOP_CHANNEL, 6, 6)
#include <RadioLib.h>
```
After that modification, `if(lora.CRCONPAYLOAD()) ...` can be used to check if CRC was used with packet.

### IRAM error
If you get "ISR not in IRAM!", add `ICACHE_RAM_ATTR` before ISR-routine name, e.g. `void ICACHE_RAM_ATTR setFlag(void) {`.

### misc
(LoRa.txt, OBD.txt, INA219_current_voltage_sensor.txt, DHT22_AM2302.txt)

