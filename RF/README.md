# RF 433MHz

## Unbranded weather station
- 29 symbols, if startmark is counted (1 + 4 + 8 + 12 + 2 + 1 + 1)
- mark duration is constant, space varies
- mark: 440us (other measurements was 442us and 447us)
- symbol "0" (mark+space): 2442us (other measurement was 2442us)
- symbol "1" (mark+space): 5010us (other measurement was 5005us)
- startmark (mark+space): 10ms
- sends 7 + 1 times (I don't know what last one is, maybe some kind of check. it is not needed)
- sends about every 35s

value | startmark | parity | serial number | temperature x 10 in celcius | channel | battery | mode
--- | --- | --- | --- | --- | --- | --- | ----
23,4 | startmark | 0101 | 11111000 | 000011101010 | 01 | 1 | 1
22,0 | startmark | 0110 | 11111000 | 000011011100 | 01 | 1 | 1

#### Generating checksum
- leave out mark and parity
- sum up all nibbles and substract 1 and leave 4 last bits
- e.g. 1111 + 1000 + 0000 + 1110 + 1010 + 0111 = (11)0110 &#x21d2; 0110 - 1 = **0101**

## Motonet
Motonet Kauko-ohjattava ulkopistorasia IP44 3600W (38-4663) / EMAX 6867 ja 68671

- 230V AC, 50Hz
- 16A, 3600W
- Frequency: 433,92MHz
- Protection: IP44
- For some reason, Arduino IDE rc-switch library example (simple and advance) didn't get anything.

button | code (0 = short, 1 = long)
--- | ---
A on |  01100011101001100000 1111 1101 1000 00
A off | 01100011101001100000 1110 1101 1001 00
B on |  01100011101001100000 1101 1101 1011 00
B off | 01100011101001100000 1100 1101 1010 00
C on |  01100011101001100000 1011 1101 1111 00
C off | 01100011101001100000 1010 1101 1110 00
D on |  01100011101001100000 0111 1101 0000 00
D off | 01100011101001100000 0110 1101 0001 00
All on | 01100011101001100000 0100 1101 0010 00
All off | 01100011101001100000 1000 1101 1100 00

- Code is 34 bits
- 34 bits take 38.8ms
- One bit takes 1.141ms
- 0 (short): mark 300µs + space 841µs
- 1 (long): mark 815µs + space 326µs
- Remote sends code 6 times. About 10ms between codes.

## LIDL
TR-502MSV
- Code is 21 bits
- 0 (short): space 1300µs + mark 600µs
- 1 (long): space 600µs + mark 1300µs
- Remote sends code 4 times. About 80ms between codes.
- 21 bits (13+2+4+2)
- 13 bits identification: randomly selected when reset button is pressed from remot
- 2 bit address: 11 = 1, 01 = 2, 10 = 3, 00 = 4 (dim sends last pressed BtnID and if "all" is pressed BtnID is 00)
- 4 bit opcode: 1011 = on, 1111 = off, 1101 = dim+, 1001 = dim-, 0011 = all on, 0111 = all off
- 2 bit checksum: last 3 bitpairs xorred with each other
Examples:

Button | RemoteID | BtnID | OpCode | Chksum
--- | --- | --- | --- | ---
1 on |  0011011101101 | 11 | 1011 | 10
1 off | 0011011101101 | 11 | 1111 | 11
2 on |  0011011101101 | 01 | 1011 | 00
2 off | 0011011101101 | 01 | 1111 | 01

## Python serial

sudo apt install python3-serial

```
#!/usr/bin/env python3

import serial, time, sys

#print('Number of arguments:', len(sys.argv), 'arguments.')
#print('Argument List:', str(sys.argv))

ser = serial.Serial('/dev/ttyUSB0', 115200)
#print(ser)
if sys.argv[1] == "on":
  print("on")
  ser.write(b'011000111010011000001111 1101100000')
if sys.argv[1] == "off":
  print("off")
  ser.write(b'011000111010011000001110 1101100100')

#ser.write(b'011000111010011000001111 1101100000')
#time.sleep(5)
#ser.write(b'011000111010011000001110 1101100100')
ser.close()

```

## Python rpi-rf
```
apt-get install python3-pip
sudo pip3 install rpi-rf

/usr/local/lib/python3.7/dist-packages/rpi_rf/rpi_rf.py:
            Protocol(350, 0, 100, 3, 8, 8, 3))

# Raspberry Pi 4: 350,0,100,3,8,8,3 (-p 100) -> 1:792 high 383 low 0:277 high 895 low
# rpi-rf_send doesn't use protocol first parameter in source file. must be given with rpi-rf_send parameter.

sleep 1; rpi-rf_send -g 17 -p 100 -t 7 -l 34 6687308644; sleep 1; rpi-rf_send -g 17 -p 100 -t 8 -l 34 6687309664
```

## Antenna
- https://www.element14.com/community/community/project14/rf/blog/2020/01/11/building-a-poor-man-s-quarter-wave-433mhz-antenna-antenna-s-construction
- 433MHz: 17,36cm
