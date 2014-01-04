PKG_DIR=net/mikekohn/java_grinder
JOBJS=\
  $(PKG_DIR)/SPI.class \
  $(PKG_DIR)/UART.class \
  $(PKG_DIR)/IOPort.class \
  $(PKG_DIR)/IOPort0.class \
  $(PKG_DIR)/IOPort1.class

default: $(JOBJS)

%.class: %.java
	javac -classpath . -d ../build $*.java

clean:
	@rm -f *.class
	@rm -f *.jar
	@rm -rf net


