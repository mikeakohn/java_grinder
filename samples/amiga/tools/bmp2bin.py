#!/usr/bin/env python

import sys, math

def get_int16(data, index):
  return data[index] | (data[index + 1] << 8)

def get_int32(data, index):
  return data[index] | \
        (data[index + 1] << 8) | \
        (data[index + 2] << 16) | \
        (data[index + 3] << 24)

# ----------------------------- fold -----------------------------

if len(sys.argv) != 3:
  print("Usage: bmp2texture.py <input filename> <output filename>")
  sys.exit(0)

filename_bmp = sys.argv[1]
filename_out = sys.argv[2]

image_name = filename_out.replace(".java", "")

filename_bin = image_name + ".bin"

print("    bmp filename: " + filename_bmp)
print("   java filename: " + filename_out)
print("    bin filename: " + filename_bin)

fp = open(filename_bmp, "rb")
java = open(filename_out, "w")
out = open(filename_bin, "wb")

data = fp.read()
bytes_read = len(data)

width = get_int32(data, 18)
height = get_int32(data, 22)
bmp_bits_per_pixel = get_int16(data, 28)
image_offset = get_int32(data, 10)
image_size = get_int32(data, 34)

print("Bytes read " + str(bytes_read))
print(" ---------- BMP File Header ----------")
print("          header: " + chr(data[0]) + chr(data[1]))
print("            size: " + str(get_int32(data, 2)))
print("        reserved: " + str(get_int16(data, 6)))
print("        reserved: " + str(get_int16(data, 8)))
print("          offset: " + str(image_offset))
print(" ---------- BMP Info Header ----------")
print("            size: " + str(get_int32(data, 14)))
print("           width: " + str(width))
print("          height: " + str(height))
print("    color planes: " + str(get_int16(data, 26)))
print("  bits per pixel: " + str(bmp_bits_per_pixel))
print("     compression: " + str(get_int32(data, 30)))
print("      image size: " + str(get_int32(data, 34)))
print("       horiz res: " + str(get_int32(data, 38)))
print("        vert res: " + str(get_int32(data, 42)))
print("  palette colors: " + str(get_int32(data, 46)))
print("important colors: " + str(get_int32(data, 50)))

if bmp_bits_per_pixel != 24:
  print("Please re-encode as 24 bit.")
  sys.exit(1)

if (width % 8) != 0:
  print("Width of image must be a multiple of 8.")
  sys.exit(1)

palette = [ ]
palette_map = { }

n = 0

while n < image_size:
  index = image_offset + n

  blue = data[index + 0] >> 4
  green = data[index + 1] >> 4
  red = data[index + 2] >> 4

  color = (red << 8) | (green << 4) | blue

  if not color in palette:
    palette_map[color] = len(palette)
    palette.append(color)

  n = n + 3

colors = len(palette)
bitplane_count = int(math.ceil(math.log2(colors)))
palette_len = int(math.pow(2, bitplane_count))

print("Total Colors: " + str(colors))
print("   Bitplanes: " + str(bitplane_count))
print(" Palette Len: " + str(palette_len))

if colors > 32:
  print("Amiga only supports 32 colors\n")
  sys.exit(1)

bitplanes = [ ]

for n in range(colors, palette_len):
  palette.append(0)

for i in range(0, bitplane_count):
  bitplanes.append([])

print(bitplanes)

java.write("\n")
java.write("import net.mikekohn.java_grinder.amiga.Copper;\n")
java.write("\n")
java.write("public class " + image_name + "\n")
java.write("{\n")

java.write("  static public short[] palette =\n")
java.write("  {")

count = 0

for color in palette:
  if (count % 16) == 0: java.write("\n   ");
  java.write(" 0x%03x," % (color))
  count += 1

java.write("\n  };\n\n")

count = 0
n = 0

while n < image_size:

  current = [ 0, 0, 0, 0, 0 ]

  for i in range(0, 8):
    index = image_offset + n

    blue = data[index + 0] >> 4
    green = data[index + 1] >> 4
    red = data[index + 2] >> 4

    color = (red << 8) | (green << 4) | blue
    c = palette_map[color]

    for r in range(0, 5):
      current[r] = c
      c = c >> 1

    n = n + 3

  for r in range(0, len(bitplanes)):
    bitplanes[r].append(current[r])

java.write("  static public byte[] bitplanes = Memory.preloadByteArray(\"" + filename_bin + "\");\n")

for n in range(0, len(bitplanes)):
  for data in bitplanes[n]:
    out.write(bytes(data));

java.write("  static public void show(Copper copper)\n")
java.write("  {\n")
java.write("    copper.stop();\n")
java.write("    copper.resetIndex();\n")

for n in range(0, len(bitplanes)):
  java.write("    copper.appendSetBitplane(" + str(n) + ", Memory.addressOf(bitplanes) + " + str(n * 8000) + ");\n")

for n in range(0, colors):
  java.write("    copper.appendSetColor(" + str(n) + ", palette[" + str(n) + "]);\n")

java.write("    copper.appendEnd();\n")
java.write("    copper.run();\n")
java.write("  }\n")

java.write("}\n\n")

fp.close()
java.close()
out.close()

