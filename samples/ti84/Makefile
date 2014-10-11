
defauit:
	javac -classpath ../../build/JavaGrinder.jar:. TI84Demo.java

grind:
	../../java_grinder TI84Demo.class ti84_demo.asm ti84plus
	naken_asm -l -I include -o ti84_demo.hex ti84_demo.asm
	rabbitsign -p -g -k include/0104.key -t 8xk -f -vv ti84_demo.hex

clean:
	@rm -f *.class *.hex *.asm *.lst

