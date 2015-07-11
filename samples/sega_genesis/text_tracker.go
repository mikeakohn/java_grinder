package main

import "fmt"
import "os"

type Tokens struct {
  text []byte
  size int
  next int
}

func (tokens *Tokens) Load(filename string) bool {
  // Open wav file for reading
  file, err := os.Open(filename)

  if err != nil {
    panic(err)
  }

  defer file.Close()

  stat, err := file.Stat()
  if err != nil {
    panic(err)
  }

  tokens.size = int(stat.Size())
  tokens.text = make([]byte, tokens.size)
  tokens.next = 0

  file.Read(tokens.text)

  return true
}

func (tokens *Tokens) Get() string {
  // Strip whitespace
  for tokens.text[tokens.next] == ' ' ||
      tokens.text[tokens.next] == '\r' ||
      tokens.text[tokens.next] == '\n' ||
      tokens.text[tokens.next] == '\t' {
    tokens.next++
    if tokens.next == tokens.size { return "" }
    continue
  }

  switch tokens.text[tokens.next] {
    case ',': tokens.next++; return ","
    case '{': tokens.next++; return "{"
    case '}': tokens.next++; return "}"
    case ';': tokens.next++; return ";"
    case ':': tokens.next++; return ":"
    case '*': tokens.next++; return "*"
    //default:
    //  panic("Error: Unexpected token:" + tokens.text[tokens.next])
  }

  i := tokens.next

  for true {
    if tokens.text[i] >= '0' && tokens.text[i] <= '9' ||
       tokens.text[i] >= 'A' && tokens.text[i] <= 'Z' ||
       tokens.text[i] >= 'a' && tokens.text[i] <= 'z' {
      i++
    } else {
      break
    }
  }

  if i == tokens.next {
    panic("Unexpected token " + string(tokens.text[i]))
  }

  j := tokens.next
  tokens.next = i

  return string(tokens.text[j:i])
}

func main() {
  fmt.Println("Text Tracker - Copyright 2015 by Michael Kohn")

  if len(os.Args) != 2 {
    fmt.Println("Usage: " + os.Args[0] + " <binfile>")
    os.Exit(0)
  }

  tokens := new(Tokens)

  if tokens.Load(os.Args[1]) == false {
    return
  }

  for true {
    token := tokens.Get()
    if token == "" { break }

    fmt.Println("'" + token + "'")
  }
}


