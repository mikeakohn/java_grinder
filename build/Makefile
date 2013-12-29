CC=gcc
DEBUG=-DDEBUG -g
#OPTIMIZATIONS=-march=nocona -mtune=nocona
INCLUDES=-I../common -I../generator
CFLAGS=-Wall -O3 $(DEBUG) $(INCLUDES) $(OPTIMIZATIONS)
LDFLAGS=
VPATH=../generator:../common

CPUS=dspic.o msp430.o
OBJS=fileio.o generator.o java_class.o java_compile.o java_stack.o $(CPUS)
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default: $(OBJS)
	gcc -o ../java_grinder ../common/java_grinder.c \
	    $(OBJS) \
	    $(CFLAGS) $(LDFLAGS)

test: $(JOBJS)

%.o: %.c %.h
	$(CC) -c $< -o $*.o $(CFLAGS)

