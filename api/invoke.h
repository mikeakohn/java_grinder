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

#ifndef _INVOKE_H
#define _INVOKE_H

#include <string>

#include "Generator.h"
#include "JavaClass.h"

void get_signature(std::string &signature, int *params, int *is_void);

void get_static_function(
  char *function,
  std::string &method_name,
  std::string &method_sig);

#endif

