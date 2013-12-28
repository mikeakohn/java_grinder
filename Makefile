
CC=gcc
DEBUG=-DDEBUG -g
#OPTIMIZATIONS=-march=nocona -mtune=nocona
CFLAGS=-Wall -O3 $(DEBUG) $(OPTIMIZATIONS)
LDFLAGS=

OBJS=fileio.o java_class.o java_compile.o java_stack.o
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default: $(OBJS)
	gcc -o java_grinder java_grinder.c \
	    $(OBJS) \
	    $(CFLAGS) $(LDFLAGS)

test: $(JOBJS)

%.o: %.c %.h
	$(CC) -c $< -o $*.o $(CFLAGS)

%.class: %.java
	javac $*.java

clean:
	@rm -f *.o java_grinder *.class
	@echo "Clean!"



