
default: SegaGenesisJavaDemo.class

grind: SegaGenesisJavaDemo.class
	../../java_grinder SegaGenesisJavaDemo.class sega_genesis_java_demo.asm sega_genesis
	naken_asm -I ~ -l -b -o sega_genesis_java_demo.bin sega_genesis_java_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class *.asm *.bin *.rpk
	@echo "Clean!"

