
defauit: Mandelbrot.class DSPTest.class CuriosityUart.class

grind: Mandelbrot.class DSPTest.class
	../../java_grinder Mandelbrot.class mandelbrot.asm dspic33fj06gs101a
	naken_asm -l -I ../../../naken_asm/include -o mandelbrot.hex mandelbrot.asm

curiosity: CuriosityUart.class
	../../java_grinder CuriosityUart.class curiosity_uart.asm dspic33ck64mc105
	naken_asm -l -I ../../../naken_asm/include -o curiosity_uart.hex curiosity_uart.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst

