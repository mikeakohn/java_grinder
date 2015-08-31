
default: SnesJavaDemo.class

grind: SnesJavaDemo.class PlayTitleSample.class
	../../java_grinder SnesJavaDemo.class snes_java_demo.asm snes
	naken_asm -I ~ -l -b -o snes_java_demo.bin snes_java_demo.asm
	go run make_cart.go

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class snes_java_demo.asm *.bin *.smc
	@rm -rf cfg nvram snap
	@echo "Clean!"

