/**
 *  Java Grinder
 *  Author: Joe Davisson
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_API_C64_H
#define JAVA_GRINDER_GENERATOR_API_C64_H

class API_C64
{
public:
  // Commodore 64 stuff
  virtual int c64_sid_frequency1() { return -1; }
  virtual int c64_sid_pulseWidth1() { return -1; }
  virtual int c64_sid_waveform1() { return -1; }
  virtual int c64_sid_adsr1() { return -1; }
  virtual int c64_sid_frequency2() { return -1; }
  virtual int c64_sid_pulseWidth2() { return -1; }
  virtual int c64_sid_waveform2() { return -1; }
  virtual int c64_sid_adsr2() { return -1; }
  virtual int c64_sid_frequency3() { return -1; }
  virtual int c64_sid_pulseWidth3() { return -1; }
  virtual int c64_sid_waveform3() { return -1; }
  virtual int c64_sid_adsr3() { return -1; }
  virtual int c64_sid_filterCutoff() { return -1; }
  virtual int c64_sid_filterResonance() { return -1; }
  virtual int c64_sid_volume() { return -1; }
  virtual int c64_sid_oscillator3() { return -1; }
  virtual int c64_sid_envelope3() { return -1; }
  virtual int c64_sid_clear() { return -1; }

  virtual int c64_vic_sprite0pos() { return -1; }
  virtual int c64_vic_sprite1pos() { return -1; }
  virtual int c64_vic_sprite2pos() { return -1; }
  virtual int c64_vic_sprite3pos() { return -1; }
  virtual int c64_vic_sprite4pos() { return -1; }
  virtual int c64_vic_sprite5pos() { return -1; }
  virtual int c64_vic_sprite6pos() { return -1; }
  virtual int c64_vic_sprite7pos() { return -1; }
  virtual int c64_vic_writeControl1() { return -1; }
  virtual int c64_vic_readControl1() { return -1; }
  virtual int c64_vic_waitRaster() { return -1; }
  virtual int c64_vic_spriteEnable() { return -1; }
  virtual int c64_vic_writeControl2() { return -1; }
  virtual int c64_vic_readControl2() { return -1; }
  virtual int c64_vic_spriteExpandY() { return -1; }
  virtual int c64_vic_writePointer() { return -1; }
  virtual int c64_vic_readPointer() { return -1; }
  virtual int c64_vic_writeInterruptStatus() { return -1; }
  virtual int c64_vic_readInterruptStatus() { return -1; }
  virtual int c64_vic_interruptControl() { return -1; }
  virtual int c64_vic_spritePriority() { return -1; }
  virtual int c64_vic_spriteMulticolorEnable() { return -1; }
  virtual int c64_vic_spriteExpandX() { return -1; }
  virtual int c64_vic_spriteCollision() { return -1; }
  virtual int c64_vic_dataCollision() { return -1; }
  virtual int c64_vic_border() { return -1; }
  virtual int c64_vic_background() { return -1; }
  virtual int c64_vic_multicolor1() { return -1; }
  virtual int c64_vic_multicolor2() { return -1; }
  virtual int c64_vic_multicolor3() { return -1; }
  virtual int c64_vic_spriteMulticolor0() { return -1; }
  virtual int c64_vic_spriteMulticolor1() { return -1; }
  virtual int c64_vic_sprite0color() { return -1; }
  virtual int c64_vic_sprite1color() { return -1; }
  virtual int c64_vic_sprite2color() { return -1; }
  virtual int c64_vic_sprite3color() { return -1; }
  virtual int c64_vic_sprite4color() { return -1; }
  virtual int c64_vic_sprite5color() { return -1; }
  virtual int c64_vic_sprite6color() { return -1; }
  virtual int c64_vic_sprite7color() { return -1; }
  virtual int c64_vic_hiresEnable() { return -1; }
  virtual int c64_vic_hiresClear() { return -1; }
  virtual int c64_vic_hiresPlot() { return -1; }
  virtual int c64_vic_makeHiresTables() { return -1; }
  virtual int c64_vic_textEnable() { return -1; }
  virtual int c64_vic_textClear() { return -1; }
  virtual int c64_vic_textCopy() { return -1; }
  virtual int c64_vic_textPlot() { return -1; }
  virtual int c64_vic_textAsciiPlot() { return -1; }
  virtual int c64_vic_textRead() { return -1; }
  virtual int c64_vic_textString() { return -1; }
  virtual int c64_vic_textFill() { return -1; }
  virtual int c64_vic_textPaint() { return -1; }
  virtual int c64_vic_textScrollLeft() { return -1; }
  virtual int c64_vic_textScrollRight() { return -1; }
  virtual int c64_vic_textScrollUp() { return -1; }
  virtual int c64_vic_textScrollDown() { return -1; }
  virtual int c64_vic_textSmoothScroll() { return -1; }
  virtual int c64_vic_makeTextTable() { return -1; }
  virtual int c64_vic_makeColorTable() { return -1; }
  virtual int c64_vic_colorRamClear() { return -1; }
  virtual int c64_vic_copyUppercase() { return -1; }
  virtual int c64_vic_copyLowercase() { return -1; }
  virtual int c64_vic_copyDataFromArray() { return -1; }
};

#endif

