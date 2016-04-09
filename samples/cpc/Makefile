
default: TestCPC.class GrafCPC.class 

grind: testcpc grafcpc 

testcpc: TestCPC.class
../../java_grinder -v TestCPC.class testcpc.asm cpc
naken_asm -l -I ../../../naken_asm/include -o testcpc.hex testcpc.asm
hex2bin testcpc.hex
CPCDiskXP -File testcpc.bin -AddAmsdosHeader 0x138 -AddToNewDsk testcpc.dsk

grafcpc: GrafCPC.class
../../java_grinder -v GrafCPC.class grafcpc.asm cpc
naken_asm -l -I ../../../naken_asm/include -o grafcpc.hex grafcpc.asm
hex2bin grafcpc.hex
CPCDiskXP -File grafcpc.bin -AddAmsdosHeader 0x138 -AddToNewDsk grafcpc.dsk

%.class: %.java
javac -classpath ../../build/JavaGrinder.jar $*.java

clean:
@rm -f *.class *.hex *.asm *.lst *.dsk *.bin

