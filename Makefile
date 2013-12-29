
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default:
	@cd build && make

test: $(JOBJS)

%.class: %.java
	javac $*.java

clean:
	@rm -f *.o java_grinder testing/*.class *.class build/*.o *.asm
	@echo "Clean!"



