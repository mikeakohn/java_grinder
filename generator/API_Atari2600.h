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

#ifndef _API_ATARI_2600_H
#define _API_ATARI_2600_H

class API_Atari2600
{
public:
  virtual int atari2600_waitHsync() { return -1; }
};

#endif

