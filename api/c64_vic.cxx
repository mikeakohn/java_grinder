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
#include "compile.h"
#include "cpu.h"

#define CHECK_FUNC(funct) \
  if (strncmp(#funct, function, sizeof(#funct)-1) == 0) \
  { \
    return c64_vic_##funct(java_class, generator); \
  }

static int c64_vic_border(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_border();
}

static int c64_vic_background(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_background();
}

static int c64_vic_sprite_enable(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite_enable();
}

static int c64_vic_sprite_pos(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_sprite_pos();
}

static int c64_vic_poke_reg(JavaClass *java_class, Generator *generator)
{
  return generator->c64_vic_poke_reg();
}

int c64_vic(JavaClass *java_class, Generator *generator, char *function)
{
  CHECK_FUNC(border)
  CHECK_FUNC(background)
  CHECK_FUNC(sprite_enable)
  CHECK_FUNC(sprite_pos)
  CHECK_FUNC(poke_reg)

  return -1;
}
