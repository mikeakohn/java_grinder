PKG_DIR=net/mikekohn/java_grinder

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
  $(SRC_DIR)/IOPort6.java \
  $(SRC_DIR)/Memory.java \
  $(SRC_DIR)/SPI.java \
  $(SRC_DIR)/SPI0.java \
  $(SRC_DIR)/SPI1.java \
  $(SRC_DIR)/TI84.java \
  $(SRC_DIR)/Timer.java \
  $(SRC_DIR)/TimerListener.java \
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


