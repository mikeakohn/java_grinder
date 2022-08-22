
INCLUDE=../../../naken_asm/include/65c816

default: SnesJavaDemo.class

grind: SnesJavaDemo.class
	../../java_grinder SnesJavaDemo.class snes_java_demo.asm snes
	naken_asm -I $(INCLUDE) -l -type bin -o snes_java_demo.bin snes_java_demo.asm
	python3 make_cart.py snes_java_demo.bin snes_java_demo.smc

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class snes_java_demo.asm *.bin *.smc
	@rm -rf cfg nvram snap
	@echo "Clean!"

