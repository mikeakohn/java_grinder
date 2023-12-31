
default: LedBlink.class IceFunDemo.class
	../../java_grinder -v LedBlink.class led_blink.asm riscv_icefun
	../../../naken_asm/naken_asm -l -type bin -o led_blink.bin led_blink.asm
	../../java_grinder -v IceFunDemo.class ice_fun_demo.asm riscv_icefun
	../../../naken_asm/naken_asm -l -type bin -o ice_fun_demo.bin ice_fun_demo.asm

debug:
	../../../naken_asm/naken_asm -l -type bin -o ice_fun_demo.bin ice_fun_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst *.bin

