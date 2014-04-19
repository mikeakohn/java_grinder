/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#ifndef _COMPILER_H
#define _COMPILER_H

#include "Generator.h"

class Compiler
{
public:
  Compiler() : generator(NULL), optimize(true) { }
  virtual ~Compiler() { }

  void disable_optimizer() { optimize = false; }
  void set_generator(Generator *generator) { this->generator = generator; }

  virtual int load_class(const char *filename) = 0;
  virtual void insert_static_field_defines() = 0;
  virtual void init_heap() = 0;
  virtual int add_static_initializers() = 0;
  virtual int compile_methods(bool do_main) = 0;

  static int get_class_type(const char *filename);

  enum class_type
  {
    INVALID,
    JAVA,
    DOT_NET,
  };

protected:
  Generator *generator;
  bool optimize;
};

#endif

