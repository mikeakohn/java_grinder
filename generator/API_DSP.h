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

#ifndef _API_DSP_H
#define _API_DSP_H

class API_DSP
{
public:
  // DSP (dsPIC stuff)
  virtual int dsp_getA() { return -1; }
  virtual int dsp_getB() { return -1; }
  virtual int dsp_getUpperA() { return -1; }
  virtual int dsp_getUpperB() { return -1; }
  virtual int dsp_getLowerA() { return -1; }
  virtual int dsp_getLowerB() { return -1; }
  virtual int dsp_getRoundedA() { return -1; }
  virtual int dsp_getRoundedB() { return -1; }
  virtual int dsp_clearA() { return -1; }
  virtual int dsp_clearB() { return -1; }
  virtual int dsp_loadA() { return -1; }
  virtual int dsp_loadB() { return -1; }
  virtual int dsp_negA() { return -1; }
  virtual int dsp_negB() { return -1; }
  virtual int dsp_addABAndStoreInA() { return -1; }
  virtual int dsp_addABAndStoreInB() { return -1; }
  virtual int dsp_subABAndStoreInA() { return -1; }
  virtual int dsp_subBAAndStoreInB() { return -1; }
  virtual int dsp_addToA() { return -1; }
  virtual int dsp_addToB() { return -1; }
  virtual int dsp_squareToA() { return -1; }
  virtual int dsp_squareToB() { return -1; }
  virtual int dsp_mulToA() { return -1; }
  virtual int dsp_mulToB() { return -1; }
  //virtual int dsp_euclideanDistanceToA() { return -1; }
  //virtual int dsp_euclideanDistanceToB() { return -1; }
  virtual int dsp_squareAndAddToA() { return -1; }
  virtual int dsp_squareAndAddToB() { return -1; }
  virtual int dsp_mulAndAddToA() { return -1; }
  virtual int dsp_mulAndAddToB() { return -1; }
  virtual int dsp_mulAndSubFromA() { return -1; }
  virtual int dsp_mulAndSubFromB() { return -1; }
  //virtual int dsp_euclideanDistanceAndAddToA() { return -1; }
  //virtual int dsp_euclideanDistanceAndAddToB() { return -1; }
  virtual int dsp_shiftA() { return -1; }
  virtual int dsp_shiftB() { return -1; }
};

#endif

