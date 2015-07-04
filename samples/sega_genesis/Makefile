
default: SegaGenesisJavaDemo.class

grind: SegaGenesisJavaDemo.class
	../../java_grinder SegaGenesisJavaDemo.class sega_genesis_java_demo.asm sega_genesis
	naken_asm -I ~ -l -b -o sega_genesis_java_demo.bin sega_genesis_java_demo.asm

z80:
	naken_asm -l -b -o z80_play_dac.bin z80_play_dac.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class sega_genesis_java_demo.asm *.bin *.rpk
	@echo "Clean!"

