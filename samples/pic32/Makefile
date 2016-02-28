
defauit: LedBlink.class

grind: LedBlink.class
	../../java_grinder -v LedBlink.class led_blink.asm pic32
	naken_asm -l -I ../../../naken_asm/include -o led_blink.hex led_blink.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

prog:
	pic32prog -d /dev/ttyUSB0 led_blink.hex

clean:
	@rm -f *.class *.hex *.asm *.lst

