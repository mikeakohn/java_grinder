/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/cpu.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return c64_vic_##funct(java_class, generator); \
  }

static int c64_vic_sprite0pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite0pos();
}

static int c64_vic_sprite1pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite1pos();
}

static int c64_vic_sprite2pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite2pos();
}

static int c64_vic_sprite3pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite3pos();
}

static int c64_vic_sprite4pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite4pos();
}

static int c64_vic_sprite5pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite5pos();
}

static int c64_vic_sprite6pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite6pos();
}

static int c64_vic_sprite7pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite7pos();
}

static int c64_vic_writeControl1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_writeControl1();
}

static int c64_vic_readControl1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_readControl1();
}

static int c64_vic_waitRaster(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_waitRaster();
}

static int c64_vic_spriteEnable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_spriteEnable();
}

static int c64_vic_writeControl2(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_writeControl2();
}

static int c64_vic_readControl2(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_readControl2();
}

static int c64_vic_spriteExpandY(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_spriteExpandY();
}

static int c64_vic_writePointer(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_writePointer();
}

static int c64_vic_readPointer(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_readPointer();
}

static int c64_vic_writeInterruptStatus(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_writeInterruptStatus();
}

static int c64_vic_readInterruptStatus(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_readInterruptStatus();
}

static int c64_vic_interruptControl(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_interruptControl();
}

static int c64_vic_spritePriority(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_spritePriority();
}

static int c64_vic_spriteMulticolorEnable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_spriteMulticolorEnable();
}

static int c64_vic_spriteExpandX(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_spriteExpandX();
}

static int c64_vic_spriteCollision(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_spriteCollision();
}

static int c64_vic_dataCollision(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_dataCollision();
}

static int c64_vic_border(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_border();
}

static int c64_vic_background(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_background();
}

static int c64_vic_multicolor1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_multicolor1();
}

static int c64_vic_multicolor2(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_multicolor2();
}

static int c64_vic_multicolor3(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_multicolor3();
}

static int c64_vic_spriteMulticolor0(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_spriteMulticolor0();
}

static int c64_vic_spriteMulticolor1(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_spriteMulticolor1();
}

static int c64_vic_sprite0color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite0color();
}

static int c64_vic_sprite1color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite1color();
}

static int c64_vic_sprite2color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite2color();
}

static int c64_vic_sprite3color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite3color();
}

static int c64_vic_sprite4color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite4color();
}

static int c64_vic_sprite5color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite5color();
}

static int c64_vic_sprite6color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite6color();
}

static int c64_vic_sprite7color(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite7color();
}

static int c64_vic_hiresEnable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_hiresEnable();
}

static int c64_vic_hiresClear(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_hiresClear();
}

static int c64_vic_hiresPlot(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_hiresPlot();
}

static int c64_vic_makeHiresTables(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_makeHiresTables();
}

static int c64_vic_textEnable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textEnable();
}

static int c64_vic_textClear(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textClear();
}

static int c64_vic_textCopy(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textCopy();
}

static int c64_vic_textPlot(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textPlot();
}

static int c64_vic_textAsciiPlot(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textAsciiPlot();
}

static int c64_vic_textRead(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textRead();
}

static int c64_vic_textString(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textString();
}

static int c64_vic_textScrollLeft(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textScrollLeft();
}

static int c64_vic_textScrollRight(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textScrollRight();
}

static int c64_vic_textScrollUp(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textScrollUp();
}

static int c64_vic_textScrollDown(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_textScrollDown();
}

static int c64_vic_makeTextTable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_makeTextTable();
}

static int c64_vic_makeColorTable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_makeColorTable();
}

static int c64_vic_colorRamClear(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_colorRamClear();
}

static int c64_vic_copyUppercase(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_copyUppercase();
}

static int c64_vic_copyLowercase(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_copyLowercase();
}

int c64_vic(JavaClass *java_class, Generator *generator, const char *function)
{
  CHECK_FUNC(sprite0pos)
  CHECK_FUNC(sprite1pos)
  CHECK_FUNC(sprite2pos)
  CHECK_FUNC(sprite3pos)
  CHECK_FUNC(sprite4pos)
  CHECK_FUNC(sprite5pos)
  CHECK_FUNC(sprite6pos)
  CHECK_FUNC(sprite7pos)
  CHECK_FUNC(writeControl1)
  CHECK_FUNC(readControl1)
  CHECK_FUNC(waitRaster)
  CHECK_FUNC(spriteEnable)
  CHECK_FUNC(writeControl2)
  CHECK_FUNC(readControl2)
  CHECK_FUNC(spriteExpandY)
  CHECK_FUNC(writePointer)
  CHECK_FUNC(readPointer)
  CHECK_FUNC(writeInterruptStatus)
  CHECK_FUNC(readInterruptStatus)
  CHECK_FUNC(interruptControl)
  CHECK_FUNC(spritePriority)
  CHECK_FUNC(spriteMulticolorEnable)
  CHECK_FUNC(spriteExpandX)
  CHECK_FUNC(spriteCollision)
  CHECK_FUNC(dataCollision)
  CHECK_FUNC(border)
  CHECK_FUNC(background)
  CHECK_FUNC(multicolor1)
  CHECK_FUNC(multicolor2)
  CHECK_FUNC(multicolor3)
  CHECK_FUNC(spriteMulticolor0)
  CHECK_FUNC(spriteMulticolor1)
  CHECK_FUNC(sprite0color)
  CHECK_FUNC(sprite1color)
  CHECK_FUNC(sprite2color)
  CHECK_FUNC(sprite3color)
  CHECK_FUNC(sprite4color)
  CHECK_FUNC(sprite5color)
  CHECK_FUNC(sprite6color)
  CHECK_FUNC(sprite7color)
  CHECK_FUNC(hiresEnable)
  CHECK_FUNC(hiresClear)
  CHECK_FUNC(hiresPlot)
  CHECK_FUNC(makeHiresTables)
  CHECK_FUNC(textEnable)
  CHECK_FUNC(textClear)
  CHECK_FUNC(textCopy)
  CHECK_FUNC(textPlot)
  CHECK_FUNC(textAsciiPlot)
  CHECK_FUNC(textRead)
  CHECK_FUNC(textString)
  CHECK_FUNC(textScrollLeft)
  CHECK_FUNC(textScrollRight)
  CHECK_FUNC(textScrollUp)
  CHECK_FUNC(textScrollDown)
  CHECK_FUNC(makeTextTable)
  CHECK_FUNC(makeColorTable)
  CHECK_FUNC(colorRamClear)
  CHECK_FUNC(copyUppercase)
  CHECK_FUNC(copyLowercase)

  return -1;
}
