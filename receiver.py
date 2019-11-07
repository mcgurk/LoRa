#!/usr/bin/env python3

""" This program sends a response whenever it receives the "INF" """

# Copyright 2018 Rui Silva.
#
# This file is part of rpsreal/pySX127x, fork of mayeranalytics/pySX127x.
#
# pySX127x is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
# License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# pySX127x is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.
#
# You can be released from the requirements of the license by obtaining a commercial license. Such a license is
# mandatory as soon as you develop commercial activities involving pySX127x without disclosing the source code of your
# own applications, or shipping pySX127x with a closed source product.
#
# You should have received a copy of the GNU General Public License along with pySX127.  If not, see
# <http://www.gnu.org/licenses/>.

import time
import datetime
import struct
from SX127x.LoRa import *
#from SX127x.LoRaArgumentParser import LoRaArgumentParser
from SX127x.board_config import BOARD

BOARD.setup()
BOARD.reset()
#parser = LoRaArgumentParser("Lora tester")


class mylora(LoRa):
    def __init__(self, verbose=False):
        super(mylora, self).__init__(verbose)
        self.set_mode(MODE.SLEEP)
        self.set_dio_mapping([0] * 6)

    def on_rx_done(self):
        BOARD.led_on()
        print(datetime.datetime.now())
        #print(self.spi.xfer([0x1C, 0])[1]) # REG.LORA.MODEM_CONFIG_2
        self.clear_irq_flags(RxDone=1, ValidHeader=1)
        payload = self.read_payload(nocheck=False )# Receive INF
        print ("Receive: ")
        print(payload)
        if (payload):
          print("counter:", payload[0])
          (t, h) = struct.unpack('>hh', bytearray(payload[1:5]))
          print(t/10.0, "°C")
          print(h/10.0, "%rh")
          #print((payload[1]*256+payload[2])/10.0, "°C")
          #print((payload[3]*256+payload[4])/10.0, "%rh")
          #mens=bytes(payload).decode("utf-8",'ignore')
          #print(mens)
        #print(self.get_modem_config_1())
        #print(self.get_modem_config_2())
        BOARD.led_off()
        #if mens=="INF":
        #    print("Received data request INF")
        #    time.sleep(2)
        #    print ("Send mens: DATA RASPBERRY PI")
        #    self.write_payload([255, 255, 0, 0, 68, 65, 84, 65, 32, 82, 65, 83, 80, 66, 69, 82, $
        #    self.set_mode(MODE.TX)
        time.sleep(2)
        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)

    def on_tx_done(self):
        print("\nTxDone")
        print(self.get_irq_flags())

    def on_cad_done(self):
        print("\non_CadDone")
        print(self.get_irq_flags())

    def on_rx_timeout(self):
        print("\non_RxTimeout")
        print(self.get_irq_flags())

    def on_valid_header(self):
        print("\non_ValidHeader")
        print(self.get_irq_flags())

    def on_payload_crc_error(self):
        print("\non_PayloadCrcError")
        print(self.get_irq_flags())

    def on_fhss_change_channel(self):
        print("\non_FhssChangeChannel")
        print(self.get_irq_flags())

    def start(self):
        while True:
            self.reset_ptr_rx()
            self.set_mode(MODE.RXCONT) # Receiver mode
            while True:
                pass;


#lora = mylora(verbose=False)
lora = mylora(verbose=True)
#lora.set_pa_config(pa_select=1, max_power=21, output_power=15)
lora.set_freq(868.0)
lora.set_bw(BW.BW125)
lora.set_spreading_factor(10)
lora.set_sync_word(0x77)

print(datetime.datetime.now())

assert(lora.get_agc_auto_on() == 1)

try:
    print("START")
    lora.start()
except KeyboardInterrupt:
    sys.stdout.flush()
    print("Exit")
    sys.stderr.write("KeyboardInterrupt\n")
finally:
    sys.stdout.flush()
    print("Exit")
    lora.set_mode(MODE.SLEEP)
BOARD.teardown()
