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

#ifndef JAVA_GRINDER_GENERATOR_API_KEYBOARD_H
#define JAVA_GRINDER_GENERATOR_API_KEYBOARD_H

class API_Keyboard
{
public:
  virtual int keyboard_isKeyPressed_I() { return -1; }
  virtual int keyboard_isKeyPressed_I(int index) { return -1; }
  virtual int keyboard_currentKeyPressed() { return -1; }
};

#endif

