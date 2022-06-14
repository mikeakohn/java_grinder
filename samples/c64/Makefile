
default: CommodoreDemo.class

grind: CommodoreDemo.class
	../../java_grinder CommodoreDemo.class c64_demo.asm c64
	naken_asm -l -type bin -o c64_demo.prg c64_demo.asm

snake: Snake.class
	../../java_grinder Snake.class c64_snake.asm c64
	naken_asm -l -type bin -o c64_snake.prg c64_snake.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst

