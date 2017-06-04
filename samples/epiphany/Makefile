
defauit: Test.class

grind: Test.class
	../../java_grinder -v Test.class test.asm epiphany
	naken_asm -l -I ../../../naken_asm/include -s -o test.srec test.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst *.srec

