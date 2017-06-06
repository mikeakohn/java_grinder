
#JAVA_SRC=\
  Arrays.java \
  CommodoreDemo.java \
  DSPTest.java \
  GasDetector.java \
  InlineTest.java \
  LedBlink.java \
  LCD.java \
  LCDDSPIC.java \
  Nokia5110.java \
  MemoryTest.java \
  MethodCall.java \
  SPITest.java \
  StringTest.java \
  Testing.java \
  Testing2.java \
  TI84Demo.java \
  TimerTest.java

default:
	@make -C c64
	@make -C dspic
	@make -C misc
	@make -C msp430
	@make -C ti84
	@make -C ti99

%.class: %.java
	javac -classpath ../build/JavaGrinder.jar:. $*.java

clean:
	@cd apple_iigs && make clean
	@cd atari_2600 && make clean
	@cd c64 && make clean
	@cd dspic && make clean
	@cd epiphany && make clean
	@cd misc && make clean
	@cd msp430 && make clean
	@cd pic32 && make clean
	@cd sega_genesis && make clean
	@cd ti84 && make clean
	@cd ti99 && make clean
	@cd trs80_coco && make clean
	@cd x86 && make clean
	@cd w65c265sxb && make clean


