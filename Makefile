
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default:
	@cd build && make
	@cd java && make java_grinder.jar

tests:
	@cd testing && make

jar:
	@cd java && make JavaGrinder.jar

%.class: %.java
	javac $*.java

clean:
	@rm -f *.o java_grinder build/*.o *.asm
	@rm -f java/*.class testing/*.class *.jar *.class *.jar
	@echo "Clean!"



