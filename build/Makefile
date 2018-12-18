CC?=gcc
CXX?=g++
DEBUG=-DDEBUG -g
INCLUDES=-I../common -I../generator -I../api
CFLAGS=-Wall -O3 $(DEBUG) $(INCLUDES)
#CFLAGS=-Wall $(DEBUG) $(INCLUDES)
LDFLAGS=
VPATH=../generator:../common:../api

API= \
  adc.o \
  appleiigs_.o \
  atari_2600.o \
  c64_sid.o \
  c64_vic.o \
  cpc_.o \
  cpu.o \
  dsp.o \
  draw3d_object.o \
  draw3d_texture.o \
  invoke.o \
  invoke_static.o \
  invoke_virtual.o \
  ioport.o \
  java_lang_system.o \
  java_lang_string.o \
  math_.o \
  memory.o \
  msx_.o \
  parallella.o \
  playstation_2.o \
  propeller_.o \
  sega_genesis.o \
  snes_.o \
  spi.o \
  sxb.o \
  ti84_.o \
  ti99_.o \
  timer.o \
  trs80_coco.o \
  uart.o \
  watchdog.o \
  Math.o

CPUS= \
  ARM.o \
  AVR8.o \
  DSPIC.o \
  Epiphany.o \
  M6502.o \
  M6502_8.o \
  MC68000.o \
  MC68020.o \
  MC6809.o \
  MCS51.o \
  MIPS32.o \
  MSP430.o \
  MSP430X.o \
  Propeller.o \
  R5900.o \
  STDC.o \
  TMS9900.o \
  W65816.o \
  X86.o \
  X86_64.o \
  Z80.o

SYSTEMS= \
  AppleIIgs.o \
  Atari2600.o \
  C64.o \
  CPC.o \
  MSX.o \
  Playstation2.o \
  Playstation2VU0.o \
  Playstation2VU1.o \
  PIC32.o \
  SegaGenesis.o \
  SNES.o \
  TI84.o \
  TI99.o \
  TRS80Coco.o \
  W65C134SXB.o \
  W65C265SXB.o

OBJS= \
  fileio.o \
  Compiler.o \
  Generator.o \
  JavaClass.o \
  JavaCompiler.o \
  Util.o \
  execute_static.o \
  table_java_instr.o \
  $(CPUS) $(SYSTEMS) $(API)

default: $(OBJS)
	$(CXX) -o ../java_grinder ../common/java_grinder.cxx \
	    $(OBJS) \
	    $(CFLAGS) $(LDFLAGS)

test: $(JOBJS)

%.o: %.cxx %.h
	$(CXX) -c $< -o $*.o \
	   $(CFLAGS)

jar:
	jar cvfm JavaGrinder.jar Manifest.txt net

