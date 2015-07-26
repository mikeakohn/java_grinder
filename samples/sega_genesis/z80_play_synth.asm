.z80

FM1_REG_SEL equ 0x4000
FM1_REG_DATA equ 0x4001
FM2_REG_SEL equ 0x4002
FM2_REG_DATA equ 0x4003

KEY_PRESS equ 0x28
CH1_OP1 equ 0x30
CH2_OP1 equ 0x31
CH3_OP1 equ 0x32
CH1_OP2 equ 0x34
CH2_OP2 equ 0x35
CH3_OP2 equ 0x36
CH1_OP3 equ 0x38
CH2_OP3 equ 0x39
CH3_OP3 equ 0x3a
CH1_OP4 equ 0x3c
CH2_OP4 equ 0x3d
CH3_OP4 equ 0x3e
CH1_FREQ_LSB equ 0xa0
CH1_FREQ_MSB equ 0xa4
CH1_SOUND_ON equ 0xb4

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

  ;; Channel 1
  ld a, CH1_SOUND_ON
  ld (FM1_REG_SEL), a
  ld a, 0xc0
  ld (FM1_REG_DATA), a

  ld a, CH1_FREQ_MSB
  ld (FM1_REG_SEL), a
  ld a, 0x22
  ld (FM1_REG_DATA), a

  ld a, CH1_FREQ_LSB
  ld (FM1_REG_SEL), a
  ld a, 0x69
  ld (FM1_REG_DATA), a

  ;; Channel 2
  ld a, CH1_SOUND_ON + 1
  ld (FM1_REG_SEL), a
  ld a, 0xc0
  ld (FM1_REG_DATA), a

  ld a, CH1_FREQ_MSB + 1
  ld (FM1_REG_SEL), a
  ld a, 0x23
  ld (FM1_REG_DATA), a

  ld a, CH1_FREQ_LSB + 1
  ld (FM1_REG_SEL), a
  ld a, 0x09
  ld (FM1_REG_DATA), a

  ;; Channel 4
  ld a, CH1_SOUND_ON
  ld (FM2_REG_SEL), a
  ld a, 0xc0
  ld (FM2_REG_DATA), a

  ld a, CH1_FREQ_MSB
  ld (FM2_REG_SEL), a
  ld a, 0x23
  ld (FM2_REG_DATA), a

  ld a, CH1_FREQ_LSB
  ld (FM2_REG_SEL), a
  ld a, 0x9c
  ld (FM2_REG_DATA), a

  ;; Play notes
  ld a, KEY_PRESS
  ld (FM1_REG_SEL), a
  ld a, 0xf0
  ld (FM1_REG_DATA), a

  ld a, KEY_PRESS
  ld (FM1_REG_SEL), a
  ld a, 0xf1
  ld (FM1_REG_DATA), a

  ld a, KEY_PRESS
  ld (FM1_REG_SEL), a
  ld a, 0xf4
  ld (FM1_REG_DATA), a

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



