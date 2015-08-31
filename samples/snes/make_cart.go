package main

import "fmt"
import "os"

func main() {
  if len(os.Args) != 3 {
    fmt.Println("Usage: " + os.Args[0] + " <binfile> <smcfile>")
    os.Exit(0)
  }

  file_in, err := os.Open(os.Args[1])

  if err != nil {
    panic(err)
  }

  defer file_in.Close()

  stat, err := file_in.Stat()
  if err != nil {
    panic(err)
  }

  size := int(stat.Size())
  cart := make([]byte, size)
  header := make([]byte, 512)

  file_in.Read(cart)
  file_in.Close()

  if (len(cart) % 8192) != 0 {
    fmt.Println("Error: ROM not a multiple of 8k")
    return
  }

  i := len(cart) / 8192

  header[0] = byte(i & 0xff)
  header[1] = byte(i >> 8)

  file_out, err := os.Create(os.Args[2])
  file_out.Write(header)
  file_out.Write(cart)
  file_out.Close()

}



