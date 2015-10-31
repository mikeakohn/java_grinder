CLASSES= \
  SegaGenesisJavaDemo.class \
  Box.class \
  Common.class \
  ImageJava.class \
  ImageJavaGrinder.class \
  ImageInside.class \
  Mandelbrots.class \
  NakenLogo.class \
  PlaySong.class \
  PlayTitleSample.class \
  Stars.class \
  PlayLaser.class \
  Sprites.class \
  Stars.class

default: $(CLASSES)

grind: $(CLASSES)
	../../java_grinder SegaGenesisJavaDemo.class sega_genesis_java_demo.asm sega_genesis
	naken_asm -I ~ -l -b -o sega_genesis_java_demo.bin sega_genesis_java_demo.asm

dac:
	naken_asm -l -b -o z80_play_dac.bin z80_play_dac.asm
	go run bin2java.go z80_play_dac.bin | grep -v INFO | sed 's/ClassName/PlayTitleSample/' | sed 's/code/z80_code/' > PlayTitleSample.java

synth:
	naken_asm -l -b -o z80_play_synth.bin z80_play_synth.asm
	go run bin2java.go z80_play_synth.bin | grep -v INFO | sed 's/ClassName/PlaySong/' | sed 's/code/z80_code/' > PlaySong.java

song:
	naken_asm -l -b -o z80_play_title_song.bin z80_play_title_song.asm
	go run bin2java.go z80_play_title_song.bin | grep -v INFO | sed 's/ClassName/PlayTitleSong/' | sed 's/code/z80_code/' > PlayTitleSong.java

laser:
	naken_asm -l -b -o z80_play_laser.bin z80_play_laser.asm
	go run bin2java.go z80_play_laser.bin | grep -v INFO | sed 's/ClassName/PlayLaser/' | sed 's/code/z80_code/' > PlayLaser.java

setup:
	naken_asm -l -b -o z80_setup_synth.bin z80_setup_synth.asm
	go run bin2java.go z80_setup_synth.bin | grep -v INFO | sed 's/ClassName/SetupSynth/' | sed 's/code/z80_code/' > SetupSynth.java

%.class: %.java
	javac -classpath ../../build/JavaGrinder.jar:. $*.java

clean:
	@rm -f *.hex *.lst *.class sega_genesis_java_demo.asm *.bin *.rpk
	@rm -rf cfg nvram snap
	@echo "Clean!"

