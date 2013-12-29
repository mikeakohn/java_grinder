
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default:
	@cd build && make
	@cd java && make

test:
	@cd testing && make

%.class: %.java
	javac $*.java

clean:
	@rm -f *.o java_grinder build/*.o *.asm
	@rm -f java/*.class testing/*.class *.jar *.class
	@echo "Clean!"



