
default: AppleIIgsJavaDemo.class

grind: AppleIIgsJavaDemo.class
	../../java_grinder AppleIIgsJavaDemo.class apple_iigs_java_demo.asm appleiigs
	naken_asm -I ~ -l -b -o apple_iigs_java_demo.bin apple_iigs_java_demo.asm

disk:
	GOPATH=/storage/git/apple2_disk_utils go run /storage/git/apple2_disk_utils/init_dos33_disk.go apple_iigs_java_demo.dsk /storage/git/apple2_disk_utils/dos33.img apple_iigs_java_demo.bin

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.asm *.class *.bin
	@echo "Clean!"

