
defauit: TestCPC.class

grind: TestCPC.class
	../../java_grinder -v TestCPC.class test_cpc.asm cpc
	naken_asm -l -I ../../../naken_asm/include -o test_cpc.hex test_cpc.asm

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst

