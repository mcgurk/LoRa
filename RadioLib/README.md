### CRC check in receiver
```
#define RADIOLIB_GODMODE
#define CRCONPAYLOAD() _mod->SPIgetRegValue(SX127X_REG_HOP_CHANNEL, 6, 6)
#include <RadioLib.h>
```
After that modification, `if(lora.CRCONPAYLOAD()) ...` can be used to check if CRC was used with packet.

### IRAM error
If you get "ISR not in IRAM!", add `ICACHE_RAM_ATTR` before ISR-routine name, e.g. `void ICACHE_RAM_ATTR setFlag(void) {`.

### Parts
- WeMos D1 mini R2 (ESP8266 module)
- OBD male connector
- AMS1117-3.3 "Power module" (3.3V, max input voltage 15V, dropout max 1.3V) http://www.advanced-monolithic.com/pdf/ds1117.pdf
- GY-INA219 module (INA219, voltage and current measure, I2C)
- AM2320(B) sensor (temperature and humidity measure, I2C) (Red=VDD, Yellow=SDA, Black=GND, White=SCL)
- LoRa SX1276 module

(LoRa.txt, OBD.txt, INA219_current_voltage_sensor.txt, DHT22_AM2302.txt)

