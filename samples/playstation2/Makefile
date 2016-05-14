
default: PS2JavaDemo.class

grind: PS2JavaDemo.class
	../../java_grinder PS2JavaDemo.class ps2_java_demo.asm playstation2
	naken_asm -I ~ -l -b -o ps2_java_demo.bin ps2_java_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class ps2_java_demo.asm *.bin
	@echo "Clean!"

