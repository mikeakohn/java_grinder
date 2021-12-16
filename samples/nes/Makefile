
INCLUDE=../../../naken_asm/include/6502

default: NesJavaDemo.class

grind: NesJavaDemo.class
	../../java_grinder NesJavaDemo.class nes_java_demo.asm nes
	naken_asm -I $(INCLUDE) -l -type bin -o nes_java_demo.nes nes_java_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.lst *.class nes_java_demo.asm *.nes
	@echo "Clean!"

