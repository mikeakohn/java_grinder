package main

import "fmt"
import "os"
import "strconv"

func readString(file_in *os.File) string {
  chars := make([]byte, 2)
  file_in.Read(chars)

  return string(chars)
}

func readInt32(file_in *os.File) uint32 {
  chars := make([]uint8, 4)
  file_in.Read(chars)

  return uint32(chars[0]) | (uint32(chars[1]) << 8) | (uint32(chars[2]) << 16) | (uint32(chars[3]) << 24)
}

func readInt16(file_in *os.File) uint32 {
  chars := make([]uint8, 2)
  file_in.Read(chars)

  return uint32(chars[0]) | (uint32(chars[1]) << 8)
}

func readBmpHeader(file_in *os.File) uint32 {
  id := readString(file_in)

  if (id != "BM") {
    fmt.Println("Not a BMP file.\n");
    return 0
  }

  bmp_size := readInt32(file_in)
  readInt32(file_in)
  offset := readInt32(file_in)

  fmt.Println("              ID: " + id)
  fmt.Println("            Size: " + strconv.Itoa(int(bmp_size)))
  fmt.Println("          Offset: " + strconv.Itoa(int(offset)))

  return bmp_size
}

func readBmpInfo(file_in *os.File) uint32 {
  header_size := readInt32(file_in)
  width := readInt32(file_in)
  height := int32(readInt32(file_in))
  color_planes := readInt16(file_in)
  bits_per_pixel := readInt16(file_in)
  compression := readInt32(file_in)
  image_size := readInt32(file_in)
  horizontal_resolution := readInt32(file_in)
  vertical_resolution := readInt32(file_in)
  palette_colors := readInt32(file_in)
  important_colors := readInt32(file_in)

  fmt.Println("     Header Size: " + strconv.Itoa(int(header_size)))
  fmt.Println("           Width: " + strconv.Itoa(int(width)))
  fmt.Println("          Height: " + strconv.Itoa(int(height)))
  fmt.Println("    Color Planes: " + strconv.Itoa(int(color_planes)))
  fmt.Println("  Bits Per Pixel: " + strconv.Itoa(int(bits_per_pixel)))
  fmt.Println("     Compression: " + strconv.Itoa(int(compression)))
  fmt.Println("      Image Size: " + strconv.Itoa(int(image_size)))
  fmt.Println("  Horizontal Res: " + strconv.Itoa(int(horizontal_resolution)))
  fmt.Println("    Vertical Res: " + strconv.Itoa(int(vertical_resolution)))
  fmt.Println("  Palette Colors: " + strconv.Itoa(int(palette_colors)))
  fmt.Println("Important Colors: " + strconv.Itoa(int(important_colors)))

  if (height < 0) { height = -height }

  if (width != 320 ||
      height != 224 ||
      compression != 0 ||
      bits_per_pixel != 24) {
    fmt.Println("This BMP isn't in the right format.")
    return 0
  }

  return width * uint32(height) * 3
}

func writeByteArray(data []int) {
  for i := 0; i < len(data); i++ {
    if i % 8 == 0 {
      fmt.Print("\n   ")
    }

    d := fmt.Sprintf("0x%03x", int16(data[i]))

    fmt.Print(" " + d + ",")
  }
}

func getPixelColor(data []uint8) int {
  return (int(data[0]) >> 5) |
         ((int(data[1]) >> 5) << 3) |
         ((int(data[2]) >> 5) << 6)
}

func getPixelLocation(x int, y int) int {
  return (x * 3) + (y * 320 *3)
}

func readData(file_in *os.File, image_size uint32) uint32 {
  var loc int
  var color int
  var palette_map map[int]int
  //dict Dict or map?

  fmt.Println("readData: " + strconv.Itoa(int(image_size)))

  data := make([]uint8, image_size)
  palette := make([]int, 16)
  palette_map = make(map[int]int)
  color_count := 0

  file_in.Read(data)

  fmt.Println("public class ClassName\n{")
  fmt.Println("  public static byte[] pattern =\n  {")

  for y := 0; y < 224; y = y + 8 {
    for x := 0; x < 320; x = x + 8 {
      loc = getPixelLocation(x, y)
      color = getPixelColor(data[loc:loc+3])
      _, ok := palette_map[color]

      if !ok {
        if color_count == 16 {
          fmt.Println("Too many colors in image")
          return 0
        }
        palette_map[color] = color_count
        palette[color_count] = color
        color_count += 1
      }
    }
  }

  fmt.Println("\n  };")

  fmt.Println("  public static byte[] image =\n  {")
  fmt.Println("\n  };")

  fmt.Print("  public static byte[] palette =\n  {")
  writeByteArray(palette)
  fmt.Println("\n  };")

  fmt.Println("}\n")

  return 1
}

func main() {
  fmt.Println("bmp2sega - Copyright 2015 by Michael Kohn")

  if len(os.Args) != 2 {
    fmt.Println("Usage: " + os.Args[0] + " <wavfile>")
    os.Exit(0)
  }

  // Open wav file for reading
  fmt.Println("Reading: " + os.Args[1])

  file_in, err := os.Open(os.Args[1])

  if err != nil {
    panic(err)
  }

  // Close the file if the program exits
  defer file_in.Close()

  if (readBmpHeader(file_in) == 0) {
    return
  }

  image_size := readBmpInfo(file_in)

  if (image_size == 0) {
    return
  }

  if (readData(file_in, image_size) == 0) {
    return
  }

}


