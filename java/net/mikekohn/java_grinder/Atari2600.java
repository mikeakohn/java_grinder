/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

package net.mikekohn.java_grinder;

abstract public class Atari2600
{
  protected Atari2600()
  {
  }

  public static final int PLAYFIELD_BALL_SIZE_8 = 0x30;
  public static final int PLAYFIELD_BALL_SIZE_4 = 0x20;
  public static final int PLAYFIELD_BALL_SIZE_2 = 0x10;
  public static final int PLAYFIELD_BALL_SIZE_1 = 0x00;
  public static final int PLAYFIELD_HIGH_PRIORITY = 0x04;
  public static final int PLAYFIELD_SCORE_COLOR = 0x02;
  public static final int PLAYFIELD_REFLECT = 0x01;

  public static final int SWITCH_RESET = 0x01;
  public static final int SWITCH_SELECT = 0x02;
  public static final int SWITCH_COLOR = 0x08;
  public static final int SWITCH_DIFFICULTY_0 = 0x40;
  public static final int SWITCH_DIFFICULTY_1 = 0x80;

  public static void waitHsync(int lines) { }
  public static void waitHsync() { }
  public static void startVblank() { }
  public static void waitVblank() { }
  public static void startOverscan() { }
  public static void waitOverscan() { }
  public static void setColorPlayer0(int color) { }
  public static void setColorPlayer1(int color) { }
  public static void setColorPlayfield(int color) { }
  public static void setColorBackground(int color) { }

  public static void resetPlayer0() { }
  public static void resetPlayer1() { }
  public static void resetMissile0() { }
  public static void resetMissile1() { }
  public static void resetBall() { }
  public static void resetMissile0ToPlayer0On() { }
  public static void resetMissile1ToPlayer1On() { }
  public static void resetMissile0ToPlayer0Off() { }
  public static void resetMissile1ToPlayer1Off() { }
  public static void applyHorizontalMotion() { }
  public static void clearMotionRegisters() { }
  public static void clearCollisionLatches() { }

  public static void enableMissile0() { }
  public static void enableMissile1() { }
  public static void enableBall() { }
  public static void disableMissile0() { }
  public static void disableMissile1() { }
  public static void disableBall() { }

  public static void enablePlayer0Reflect() { }
  public static void enablePlayer1Reflect() { }
  public static void disablePlayer0Reflect() { }
  public static void disablePlayer1Reflect() { }

  public static void setPlayfieldMode(int mode) { }

  public static void setPlayfield0(byte value) { }
  public static void setPlayfield1(byte value) { }
  public static void setPlayfield2(byte value) { }

  public static void setPlayfieldData(byte[] data) { }
  public static void setPlayfieldLength(byte value) { }

  public static void setPlayer0Position(byte x, byte y) { }
  public static void setPlayer1Position(byte x, byte y) { }
  public static void setMissile0Position(byte x, byte y) { }
  public static void setMissile1Position(byte x, byte y) { }
  public static void setBallPosition(byte x, byte y) { }

  public static void setPlayer0Sprite(byte[] data) { }
  public static void setPlayer1Sprite(byte[] data) { }
  public static void setMissile0Sprite(byte[] data) { }
  public static void setMissile1Sprite(byte[] data) { }
  public static void setBallSprite(byte[] data) { }

  public static void drawScreen() { }

  public static boolean isCollisionMissile0Player1() { return false; }
  public static boolean isCollisionMissile0Player0() { return false; }
  public static boolean isCollisionMissile1Player0() { return false; }
  public static boolean isCollisionMissile1Player1() { return false; }
  public static boolean isCollisionPlayer0PlayField() { return false; }
  public static boolean isCollisionPlayer0Ball() { return false; }
  public static boolean isCollisionPlayer1PlayField() { return false; }
  public static boolean isCollisionPlayer1Ball() { return false; }
  public static boolean isCollisionMissile0Playfield() { return false; }
  public static boolean isCollisionMissile0Ball() { return false; }
  public static boolean isCollisionMissile1Playfield() { return false; }
  public static boolean isCollisionMissile1Ball() { return false; }
  public static boolean isCollisionBallPlayfield() { return false; }
  public static boolean isCollisionPlayer0Player1() { return false; }
  public static boolean isCollisionMissile0Missile1() { return false; }

  /** Audio sound type, 4 bit number */
  public static void setAudioControl0(byte value) { }

  /** Audio sound type, 4 bit number */
  public static void setAudioControl1(byte value) { }

  /** Audio frequency, 5 bit number */
  public static void setAudioFrequency0(byte value) { }

  /** Audio frequency, 5 bit number */
  public static void setAudioFrequency1(byte value) { }

  /** Audio volume, 4 bit number */
  public static void setAudioVolume0(byte value) { }

  /** Audio volume, 4 bit number */
  public static void setAudioVolume1(byte value) { }

  public static boolean isJoystick0Right() { return false; }
  public static boolean isJoystick0Left() { return false; }
  public static boolean isJoystick0Down() { return false; }
  public static boolean isJoystick0Up() { return false; }
  public static boolean isJoystick0ButtonDown() { return false; }
  public static boolean isJoystick1Right() { return false; }
  public static boolean isJoystick1Left() { return false; }
  public static boolean isJoystick1Down() { return false; }
  public static boolean isJoystick1Up() { return false; }
  public static boolean isJoystick1ButtonDown() { return false; }

  public static byte getSwitches() { return -1; }
  public static boolean isSwitchResetDown() { return false; }
  public static boolean isSwitchSelectDown() { return false; }
  public static boolean isSwitchColorSet() { return false; }
  public static boolean isSwitchDifficultyP0Down() { return false; }
  public static boolean isSwitchDifficultyP1Down() { return false; }

  public static void setScore0(byte value) { }
  public static void setScore1(byte value) { }

  public static void setTitlePos(int value) { }
  public static void setTitleColor(int value) { }
  public static void drawTitleScreen() { }

  /** Change ROM bank.  Must be a constant and requires concatinating
      more than 1 bin file together */
  public static void setBank(byte index) { }
}

