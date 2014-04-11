/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "cpu.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return ti84_##funct(java_class, generator); \
  }

#if 0
static int ti84_setClock16(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_setClock16();
}
#endif

static int ti84_clearScreen(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_clearScreen();
}

static int ti84_clearRect(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_clearRect();
}

static int ti84_drawLine(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_drawLine();
}

static int ti84_drawPoint(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_drawPoint();
}

static int ti84_print(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_print();
}

static int ti84_printCenter(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_printCenter();
}

static int ti84_printHL(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_printHL();
}

static int ti84_putc(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_putc();
}

static int ti84_fillRect(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_fillRect();
}

static int ti84_setCursorX(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_setCursorX();
}

static int ti84_setCursorY(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_setCursorY();
}

static int ti84_setDrawBGColor(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_setDrawBGColor();
}

static int ti84_setDrawBGWhite(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_setDrawBGWhite();
}

static int ti84_setDrawColor(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_setDrawColor();
}

static int ti84_setFillColor(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_setFillColor();
}

static int ti84_setTextBGColor(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_setTextBGColor();
}

static int ti84_setTextColor(JavaClass *java_class, Generator *generator)
{
  return generator->ti84_setTextColor();
}

int ti84(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(clearScreen)
  CHECK_FUNC(clearRect)
  CHECK_FUNC(drawLine)
  CHECK_FUNC(drawPoint)
  CHECK_FUNC(print)
  CHECK_FUNC(printCenter)
  CHECK_FUNC(printHL)
  CHECK_FUNC(putc)
  CHECK_FUNC(fillRect)
  CHECK_FUNC(setCursorX)
  CHECK_FUNC(setCursorY)
  CHECK_FUNC(setDrawBGColor)
  CHECK_FUNC(setDrawBGWhite)
  CHECK_FUNC(setDrawColor)
  CHECK_FUNC(setFillColor)
  CHECK_FUNC(setTextBGColor)
  CHECK_FUNC(setTextColor)

  return -1;
}



