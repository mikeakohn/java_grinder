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

#include <string.h>

#include "Util.h"

void Util::method_sanitize(std::string &method_name, std::string &method_sig, int &param_count)
{
  char data[method_sig.length() + 1];
  char *s = data + 1;

  memcpy(data, method_sig.c_str(), sizeof(data));

  param_count = 0;

  while(*s != ')' && *s != 0)
  {
    if (*s == 'L')
    {
      // Replace "Ljava/lang/String;" with "X"
      const int len = sizeof("Ljava/lang/String;") - 1;

      if (strncmp(s, "Ljava/lang/String;", len) == 0)
      {
        char *end = s + len;
        char *start = s + 1;

        *s = 'X';
        s++;

        while(true)
        {
          *start = *end;

          if (*end == 0) { break; }

          start++;
          end++;
        }

        continue;
      }
        else
      {
        while(*s != ';' && *s != 0)
        {
          // Replace '/' with '_'.
          if (*s == '/') { *s = '_'; }

          s++;
        }

        if (*s == ';')
        {
          // Replace ';' with '_'.
          *s = '_';
        }
          else
        if (*s == 0)
        {
          s--;
        }
      }
    }
      else
    if (*s == '[')
    {
      // Replace '[' (array) with 'a'.
      *s = 'a';

      s++;
    }

    param_count++;
    s++;
  }

  *s = 0;

  data[0] = '_';

  if (data[1] != 0 )
  {
    method_name += data;
  }
}

