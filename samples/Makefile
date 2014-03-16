
JOBJS=Arrays.class \
      CommodoreDemo.class \
      DSPTest.class \
      GasDetector.class \
      LedBlink.class \
      LCD.class \
      LCDDSPIC.class \
      MemoryTest.class \
      MethodCall.class \
      SPITest.class \
      StringTest.class \
      Testing.class \
      Testing2.class

default: $(JOBJS)

%.class: %.java
	javac -classpath ../build/JavaGrinder.jar:. $*.java

