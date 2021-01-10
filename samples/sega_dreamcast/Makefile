INCLUDES=../../../naken_asm/include/dreamcast
CLASSES= \
  SegaDreamcastDemo.class

default: $(CLASSES)

grind: $(CLASSES)
	../../java_grinder SegaDreamcastDemo.class sega_dreamcast_demo.asm sega_dreamcast
	../../../naken_asm/naken_asm -I $(INCLUDES) -l -type bin -o sega_dreamcast_demo.bin sega_dreamcast_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class sega_dreamcast_demo.asm *.bin *.rpk
	@echo "Clean!"

