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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "uart.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return uart_##funct(java_class, generator, port); \
  }

static int uart_init_III(JavaClass *java_class, Generator *generator, int port)
{
  return generator->uart_init(port);
}

static int uart_send_B(JavaClass *java_class, Generator *generator, int port)
{
  return generator->uart_send(port);
}

static int uart_read(JavaClass *java_class, Generator *generator, int port)
{
  return generator->uart_read(port);
}

static int uart_isDataAvailable(JavaClass *java_class, Generator *generator, int port)
{
  return generator->uart_isDataAvailable(port);
}

static int uart_isSendReady(JavaClass *java_class, Generator *generator, int port)
{
  return generator->uart_isSendReady(port);
}

int uart(JavaClass *java_class, Generator *generator, char *function, int port)
{
  CHECK_FUNC(init_III)
  CHECK_FUNC(send_B)
  CHECK_FUNC(read)
  CHECK_FUNC(isDataAvailable)
  CHECK_FUNC(isSendReady)

  return -1;
}



