
default: JavaTrs80Demo.class

grind: JavaTrs80Demo.class
	../../java_grinder JavaTrs80Demo.class java_trs80_demo.asm trs80_coco
	naken_asm -I ../../../naken_asm/include/ti99 -l -b -o java_trs80_demo.bin java_trs80_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class *.asm *.bin
	@echo "Clean!"

