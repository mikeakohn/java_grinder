
defauit: Mandelbrot.class DSPTest.class

grind: Mandelbrot.class DSPTest.class
	../../java_grinder Mandelbrot.class mandelbrot.asm dspic33fj06gs101a
	naken_asm -l -I ../../../naken_asm/include -o mandelbrot.hex mandelbrot.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst

