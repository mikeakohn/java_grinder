
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default:
	@+make -C build

.PHONY: samples
samples:
	@+make -C samples

.PHONY: java
java:
	@+make -C java
	@make -C build jar

.PHONY: tests
tests:
	@cd tests && sh run_tests.sh

%.class: %.java
	javac $*.java

grind: samples
	./java_grinder samples/LCD.class lcd_msp430.asm msp430g2231
	naken_asm -I /storage/git/naken_asm/include/msp430 -l -o lcd_msp430.hex lcd_msp430.asm
	./java_grinder samples/MethodCall.class method_call_msp430.asm msp430g2231
	naken_asm -I /storage/git/naken_asm/include/msp430 -l -o method_call_msp430.hex method_call_msp430.asm
	./java_grinder samples/GasDetector.class gas_detector.asm msp430g2231
	naken_asm -I /storage/git/naken_asm/include/msp430 -l -o gas_detector.hex gas_detector.asm

dsp: samples
	./java_grinder samples/LedBlink.class led_blink.asm dspic33fj06gs101a
	naken_asm -l -I /storage/git/naken_asm/include -o led_blink.hex led_blink.asm
	./java_grinder samples/LCDDSPIC.class lcd_dspic.asm dspic33fj06gs101a
	naken_asm -l -I /storage/git/naken_asm/include -o lcd_dspic.hex lcd_dspic.asm

array:
	./java_grinder samples/Arrays.class arrays.asm msp430g2231
	naken_asm -I /storage/git/naken_asm/include/msp430 -l -o arrays.hex arrays.asm

timer:
	./java_grinder samples/TimerTest.class timer_test.asm msp430g2231
	naken_asm -I /storage/git/naken_asm/include/msp430 -l -o timer_test.hex timer_test.asm

c64: samples
	./java_grinder samples/CommodoreDemo.class demo.asm c64
	naken_asm -l -b -o demo.prg demo.asm

ti84: samples
	./java_grinder samples/TI84Demo.class ti84_demo.asm ti84plus
	naken_asm -l -I include -o ti84_demo.hex ti84_demo.asm
	rabbitsign -p -g -k include/0104.key -t 8xk -f -vv ti84_demo.hex

clean:
	@rm -f *.o java_grinder build/*.o *.asm *.lst *.hex *.prg
	@rm -f java/*.class samples/*.class build/*.jar
	@cd samples && make clean
	@rm -rf build/net
	@echo "Clean!"

distclean: clean


