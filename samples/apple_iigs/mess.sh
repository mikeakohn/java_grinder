#!/usr/bin/env bash

extra=""

if [ -e /storage/roms/apple2gs ]
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

mess apple2gs -window -rompath ${ROMPATH} \
  -flop1 apple_iigs_java_demo.dsk \
  ${extra}

if [ "$1" = "avi" ]
then
  #mencoder -oac mp3lame -ovc x264 -o video.avi -vf scale=640:480 ~/.mess/snap/out.avi
  ffmpeg -i snap/out.avi -vf hflip -s 640x480 out.mp4
  ffmpeg -i out.mp4 -vf hflip out2.mp4
fi

