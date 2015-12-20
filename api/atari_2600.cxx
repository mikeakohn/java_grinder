/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "atari_2600.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->atari2600_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->atari2600_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->atari2600_##funct##sig(const_val1, const_val2); \
  }

int atari_2600(JavaClass *java_class, Generator *generator, char *method_name)
{
  CHECK_FUNC(waitHsync,_I)
  CHECK_FUNC(waitHsync,)
  CHECK_FUNC(startVblank,)
  CHECK_FUNC(waitVblank,)
  CHECK_FUNC(startOverscan,)
  CHECK_FUNC(waitOverscan,)
  CHECK_FUNC(setColorPlayer0,_I)
  CHECK_FUNC(setColorPlayer1,_I)
  CHECK_FUNC(setColorPlayfield,_I)
  CHECK_FUNC(setColorBackground,_I)

  CHECK_FUNC(resetPlayer0,)
  CHECK_FUNC(resetPlayer1,)
  CHECK_FUNC(resetMissile0,)
  CHECK_FUNC(resetMissile1,)
  CHECK_FUNC(resetBall,)
  CHECK_FUNC(resetMissile0ToPlayer0On,)
  CHECK_FUNC(resetMissile1ToPlayer1On,)
  CHECK_FUNC(resetMissile0ToPlayer0Off,)
  CHECK_FUNC(resetMissile1ToPlayer1Off,)
  CHECK_FUNC(applyHorizontalMotion,)
  CHECK_FUNC(clearMotionRegisters,)
  CHECK_FUNC(clearCollisionLatches,)

  CHECK_FUNC(enableMissile0,)
  CHECK_FUNC(enableMissile1,)
  CHECK_FUNC(enableBall,)
  CHECK_FUNC(disableMissile0,)
  CHECK_FUNC(disableMissile1,)
  CHECK_FUNC(disableBall,)

  CHECK_FUNC(enablePlayer0Reflect,)
  CHECK_FUNC(enablePlayer1Reflect,)
  CHECK_FUNC(disablePlayer0Reflect,)
  CHECK_FUNC(disablePlayer1Reflect,)

  CHECK_FUNC(setPlayfield0,_B)
  CHECK_FUNC(setPlayfield1,_B)
  CHECK_FUNC(setPlayfield2,_B)

  return -1;
}

int atari_2600(JavaClass *java_class, Generator *generator, char *method_name, int const_val)
{
  CHECK_FUNC_CONST(waitHsync,_I)

  CHECK_FUNC_CONST(setColorPlayer0,_I)
  CHECK_FUNC_CONST(setColorPlayer1,_I)
  CHECK_FUNC_CONST(setColorPlayfield,_I)
  CHECK_FUNC_CONST(setColorBackground,_I)

  CHECK_FUNC_CONST(setPlayfield0,_B)
  CHECK_FUNC_CONST(setPlayfield1,_B)
  CHECK_FUNC_CONST(setPlayfield2,_B)

  return -1;
}

int atari_2600(JavaClass *java_class, Generator *generator, char *function, int const_val1, int const_val2)
{
  //CHECK_FUNC_CONST_2(someFunction,_II)

  return -1;
}

