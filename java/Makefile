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
  $(PKG_DIR)/UART1.class \
  $(PKG_DIR)/C64/SID.class \
  $(PKG_DIR)/C64/VIC.class

SRC_DIR=net/mikekohn/java_grinder
JAVA_SRC=\
  $(SRC_DIR)/ADC.java \
  $(SRC_DIR)/CPU.java \
  $(SRC_DIR)/DSP.java \
  $(SRC_DIR)/IOPort.java \
  $(SRC_DIR)/IOPort0.java \
  $(SRC_DIR)/IOPort1.java \
  $(SRC_DIR)/IOPort2.java \
  $(SRC_DIR)/IOPort3.java \
  $(SRC_DIR)/IOPort4.java \
  $(SRC_DIR)/IOPort5.java \
  $(SRC_DIR)/Memory.java \
  $(SRC_DIR)/SPI.java \
  $(SRC_DIR)/SPI0.java \
  $(SRC_DIR)/SPI1.java \
  $(SRC_DIR)/UART.java \
  $(SRC_DIR)/UART0.java \
  $(SRC_DIR)/UART1.java \
  $(SRC_DIR)/C64/SID.java \
  $(SRC_DIR)/C64/VIC.java

default:
	javac -classpath . -d ../build $(JAVA_SRC) 

asdf: $(JOBJS)

%.class: %.java
	javac -classpath . -d ../build $*.java

clean:
	@rm -f *.class
	@rm -f *.jar
	@rm -rf net


