
default: JavaTrs80Demo.class

grind: JavaTrs80Demo.class
	../../java_grinder -v JavaTrs80Demo.class java_trs80_demo.asm trs80_coco
	naken_asm -l -type bin -o java_trs80_demo.bin java_trs80_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class *.asm *.bin
	@echo "Clean!"

