
default: LedBlink.class Arrays.class

grind: LedBlink.class
	../../java_grinder -v LedBlink.class led_blink.asm f100_l
	../../../naken_asm/naken_asm -l -type bin -o led_blink.bin led_blink.asm
	../../java_grinder -v Arrays.class arrays.asm f100_l
	../../../naken_asm/naken_asm -l -type bin -o arrays.bin arrays.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst *.bin

