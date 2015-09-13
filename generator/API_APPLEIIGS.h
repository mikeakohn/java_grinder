/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#ifndef _API_APPLEIIGS_H
#define _API_APPLEIIGS_H

class API_AppleIIgs
{
public:
  // AppleIIgs
  virtual int appleiigs_plotChar_IC() { return -1; }
  virtual int appleiigs_printChar_C() { return -1; }
  virtual int appleiigs_hiresEnable() { return -1; }
  virtual int appleiigs_hiresPlot_II() { return -1; }
  virtual int appleiigs_hiresSpan_III() { return -1; }
  virtual int appleiigs_hiresRead_I() { return -1; }
  virtual int appleiigs_hiresBlit_aBIII() { return -1; }
  virtual int appleiigs_hiresPalette_IaI() { return -1; }
  virtual int appleiigs_hiresSetLine_II() { return -1; }

  // Sound Chip
  virtual int appleiigs_loadWaveTable_BA() { return -1; }
  virtual int appleiigs_enableOscillators_I() { return -1; }
  virtual int appleiigs_setMasterVolume_I() { return -1; }
  virtual int appleiigs_setSoundVolume_II() { return -1; }
  virtual int appleiigs_setSoundFrequency_II() { return -1; }
};

#endif

