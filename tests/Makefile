
JOBJS=*.class

default: $(JOBJS)

%.class: %.java
	javac -classpath ../build/JavaGrinder.jar:. $*.java
	../java_grinder $*.class $*.asm msp430g2231
	naken_asm -l -I/storage/git/naken_asm/include -o $*.hex $*.asm

clean:
	@rm -f *.class *.asm *.hex *.lst

