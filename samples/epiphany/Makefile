
defauit: Test.class Mandelbrot.class

mandel: Mandelbrot.class
	../../java_grinder -v Mandelbrot.class mandelbrot.asm epiphany
	naken_asm -l -I ../../../naken_asm/include/epiphany -s -o mandelbrot.srec mandelbrot.asm

grind: Test.class
	../../java_grinder -v Test.class test.asm epiphany
	naken_asm -l -I ../../../naken_asm/include/epiphany -s -o test.srec test.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst *.srec

