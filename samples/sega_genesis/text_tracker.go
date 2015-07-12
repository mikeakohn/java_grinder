package main

import "fmt"
import "os"
import "strconv"

// Not sure if anyone is looking at this source file, but if so they'd probably
// ask why I didn't separate this out into smaller modules.  The reason is
// I didn't want the Sega Genesis samples directory littered with Go files,
// there are already enough of them there.  If I ever move these utils to a
// new directory I'll modularize it. 

type Tokens struct {
  text []byte
  size int
  next int
  line int
}

type Note struct {
  value uint8
  length uint8
}

type Pattern struct {
  voices [][]Note
  drums []int
}

type Song struct {
  voices [][]Note
  drums []int
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
  tokens.line = 1

  file.Read(tokens.text)

  return true
}

func (tokens *Tokens) Get() string {
  // Strip whitespace
  for tokens.text[tokens.next] == ' ' ||
      tokens.text[tokens.next] == '\r' ||
      tokens.text[tokens.next] == '\n' ||
      tokens.text[tokens.next] == '\t' {
    if tokens.text[tokens.next] == '\n' { tokens.line++ }
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

func (pattern *Pattern) Init(divisions int) {
  pattern.voices = make([][]Note, 6)

  for i := 0; i < 6; i++ {
    pattern.voices[i] = make([]Note, divisions)
  }

  pattern.drums = make([]int, divisions)
}

func ParseDivisions(tokens *Tokens) int {
  divisions := tokens.Get()

  if tokens.Get() != ";" {
    fmt.Print("Error: Missing semicolon on line: %d", tokens.line)
  }

  n, _ := strconv.Atoi(divisions)

  return n
}

func ParseBpm(tokens *Tokens) int {
  bpm := tokens.Get()

  if tokens.Get() != ";" {
    fmt.Print("Error: Missing semicolon on line: %d", tokens.line)
  }

  n, _ := strconv.Atoi(bpm)

  return n
}

func ConvertNote(note string) int {
  note_table := []uint8{ 0, 2, 3, 5, 7, 8, 10 }

  if len(note) != 2 && len(note) != 3 {
    fmt.Println("Error: Illegal note " + note)
    return 0
  }

  note_num := int(note_table[note[0] - 'A'])
  octave := int(note[1] - '0')

  if len(note) > 2 {
    if note[2] == '#' {
      note_num++
    } else if note[2] == 'b' {
      note_num--
    } else {
      fmt.Print("Error: Unknown modifier " + note)
      return 0
    }
  }

  return 21 + (octave * 12) + note_num
}

func ParsePatternVoice(tokens *Tokens, voice []Note) bool {
  if tokens.Get() != ":" {
    fmt.Printf("Error: Missing : on line: %d", tokens.line)
    return false
  }

  for true {
    beat, _ := strconv.Atoi(tokens.Get())
    note := ConvertNote(tokens.Get())
    length, _ := strconv.Atoi(tokens.Get())
    separator := tokens.Get()

    if note == 0 || length == 0 {
      fmt.Printf("Error: Problem on line %d\n", tokens.line)
      return false
    }

    voice[beat].value = uint8(note)
    voice[beat].length = uint8(length)

    if separator == ";" { break }
    if separator != "," {
      fmt.Printf("Error: Expecting ; or , on line %d\n", tokens.line)
      return false
    }
  }

  return true
}

func ParsePattern(tokens *Tokens, divisions int) (*Pattern,string) {
  pattern := new(Pattern)
  pattern.Init(divisions)
  name := tokens.Get()

  if divisions == 0 {
    fmt.Printf("Error: No divisions set on line: %d", tokens.line)
    return nil, ""
  }

  if tokens.Get() != "{" {
    fmt.Printf("Error: Missing { on line: %d", tokens.line)
    return nil, ""
  }

  for true {
    token := tokens.Get()
    if token == "}" { break }
    if token == "" {
      fmt.Println("Error: Unexpected end of file")
      return nil, ""
    }
    if token == "voice" {
      token = tokens.Get()
      voice, err := strconv.Atoi(token)
      if err != nil {
        fmt.Println("Error: Unexpected token " + token)
        return nil, ""
      }
      if !ParsePatternVoice(tokens, pattern.voices[voice]) { return nil, "" }
    } else {
      fmt.Println("Error: Unexpected token " + token)
      return nil, ""
    }
  }

  return pattern, name
}

func ParseSong(tokens *Tokens, patterns map[string]*Pattern) *Song {
  song := new(Song)

  song.voices = make([][]Note, 6)

  if tokens.Get() != "{" {
    fmt.Print("Error: Missing { on line: %d", tokens.line)
  }

  for true {
    name := tokens.Get()
    multiplyer := 1

    if name == "}" { break }

    pattern := patterns[name]

    if pattern == nil {
      fmt.Println("Error: Unknown pattern name " + name)
      return nil
    }

    token := tokens.Get()
    if token == "*" {
      token = tokens.Get()
      n, err := strconv.Atoi(token)
      if err != nil {
        fmt.Printf("Error: Not a number on line %d\n", tokens.line)
        return nil
      }
      multiplyer = n
      token = tokens.Get()
    }

    fmt.Printf("Pattern %s %d times\n", name, multiplyer)
    for i := 0; i < multiplyer; i++ {
      for voice := 0; voice < 6; voice++ {
        song.voices[voice] = append(song.voices[voice], pattern.voices[voice]...)
      }

      song.drums = append(song.drums, pattern.drums...)
    }

    if token == "}" { break }
    if token != "," {
      fmt.Printf("Error: Unexpected token %s on line %d\n", token, tokens.line)
      return nil
    }
  }

  return song
}

func WriteInt32(file *os.File, value int) {
  data := make([]uint8, 4)
  data[0] = uint8((value >> 24) & 0xff)
  data[1] = uint8((value >> 16) & 0xff)
  data[2] = uint8((value >> 8) & 0xff)
  data[3] = uint8(value & 0xff)
  file.Write(data)
}

func WriteInt16(file *os.File, value int) {
  data := make([]uint8, 2)
  data[0] = uint8((value >> 8) & 0xff)
  data[1] = uint8(value & 0xff)
  file.Write(data)
}

func WriteVarInt(time int) []uint8 {
  data := make([]uint8, 0)

  for true {
    if (time & 0x7f) == 0 {
      data = append([]uint8{ uint8(time & 0x7f) }, data...)
      break
    } else {
      data = append([]uint8{ uint8((time & 0x7f) | 0x80) }, data...)
      time >>= 7
    }
  }

  return data
}

func WriteNote(delay int, channel int, value uint8, is_on bool) []uint8 {
  var command uint8

  track_data := WriteVarInt(delay)

  if is_on { command = 0x90 } else { command = 0x80 }
  command |= uint8(channel)

  track_data = append(track_data, []uint8{ command, value, 127 }...)

  return track_data
}

func CreateMidi(song *Song, divisions int, bpm int) {
  file, err := os.Create("out.mid")
  if err != nil { panic(err) }

  track_has_data := make([]bool,6)
  tracks := 0

  for voice := 0; voice < 6; voice++ {
    pattern := song.voices[voice]
    for i := 0; i < len(pattern); i++ {
      if pattern[i].value != 0 {
        tracks++
        track_has_data[voice] = true
        break
      }
    }
  }

  file.WriteString("MThd")
  WriteInt32(file, 6)         // length (always 6)
  WriteInt16(file, 1)         // 0=single track, 1=multiple track, 2=multiple song
  WriteInt16(file, tracks)    // number of track chunks
  WriteInt16(file, divisions) // divisions

  for voice := 0; voice < 6; voice++ {
    track_data := make([]uint8, 0)
    pattern := song.voices[voice]
    last_note := 0

    for i := 0; i < len(pattern); i++ {
      if pattern[i].value != 0 {
        delay := i - last_note
        track_data = append(track_data, WriteNote(delay, voice, pattern[i].value, true)...)
        track_data = append(track_data, WriteNote(delay + int(pattern[i].length), voice, pattern[i].value, false)...)
        last_note = i + int(pattern[i].length)
      }
    }

    file.WriteString("MTrk")
    WriteInt32(file, len(track_data))
    file.Write(track_data)
  }
}

func main() {
  var patterns map[string]*Pattern
  var song *Song
  divisions := 8
  bpm := 60

  fmt.Println("Text Tracker - Copyright 2015 by Michael Kohn")

  if len(os.Args) != 2 {
    fmt.Println("Usage: " + os.Args[0] + " <binfile>")
    os.Exit(0)
  }

  tokens := new(Tokens)

  if tokens.Load(os.Args[1]) == false {
    return
  }

  patterns = make(map[string]*Pattern)

  for true {
    token := tokens.Get()
    if token == "" { break }

    if token == "divisions" {
      divisions = ParseDivisions(tokens)

      if divisions == 0 {
        break
      }
    } else if token == "bpm" {
      bpm = ParseBpm(tokens)

      if bpm == 0 {
        break
      }
    } else if token == "pattern" {
      pattern, name := ParsePattern(tokens, divisions)

      if pattern == nil {
        break
      }

      patterns[name] = pattern
    } else if token == "song" {
      song = ParseSong(tokens, patterns)

      if song == nil {
        break
      }
    } else {
      panic("Unexpected token " + token)
    }

    //fmt.Println("'" + token + "'")
  }

  fmt.Printf("Divisions: %d\n", divisions)
  fmt.Printf("      BMP: %d\n", bpm)

  CreateMidi(song, divisions, bpm)
}


