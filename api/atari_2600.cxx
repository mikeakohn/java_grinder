/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/joystick.h"

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

int atari_2600(JavaClass *java_class, Generator *generator, const char *method_name)
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

  CHECK_FUNC(setPlayfieldMode,_I)

  CHECK_FUNC(setPlayfield0,_B)
  CHECK_FUNC(setPlayfield1,_B)
  CHECK_FUNC(setPlayfield2,_B)

  CHECK_FUNC(setPlayfieldData,_aB)
  CHECK_FUNC(setPlayfieldLength,_B)

  CHECK_FUNC(setPlayer0Position,_BB)
  CHECK_FUNC(setPlayer1Position,_BB)
  CHECK_FUNC(setMissile0Position,_BB)
  CHECK_FUNC(setMissile1Position,_BB)
  CHECK_FUNC(setBallPosition,_BB)

  CHECK_FUNC(setPlayer0Sprite,_aB)
  CHECK_FUNC(setPlayer1Sprite,_aB)
  CHECK_FUNC(setMissile0Sprite,_aB)
  CHECK_FUNC(setMissile1Sprite,_aB)
  CHECK_FUNC(setBallSprite,_aB)

  CHECK_FUNC(drawScreen,)

  CHECK_FUNC(isCollisionMissile0Player1,)
  CHECK_FUNC(isCollisionMissile0Player0,)
  CHECK_FUNC(isCollisionMissile1Player0,)
  CHECK_FUNC(isCollisionMissile1Player1,)
  CHECK_FUNC(isCollisionPlayer0PlayField,)
  CHECK_FUNC(isCollisionPlayer0Ball,)
  CHECK_FUNC(isCollisionPlayer1PlayField,)
  CHECK_FUNC(isCollisionPlayer1Ball,)
  CHECK_FUNC(isCollisionMissile0Playfield,)
  CHECK_FUNC(isCollisionMissile0Ball,)
  CHECK_FUNC(isCollisionMissile1Playfield,)
  CHECK_FUNC(isCollisionMissile1Ball,)
  CHECK_FUNC(isCollisionBallPlayfield,)
  CHECK_FUNC(isCollisionPlayer0Player1,)
  CHECK_FUNC(isCollisionMissile0Missile1,)

  CHECK_FUNC(setAudioControl0,_B)
  CHECK_FUNC(setAudioControl1,_B)
  CHECK_FUNC(setAudioFrequency0,_B)
  CHECK_FUNC(setAudioFrequency1,_B)
  CHECK_FUNC(setAudioVolume0,_B)
  CHECK_FUNC(setAudioVolume1,_B)

  CHECK_FUNC(getSwitches,)
  CHECK_FUNC(isSwitchResetDown,)
  CHECK_FUNC(isSwitchSelectDown,)
  CHECK_FUNC(isSwitchColorSet,)
  CHECK_FUNC(isSwitchDifficultyP0Down,)
  CHECK_FUNC(isSwitchDifficultyP1Down,)

  CHECK_FUNC(setScore0,_B)
  CHECK_FUNC(setScore1,_B)

  CHECK_FUNC(setTitlePos,_I)
  CHECK_FUNC(setTitleColor,_I)
  CHECK_FUNC(drawTitleScreen,)

  CHECK_FUNC(setBank,_B)

  return -1;
}

int atari_2600(JavaClass *java_class, Generator *generator, const char *method_name, int const_val)
{
  CHECK_FUNC_CONST(waitHsync,_I)

  CHECK_FUNC_CONST(setColorPlayer0,_I)
  CHECK_FUNC_CONST(setColorPlayer1,_I)
  CHECK_FUNC_CONST(setColorPlayfield,_I)
  CHECK_FUNC_CONST(setColorBackground,_I)

  CHECK_FUNC_CONST(setPlayfieldMode,_I)

  CHECK_FUNC_CONST(setPlayfield0,_B)
  CHECK_FUNC_CONST(setPlayfield1,_B)
  CHECK_FUNC_CONST(setPlayfield2,_B)

  CHECK_FUNC_CONST(setAudioControl0,_B)
  CHECK_FUNC_CONST(setAudioControl1,_B)
  CHECK_FUNC_CONST(setAudioFrequency0,_B)
  CHECK_FUNC_CONST(setAudioFrequency1,_B)
  CHECK_FUNC_CONST(setAudioVolume0,_B)
  CHECK_FUNC_CONST(setAudioVolume1,_B)
  CHECK_FUNC_CONST(setBank,_B)

  return -1;
}

int atari_2600(JavaClass *java_class, Generator *generator, const char *function, int const_val1, int const_val2)
{
  //CHECK_FUNC_CONST_2(someFunction,_II)

  return -1;
}

