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
      #out.append(0x00)
    if c == '1':
      out.append(0xFC)
      #out.append(0xff)
  return out

spi = spidev.SpiDev()
spi.open(0,0)

print(spi.max_speed_hz)

def send(data):
  # Goal: 1/0,001141*8 -> 7011,394Hz
  # Raspi4 600MHz/250MHz (powersave) multiplayer for real freq ~3,38???
  #spi.max_speed_hz = 25000 # ~7392Hz? works
  #spi.max_speed_hz = 32000 # ~9488Hz? works
  #spi.max_speed_hz = 16000 # ~4720Hz? doesn't work
  #spi.max_speed_hz = 20000 # ~5936Hz? doesn't work
  #spi.max_speed_hz = 40000 # ~11792Hz? doesn't work
  #spi.max_speed_hz = 23000 # ~6800Hz? works
  #spi.max_speed_hz = 24000 # ~7152Hz? works
  #spi.max_speed_hz = 23500 # ~6960Hz? works
  spi.max_speed_hz = 23700 # ~7008Hz? works
  print(spi.max_speed_hz)
  for i in range(6):
    time.sleep(0.01)
    spi.writebytes(data)
    #spi.xfer(data, 25000)

#code = "".join(sys.argv[1:])
#print(code)
#out = generate(code)
#print(out)
#send(out)
#time.sleep(0.1)
#send(out)

on_code = "01100011101001100000 1111 1101 1000 00"
off_code = "01100011101001100000 1110 1101 1001 00"
on = generate(on_code)
off = generate(off_code)

while True:
  send(on)
  time.sleep(1)
  send(off)
  time.sleep(1)

