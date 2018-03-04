INCLUDE_PATH=../../../naken_asm/include/playstation2

default: PS2JavaDemo.class

grind: PS2JavaDemo.class
	../../java_grinder -v PS2JavaDemo.class ps2_java_demo.asm playstation2
	naken_asm -I $(INCLUDE_PATH) -l -e -o ps2_java_demo.elf ps2_java_demo.asm

debug:
	naken_asm -I $(INCLUDE_PATH) -l -e -dump_symbols -o ps2_java_demo.elf ps2_java_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class ps2_java_demo.asm *.elf *.bin
	@echo "Clean!"

