CC=gcc
CXX=g++
DEBUG=-DDEBUG -g
#OPTIMIZATIONS=-march=nocona -mtune=nocona
INCLUDES=-I../common -I../generator
CFLAGS=-Wall -O3 $(DEBUG) $(INCLUDES) $(OPTIMIZATIONS)
LDFLAGS=
VPATH=../generator:../common

CPUS=DSPIC.o MSP430.o
OBJS=fileio.o Generator.o JavaClass.o java_compile.o JavaStack.o table_java_instr.o $(CPUS)

default: $(OBJS)
	gcc -o ../java_grinder ../common/java_grinder.c \
	    $(OBJS) \
	    $(CFLAGS) $(LDFLAGS)

test: $(JOBJS)

%.o: %.cxx %.h
	$(CXX) -c $< -o $*.o $(CFLAGS)


