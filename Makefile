
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default:
	@+make -C build

tests:
	@+make -C testing

.PHONY: java
java:
	@+make -C java
	@make -C build jar

%.class: %.java
	javac $*.java

clean:
	@rm -f *.o java_grinder build/*.o *.asm
	@rm -f java/*.class testing/*.class build/*.jar
	@rm -rf build/net
	@echo "Clean!"



