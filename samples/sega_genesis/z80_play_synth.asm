.z80

FM1_REG_SEL equ 0x4000
FM1_REG_DATA equ 0x4001
FM2_REG_SEL equ 0x4002
FM2_REG_DATA equ 0x4003

KEY_PRESS equ 0x28
CH1_FREQ_LSB equ 0xa0
CH1_FREQ_MSB equ 0xa4
CH1_SOUND_ON equ 0xb4

PSG_DATA equ 0x7f11

start:
  ld sp, 0x2000

  ld ix, setup
  ld c, 0x30
  ld b, setup_end - setup

load_setup:
  call init_channel
  call init_channel
  call init_channel
  inc c
  inc ix
  djnz load_setup

  ld a, CH1_SOUND_ON
  ld (FM1_REG_SEL), a
  ld a, 0xc0
  ld (FM1_REG_DATA), a

  ld a, CH1_SOUND_ON+1
  ld (FM1_REG_SEL), a
  ld a, 0xc0
  ld (FM1_REG_DATA), a

  ld a, CH1_SOUND_ON+2
  ld (FM1_REG_SEL), a
  ld a, 0xc0
  ld (FM1_REG_DATA), a

  ld a, CH1_SOUND_ON
  ld (FM2_REG_SEL), a
  ld a, 0xc0
  ld (FM2_REG_DATA), a

  ld a, CH1_SOUND_ON+1
  ld (FM2_REG_SEL), a
  ld a, 0xc0
  ld (FM2_REG_DATA), a

  ld a, CH1_SOUND_ON+2
  ld (FM2_REG_SEL), a
  ld a, 0xc0
  ld (FM2_REG_DATA), a

.if 0
  ld a, CH1_FREQ_MSB
  ld (FM1_REG_SEL), a
  ld a, 0x22
  ld (FM1_REG_DATA), a

  ld a, CH1_FREQ_LSB
  ld (FM1_REG_SEL), a
  ld a, 0x69
  ld (FM1_REG_DATA), a

  ld a, KEY_PRESS
  ld (FM1_REG_SEL), a
  ld a, 0xf0
  ld (FM1_REG_DATA), a
.endif

  ;; Turn volume off for all voices of PSG
  ld a, 0x8f | (0 << 5) | (1 << 4)
  ld (PSG_DATA), a
  ld a, 0x8f | (1 << 5) | (1 << 4)
  ld (PSG_DATA), a
  ld a, 0x8f | (2 << 5) | (1 << 4)
  ld (PSG_DATA), a
  ld a, 0x8f | (3 << 5) | (1 << 4)
  ld (PSG_DATA), a

  ;; Done setting up, now play song
  ld d, 0

  ld iy, song
next_pattern:
  ;; hl = next pattern from list
  ld l, (iy)
  ld h, (iy+1)
  ld a, h
  ;; if pattern == 0, break
  or l
  jp z, song_end

play_pattern:
  ;; read in 3 bytes (time=a, channel=a, note=c) from &hl
  ld a, (hl)
  inc hl

  ;; delay 'a' times
play_pattern_delay:
  cp 0
  jp z, play_pattern_delay_end
  ld d, 50                 ; this can control tempo
delay_loop_outer:
  ld b, 200
delay_loop:
  djnz delay_loop          ; should be 13 cycles per loop
  dec d
  jp nz, delay_loop_outer
  dec a
  jp play_pattern_delay
play_pattern_delay_end:

  ld a, (hl)
  inc hl
  ;; if a(channel) == 0xff, break
  cp 0xff
  jp z, end_pattern
  ld b, a

  ;; Convert MIDI frequency to Yahama constant
  ld e, (hl)
  inc hl
  ld ix, frequency
  add ix, de 
  add ix, de

  ;; Call correct code for channels 0-2 or 3-5
  cp 0x03
  jp nc, channels345

channels012:
  ld c, a

  ld a, KEY_PRESS
  ld (FM1_REG_SEL), a
  ld a, 0x00
  add a, b
  ld (FM1_REG_DATA), a

  ld a, CH1_FREQ_MSB
  add a, c
  ld (FM1_REG_SEL), a
  ld a, (ix+1)
  ld (FM1_REG_DATA), a

  ld a, CH1_FREQ_LSB
  add a, c
  ld (FM1_REG_SEL), a
  ld a, (ix)
  ld (FM1_REG_DATA), a

  ld a, KEY_PRESS
  ld (FM1_REG_SEL), a
  ld a, 0xf0
  add a, b
  ld (FM1_REG_DATA), a

  jp play_pattern

channels345:
  cp 0x07
  jp z, channels7

  sub 3
  ld c, a

  ld a, KEY_PRESS
  ld (FM1_REG_SEL), a
  ld a, 0x00
  add a, b
  ld (FM1_REG_DATA), a

  ld a, CH1_FREQ_MSB
  add a, c
  ld (FM2_REG_SEL), a
  ld a, (ix+1)
  ld (FM2_REG_DATA), a

  ld a, CH1_FREQ_LSB
  add a, c
  ld (FM2_REG_SEL), a
  ld a, (ix)
  ld (FM2_REG_DATA), a

  ld a, KEY_PRESS
  ld (FM1_REG_SEL), a
  ld a, 0xf0
  add a, b
  ld (FM1_REG_DATA), a

  jp play_pattern

channels7:
  ld a, e

  cp 0x00
  jp nz, psg_not_off

  ;; Turn PSG noise voice off
  ld a, 0xff
  ld (PSG_DATA), a
  jp play_pattern

psg_not_off:
  ;; Turn PSG noise voice on
  ld a, 0xf0
  ld (PSG_DATA), a

  ;; 38 = bass, 36=snare
  cp 36
  jp z, psg_snare_drum

psg_bass_drum:
  ld a, 0xe4
  ld (PSG_DATA), a
  jp play_pattern

psg_snare_drum:
  ld a, 0xe7
  ld (PSG_DATA), a
  jp play_pattern

end_pattern:
  inc iy
  inc iy
  jp next_pattern

song_end:

while_1:
  jp while_1

init_channel:
  ld a, c
  ld (FM1_REG_SEL), a
  ld a, (ix)
  ld (FM1_REG_DATA), a
  ld a, c
  ld (FM2_REG_SEL), a
  ld a, (ix)
  ld (FM2_REG_DATA), a
  inc c
  ret

setup:
  db 0x71, 0x0d, 0x33, 0x01
  db 0x23, 0x2d, 0x26, 0x00
  db 0x5f, 0x99, 0x5f, 0x94
  db 0x05, 0x05, 0x05, 0x07
  db 0x02, 0x02, 0x02, 0x02
  db 0x11, 0x11, 0x11, 0xa6
  db 0x00, 0x00, 0x00, 0x00
setup_end:

frequency:
  dw  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000, ; -3
  dw  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,
  dw  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000, ; -2
  dw  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,
  dw  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000, ; -1
  dw  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,  0x0000,
  dw  0x0269,  0x028d,  0x02b4,  0x02dd,  0x0309,  0x0337, ; 0
  dw  0x0368,  0x039c,  0x03d3,  0x040d,  0x044b,  0x048c,
  dw  0x0a69,  0x0a8d,  0x0ab4,  0x0add,  0x0b09,  0x0b37, ; 1
  dw  0x0b68,  0x0b9c,  0x0bd3,  0x0c0d,  0x0c4b,  0x0c8c,
  dw  0x1269,  0x128d,  0x12b4,  0x12dd,  0x1309,  0x1337, ; 2
  dw  0x1368,  0x139c,  0x13d3,  0x140d,  0x144b,  0x148c,
  dw  0x1a69,  0x1a8d,  0x1ab4,  0x1add,  0x1b09,  0x1b37, ; 3
  dw  0x1b68,  0x1b9c,  0x1bd3,  0x1c0d,  0x1c4b,  0x1c8c,
  dw  0x2269,  0x228d,  0x22b4,  0x22dd,  0x2309,  0x2337, ; 4
  dw  0x2368,  0x239c,  0x23d3,  0x240d,  0x244b,  0x248c,
  dw  0x2a69,  0x2a8d,  0x2ab4,  0x2add,  0x2b09,  0x2b37, ; 5
  dw  0x2b68,  0x2b9c,  0x2bd3,  0x2c0d,  0x2c4b,  0x2c8c,
  dw  0x3269,  0x328d,  0x32b4,  0x32dd,  0x3309,  0x3337, ; 6
  dw  0x3368,  0x339c,  0x33d3,  0x340d,  0x344b,  0x348c,
  dw  0x3a69,  0x3a8d,  0x3ab4,  0x3add,  0x3b09,  0x3b37, ; 7
  dw  0x3b68,  0x3b9c,  0x3bd3,  0x3c0d,  0x3c4b,  0x3c8c,
  dw  0x4269,  0x428d,  0x42b4,  0x42dd,  0x4309,  0x4337, ; 8
  dw  0x4368,  0x439c,  0x43d3,  0x440d,  0x444b,  0x448c,

intro:
  db 0x00, 0x00, 0x59, 0x00, 0x01, 0x51, 0x00, 0x02, 0x54, 0x30, 0x00, 0x59,
  db 0x00, 0x01, 0x51, 0x00, 0x02, 0x56, 0x30, 0x00, 0x59, 0x00, 0x01, 0x51,
  db 0x00, 0x02, 0x58, 0x30, 0x00, 0x59, 0x00, 0x01, 0x50, 0x00, 0x02, 0x59,
  db 0x30, 0xff, 0xff,
pattern_1:
  db 0x00, 0x00, 0x51, 0x00, 0x01, 0x54, 0x00, 0x02, 0x58, 0x60, 0x00, 0x54,
  db 0x00, 0x01, 0x58, 0x00, 0x02, 0x5b, 0x60, 0xff, 0xff,
pattern_2:
  db 0x00, 0x00, 0x5d, 0x00, 0x01, 0x60, 0x00, 0x02, 0x64, 0xc0, 0xff, 0xff,

song:
  dw intro, intro, intro, intro,
  dw pattern_1, pattern_1, pattern_1, pattern_1,
  dw pattern_2, pattern_2, pattern_2, pattern_2,
  dw 0


