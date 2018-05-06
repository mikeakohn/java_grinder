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

#ifndef _API_DRAW_3D_H
#define _API_DRAW_3D_H

#define DRAW3D_TYPE_POINT equ 0
#define DRAW3D_TYPE_LINE equ 1
#define DRAW3D_TYPE_LINE_STRIP equ 2
#define DRAW3D_TYPE_TRIANGLE equ 3
#define DRAW3D_TYPE_TRIANGLE_STRIP equ 4
#define DRAW3D_TYPE_TRIANGLE_FAN equ 5
#define DRAW3D_TYPE_SPRITE equ 6

class API_Draw3D
{
public:
  virtual int draw3d_object_Constructor_X(int type, bool with_texture) { return -1; }
  virtual int draw3d_object_Constructor_I(int type, bool with_texture) { return -1; }
  virtual int draw3d_object_setContext_I() { return -1; }
  virtual int draw3d_object_rotateX512_I() { return -1; }
  virtual int draw3d_object_rotateY512_I() { return -1; }
  virtual int draw3d_object_rotateZ512_I() { return -1; }
  virtual int draw3d_object_setPosition_FFF() { return -1; }
  virtual int draw3d_object_setPoint_IFFF() { return -1; }
  virtual int draw3d_object_setPointColor_II() { return -1; }
  virtual int draw3d_object_setPoints_aF() { return -1; }
  virtual int draw3d_object_setPointColors_aI() { return -1; }
  virtual int draw3d_object_with_texture_setPoint_IFFF() { return -1; }
  virtual int draw3d_object_with_texture_setPointColor_II() { return -1; }
  virtual int draw3d_object_with_texture_setPoints_aF() { return -1; }
  virtual int draw3d_object_with_texture_setPointColors_aI() { return -1; }
  virtual int draw3d_object_setTextureCoord_IFF() { return -1; }
  virtual int draw3d_object_setTextureCoords_aF() { return -1; }
  virtual int draw3d_object_disableGouraudShading() { return -1; }
  virtual int draw3d_object_enableGouraudShading() { return -1; }
  virtual int draw3d_object_disableFogging() { return -1; }
  virtual int draw3d_object_enableFogging() { return -1; }
  virtual int draw3d_object_disableTexture() { return -1; }
  virtual int draw3d_object_enableTexture() { return -1; }
  virtual int draw3d_object_disableAlphaBlending() { return -1; }
  virtual int draw3d_object_enableAlphaBlending() { return -1; }
  virtual int draw3d_object_disableAntialiasing() { return -1; }
  virtual int draw3d_object_enableAntialiasing() { return -1; }
  virtual int draw3d_object_draw() { return -1; }

  virtual int draw3d_texture_Constructor_II(int size) { return -1; }
  virtual int draw3d_texture16_setPixel_II() { return -1; }
  virtual int draw3d_texture16_setPixels_IaS() { return -1; }
  virtual int draw3d_texture16_setPixelsRLE16_IaB() { return -1; }
  virtual int draw3d_texture24_setPixel_II() { return -1; }
  virtual int draw3d_texture24_setPixels_IaI() { return -1; }
  virtual int draw3d_texture32_setPixel_II() { return -1; }
  virtual int draw3d_texture32_setPixels_IaI() { return -1; }
  virtual int draw3d_texture_enableTransparency() { return -1; }
  virtual int draw3d_texture_enableTransparencyOnBlack() { return -1; }
  virtual int draw3d_texture_disableTransparency() { return -1; }
  virtual int draw3d_texture_upload() { return -1; }
};

#endif

