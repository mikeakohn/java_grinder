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
    return ti84c_##funct(java_class, generator); \
  }

#if 0
static int ti84c_setClock16(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_setClock16();
}
#endif

static int ti84c_clearRect(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_clearRect();
}

static int ti84c_drawHL(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_drawHL();
}

static int ti84c_drawLine(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_drawLine();
}

static int ti84c_drawPoint(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_drawPoint();
}

static int ti84c_drawString(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_drawString();
}

static int ti84c_drawStringCenter(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_drawStringCenter();
}

static int ti84c_fillRect(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_fillRect();
}

static int ti84c_setCursorX(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_setCursorX();
}

static int ti84c_setCursorY(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_setCursorY();
}

static int ti84c_setDrawBGColor(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_setDrawBGColor();
}

static int ti84c_setDrawBGWhite(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_setDrawBGWhite();
}

static int ti84c_setDrawColor(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_setDrawColor();
}

static int ti84c_setTextBGColor(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_setTextBGColor();
}

static int ti84c_setTextColor(JavaClass *java_class, Generator *generator)
{
  return generator->ti84c_setTextColor();
}

int ti84c(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(clearRect)
  CHECK_FUNC(drawHL)
  CHECK_FUNC(drawLine)
  CHECK_FUNC(drawPoint)
  CHECK_FUNC(drawString)
  CHECK_FUNC(drawStringCenter)
  CHECK_FUNC(fillRect)
  CHECK_FUNC(setCursorX)
  CHECK_FUNC(setCursorY)
  CHECK_FUNC(setDrawBGColor)
  CHECK_FUNC(setDrawBGWhite)
  CHECK_FUNC(setDrawColor)
  CHECK_FUNC(setTextBGColor)
  CHECK_FUNC(setTextColor)

  return -1;
}



