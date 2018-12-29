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

#include "api/ti84.h"

// FIXME - strncmp?  why?

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return generator->ti84_##funct(); \
  }

int ti84(JavaClass *java_class, Generator *generator, const char *function)
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



