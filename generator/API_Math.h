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

#ifndef _API_MATH_H
#define _API_MATH_H

class API_Math
{
public:
  virtual int math_abs_I() { return -1; }
  virtual int math_xor_I() { return -1; }
  virtual int math_min_II() { return -1; }
  virtual int math_max_II() { return -1; }
  virtual int math_sin512_I() { return -1; }
  virtual int math_cos512_I() { return -1; }
};

#endif

