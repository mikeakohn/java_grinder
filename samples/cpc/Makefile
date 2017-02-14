#kicked out Mandel for crashing and ITOA because of to much unimplemented opcdoes in string/return/char at the moment

default: TestCPC.class GrafCPC.class ScreenCPC.class SpriteCPC.class DivCPC.class
#MandelCPC.class ITOA.class

grind: testcpc grafcpc screencpc spritecpc divcpc
#mandelcpc itoa

testcpc: TestCPC.class
	../../java_grinder TestCPC.class testcpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o testcpc.hex testcpc.asm
	hex2bin testcpc.hex
	CPCDiskXP -File testcpc.bin -AddAmsdosHeader 0x138 -AddToNewDsk javacpc.dsk
	
grafcpc: GrafCPC.class
	../../java_grinder GrafCPC.class grafcpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o grafcpc.hex grafcpc.asm
	hex2bin grafcpc.hex
	CPCDiskXP -File grafcpc.bin -AddAmsdosHeader 0x138 -AddToExistingDsk javacpc.dsk

screencpc: ScreenCPC.class
	../../java_grinder ScreenCPC.class screencpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o screencpc.hex screencpc.asm
	hex2bin screencpc.hex
	CPCDiskXP -File screencpc.bin -AddAmsdosHeader 0x138 -AddToExistingDsk javacpc.dsk

spritecpc: SpriteCPC.class
	../../java_grinder SpriteCPC.class spritecpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o spritecpc.hex spritecpc.asm
	hex2bin spritecpc.hex
	CPCDiskXP -File spritecpc.bin -AddAmsdosHeader 0x138 -AddToExistingDsk javacpc.dsk

divcpc: DivCPC.class
	../../java_grinder DivCPC.class divcpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o divcpc.hex divcpc.asm
	hex2bin divcpc.hex
	CPCDiskXP -File divcpc.bin -AddAmsdosHeader 0x138 -AddToExistingDsk javacpc.dsk

#itoa: ITOA.class
#	../../java_grinder ITOA.class ITOA.asm cpc
#	naken_asm -l -I ../../../naken_asm/include -o ITOA.hex ITOAcpc.asm
#	hex2bin ITOA.hex
# CPCDiskXP -File ITOA.bin -AddAmsdosHeader 0x138 -AddToExistingDsk javacpc.dsk
	
%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst *.dsk *.bin

