
JOBJS=Testing.class \
      Testing2.class \
      DSPTest.class \
      LedBlink.class \
      LCD.class \
      LCDDSPIC.class \
      MemoryTest.class \
      MethodCall.class \
      SPITest.class

default: $(JOBJS)

%.class: %.java
	javac -classpath ../build/JavaGrinder.jar:. $*.java

