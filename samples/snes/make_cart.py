#!/usr/bin/env python3

import sys

if len(sys.argv) != 3:
  print("Usage: python3 make_cart.py <binfile> <smcfile>")
  sys.exit(0)

fp = open(sys.argv[1], "rb")
cart = bytearray(fp.read())
fp.close()

if (len(cart) % 8192) != 0:
  print("Error: ROM not a multiple of 8k")
  sys.exit(1)

# FIXME: This checksum calculation doesn't seem to work.
checksum = 0

for i in range(0, len(cart)):
  checksum += cart[i]

checksum &= 0xffff

cart[0xffde - 32768] = checksum & 0xff
cart[0xffdf - 32768] = checksum >> 8
checksum = checksum ^ 0xffff
cart[0xffdc - 32768] = checksum & 0xff
cart[0xffdd - 32768] = checksum >> 8

header = bytearray(512)
blocks = int(len(cart) / 8192)

header[0] = blocks & 0xff
header[1] = blocks >> 8

fp = open(sys.argv[2], "wb")
fp.write(header)
fp.write(cart)
fp.close()

