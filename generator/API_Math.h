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

#ifndef _API_MATH_H
#define _API_MATH_H

class API_Math
{
public:
  virtual int math_abs_I() { return -1; }
  virtual int math_xor_I() { return -1; }
  virtual int math_min_II() { return -1; }
  virtual int math_max_II() { return -1; }
};

#endif

