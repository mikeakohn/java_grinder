.z80

PSG_DATA equ 0x7f11

start:
  ld de, 0xffff

  ;; Set volume full blast for all 3 channels
  ld a, 0x80 | (0 << 5) | (1 << 4)
  ld (PSG_DATA), a
  ld a, 0x80 | (1 << 5) | (1 << 4)
  ld (PSG_DATA), a
  ld a, 0x80 | (2 << 5) | (1 << 4)
  ld (PSG_DATA), a

play_loop:
  ;; Channel 0 G2
  ld a, (tones+0)
  or 0x80 | (0 << 5)
  ld (PSG_DATA), a
  ld a, (tones+1)
  ld (PSG_DATA), a

  ;; Channel 1 B2
  ld a, (tones+2)
  or 0x80 | (1 << 5)
  ld (PSG_DATA), a
  ld a, (tones+3)
  ld (PSG_DATA), a

  ;; Channel 2 E3
  ld a, (tones+4)
  or 0x80 | (2 << 5)
  ld (PSG_DATA), a
  ld a, (tones+5)
  ld (PSG_DATA), a

  ;; DELAY
  ld hl, 100
play_loop_delay_outer:
  ld b, 255
play_loop_delay:
  djnz play_loop_delay
  add hl, de
  ld a, l
  cp 0
  jr nz, play_loop_delay_outer
  ld a, h
  cp 0
  jr nz, play_loop_delay_outer

  ld hl, (tones)
  add hl, de
  ld (tones), hl

  ld hl, (tones+2)
  add hl, de
  ld (tones+2), hl

  ld hl, (tones+4)
  add hl, de
  ld (tones+4), hl

  ld hl, (count)
  add hl, de
  ld (count), hl

  ld a, l
  cp 0
  jr nz, play_loop
  ld a, h
  cp 0
  jr nz, play_loop

  ;; Set volume to 0 for all 3 channels
  ld a, 0x8f | (0 << 5) | (1 << 4)
  ld (PSG_DATA), a
  ld a, 0x8f | (1 << 5) | (1 << 4)
  ld (PSG_DATA), a
  ld a, 0x8f | (2 << 5) | (1 << 4)
  ld (PSG_DATA), a

while_1:
  jp while_1

tones:
  db 0x0e, 0x13,
  db 0x0d, 0x0f,
  db 0x0d, 0x0b,
count:
  dw 27
delay:
  dw 0
marker:
  dw 0xadde


