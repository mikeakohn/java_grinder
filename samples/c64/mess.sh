#!/usr/bin/env bash

extra=""

if [ -e /storage/roms/c64 ]
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
  extra="-aviwrite out.avi -snapsize 640x480"
fi

mess c64c -window -rompath ${ROMPATH} \
  -flop java.d64 \
  ${extra}

if [ "$1" = "avi" ]
then
  mencoder -oac mp3lame -ovc x264 -o video.avi -vf scale=640:480 ~/.mess/snap/out.avi
fi


