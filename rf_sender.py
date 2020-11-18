#!/usr/bin/env python3

# sudo raspi-config
# 3 Interface Options -> P4 SPI # effective instantly without booting
# sudo apt install python3-spidev

import sys, time, spidev

def generate(code):
  out = []
  for c in code:
    if c == '0':
      out.append(0xC0)
    if c == '1':
      out.append(0xFC)
  return out

spi = spidev.SpiDev()
spi.open(0,0)

#spi.max_speed_hz = 21000 # one byte 1.141ms

def send(data):
  spi.xfer(data, 21000)
  #spi.xfer(data, 11000)
  #spi.writebytes(out)

code = "".join(sys.argv[1:])
print(code)
out = generate(code)
print(out)
send(out)
time.sleep(0.1)
send(out)
