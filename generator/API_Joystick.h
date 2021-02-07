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

#ifndef JAVA_GRINDER_GENERATOR_API_JOYSTICK_H
#define JAVA_GRINDER_GENERATOR_API_JOYSTICK_H

class API_Joystick
{
public:
  virtual int joystick_isRight_I() { return -1; }
  virtual int joystick_isRight_I(int index) { return -1; }
  virtual int joystick_isLeft_I() { return -1; }
  virtual int joystick_isLeft_I(int index) { return -1; }
  virtual int joystick_isDown_I() { return -1; }
  virtual int joystick_isDown_I(int index) { return -1; }
  virtual int joystick_isUp_I() { return -1; }
  virtual int joystick_isUp_I(int index) { return -1; }
  virtual int joystick_isButtonDown_0_I() { return -1; }
  virtual int joystick_isButtonDown_0_I(int index) { return -1; }
};

#endif

