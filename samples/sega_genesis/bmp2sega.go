package main

import "fmt"
import "os"
import "strconv"

// convert -scale 320x224! -colors 16 -type truecolor -flip ~/3_billion_devices.bmp 3_billion_devices.bmp

type BmpImage struct {
  file_in *os.File
  data []uint8
  width int
  height int
  image_size uint32
}

type SegaImage struct {
  data []uint8
  width int
  height int
  patterns [][]uint8
  image []uint16
  palette []int
  palette_map map[int]int
  color_count int
}

func (bmp_image *BmpImage) ReadString() string {
  chars := make([]byte, 2)
  bmp_image.file_in.Read(chars)

  return string(chars)
}

func (bmp_image *BmpImage) ReadInt32() uint32 {
  chars := make([]uint8, 4)
  bmp_image.file_in.Read(chars)

  return uint32(chars[0]) | (uint32(chars[1]) << 8) | (uint32(chars[2]) << 16) | (uint32(chars[3]) << 24)
}

func (bmp_image *BmpImage) ReadInt16() uint32 {
  chars := make([]uint8, 2)
  bmp_image.file_in.Read(chars)

  return uint32(chars[0]) | (uint32(chars[1]) << 8)
}

func (bmp_image *BmpImage) Init(file_name string) int {
  // Open BMP file for reading
  fmt.Println("Reading: " + file_name)

  file_in, err := os.Open(file_name)

  if err != nil {
    panic(err)
  }

  bmp_image.file_in = file_in

  bmp_image.width = 0
  bmp_image.height = 0

  // Close the file if the program exits
  //defer bmp_image.file_in.Close()

  offset := bmp_image.ReadBmpHeader()

  if (offset == 0) {
    return  -1
  }

  bmp_image.image_size = bmp_image.ReadBmpInfo()

  if (bmp_image.image_size == 0) {
    return -1
  }

  bmp_image.file_in.Seek(int64(offset), os.SEEK_SET)

  bmp_image.data = make([]uint8, bmp_image.image_size)
  file_in.Read(bmp_image.data)

  return 0
}

func (bmp_image *BmpImage) ReadBmpHeader() uint32 {
  id := bmp_image.ReadString()

  if (id != "BM") {
    fmt.Println("Not a BMP file.\n");
    return 0
  }

  bmp_size := bmp_image.ReadInt32()
  bmp_image.ReadInt32()
  offset := bmp_image.ReadInt32()

  fmt.Println("              ID: " + id)
  fmt.Println("            Size: " + strconv.Itoa(int(bmp_size)))
  fmt.Println("          Offset: " + strconv.Itoa(int(offset)))

  return offset
}

func (bmp_image *BmpImage) ReadBmpInfo() uint32 {
  header_size := bmp_image.ReadInt32()
  bmp_image.width = int(bmp_image.ReadInt32())
  bmp_image.height = int(bmp_image.ReadInt32())
  color_planes := bmp_image.ReadInt16()
  bits_per_pixel := bmp_image.ReadInt16()
  compression := bmp_image.ReadInt32()
  image_size := bmp_image.ReadInt32()
  horizontal_resolution := bmp_image.ReadInt32()
  vertical_resolution := bmp_image.ReadInt32()
  palette_colors := bmp_image.ReadInt32()
  important_colors := bmp_image.ReadInt32()

  fmt.Println("     Header Size: " + strconv.Itoa(int(header_size)))
  fmt.Println("           Width: " + strconv.Itoa(int(bmp_image.width)))
  fmt.Println("          Height: " + strconv.Itoa(int(bmp_image.height)))
  fmt.Println("    Color Planes: " + strconv.Itoa(int(color_planes)))
  fmt.Println("  Bits Per Pixel: " + strconv.Itoa(int(bits_per_pixel)))
  fmt.Println("     Compression: " + strconv.Itoa(int(compression)))
  fmt.Println("      Image Size: " + strconv.Itoa(int(image_size)))
  fmt.Println("  Horizontal Res: " + strconv.Itoa(int(horizontal_resolution)))
  fmt.Println("    Vertical Res: " + strconv.Itoa(int(vertical_resolution)))
  fmt.Println("  Palette Colors: " + strconv.Itoa(int(palette_colors)))
  fmt.Println("Important Colors: " + strconv.Itoa(int(important_colors)))

  if (bmp_image.height < 0) { bmp_image.height = -bmp_image.height }

  if (compression != 0 || bits_per_pixel != 24) {
    fmt.Println("This BMP isn't in the right format.")
    return 0
  }

  return uint32(bmp_image.width) * uint32(bmp_image.height) * 3
}

func writeImageArray(data []uint16) {
  for i := 0; i < len(data); i++ {
    if i % 8 == 0 {
      fmt.Print("\n   ")
    }

    fmt.Printf(" 0x%04x,", data[i])
  }
}

func writeInt16Array(data []int) {
  for i := 0; i < len(data); i++ {
    if i % 8 == 0 {
      fmt.Print("\n   ")
    }

    fmt.Printf(" 0x%03x,", data[i])
  }
}

func writeInt32Array(data []uint8) {
  for i := 0; i < len(data); i = i + 4 {
    if i % 16 == 0 {
      fmt.Print("\n   ")
    }

    n := (uint32(data[i]) << 24) |
         (uint32(data[i+1]) << 16) |
         (uint32(data[i+2]) << 8) |
         (uint32(data[i+3]))

    fmt.Printf(" 0x%08x,", int(n))
  }

  fmt.Println()
}

func (sega_image *SegaImage) Init (data []uint8, width int, height int) {
  //sega_image.data = make([]uint8, image_size)
  sega_image.data = data
  sega_image.width = width
  sega_image.height = height
  sega_image.palette = make([]int, 16)
  sega_image.palette_map = make(map[int]int)
  sega_image.color_count = 0

  //file_in.Read(sega_image.data)
}

func (sega_image *SegaImage) GetPixelColor(x int, y int) int {
  loc := (x * 3) + (y * sega_image.width *3)

  return (int(sega_image.data[loc + 0] >> 5) << 9) |
         ((int(sega_image.data[loc + 1] >> 5)) << 5) |
         ((int(sega_image.data[loc + 2] >> 5)) << 1)
}

func doesPatternMatch(a []uint8, b []uint8) bool {
  if len(a) != len(b) { fmt.Println("Problem in match") }

  for i := 0; i < len(a); i++ {
    if a[i] != b[i] { return false; }
  }

  return true
}

func (sega_image *SegaImage) GetPattern (x0 int, y0 int) int {
  var color int
  var pattern_nybble int

  // Ouch.  There's faster ways to do this, but this should be pretty
  // quick anyway, so who cares.  For each 8x8 block of the image, set up
  // an 8x8 pixel x 4 bit color, 32 byte pattern.  To save ROM space, do
  // not duplicate patterns.

  pattern := make([]uint8, 32)
  pattern_nybble = 0

  // Get 8x8 pattern out of image
  for y := 0; y < 8; y++ {
    for x := 0; x < 8; x++ {
      color = sega_image.GetPixelColor(x0 + x, y0 + y)
      _, ok := sega_image.palette_map[color]

      if !ok {
        if sega_image.color_count == 16 {
          for _, color := range(sega_image.palette_map) {
            fmt.Printf("  %03x\n", color)
          }
          fmt.Println("Too many colors in image")
          return 0
        }
        sega_image.palette_map[color] = sega_image.color_count
        sega_image.palette[sega_image.color_count] = color
        sega_image.color_count += 1
      }

      color = sega_image.palette_map[color]

      if (pattern_nybble & 1) == 0 {
        pattern[pattern_nybble >> 1] = uint8(color) << 4
      } else {
        pattern[pattern_nybble >> 1] |= uint8(color)
      }

      pattern_nybble++
    }
  }
  match := false

  // Check if this pattern already exists
  for index, element := range sega_image.patterns {
    //fmt.Printf("%q\n", element)
    if doesPatternMatch(element, pattern) {
      sega_image.image = append(sega_image.image, uint16(index))
      match = true
      break
    }
  }

  if match == false {
    sega_image.image = append(sega_image.image, uint16(len(sega_image.patterns)))
    sega_image.patterns = append(sega_image.patterns, pattern)
  }

  return 1
}

func (sega_image *SegaImage) Print () {
  fmt.Println("  public static int[] pattern =\n  {")
  for index, pattern := range sega_image.patterns {
    fmt.Printf("    // Pattern %d", index)
    writeInt32Array(pattern)
  }
  fmt.Println("\n  };\n")

  fmt.Print("  public static short[] image =\n  {")
  writeImageArray(sega_image.image)
  fmt.Println("\n  };\n")

  fmt.Print("  public static short[] palette =\n  {")
  writeInt16Array(sega_image.palette)
  fmt.Println("\n  };")

  fmt.Println("}\n")
}

func readScreenData(bmp_image *BmpImage) uint32 {

  fmt.Println("readScreenData: %d\n", len(bmp_image.data))

  sega_image := new(SegaImage)
  sega_image.Init(bmp_image.data, bmp_image.width, bmp_image.height)

  fmt.Println("public class ClassName\n{")

  for y0 := 0; y0 < 224; y0 = y0 + 8 {
    for x0 := 0; x0 < 320; x0 = x0 + 8 {
      if sega_image.GetPattern(x0, y0) == 0 { return 0; }
    }
  }

  if len(sega_image.image) != 40 * 28 {
    fmt.Printf("Image isn't 40x28? %d\n", len(sega_image.image))
    return 0
  }

  sega_image.Print()

  return 1
}

func readSpriteData(bmp_image *BmpImage) uint32 {

  fmt.Println("readSpriteData: %d\n", len(bmp_image.data))

  if (bmp_image.width % 8) != 0 || (bmp_image.height % 8) != 0 {
    fmt.Println("Error: Image width/height aren't multiples of 8 pixels")
    return 0
  }

  width_factor := bmp_image.width / 8
  height_factor := bmp_image.height / 8

  if width_factor < 1 || width_factor > 4 {
    fmt.Println("Error: Image width can only be 8, 16, 24, or 32 pixels\n")
    return 0
  }

  if height_factor < 1 || height_factor > 4 {
    fmt.Println("Error: Image height can only be 8, 16, 24, or 32 pixels\n")
    return 0
  }

  sega_image := new(SegaImage)
  sega_image.Init(bmp_image.data, bmp_image.width, bmp_image.height)

  fmt.Println("public class ClassName\n{")

  for x0 := 0; x0 < bmp_image.width; x0 = x0 + 8 {
    for y0 := 0; y0 < bmp_image.height; y0 = y0 + 8 {
      if sega_image.GetPattern(x0, y0) == 0 { return 0; }
    }
  }

  sega_image.Print()

  return 1
}

func main() {
  fmt.Println("bmp2sega - Copyright 2015 by Michael Kohn")

  if len(os.Args) != 3 {
    fmt.Println("Usage: " + os.Args[0] + " <file.bmp> <screen/sprite>")
    os.Exit(0)
  }

  bmp_image := new(BmpImage)
  if bmp_image.Init(os.Args[1]) != 0 {
    return
  }

  if os.Args[2] == "screen" {
    if readScreenData(bmp_image) == 0 {
      return
    }
  } else if os.Args[2] == "sprite" {
    if readSpriteData(bmp_image) == 0 {
      return
    }
  } else {
    fmt.Println("Unknown option " + os.Args[2])
  }
}


