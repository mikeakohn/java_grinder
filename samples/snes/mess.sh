#!/usr/bin/env bash

extra=""

if [ -e /storage/roms/snes ]
then
  ROMPATH=/storage/roms
else
  ROMPATH=/usbdisk/roms
fi


if [ "$1" = "debug" ]
then
  extra="-debug"
fi

if [ "$1" = "avi" ]
then
  extra="-aviwrite out.avi"
fi

mame snes -window -rompath ${ROMPATH} \
  -cartridge snes_java_demo.smc \
  ${extra}

if [ "$1" = "avi" ]
then
  mencoder -oac mp3lame -ovc x264 -o video.avi -vf scale=640:480 ~/.mess/snap/out.avi
fi

