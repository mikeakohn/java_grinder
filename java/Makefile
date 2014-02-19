PKG_DIR=net/mikekohn/java_grinder
JOBJS=\
  $(PKG_DIR)/ADC.class \
  $(PKG_DIR)/CPU.class \
  $(PKG_DIR)/DSP.class \
  $(PKG_DIR)/IOPort.class \
  $(PKG_DIR)/IOPort0.class \
  $(PKG_DIR)/IOPort1.class \
  $(PKG_DIR)/IOPort2.class \
  $(PKG_DIR)/IOPort3.class \
  $(PKG_DIR)/IOPort4.class \
  $(PKG_DIR)/IOPort5.class \
  $(PKG_DIR)/Memory.class \
  $(PKG_DIR)/SPI.class \
  $(PKG_DIR)/SPI0.class \
  $(PKG_DIR)/SPI1.class \
  $(PKG_DIR)/UART.class \
  $(PKG_DIR)/UART0.class \
  $(PKG_DIR)/UART1.class

default: $(JOBJS)

%.class: %.java
	javac -classpath . -d ../build $*.java

clean:
	@rm -f *.class
	@rm -f *.jar
	@rm -rf net


