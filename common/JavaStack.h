/**
 *  Java Grinder
 *  Author: The Naken Crew
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2013 by The Naken Crew
 *
 */

#ifndef _JAVA_STACK_H
#define _JAVA_STACK_H

class JavaStack
{
public:
  JavaStack(int len);
  ~JavaStack();

  void *values;
  void *types;
  int len;
};

#endif

