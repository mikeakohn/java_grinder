
default: AppleIIgsJavaDemo.class

grind: AppleIIgsJavaDemo.class
	../../java_grinder AppleIIgsJavaDemo.class apple_iigs_java_demo.asm appleiigs
	naken_asm -I ~ -l -b -o apple_iigs_java_demo.bin apple_iigs_java_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.asm *.class *.bin
	@echo "Clean!"

