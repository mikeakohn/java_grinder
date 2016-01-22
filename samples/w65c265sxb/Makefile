
defauit: BlinkLED.class

grind: BlinkLED.class
	../../java_grinder BlinkLED.class blink_led.asm w65c265sxb
	naken_asm blink_led.asm -o blink_led.hex

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst

