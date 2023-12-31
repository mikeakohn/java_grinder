#!/usr/bin/env python3

import sys

notes = {
  "p"  : 0,
  "c"  : 60,
  "c#" : 61,
  "d" :  62,
  "d#" : 63,
  "e" :  64,
  "f" :  65,
  "f#" : 66,
  "g" :  67,
  "g#" : 68,
  "a" :  69,
  "a#" : 70,
  "b" :  71
}

# --------------------------- fold here -----------------------------

if len(sys.argv) != 2:
  print("Usage: python3 create_rom.py <song.rtttl>")
  sys.exit(0)

fp = open(sys.argv[1], "r")

line = fp.read().strip()
(name, info, song) = line.split(":")

infos = info.split(",")
if song[-1] == ";": song = song[:-1]

print(name)
print(info)
print(song)

d = 4

for option in infos:
  if option.startswith("o="): o = option.split("=")[1]
  if option.startswith("d="): d = option.split("=")[1]

b = [ ]

for note in song.split(","):
  l = ""

  original = note

  while note[0].isdigit():
    l += note[0]
    note = note[1:]

  if note[-1].isdigit():
    octave = note[-1]
    note = note[:-1]
  else:
    octave = o

  if l == "": l = d

  print(original + " " + note + " " + str(l) + " " + str(octave))

  dot = False

  if "." in note:
    dot = True
    note = note.replace(".", "")

  n = notes[note]
  if n != 0: n += (int(octave) - 4) * 12

  length = int(2000 / int(l))
  if dot: length = int(length + (length / 2))

  b.append(n)
  b.append(length)

fp.close()

print(b)

