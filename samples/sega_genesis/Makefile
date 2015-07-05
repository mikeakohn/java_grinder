
default: SegaGenesisJavaDemo.class

grind: SegaGenesisJavaDemo.class PlayTitleSample.class
	../../java_grinder SegaGenesisJavaDemo.class sega_genesis_java_demo.asm sega_genesis
	naken_asm -I ~ -l -b -o sega_genesis_java_demo.bin sega_genesis_java_demo.asm

z80:
	naken_asm -l -b -o z80_play_dac.bin z80_play_dac.asm
	go run bin2java.go z80_play_dac.bin | grep -v INFO | sed 's/ClassName/PlayTitleSample/' | sed 's/code/z80_code/' > PlayTitleSample.java

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class sega_genesis_java_demo.asm *.bin *.rpk
	@rm -rf cfg nvram snap
	@echo "Clean!"

