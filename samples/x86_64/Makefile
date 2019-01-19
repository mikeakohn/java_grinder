
default: Test.class test.asm test.o
	gcc -o test_java test_java.c test.o -Wall -g

test.o: test.asm
	nasm -f elf test.asm

Test.class: Test.java
	javac Test.java

test.asm: Test.class
	../../java_grinder -v Test.class test.asm x86_64

clean:
	@rm -f *.class *.o test.asm test_java
	@echo "Clean!"

