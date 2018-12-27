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

#ifndef API_APPLEIIGS_H
#define API_APPLEIIGS_H

class API_AppleIIgs
{
public:
  // AppleIIgs
  virtual int appleiigs_plotChar_IC() { return -1; }
  virtual int appleiigs_printChar_C() { return -1; }
  virtual int appleiigs_hiresEnable_I() { return -1; }
  virtual int appleiigs_hiresClear_I() { return -1; }
  virtual int appleiigs_hiresUpdate_II() { return -1; }
  virtual int appleiigs_hiresPlot_III() { return -1; }
  virtual int appleiigs_hiresLine_IIIII() { return -1; }
  virtual int appleiigs_hiresSpan_IIII() { return -1; }
  virtual int appleiigs_hiresRead_II() { return -1; }
  virtual int appleiigs_hiresSprite_aBIIII() { return -1; }
  virtual int appleiigs_hiresPalette_IaI() { return -1; }
  virtual int appleiigs_hiresSetRow_II() { return -1; }
  virtual int appleiigs_rnd() { return -1; }

  // Sound Chip
  virtual int appleiigs_loadWaveTable_aBI() { return -1; }
  virtual int appleiigs_enableOscillators_I() { return -1; }
  virtual int appleiigs_setMasterVolume_I() { return -1; }
  virtual int appleiigs_setSoundVolume_II() { return -1; }
  virtual int appleiigs_setSoundFrequency_II() { return -1; }
  virtual int appleiigs_setWaveTable_III() { return -1; }
};

#endif

