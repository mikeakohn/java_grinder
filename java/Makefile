
JAVA_FILES=SPI.java UART.java IOPort.java IOPort0.java IOPort1.java
JOBJS=SPI.class UART.class IOPort.class IOPort0.class IOPort1.class

default:
	javac $(JAVA_FILES)

JavaGrinder.jar: $(JAVA_FILES)
	mkdir -p net/mikekohn/java_grinder
	javac -d net/mikekohn/java_grinder $(JAVA_FILES)
	jar cvfm JavaGrinder.jar Manifest.txt net

asdf: $(JOBJS)

%.class: %.java
	javac -classpath . -d net/mikekohn/java_grinder $*.java

clean:
	@rm -f *.class
	@rm -f *.jar
	@rm -rf net


