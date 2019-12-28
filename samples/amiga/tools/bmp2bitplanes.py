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

print("    bmp filename: " + filename_bmp)
print("    out filename: " + filename_out)

fp = open(filename_bmp, "rb")
out = open(filename_out, "w")

image_name = filename_out.replace(".java", "")

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
print("Image Offset: " + str(image_offset))

if colors > 32:
  print("Amiga only supports 32 colors\n")
  sys.exit(1)

bitplanes = [ ]

for n in range(colors, palette_len):
  palette.append(0)

for i in range(0, bitplane_count):
  bitplanes.append([])

print(bitplanes)

out.write("\n")
out.write("import net.mikekohn.java_grinder.amiga.Copper;\n")
out.write("\n")
out.write("public class " + image_name + "\n")
out.write("{\n")

out.write("  static public short[] palette =\n")
out.write("  {")

count = 0

for color in palette:
  if (count % 16) == 0: out.write("\n   ");
  out.write(" 0x%03x," % (color))
  count += 1

out.write("\n  };\n\n")

count = 0
n = 0

x = 0
y = height - 1

byte_width = width * 3

while y >=  0:

  current = [ 0, 0, 0, 0, 0 ]

  n = y * byte_width + x * 3

  for i in range(0, 8):
    index = image_offset + n

    blue = data[index + 0] >> 4
    green = data[index + 1] >> 4
    red = data[index + 2] >> 4

    color = (red << 8) | (green << 4) | blue
    c = palette_map[color]

    for r in range(0, 5):
      current[r] |= ((c & 1) << (7 - i))
      c = c >> 1

    n = n + 3
    x = x + 1

    if x == width:
      y -= 1
      x = 0

  for r in range(0, len(bitplanes)):
    bitplanes[r].append(current[r])

for n in range(0, len(bitplanes)):
  out.write("  static public byte[] bitplane_" + str(n) + " =\n")
  out.write("  {")

  count = 0

  for data in bitplanes[n]:
    if (count % 8) == 0: out.write("\n   ")

    if (data & 0x80) != 0:
      data = -((data ^ 0xff) + 1)

    out.write(" %d," % (data));

    count += 1

  out.write("\n  };\n\n")

out.write("  static public void show(Copper copper)\n")
out.write("  {\n")
out.write("    copper.stop();\n")
out.write("    copper.resetIndex();\n")

for n in range(0, len(bitplanes)):
  out.write("    copper.appendSetBitplane(" + str(n) + ", bitplane_" + str(n) + ");\n")

for n in range(0, colors):
  out.write("    copper.appendSetColor(" + str(n) + ", palette[" + str(n) + "]);\n")

out.write("    copper.appendEnd();\n")
out.write("    copper.run();\n")
out.write("  }\n")

out.write("}\n\n")

fp.close()
out.close()

