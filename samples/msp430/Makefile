
defauit:
	javac -classpath ../../build/JavaGrinder.jar:. GasDetector.java
	javac -classpath ../../build/JavaGrinder.jar:. LedScroll.java
	javac -classpath ../../build/JavaGrinder.jar:. Nokia5110.java
	javac -classpath ../../build/JavaGrinder.jar:. LCD.java

grind:
	../../java_grinder LCD.class lcd_msp430.asm msp430g2231
	naken_asm -l -I ../../../naken_asm/include -o lcd_msp430.hex lcd_msp430.asm
	../../java_grinder GasDetector.class gas_detector.asm msp430g2231
	naken_asm -l -I ../../../naken_asm/include -o gas_detector.hex gas_detector.asm

clean:
	@rm -f *.class *.hex *.asm *.lst

