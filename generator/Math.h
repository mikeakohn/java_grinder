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

#ifndef _MATH_H
#define _MATH_H

class Math
{
public:
  static void add_sin_table(FILE *out);
  static void add_cos_table(FILE *out);

private:
  Math() { }
  ~Math() { }
};

#endif

