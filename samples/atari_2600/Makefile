
default: Atari2600JavaDemo.class

grind: Atari2600JavaDemo.class
	../../java_grinder Atari2600JavaDemo.class atari_2600_java_demo.asm atari2600
	naken_asm -I ~ -l -b -o atari_2600_java_demo.bin atari_2600_java_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.asm *.class *.bin *.lst
	@echo "Clean!"

