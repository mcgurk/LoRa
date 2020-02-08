```
#define RADIOLIB_GODMODE
#define CRCONPAYLOAD() _mod->SPIgetRegValue(SX127X_REG_HOP_CHANNEL, 6, 6)
#include <RadioLib.h>
```
Now `if(lora.CRCONPAYLOAD()) ...` can be used to check if CRC was used with packet.
