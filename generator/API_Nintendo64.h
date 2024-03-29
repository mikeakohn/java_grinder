/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2023 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_API_NINTENDO64_H
#define JAVA_GRINDER_GENERATOR_API_NINTENDO64_H

class API_Nintendo64
{
public:
  virtual int nintendo64_waitVsync() { return -1; }
  virtual int nintendo64_clearScreen() { return -1; }
  virtual int nintendo64_resetZBuffer() { return -1; }
  virtual int nintendo64_setScreen_I() { return -1; }
  virtual int nintendo64_plot_II() { return -1; }
  virtual int nintendo64_plot_III() { return -1; }
  virtual int nintendo64_loadTexture_aSII() { return -1; }
  virtual int nintendo64_waitForPolygon() { return -1; }

  virtual int nintendo64_getAudioStatus() { return -1; }
  virtual int nintendo64_setAudioDACRate_I() { return -1; }
  virtual int nintendo64_setAudioBitRate_I() { return -1; }
  virtual int nintendo64_playAudio_aSII() { return -1; }

  virtual int nintendo64_n64_triangle_Constructor() { return -1; }
  virtual int nintendo64_n64_triangle_setColor_I() { return -1; }
  virtual int nintendo64_n64_triangle_setPosition_III() { return -1; }
  virtual int nintendo64_n64_triangle_setRotation_III() { return -1; }
  virtual int nintendo64_n64_triangle_setVertex0_III() { return -1; }
  virtual int nintendo64_n64_triangle_setVertex1_III() { return -1; }
  virtual int nintendo64_n64_triangle_setVertex2_III() { return -1; }
  virtual int nintendo64_n64_triangle_setVertexes_aS() { return -1; }
  virtual int nintendo64_n64_triangle_setVertexes_aSI() { return -1; }
  virtual int nintendo64_n64_triangle_setZBuffer_Z() { return -1; }
  virtual int nintendo64_n64_triangle_draw() { return -1; }

  virtual int nintendo64_n64_rectangle_Constructor() { return -1; }
  virtual int nintendo64_n64_rectangle_setColor_I() { return -1; }
  virtual int nintendo64_n64_rectangle_setPosition_II() { return -1; }
  virtual int nintendo64_n64_rectangle_setSize_II() { return -1; }
  virtual int nintendo64_n64_rectangle_setTextureEnabled_II() { return -1; }
  virtual int nintendo64_n64_rectangle_setTextureDisabled() { return -1; }
  virtual int nintendo64_n64_rectangle_draw() { return -1; }
};

#endif

