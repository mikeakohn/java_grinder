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

#ifndef INVOKE_H
#define INVOKE_H

#include <string>

void get_signature(std::string &signature, int *params, int *is_void);

std::string get_static_function(
  std::string &method_name,
  std::string &method_sig);

#endif

