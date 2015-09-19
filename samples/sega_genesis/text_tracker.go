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

type TimeSignature struct {
  beats int
  note_value int
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

func (time_signature *TimeSignature) Init() {
  time_signature.beats = 4
  time_signature.note_value = 4
}

func (tokens *Tokens) Strip() {
  // Strip whitespace
  for tokens.text[tokens.next] == ' ' ||
      tokens.text[tokens.next] == '\r' ||
      tokens.text[tokens.next] == '\n' ||
      tokens.text[tokens.next] == '\t' {
    if tokens.text[tokens.next] == '\n' { tokens.line++ }
    tokens.next++
    if tokens.next == tokens.size { break }
    continue
  }
}

func (tokens *Tokens) Get() string {
  tokens.Strip()
  if tokens.next == tokens.size { return "" }

  // Remove commented lines
  for tokens.text[tokens.next] == '/' && tokens.text[tokens.next + 1] == '/' {
    for tokens.text[tokens.next] != '\n' { tokens.next++ }
    tokens.Strip()
    if tokens.next == tokens.size { return "" }
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
       tokens.text[i] >= 'a' && tokens.text[i] <= 'z' ||
       tokens.text[i] == '#' || tokens.text[i] == '.' ||
       tokens.text[i] == '_' {
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

func ParseTimeSignature(tokens *Tokens, time_signature *TimeSignature) {
  beats := tokens.Get()

  if tokens.Get() != "/" {
    fmt.Print("Error: Missing / on line: %d", tokens.line)
  }

  note_value := tokens.Get()

  if tokens.Get() != ";" {
    fmt.Print("Error: Missing semicolon on line: %d", tokens.line)
  }

  time_signature.beats, _ = strconv.Atoi(beats)
  time_signature.note_value, _ = strconv.Atoi(note_value)
}

func ConvertNote(note string) int {
  var octave int
  note_table := []uint8{ 0, 2, 3, 5, 7, 8, 10 }

  if len(note) != 2 && len(note) != 3 {
    fmt.Println("Error: Illegal note " + note)
    return 0
  }

  note_num := int(note_table[note[0] - 'A'])

  if note[1] == 'b' || note[1] == '#' {
    if note[1] == 'b' {
      note_num--
    } else if note[1] == '#' {
      note_num++
    } else {
      fmt.Print("Error: Unknown modifier " + note)
      return 0
    }
    octave = int(note[2] - '0')
  } else {
    octave = int(note[1] - '0')
  }

  return 21 + (octave * 12) + note_num
}

func ParsePatternVoice(tokens *Tokens, voice []Note, divisions int) bool {
  if tokens.Get() != ":" {
    fmt.Printf("Error: Missing : on line: %d", tokens.line)
    return false
  }

  for true {
    beat_float, _ := strconv.ParseFloat(tokens.Get(), 64)
    note := ConvertNote(tokens.Get())
    length, _ := strconv.Atoi(tokens.Get())
    separator := tokens.Get()

    beat := int(float64(divisions) * beat_float)

    // fmt.Printf("  %d %d %d\n", beat, note, length)

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

func ParsePatternDrums(tokens *Tokens, drums []int, divisions int) bool {
  if tokens.Get() != ":" {
    fmt.Printf("Error: Missing : on line: %d", tokens.line)
    return false
  }

  for true {
    beat_float, _ := strconv.ParseFloat(tokens.Get(), 64)
    drum := tokens.Get()
    separator := tokens.Get()

    beat := int(float64(divisions) * beat_float)

    if drum == "bass" {
      drums[beat] = 36
    } else if drum == "snare" {
      drums[beat] = 38
    } else {
      fmt.Printf("Error: Unexpected token %s on line %d\n", drum, tokens.line)
      return false
    }

    if separator == ";" { break }
    if separator != "," {
      fmt.Printf("Error: Expecting ; or , on line %d\n", tokens.line)
      return false
    }
  }

  return true
}

func ParsePattern(tokens *Tokens, time_signature *TimeSignature, divisions int) (*Pattern, string) {
  pattern := new(Pattern)
  divisions_in_measure := divisions * time_signature.beats
  pattern.Init(divisions_in_measure)
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
      if !ParsePatternVoice(tokens, pattern.voices[voice], divisions) {
        return nil, ""
      }
    } else if token == "drums" {
      if !ParsePatternDrums(tokens, pattern.drums, divisions) {
        return nil, ""
      }
    } else {
      fmt.Println("Error: Unexpected token " + token)
      return nil, ""
    }
  }

  return pattern, name
}

func ParseSong(tokens *Tokens, patterns map[string]*Pattern) (*Song, []string) {
  song := new(Song)
  song_patterns := make([]string, 0)

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
      return nil, nil
    }

    token := tokens.Get()
    if token == "*" {
      token = tokens.Get()
      n, err := strconv.Atoi(token)
      if err != nil {
        fmt.Printf("Error: Not a number on line %d\n", tokens.line)
        return nil, nil
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
      song_patterns = append(song_patterns, name)
    }

    if token == "}" { break }
    if token != "," {
      fmt.Printf("Error: Unexpected token %s on line %d\n", token, tokens.line)
      return nil, nil
    }
  }

  return song, song_patterns
}

func CreateZ80(patterns map[string]*Pattern, song_patterns []string, divisions_in_measure int) {
  var last_division int

  for name, _ := range patterns {
    fmt.Printf("%s:\n  db", name)

    pattern := patterns[name]
    count := 0
    last_division = 0

    for i := 0; i < divisions_in_measure; i++ {
      for voice := 0; voice < 6; voice++ {
        note := pattern.voices[voice][i]
        if note.value != 0 {
          distance := i - last_division
          last_division = i

          if count >= 4 {
            fmt.Printf("\n  db")
            count = 0
          }

          fmt.Printf(" 0x%02x, 0x%02x, 0x%02x,", distance >> 2, voice, note.value)
          count += 1
        }
      }
    }

    // Add an extra event to the end of the pattern to signify the end
    if count >= 4 { fmt.Printf("\n  db") }
    fmt.Printf(" 0x%02x, 0xff, 0xff,",
      (divisions_in_measure - last_division) >> 2)

    fmt.Println()
  }

  fmt.Println("song:")
  for i, _ := range song_patterns {
    if (i % 4) == 0 {
      fmt.Print("\n  dw")
    }
    fmt.Print(" " + song_patterns[i] + ",")
  }
  fmt.Println("\n  dw 0\n")
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

func GetVarInt(time int) []uint8 {
  data := make([]uint8, 0)
  var bit7 uint8 = 0x00

  for true {
    data = append([]uint8{ uint8(time & 0x7f) | bit7 }, data...)
    bit7 = 0x80

    time >>= 7
    if time == 0 { break }
  }

  return data
}

func WriteNote(delay int, channel int, value uint8, is_on bool) []uint8 {
  var command uint8

  track_data := GetVarInt(delay)

  if is_on { command = 0x90 } else { command = 0x80 }
  command |= uint8(channel)

  track_data = append(track_data, []uint8{ command, value, 127 }...)

  return track_data
}

func CreateMidi(song *Song, time_signature *TimeSignature, divisions int, bpm int) {
  var note_value uint8

  file, err := os.Create("out.mid")
  if err != nil { panic(err) }

  track_has_data := make([]bool, 6)
  drums_has_data := false
  tracks := 1

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

  for i := 0; i < len(song.drums); i++ {
    if song.drums[i] != 0 {
      drums_has_data = true
      tracks++
      break
    }
  }

  switch time_signature.note_value {
    case 1: note_value = 0; break
    case 2: note_value = 1; break
    case 4: note_value = 2; break
    case 8: note_value = 4; break
    case 16: note_value = 5; break
    case 32: note_value = 6; break
    default:
      fmt.Println("Bad time signature note value")
      return
  }

  // Write MIDI SMF header
  file.WriteString("MThd")
  WriteInt32(file, 6)         // length (always 6)
  WriteInt16(file, 1)         // 0=single track, 1=multiple track, 2=multiple song
  WriteInt16(file, tracks)    // number of track chunks
  WriteInt16(file, divisions) // divisions

  // Track 0 is just: time signature, (tempo), end of track
  tempo := 60000000 / bpm
  fmt.Printf("tempo=%d\n", tempo)

  track_data := make([]uint8, 0)
  track_data = append(track_data, []uint8{ 0, 0xff, 0x58, 0x04, uint8(time_signature.beats), note_value, uint8(divisions), 8 }...)
  track_data = append(track_data, []uint8{ 0, 0xff, 0x51, 0x03, uint8(tempo >> 16), uint8((tempo >> 8) & 0xff), uint8(tempo & 0xff) }...)
  track_data = append(track_data, []uint8{ 0, 0xff, 0x2f, 0x00 }...)

  file.WriteString("MTrk")
  WriteInt32(file, len(track_data))
  file.Write(track_data)

  // Write music data to tracks
  for voice := 0; voice < 6; voice++ {
    if !track_has_data[voice] { continue }

    track_data := make([]uint8, 0)
    pattern := song.voices[voice]
    last_note := 0

    for i := 0; i < len(pattern); i++ {
      if pattern[i].value != 0 {
        delay := i - last_note
        track_data = append(track_data, WriteNote(delay, 0, pattern[i].value, true)...)
        track_data = append(track_data, WriteNote(delay + int(pattern[i].length), 0, pattern[i].value, false)...)
        last_note = i + int(pattern[i].length)
      }
    }

    track_data = append(track_data, []uint8{ 0, 0xff, 0x2f, 0x00 }...)

    file.WriteString("MTrk")
    WriteInt32(file, len(track_data))
    file.Write(track_data)
  }

  // Write drum data to track
  if drums_has_data {
    track_data := make([]uint8, 0)
    last_note := 0

    for i := 0; i < len(song.drums); i++ {
      if song.drums[i] != 0 {
        delay := i - last_note
        track_data = append(track_data, WriteNote(delay, 9, uint8(song.drums[i]), true)...)
        last_note = i
      }
    }

    track_data = append(track_data, []uint8{ 0, 0xff, 0x2f, 0x00 }...)

    file.WriteString("MTrk")
    WriteInt32(file, len(track_data))
    file.Write(track_data)
  }
}

func main() {
  var patterns map[string]*Pattern
  var song *Song
  var song_patterns []string
  var time_signature TimeSignature
  divisions := 8
  bpm := 60

  time_signature.Init()

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
    } else if token == "time_signature" {
      ParseTimeSignature(tokens, &time_signature)

      if time_signature.beats == 0 || time_signature.note_value == 0 {
        break
      }
    } else if token == "pattern" {
      pattern, name := ParsePattern(tokens, &time_signature, divisions)

      if pattern == nil {
        break
      }

      patterns[name] = pattern
    } else if token == "song" {
      song, song_patterns = ParseSong(tokens, patterns)

      if song == nil {
        break
      }
    } else {
      panic("Unexpected token " + token)
    }
  }

  fmt.Printf(     "Divisions: %d\n", divisions)
  fmt.Printf("           BPM: %d\n", bpm)
  fmt.Printf("Time Signature: %d/%d\n", time_signature.beats, time_signature.note_value)

  CreateMidi(song, &time_signature, divisions, bpm)
  CreateZ80(patterns, song_patterns, divisions * time_signature.beats)
}


