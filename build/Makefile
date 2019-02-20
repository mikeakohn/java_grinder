CC?=gcc
CXX?=g++
DEBUG=-DDEBUG -g
INCLUDES=-I..
CFLAGS=-Wall -O3 -std=c++11 $(DEBUG) $(INCLUDES)
#CFLAGS=-Wall $(DEBUG) $(INCLUDES)
LDFLAGS=
VPATH=../api:../common:../generator:..

API= \
  api/adc.o \
  api/amiga.o \
  api/appleiigs.o \
  api/atari_2600.o \
  api/c64_sid.o \
  api/c64_vic.o \
  api/cpc.o \
  api/cpu.o \
  api/dsp.o \
  api/draw3d_object.o \
  api/draw3d_texture.o \
  api/invoke.o \
  api/invoke_static.o \
  api/invoke_virtual.o \
  api/ioport.o \
  api/java_lang_system.o \
  api/java_lang_string.o \
  api/math.o \
  api/memory.o \
  api/msx.o \
  api/parallella.o \
  api/playstation_2.o \
  api/propeller.o \
  api/sega_genesis.o \
  api/snes.o \
  api/spi.o \
  api/sxb.o \
  api/ti84.o \
  api/ti99.o \
  api/timer.o \
  api/trs80_coco.o \
  api/uart.o \
  api/watchdog.o \
  generator/Math.o

CPUS= \
  generator/ARM.o \
  generator/AVR8.o \
  generator/DotNet.o \
  generator/DSPIC.o \
  generator/Epiphany.o \
  generator/M6502.o \
  generator/M6502_8.o \
  generator/MC68000.o \
  generator/MC68020.o \
  generator/MC6809.o \
  generator/MCS51.o \
  generator/MIPS32.o \
  generator/MSP430.o \
  generator/MSP430X.o \
  generator/Propeller.o \
  generator/R5900.o \
  generator/STDC.o \
  generator/TMS9900.o \
  generator/W65816.o \
  generator/WebAssembly.o \
  generator/X86.o \
  generator/X86_64.o \
  generator/Z80.o

SYSTEMS= \
  generator/Amiga.o \
  generator/AppleIIgs.o \
  generator/Atari2600.o \
  generator/C64.o \
  generator/CPC.o \
  generator/MSX.o \
  generator/Playstation2.o \
  generator/Playstation2VU0.o \
  generator/Playstation2VU1.o \
  generator/PIC32.o \
  generator/SegaGenesis.o \
  generator/SNES.o \
  generator/TI84.o \
  generator/TI99.o \
  generator/TRS80Coco.o \
  generator/W65C134SXB.o \
  generator/W65C265SXB.o

OBJS= \
  common/fileio.o \
  common/Compiler.o \
  common/JavaClass.o \
  common/JavaCompiler.o \
  common/Util.o \
  common/execute_static.o \
  common/table_java_instr.o \
  generator/Generator.o \
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

