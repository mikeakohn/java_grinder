
defauit:
	javac -classpath ../../build/JavaGrinder.jar:. CommodoreDemo.java

grind:
	../../java_grinder CommodoreDemo.class c64_demo.asm c64
	naken_asm -l -b -o c64_demo.prg c64_demo.asm

clean:
	@rm -f *.class *.hex *.asm *.lst

