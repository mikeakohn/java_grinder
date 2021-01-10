package main

import "fmt"
import "os"

func main() {
  //fmt.Println("INFO: bin2spin - Copyright 2016 by Michael Kohn")

  if len(os.Args) != 2 {
    fmt.Println("Usage: " + os.Args[0] + " <binfile>")
    os.Exit(0)
  }

  file_in, err := os.Open(os.Args[1])

  if err != nil {
    panic(err)
  }

  // Close the file if the program exits
  defer file_in.Close()

  stat, err := file_in.Stat()
  if err != nil {
    panic(err)
  }

  file_size := int(stat.Size())

  data := make([]byte, file_size)
  file_in.Read(data)

  fmt.Println("Pub Main")
  fmt.Println("  cognew(@code, 0)\n")
  fmt.Println("DAT")
  fmt.Print("  code byte ")

  for i :=0; i < len(data); i++ {
    if i != 0 && (i % 8) == 0 {
      fmt.Print("\n       byte")
    }

    if (i % 8) != 0 { fmt.Print(",") }
    fmt.Printf(" %d", data[i])
  }
}

