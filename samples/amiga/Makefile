
NAKEN_INCLUDE=../../../naken_asm/include

default: AmigaDemo.class

grind: AmigaDemo.class
	../../java_grinder AmigaDemo.class amiga_demo.asm amiga
	../../../naken_asm/naken_asm -I $(NAKEN_INCLUDE) -l -b -o amiga_demo amiga_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.lst *.class amiga_demo.asm amiga_demo
	@rm -rf cfg nvram snap
	@echo "Clean!"

