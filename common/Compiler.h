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

#ifndef _COMPILER_H
#define _COMPILER_H

#include "generator/Generator.h"

#define DEBUG_PRINT(a, ...) if (verbose == 1) { printf(a, ##__VA_ARGS__); }

class Compiler
{
public:
  Compiler() : generator(NULL), optimize(true), verbose(false) { }
  virtual ~Compiler() { }

  void disable_optimizer() { optimize = false; }
  void set_verbose() { verbose = true; }
  void set_generator(Generator *generator) { this->generator = generator; }

  virtual int load_class(const char *filename) = 0;
  virtual void insert_static_field_defines() = 0;
  virtual void init_heap() = 0;
  virtual int add_static_initializers() = 0;
  virtual int compile_methods(bool do_main) = 0;
  virtual int add_constants() = 0;

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
  bool verbose;
};

#endif

