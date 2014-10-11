#!/usr/bin/env bash

if [ -e /storage/roms/ti99_4a ]
then
  ROMPATH=/storage/roms
else
  ROMPATH=/usbdisk/roms/ti99
fi

mess ti99_4a -window -rompath ${ROMPATH} \
  -resolution0 640x480 \
  -cart1 grinder_demo_ti99.rpk \
  #-debug

