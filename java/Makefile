
JAVA_FILES=SPI.java UART.java IOPort.java IOPort0.java IOPort1.java
JOBJS=SPI.class UART.class IOPort.class IOPort0.class IOPort1.class

default:
	javac $(JAVA_FILES)

asdf: $(JOBJS)

%.class: %.java
	javac -classpath . -d net/mikekohn/java_grinder $*.java

