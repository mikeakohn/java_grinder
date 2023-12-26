
defauit: LedBlink.class

grind: LedBlink.class
	../../java_grinder -v LedBlink.class led_blink.asm f100_l
	../../../naken_asm/naken_asm -l -type bin -o led_blink.bin led_blink.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst *.bin

