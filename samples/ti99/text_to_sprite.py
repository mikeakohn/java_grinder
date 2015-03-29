#!/usr/bin/env bash

import sys

def column_to_byte(column):
  num = 0
  bit = 128

  for a in column:
    #print bit
    if a == '*': num += bit
    bit >>= 1

  if num > 127: num = -((num ^ 0xff) + 1)

  return str(num)

def column_to_bytes(columns):
  count = 0

  for column in columns:
    num = column_to_byte(column)
    print " %3s," % num,
    count += 1
    if (count % 8) == 0: print

  #print

# -------------------------- fold here ------------------------------

column_1 = []
column_2 = []

fp = open(sys.argv[1], "rb")

for line in fp:
  line = line[:-1]
  if line[0] == '0': continue
  line = line + (" " * (len(line) - 16))
  #print line
  column_1.append(line[0:9])
  column_2.append(line[9:17])

fp.close()

print column_1
print column_2

print "  static byte[] sprite_ ="
print "  {"
column_to_bytes(column_1)
column_to_bytes(column_2)
print "  };"


