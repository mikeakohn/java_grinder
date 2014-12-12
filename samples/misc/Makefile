
JAVA_SRC=\
  Arrays.class \
  InlineTest.class \
  LedBlink.class \
  MemoryTest.class \
  MethodCall.class \
  SPITest.class \
  StringTest.class \
  Testing2.class \
  Testing.class \
  UARTTest.class

default: $(JAVA_SRC)

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.class *.hex *.asm *.lst

