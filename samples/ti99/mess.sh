#!/usr/bin/env bash

if [ -e /storage/roms/ti99_4a ]
then
  ROMPATH=/storage/roms
else
  ROMPATH=/usbdisk/roms/ti99
fi

extra=""

if [ "$1" = "debug" ]
then
  extra="-debug"
fi

if [ "$1" = "avi" ]
then
  extra="-aviwrite out.avi"
fi

mame ti99_4a -window -rompath ${ROMPATH} \
  -resolution0 640x480 \
  -cart1 grinder_demo_ti99.rpk \
  ${extra}

if [ "$1" = "avi" ]
then
  mencoder -oac mp3lame -ovc x264 -o video.avi -vf scale=640:480 ~/.mess/snap/out.avi
fi

