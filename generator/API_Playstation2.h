/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#ifndef _API_PLAYSTATION2_H
#define _API_PLAYSTATION2_H

class API_Playstation2
{
public:
  virtual int playstation2_setVideoMode_III() { return -1; }
  virtual int playstation2_setVideoMode_III(int interlaced, int video_type, int frame) { return -1; }
  virtual int playstation2_setFrameBuffer1_IIIII() { return -1; }
  virtual int playstation2_setFrameBuffer1_IIIII(int base, int width, int pixel_format, int position_x, int position_y) { return -1; }
  virtual int playstation2_setDisplay1_IIIIII() { return -1; }
  virtual int playstation2_setDisplay1_IIIIII(int dx, int dy, int magh, int magv, int dw, int dh) { return -1; }
  virtual int playstation2_waitVsync() { return -1; }
};

#endif

