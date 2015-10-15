package main

import "fmt"
import "os"

func main() {
  fmt.Println("INFO: bin2java - Copyright 2015 by Michael Kohn")

  if len(os.Args) != 2 {
    fmt.Println("Usage: " + os.Args[0] + " <binfile>")
    os.Exit(0)
  }

  // Open wav file for reading
  //fmt.Println("Reading: " + os.Args[1])

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

  fmt.Println("INFO:", file_size)
  fmt.Println()

  data := make([]byte, file_size)
  file_in.Read(data)

  fmt.Println("public class ClassName\n{")
  fmt.Println("  public static byte[] code =\n  {")

  for i := 0; i < len(data); i++ {
    if i % 8 == 0 {
      fmt.Print("\n   ")
    }

    d := fmt.Sprintf("%4d", int8(data[i]))

    fmt.Print(" " + d + ",")
  }

  fmt.Println("\n  };")
  fmt.Println("}\n")
}


