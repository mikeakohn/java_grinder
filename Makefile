
DEBUG=-DDEBUG -g
#OPTIMIZATIONS=-march=nocona -mtune=nocona
CFLAGS=-Wall -O3 $(DEBUG) $(OPTIMIZATIONS) -g
LDFLAGS=

#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default:
	gcc -c fileio.c $(CFLAGS)
	gcc -c java_class.c $(CFLAGS)
	gcc -c java_execute.c $(CFLAGS)
	gcc -c java_thread.c $(CFLAGS)
	gcc -o java_grinder java_grinder.c fileio.o java_class.o \
	    java_execute.o java_thread.o \
	    $(CFLAGS) $(LDFLAGS)

test: $(JOBJS)

%.class: %.java
	javac $*.java

clean:
	@rm -f *.o java_grinder *.class
	@echo "Clean!"

