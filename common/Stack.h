/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_COMMON_STACK_H
#define JAVA_GRINDER_COMMON_STACK_H

#include <vector>

struct Stack
{
  Stack()
  {
  }

  void reset() { data.clear(); }

  int length() { return data.size(); }

  void push(uint32_t value) { data.push_back(value); }

  void push_float(float value) { data.push_back(value); }

  uint32_t pop()
  {
    uint32_t value = data.back().i;
    data.pop_back();
    return value;
  }

  float pop_float()
  {
    float value = data.back().f;
    data.pop_back();
    return value;
  }

private:
  union Data
  {
    Data(uint32_t value) { i = value; }
    Data(float value) { f = value; }

    uint32_t i;
    float f;
  };

  std::vector<Data> data;
};

#endif

