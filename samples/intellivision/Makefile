
JAVA_GRINDER=../../java_grinder
NAKEN_ASM=../../../naken_asm/naken_asm

defauit: IntellivisionDemo.class

grind: IntellivisionDemo.class
	$(JAVA_GRINDER) -v IntellivisionDemo.class intellivision_demo.asm intellivision
	$(NAKEN_ASM) -l -I ../../../naken_asm/include -type bin -o intellivision_demo.bin intellivision_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst *.bin

