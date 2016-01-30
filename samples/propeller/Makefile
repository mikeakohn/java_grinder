
default: JavaPropellerDemo.class

grind: JavaPropellerDemo.class
	../../java_grinder -v JavaPropellerDemo.class java_propeller_demo.asm propeller
	naken_asm -l -b -o java_propeller_demo.bin java_propeller_demo.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class *.asm *.bin
	@echo "Clean!"

