
defauit: GasDetector.class LedScroll.class Nokia5110.class LCD.class

grind: GasDetector.class LedScroll.class Nokia5110.class LCD.class
	../../java_grinder LCD.class lcd_msp430.asm msp430g2231
	naken_asm -l -I ../../../naken_asm/include -o lcd_msp430.hex lcd_msp430.asm
	../../java_grinder GasDetector.class gas_detector.asm msp430g2231
	naken_asm -l -I ../../../naken_asm/include -o gas_detector.hex gas_detector.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst

