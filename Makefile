
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

grind:
	./java_grinder testing/MethodCall.class out.asm msp430g2231
	naken_asm -I /storage/git/naken_asm/include/msp430_old -l out.asm

clean:
	@rm -f *.o java_grinder build/*.o *.asm *.lst *.hex
	@rm -f java/*.class testing/*.class build/*.jar
	@rm -rf build/net
	@echo "Clean!"



