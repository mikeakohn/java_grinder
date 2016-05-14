/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

#ifndef _API_PLAYSTATION2_H
#define _API_PLAYSTATION2_H

class API_Playstation2
{
public:
  virtual int playstation2_waitVsync() { return -1; }
};

#endif

