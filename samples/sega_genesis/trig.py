#!/usr/bin/env python

import math

dr = (3.14 * 2) / 90;

print "  static short[] cos ="
print "  {"

s = "    "
r = 0
for t in range(0, 90):
  a = math.cos(r)
  a = a * 64
  a = int(math.floor(a))
  s = s + (" %3d," % a)
  if ((t + 1) % 10) == 0: print s; s = "    "
  r += dr

print "  };\n"

print "  static short[] sin ="
print "  {"

s = "    "
r = 0
for t in range(0, 90):
  a = math.sin(r)
  a = a * 64
  a = int(math.floor(a))
  s = s + (" %3d," % a)
  if ((t + 1) % 10) == 0: print s; s = "    "
  r += dr

print "  };\n"

