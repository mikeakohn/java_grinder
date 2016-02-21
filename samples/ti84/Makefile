
defauit: TI84Demo.class

grind:
	../../java_grinder TI84Demo.class ti84_demo.asm ti84plus
	naken_asm -l -I ../../include -o ti84_demo.hex ti84_demo.asm
	rabbitsign -p -g -k ../../include/0104.key -t 8xk -f -vv ti84_demo.hex

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst

