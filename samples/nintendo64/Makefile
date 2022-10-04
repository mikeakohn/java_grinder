
JAVA_GRINDER=../../java_grinder
NAKEN_ASM=../../../naken_asm/naken_asm
INCLUDE_PATH=../../../naken_asm/include/nintendo64

defauit: Nintendo64Demo.class

grind: Nintendo64Demo.class
	$(JAVA_GRINDER) -v Nintendo64Demo.class nintendo64_demo.asm nintendo64
	$(NAKEN_ASM) -l -I $(INCLUDE_PATH) -type bin -o nintendo64_demo.z64 nintendo64_demo.asm
	./n64crc nintendo64_demo.z64

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst *.bin *.z64

