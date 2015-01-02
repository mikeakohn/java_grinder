/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "uart.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->uart_##funct(port); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->uart_##funct(port, const_val); \
  }

int uart(JavaClass *java_class, Generator *generator, char *method_name, int port)
{
  CHECK_FUNC(init,_I)
  CHECK_FUNC(send,_I)
  CHECK_FUNC(read,)
  CHECK_FUNC(isDataAvailable,)
  CHECK_FUNC(isSendReady,)

  return -1;
}

int uart(JavaClass *java_class, Generator *generator, char *method_name, int port, int const_val)
{
  CHECK_FUNC_CONST(init,_I)

  return -1;
}



