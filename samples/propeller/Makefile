
default: JavaPropellerDemo.class

grind: JavaPropellerDemo.class
	../../java_grinder -v JavaPropellerDemo.class java_propeller_demo.asm propeller
	naken_asm -l -b -o java_propeller_demo.bin java_propeller_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

program:
	go run bin2spin.go java_propeller_demo.bin > out.spin
	openspin out.spin
	propeller-load -r -p /dev/ttyUSB0 out.binary

test:
	openspin new.spin
	propeller-load -r -p /dev/ttyUSB0 new.binary

clean:
	@rm -f *.hex *.lst *.class *.asm *.bin *.spin *.binary
	@echo "Clean!"

