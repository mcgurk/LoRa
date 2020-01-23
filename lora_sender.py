#!/usr/bin/env python3

from time import sleep
from SX127x.LoRa import *
from SX127x.LoRaArgumentParser import LoRaArgumentParser
from SX127x.board_config import BOARD

BOARD.setup()

parser = LoRaArgumentParser("LoRa sender.")

class LoRaSender(LoRa):
    def __init__(self, verbose=False):
        super(LoRaSender, self).__init__(verbose)
        self.set_mode(MODE.SLEEP)
        self.set_dio_mapping([1,0,0,0,0,0])
        #self.set_dio_mapping([0] * 6)

    def on_rx_done(self):
        print("\nRxDone")
        print(self.get_irq_flags())
        print(map(hex, self.read_payload(nocheck=True)))
        self.set_mode(MODE.SLEEP)
        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)

    def on_tx_done(self):
        global args
        self.set_mode(MODE.STDBY)
        self.clear_irq_flags(TxDone=1)
        sys.stdout.flush()
        self.tx_counter += 1
        sys.stdout.write("\rtx #%d" % self.tx_counter)
        BOARD.led_off()
        sleep(1)
        #self.write_payload([0x0e])
        self.write_payload([0x0e,0x00,0x00,0x00,0x00,0x00])
        BOARD.led_on()
        self.set_mode(MODE.TX)
        sys.stdout.write("on_tx_done end")

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
        global args
        sys.stdout.write("\rstart")
        self.tx_counter = 0
        BOARD.led_on()
        self.write_payload([0x0f])
        self.set_mode(MODE.TX)
        while True:
            sleep(1)

lora = LoRaSender(verbose=False)
args = parser.parse_args(lora) # configs in LoRaArgumentParser.py

#lora.set_mode(MODE.STDBY)
lora.set_pa_config(pa_select=1, max_power=21, output_power=2)
lora.set_freq(868.0)
lora.set_bw(BW.BW125)
lora.set_spreading_factor(10)
lora.set_sync_word(0x77)
lora.set_rx_crc(True)
#lora.set_rx_crc(False)

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
