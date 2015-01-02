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

#ifndef _STACK_H
#define _STACK_H

struct _stack
{
  void reset() { ptr = 0; }
  int length() { return ptr; }
  void push(uint32_t value) { data[ptr++] = value; }
  uint32_t pop() { return data[--ptr]; }

  int ptr;
  uint32_t data[];
};

#endif

