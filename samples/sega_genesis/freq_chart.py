#!/usr/bin/env python

freqs = [ 617, 653, 692, 733, 777, 823, 872, 924, 979, 1037, 1099, 1164 ]

print len(freqs)


for i in range (-1, 9):
  count = 0

  for freq in freqs:
    if count == 0: print "  dw",
    elif count == 6: print "; " + str(i) + "\n  dw",

    if i == -1:
      data = 0
    else:
      data = (i << 11) | freq

    print " 0x%04x," % data,
    count += 1

  print



