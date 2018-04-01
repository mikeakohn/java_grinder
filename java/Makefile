PKG_DIR=net/mikekohn/java_grinder

SRC_DIR=net/mikekohn/java_grinder
JAVA_SRC=\
  $(SRC_DIR)/AppleIIgs.java \
  $(SRC_DIR)/Atari2600.java \
  $(SRC_DIR)/ADC.java \
  $(SRC_DIR)/C64/SID.java \
  $(SRC_DIR)/C64/VIC.java \
  $(SRC_DIR)/CPC.java \
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
  $(SRC_DIR)/IOPort7.java \
  $(SRC_DIR)/Math.java \
  $(SRC_DIR)/Memory.java \
  $(SRC_DIR)/MSX.java \
  $(SRC_DIR)/Parallella.java \
  $(SRC_DIR)/Playstation2.java \
  $(SRC_DIR)/Propeller.java \
  $(SRC_DIR)/Draw3D/Draw3DObject.java \
  $(SRC_DIR)/Draw3D/Draw3DObjectWithTexture.java \
  $(SRC_DIR)/Draw3D/Draw3DPoints.java \
  $(SRC_DIR)/Draw3D/Draw3DLine.java \
  $(SRC_DIR)/Draw3D/Draw3DLineStrip.java \
  $(SRC_DIR)/Draw3D/Draw3DTriangle.java \
  $(SRC_DIR)/Draw3D/Draw3DTriangleWithTexture.java \
  $(SRC_DIR)/Draw3D/Draw3DTriangleStrip.java \
  $(SRC_DIR)/Draw3D/Draw3DTriangleStripWithTexture.java \
  $(SRC_DIR)/Draw3D/Draw3DTriangleFan.java \
  $(SRC_DIR)/Draw3D/Draw3DTriangleFanWithTexture.java \
  $(SRC_DIR)/Draw3D/Draw3DTexture.java \
  $(SRC_DIR)/SegaGenesis.java \
  $(SRC_DIR)/SNES.java \
  $(SRC_DIR)/SPI.java \
  $(SRC_DIR)/SPI0.java \
  $(SRC_DIR)/SPI1.java \
  $(SRC_DIR)/SXB.java \
  $(SRC_DIR)/TI84.java \
  $(SRC_DIR)/TI99.java \
  $(SRC_DIR)/Timer.java \
  $(SRC_DIR)/TimerListener.java \
  $(SRC_DIR)/TRS80Coco.java \
  $(SRC_DIR)/UserInterruptListener.java \
  $(SRC_DIR)/UART.java \
  $(SRC_DIR)/UART0.java \
  $(SRC_DIR)/UART1.java \
  $(SRC_DIR)/Watchdog.java

default:
	javac -classpath . -d ../build $(JAVA_SRC) 

asdf: $(JOBJS)

%.class: %.java
	javac -classpath . -d ../build $*.java

clean:
	@rm -f *.class
	@rm -f *.jar
	@rm -rf net


