
default: GrinderDemoTi99.class

grind: GrinderDemoTi99.class
	../../java_grinder GrinderDemoTi99.class grinder_demo_ti99.asm ti99
	naken_asm -I ../../../naken_asm/include/ti99 -l -type bin -o grinder_demo_ti99.bin grinder_demo_ti99.asm
	zip grinder_demo_ti99.rpk grinder_demo_ti99.bin layout.xml

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class *.asm *.bin *.rpk
	@echo "Clean!"

