/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 */

#ifndef _PLAYSTATION_2_H
#define _PLAYSTATION_2_H

#include "MIPS64.h"

class Playstation2 : public MIPS64
{
public:
  Playstation2();
  virtual ~Playstation2();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int new_object(const char *object_name, int field_count);
  virtual int draw3d_Constructor_X(int type);
  virtual int draw3d_Constructor_I(int type);
  virtual int draw3d_rotate_III();
  virtual int draw3d_setPosition_III();
  virtual int draw3d_setPointPosition_IIII();
  virtual int draw3d_setPointColor_II();
  virtual int draw3d_draw();

  //virtual int playstation2_setVideoMode_III();
  //virtual int playstation2_setFrameBuffer1_IIIII();
  //virtual int playstation2_setFrameBuffer2_IIIII();
  //virtual int playstation2_setDisplay1_IIIIII();
  //virtual int playstation2_setDisplay2_IIIIII();
  virtual int playstation2_waitVsync();

private:
  void playstation2_addDMAReset();
  //virtual int playstation2_setFrameBuffer(int index);
  //virtual int playstation2_setDisplay(int index);
};

#endif

