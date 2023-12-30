
default: LedBlink.class LCD.class

grind: LedBlink.class
	../../java_grinder -v LedBlink.class led_blink.asm riscv_icefun
	../../../naken_asm/naken_asm -l -type bin -o led_blink.bin led_blink.asm
	../../java_grinder -v LCD.class lcd.asm riscv_icefun
	../../../naken_asm/naken_asm -l -type bin -o lcd.bin lcd.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst *.bin

