
default: CommodoreDemo.class

grind: CommodoreDemo.class
	../../java_grinder CommodoreDemo.class c64_demo.asm c64
	naken_asm -l -type bin -o c64_demo.prg c64_demo.asm

snake: Snake.class
	../../java_grinder Snake.class c64_snake.asm c64
	naken_asm -l -type bin -o c64_snake.prg c64_snake.asm

sound: Sound.class
	../../java_grinder Sound.class c64_sound.asm c64
	naken_asm -l -type bin -o c64_sound.prg c64_sound.asm

disk:
	c1541 -format diskname,id d64 java_grinder.d64
	c1541 -attach java_grinder.d64 -write c64_demo.prg
	c1541 -attach java_grinder.d64 -write c64_snake.prg
	c1541 -attach java_grinder.d64 -write c64_sound.prg
	c1541 -attach java_grinder.d64 -rename c64_demo.prg demo
	c1541 -attach java_grinder.d64 -rename c64_snake.prg snake
	c1541 -attach java_grinder.d64 -rename c64_sound.prg sound

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.prg *.asm *.lst

