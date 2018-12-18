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

#ifndef _UTIL_H
#define _UTIL_H

class Util
{
public:
  static void method_sanitize(char *method_name, char *method_sig, int &param_count);

private:
  Util();
  ~Util();
};

#endif

