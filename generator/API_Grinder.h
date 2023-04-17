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

#ifndef JAVA_GRINDER_GENERATOR_API_GRINDER_H
#define JAVA_GRINDER_GENERATOR_API_GRINDER_H

class API_Grinder
{
public:
  virtual int grinder_largeJavaStack() { return -1; }
};

#endif

