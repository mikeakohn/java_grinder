
default: TestCPC.class GrafCPC.class ScreenCPC.class DivCPC.class MandelCPC.class

grind: testcpc grafcpc screencpc divcpc mandelcpc

testcpc: TestCPC.class
	../../java_grinder TestCPC.class testcpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o testcpc.hex testcpc.asm
	hex2bin testcpc.hex
	CPCDiskXP -File testcpc.bin -AddAmsdosHeader 0x138 -AddToNewDsk testcpc.dsk
	
grafcpc: GrafCPC.class
	../../java_grinder GrafCPC.class grafcpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o grafcpc.hex grafcpc.asm
	hex2bin grafcpc.hex
	CPCDiskXP -File grafcpc.bin -AddAmsdosHeader 0x138 -AddToNewDsk grafcpc.dsk

screencpc: ScreenCPC.class
	../../java_grinder ScreenCPC.class screencpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o screencpc.hex screencpc.asm
	hex2bin screencpc.hex
	CPCDiskXP -File screencpc.bin -AddAmsdosHeader 0x138 -AddToNewDsk screencpc.dsk

divcpc: DivCPC.class
	../../java_grinder DivCPC.class divcpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o divcpc.hex divcpc.asm
	hex2bin divcpc.hex
	CPCDiskXP -File divcpc.bin -AddAmsdosHeader 0x138 -AddToNewDsk divcpc.dsk

mandelcpc: MandelCPC.class
	../../java_grinder MandelCPC.class mandelcpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o mandelcpc.hex mandelcpc.asm
	hex2bin mandelcpc.hex
	CPCDiskXP -File mandelcpc.bin -AddAmsdosHeader 0x138 -AddToNewDsk mandelcpc.dsk
	
%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst *.dsk *.bin

