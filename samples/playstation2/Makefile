INCLUDE_PATH=../../../naken_asm/include/playstation2

default: PS2JavaDemo.class SimpleTest.class

grind: PS2JavaDemo.class
	../../java_grinder -v PS2JavaDemo.class ps2_java_demo.asm playstation2
	../../../naken_asm/naken_asm -I $(INCLUDE_PATH) -l -type elf -o ps2_java_demo.elf ps2_java_demo.asm

simple: SimpleTest.class
	../../java_grinder -v SimpleTest.class simple_test.asm playstation2
	../../../naken_asm/naken_asm -I $(INCLUDE_PATH) -l -type elf -o simple_test.elf simple_test.asm

debug:
	../../../naken_asm/naken_asm -I $(INCLUDE_PATH) -l -type elf -dump_symbols -o ps2_java_demo.elf ps2_java_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

vu0: bin2array
	../../../naken_asm/naken_asm -l -b -o test_vu0.bin test_vu0.asm
	./bin2array test_vu0.bin

bin2array: bin2array.rs
	rustc bin2array.rs

clean:
	@rm -f *.hex *.lst *.class ps2_java_demo.asm *.elf *.bin bin2array simple_test.asm
	@echo "Clean!"

