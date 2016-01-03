
default: Atari2600JavaDemo.class SpaceRevenge.class SpaceRevengeTitle.class

grind: demo space_revenge space_revenge_title

demo:
	../../java_grinder Atari2600JavaDemo.class atari_2600_java_demo.asm atari2600
	naken_asm -I ~ -l -b -o atari_2600_java_demo.bin atari_2600_java_demo.asm

space_revenge: SpaceRevenge.class
	../../java_grinder SpaceRevenge.class space_revenge.asm atari2600
	naken_asm -I ~ -l -b -o space_revenge.bin space_revenge.asm

space_revenge_title: SpaceRevengeTitle.class
	../../java_grinder SpaceRevengeTitle.class space_revenge_title.asm atari2600
	naken_asm -I ~ -l -b -o space_revenge_title.bin space_revenge_title.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.asm *.class *.bin *.lst *.mp4
	@rm -rf snap cfg
	@echo "Clean!"

