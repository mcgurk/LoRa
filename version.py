#!/usr/bin/env python3
import spidev
spi = spidev.SpiDev()
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(25, GPIO.OUT) # RST
GPIO.output(25, GPIO.HIGH)
spi.open(0,0)
spi.max_speed_hz = 5000000
ver=spi.xfer([0x42, 0])[1]
print(hex(ver))
GPIO.cleanup()
