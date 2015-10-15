package main

import "fmt"
import "os"

func readString(file_in *os.File) string {
  chars := make([]byte, 4)
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

func readRiffHeader(file_in *os.File) uint32 {
  id := readString(file_in)

  if (id != "RIFF") {
    fmt.Println("Not a RIFF file.\n");
    return 0
  }

  wav_size := readInt32(file_in)
  format := readString(file_in)

  if (format != "WAVE") {
    fmt.Println("Not a WAV file.\n");
    return 0
  }

  fmt.Println("              ID:", id)
  fmt.Println("            Size:", wav_size)
  fmt.Println("          Format:", format)

  return wav_size
}

func readFmtHeader(file_in *os.File) uint32 {
  id := readString(file_in)
  chunk_size := readInt32(file_in)
  format_code := readInt16(file_in)
  channels := readInt16(file_in)
  sample_rate := readInt32(file_in)
  bytes_per_second := readInt32(file_in)
  bytes_per_sample := readInt16(file_in)
  bits_per_sample := readInt16(file_in)

  if (id != "fmt ") {
    fmt.Println("Unknown chunk id " + id)
    return 0
  }

  fmt.Println("              ID:", id)
  fmt.Println("      Chunk Size:", chunk_size)
  fmt.Println("     Format Code:", format_code)
  fmt.Println("        Channels:", channels)
  fmt.Println("     Sample Rate:", sample_rate)
  fmt.Println("Bytes Per Second:", bytes_per_second)
  fmt.Println("Bytes Per Sample:", bytes_per_sample)
  fmt.Println(" Bits Per Sample:", bits_per_sample)

  if (bits_per_sample != 8 || channels != 1) {
    fmt.Println("This wav isn't in the right format.")
    return 0
  }

  return chunk_size
}

func readDataHeader(file_in *os.File) uint32 {
  id := readString(file_in)
  chunk_size := readInt32(file_in)

  if (id != "data") {
    fmt.Println("Unknown chunk id " + id)
    return 0
  }

  fmt.Println("              ID:", id)
  fmt.Println("      Chunk Size:", chunk_size)

  data := make([]uint8, chunk_size)
  file_in.Read(data)

  for i := 0; i < len(data); i++ {
    if i % 8 == 0 {
      fmt.Print("\n  db")
    }

    d := fmt.Sprintf("0x%02x", int(data[i]))

    if i % 8 == 0 {
      fmt.Print(" " + d)
    } else {
      fmt.Print(", " + d)
    }
  }

  fmt.Println("")

  return chunk_size
}

func main() {
  fmt.Println("wav2dac - Copyright 2015 by Michael Kohn")

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

  if (readRiffHeader(file_in) == 0) {
    return
  }

  if (readFmtHeader(file_in) == 0) {
    return
  }

  if (readDataHeader(file_in) == 0) {
    return
  }

}


