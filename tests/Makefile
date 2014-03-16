
JOBJS=*.class

default: $(JOBJS)

%.class: %.java
	javac -classpath ../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.asm *.hex *.lst

