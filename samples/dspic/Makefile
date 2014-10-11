
defauit:
	javac -classpath ../../build/JavaGrinder.jar:. Mandelbrot.java
	javac -classpath ../../build/JavaGrinder.jar:. DSPTest.java

grind:
	../../java_grinder Mandelbrot.class mandelbrot.asm dspic33fj06gs101a
	naken_asm -l -I ../../../naken_asm/include -o mandelbrot.hex mandelbrot.asm

clean:
	@rm -f *.class *.hex *.asm *.lst

