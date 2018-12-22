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

void Util::method_sanitize(
  std::string &method_name,
  std::string &method_sig,
  int &param_count)
{
  const char *s = method_sig.c_str() + 1;
  char data[method_sig.length() + 1];
  char *d = data + 1;

  data[0] = '_';

  param_count = 0;

  while(*s != ')' && *s != 0)
  {
    if (*s == 'L')
    {
      // Replace "Ljava/lang/String;" with "X"
      const int len = sizeof("Ljava/lang/String;") - 1;

      if (strncmp(s, "Ljava/lang/String;", len) == 0)
      {
        *d = 'X';
        d++;

        s = s + len;
      }
        else
      {
        while(*s != ';' && *s != 0)
        {
          // Replace '/' with '_'.
          if (*s == '/')
          {
            *d = '_';
          }
            else
          {
            *d = *s;
          }

          s++;
          d++;
        }

        if (*s == ';')
        {
          // Replace ';' with '_'.
          *d = '_';
          d++;
        }
          else
        if (*s == 0)
        {
          s--;
        }
          else
        {
          *d = *s;
          d++;
        }

        s++;
      }

      param_count++;

      continue;
    }
      else
    if (*s == '[')
    {
      // Replace '[' (array) with 'a'.
      *d = 'a';
      s++;
      d++;
      continue;
    }

    *d = *s;
    s++;
    d++;

    param_count++;
  }

  *d = 0;

  if (data[1] != 0 )
  {
    method_name += data;
  }
}

