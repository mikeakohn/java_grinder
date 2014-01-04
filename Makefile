
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default:
	@cd build && make

tests:
	@cd testing && make

java:
	@cd java && make
	@cd build && make jar

%.class: %.java
	javac $*.java

clean:
	@rm -f *.o java_grinder build/*.o *.asm
	@rm -f java/*.class testing/*.class build/*.jar
	@rm -rf build/net
	@echo "Clean!"



