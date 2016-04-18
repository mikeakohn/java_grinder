
defauit: LedBlink.class

grind: LedBlink.class
	../../java_grinder -v LedBlink.class led_blink.asm pic32
	naken_asm -l -I ../../../naken_asm/include -o led_blink.hex led_blink.asm

display: Display.class
	../../java_grinder -v Display.class display.asm pic32
	naken_asm -l -I ../../../naken_asm/include -o display.hex display.asm
	pic32prog -d /dev/ttyUSB0 display.hex

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

prog:
	pic32prog -d /dev/ttyUSB0 led_blink.hex

clean:
	@rm -f *.class *.hex *.asm *.lst

