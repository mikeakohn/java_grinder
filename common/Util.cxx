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

std::string Util::get_virtual_function(
  std::string &method_name,
  std::string &method_sig,
  std::string &field_name,
  std::string &field_class)
{
  const char *s = field_class.c_str();
  const int length = method_name.length() +
                     method_sig.length() +
                     field_name.length() +
                     field_class.length() +
                     4;
  char data[length];
  int ptr = 0;

  while(*s != 0)
  {
    if (*s == '/') { ptr = 0; s++; continue; }
    data[ptr++] = *s | 32;
    s++;
  }

  data[ptr++] = '_';
  s = field_name.c_str();

  while (*s != 0) { data[ptr++] = *s; s++; }

  data[ptr++] = '_';
  s = method_name.c_str();

  while (*s != 0) { data[ptr++] = *s; s++; }

  data[ptr++] = '_';
  s = method_sig.c_str() + 1;

  while(*s != 0)
  {
    if (*s == ')') { break; }
    data[ptr++] = *s;
    s++;
  }

  data[ptr] = 0;

  return std::string(data);
}

