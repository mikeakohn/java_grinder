
defauit: CommodoreDemo.class

grind: CommodoreDemo.class
	../../java_grinder CommodoreDemo.class c64_demo.asm c64
	naken_asm -l -type bin -o c64_demo.prg c64_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst

