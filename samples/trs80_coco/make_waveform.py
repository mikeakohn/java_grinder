#!/usr/bin/env python

import sys, math

rate = 8000
samples = []

if len(sys.argv) != 3:
  print "Usage " + sys.argv[0] + " <freq> <sine/square/sawtooth>"
  sys.exit(0)

print "Rate is " + str(rate) + " samples a second"

size = rate / 440

if sys.argv[2] == "square":
  for i in range(0, size):
    if i < size / 2: samples.append(63)
    else: samples.append(0)
elif sys.argv[2] == "sawtooth":
  dx = float(63) / float(size - 1)
  sample = float(63)

  for i in range(0, size):
    samples.append(int(sample))
    sample -= dx
elif sys.argv[2] == "sine":
  dx = float(3.14 * 2) / float(size)
  x = 0

  for i in range(0, size):
    samples.append(int(31 * math.sin(x) + 32))
    x += dx

print "  public static byte[] waveform = {",
for i in range(0, len(samples)):
  if (i % 8) == 0: print "\n    ",
  print " 0x%02x," % (samples[i] << 2),
print "  };"

