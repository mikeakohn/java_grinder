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

#ifndef STACK_H
#define STACK_H

struct _stack
{
  void reset() { ptr = 0; }
  int length() { return ptr; }
  void push(uint32_t value) { data[ptr++].i = value; }
  void push_float(float value) { data[ptr++].f = value; }
  uint32_t pop() { return data[--ptr].i; }
  float pop_float() { return data[--ptr].f; }

  int ptr;

private:
  union
  {
    uint32_t i;
    float f;
  } data[];
};

#endif

