PKG_DIR=net/mikekohn/java_grinder

SRC_DIR=net/mikekohn/java_grinder
JAVA_SRC=\
  $(SRC_DIR)/amiga/Amiga.java \
  $(SRC_DIR)/amiga/Blitter.java \
  $(SRC_DIR)/amiga/Copper.java \
  $(SRC_DIR)/AppleIIgs.java \
  $(SRC_DIR)/Atari2600.java \
  $(SRC_DIR)/ADC.java \
  $(SRC_DIR)/c64/SID.java \
  $(SRC_DIR)/c64/VIC.java \
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
  $(SRC_DIR)/draw3d/Draw3DObject.java \
  $(SRC_DIR)/draw3d/Draw3DObjectWithTexture.java \
  $(SRC_DIR)/draw3d/Points.java \
  $(SRC_DIR)/draw3d/Line.java \
  $(SRC_DIR)/draw3d/LineStrip.java \
  $(SRC_DIR)/draw3d/Sprite.java \
  $(SRC_DIR)/draw3d/SpriteWithTexture.java \
  $(SRC_DIR)/draw3d/Triangle.java \
  $(SRC_DIR)/draw3d/TriangleWithTexture.java \
  $(SRC_DIR)/draw3d/TriangleStrip.java \
  $(SRC_DIR)/draw3d/TriangleStripWithTexture.java \
  $(SRC_DIR)/draw3d/TriangleFan.java \
  $(SRC_DIR)/draw3d/TriangleFanWithTexture.java \
  $(SRC_DIR)/draw3d/Texture.java \
  $(SRC_DIR)/draw3d/Texture16.java \
  $(SRC_DIR)/draw3d/Texture24.java \
  $(SRC_DIR)/draw3d/Texture32.java \
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

