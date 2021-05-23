#!/usr/bin/env python

fp = open("opcodes.txt","r")

last_opcode = -1

while 1:
  line = fp.readline()
  if not line: break
  line = line.strip()
  if not line: continue
  if line[0] == '#': continue

  tokens = line.split()
  opcode = int(tokens[0])
  if opcode-1 != last_opcode:
    for t in range(last_opcode+1, opcode):
      print "  { \"???\", 0, 0 }, // undefined (0x%02x)" % t

  print "  { \"" + tokens[2] + "\", 1, 0 }, // "+tokens[2]+" "+tokens[1]

  last_opcode = opcode

fp.close()




