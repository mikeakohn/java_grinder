
JAVA_SRC=\
  Arrays.java \
  CommodoreDemo.java \
  DSPTest.java \
  GasDetector.java \
  LedBlink.java \
  LCD.java \
  LCDDSPIC.java \
  MemoryTest.java \
  MethodCall.java \
  SPITest.java \
  StringTest.java \
  Testing.java \
  Testing2.java \
  TI84Demo.java

default:
	javac -classpath ../build/JavaGrinder.jar:. $(JAVA_SRC)

%.class: %.java
	javac -classpath ../build/JavaGrinder.jar:. $*.java

