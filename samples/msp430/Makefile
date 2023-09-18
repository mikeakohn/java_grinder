NAKEN_ASM=../../../naken_asm/naken_asm

defauit: GasDetector.class LedScroll.class Nokia5110.class LCD.class FaceImage.class Sound.class TimerTest.class

grind: GasDetector.class LedScroll.class Nokia5110.class LCD.class Sound.class TimerTest.class
	../../java_grinder LCD.class lcd_msp430.asm msp430g2231
	$(NAKEN_ASM) -l -I ../../../naken_asm/include -o lcd_msp430.hex lcd_msp430.asm
	../../java_grinder GasDetector.class gas_detector.asm msp430g2231
	$(NAKEN_ASM) -l -I ../../../naken_asm/include -o gas_detector.hex gas_detector.asm
	../../java_grinder Nokia5110.class nokia_5110.asm msp430g2231
	$(NAKEN_ASM) -l -I ../../../naken_asm/include -o nokia_5110.hex nokia_5110.asm
	../../java_grinder TimerTest.class timer_test.asm msp430g2231
	$(NAKEN_ASM) -l -I ../../../naken_asm/include -o timer_test.hex timer_test.asm
	../../java_grinder Sound.class sound.asm msp430g2231
	$(NAKEN_ASM) -l -I ../../../naken_asm/include -o sound.hex sound.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst

