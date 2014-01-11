
JOBJS=Testing.class \
      Testing2.class \
      LedBlink.class \
      MemoryTest.class \
      MethodCall.class \
      SPITest.class

default: $(JOBJS)

%.class: %.java
	javac -classpath ../build/JavaGrinder.jar:. $*.java

