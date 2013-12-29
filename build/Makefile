CC=gcc
DEBUG=-DDEBUG -g
#OPTIMIZATIONS=-march=nocona -mtune=nocona
CFLAGS=-Wall -O3 $(DEBUG) $(OPTIMIZATIONS)
LDFLAGS=
VPATH=../generator:../common

OBJS=fileio.o java_class.o java_compile.o java_stack.o
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default: $(OBJS)
	gcc -o ../java_grinder ../common/java_grinder.c \
	    $(OBJS) \
	    $(CFLAGS) $(LDFLAGS)

test: $(JOBJS)

%.o: %.c %.h
	$(CC) -c $< -o $*.o $(CFLAGS)

