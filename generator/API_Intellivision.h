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

#ifndef JAVA_GRINDER_GENERATOR_API_INTELLIVISION_H
#define JAVA_GRINDER_GENERATOR_API_INTELLIVISION_H

class API_Intellivision
{
public:
  virtual int intellivision_plot_II() { return -1; }
  virtual int intellivision_plot_III() { return -1; }
  virtual int intellivision_readDisplay_II() { return -1; }
  virtual int intellivision_waitForVerticalBlank() { return -1; }
  virtual int intellivision_setHorizontalDelay_I() { return -1; }
  virtual int intellivision_setVerticalDelay_I() { return -1; }
  virtual int intellivision_setVideoMode_I() { return -1; }
  virtual int intellivision_setColorStack_II() { return -1; }
  virtual int intellivision_getControllerLeft() { return -1; }
  virtual int intellivision_getControllerRight() { return -1; }
  virtual int intellivision_setSoundChannelPeriod_II() { return -1; }
  virtual int intellivision_setSoundChannelVolume_II() { return -1; }
  virtual int intellivision_setSoundEnvelopePeriod_I() { return -1; }
  virtual int intellivision_setSoundEnvelopeType_I() { return -1; }
  virtual int intellivision_setSoundNoisePeriod_I() { return -1; }
  virtual int intellivision_setSoundType_I() { return -1; }
};

#endif

