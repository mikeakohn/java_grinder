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

#ifndef JAVA_GRINDER_COMMON_UTIL_H
#define JAVA_GRINDER_COMMON_UTIL_H

#include <string>

class Util
{
public:
  static void method_sanitize(
    std::string &method_name,
    std::string &method_sig,
    int &param_count);

  static std::string get_virtual_function(
    std::string &method_name,
    std::string &method_sig,
    std::string &field_name,
    std::string &field_class);

private:
  Util();
  ~Util();
};

#endif

