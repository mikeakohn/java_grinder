
INCLUDE=../../../naken_asm/include/8051

default: SleepyBeeDemo.class Breakout.class

grind: Breakout.class
	../../java_grinder -v Breakout.class breakout.asm sleepy_bee
	naken_asm -I $(INCLUDE) -l -o breakout.hex breakout.asm

demo: SleepyBeeDemo.class
	../../java_grinder -v SleepyBeeDemo.class sleepy_bee_demo.asm sleepy_bee
	naken_asm -I $(INCLUDE) -l -o sleepy_bee_demo.hex sleepy_bee_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.lst *.class sleepy_bee_demo.asm *.hex breakout.asm
	@echo "Clean!"

