
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default:
	@+make -C build

tests:
	@+make -C testing

.PHONY: java
java:
	@+make -C java
	@make -C build jar

%.class: %.java
	javac $*.java

grind: tests
	./java_grinder testing/LCD.class lcd_msp430.asm msp430g2231
	naken_asm -I /storage/git/naken_asm/include/msp430 -l -o lcd_msp430.hex lcd_msp430.asm
	./java_grinder testing/MethodCall.class method_call_msp430.asm msp430g2231
	naken_asm -I /storage/git/naken_asm/include/msp430 -l -o method_call_msp430.hex method_call_msp430.asm
	./java_grinder testing/GasDetector.class gas_detector.asm msp430g2231
	naken_asm -I /storage/git/naken_asm/include/msp430 -l -o gas_detector.hex gas_detector.asm

dsp: tests
	./java_grinder testing/LedBlink.class led_blink.asm dspic33fj06gs101a
	naken_asm -l -I /storage/git/naken_asm/include -o led_blink.hex led_blink.asm
	./java_grinder testing/LCDDSPIC.class lcd_dspic.asm dspic33fj06gs101a
	naken_asm -l -I /storage/git/naken_asm/include -o lcd_dspic.hex lcd_dspic.asm

array:
	./java_grinder testing/Arrays.class arrays.asm msp430g2231
	naken_asm -I /storage/git/naken_asm/include/msp430 -l -o arrays.hex arrays.asm

clean:
	@rm -f *.o java_grinder build/*.o *.asm *.lst *.hex
	@rm -f java/*.class testing/*.class build/*.jar
	@rm -rf build/net
	@echo "Clean!"



