### CRC check in receiver
```
#define RADIOLIB_GODMODE
#define CRCONPAYLOAD() _mod->SPIgetRegValue(SX127X_REG_HOP_CHANNEL, 6, 6)
#include <RadioLib.h>
```
With that `if(lora.CRCONPAYLOAD()) ...` can be used to check if CRC was used with packet.

### IRAM error
If you get "ISR not in IRAM!", add `ICACHE_RAM_ATTR` before ISR-routine name, e.g. `void ICACHE_RAM_ATTR setFlag(void) {`.
