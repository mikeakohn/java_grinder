
default: ColecoDemo.class
	../../java_grinder ColecoDemo.class coleco_demo.asm colecovision
	naken_asm -I ../../../naken_asm/include/ti99 -l -type bin -o coleco_demo.col coleco_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class *.asm *.col
	@echo "Clean!"

