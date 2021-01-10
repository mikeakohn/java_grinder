INCLUDE_DIR=../../../naken_asm/include/6502/

default: Atari2600JavaDemo.class SpaceRevenge.class SpaceRevengeTitle.class out.bin

grind: demo space_revenge space_revenge_title

out.bin: grind
	cat space_revenge_title.bin space_revenge.bin > out.bin
	mv out.bin space_revenge.bin

demo:
	../../java_grinder Atari2600JavaDemo.class atari_2600_java_demo.asm atari2600
	naken_asm -I $(INCLUDE_DIR)  -l -type bin -o atari_2600_java_demo.bin atari_2600_java_demo.asm

space_revenge: SpaceRevenge.class
	../../java_grinder SpaceRevenge.class space_revenge.asm atari2600
	naken_asm -I $(INCLUDE_DIR) -l -type bin -o space_revenge.bin space_revenge.asm

space_revenge_title: SpaceRevengeTitle.class
	../../java_grinder SpaceRevengeTitle.class space_revenge_title.asm atari2600
	naken_asm -I $(INCLUDE_DIR) -l -type bin -o space_revenge_title.bin space_revenge_title.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.asm *.class *.bin *.lst *.mp4
	@rm -rf snap cfg
	@echo "Clean!"

