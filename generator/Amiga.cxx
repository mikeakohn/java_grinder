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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "generator/Amiga.h"

// NOTE: a3 points to Amiga hardware registers.

Amiga::Amiga()
{
  //start_org = 0x8000;
}

Amiga::~Amiga()
{
  fprintf(out,
    ".align_bytes 4\n"
    "_amiga_grind_end:\n\n"
    "  ;; Hunk file end\n"
    "  dc32 0x0000_03f2\n\n"
  );

}

int Amiga::open(const char *filename)
{
  if (MC68000::open(filename) != 0) { return -1; }

  return 0;
}

int Amiga::start_init()
{
  // Add any set up items (stack, registers, etc).
  //MC68000::start_init();

  fprintf(out,
    ".include \"amiga/hardware.inc\"\n"
    ".include \"amiga/exec.inc\"\n"
  );

  fprintf(out,
    "\n"
    "  ;; Hunk file header\n"
    "  dc32 0x0000_03f3  ; magic_cookie\n"
    "  dc32 0x0000_0000  ; name_length\n"
    "  dc32 0x0000_0001  ; table_length\n"
    "  dc32 0x0000_0000  ; first_hunk\n"
    "  dc32 0x0000_0000  ; last_hunk\n"
    "  dc32 (_amiga_grind_end - _amiga_grind_start) / 4\n"
    "  dc32 0x0000_03e9  ; hunk_code\n\n"
    "  dc32 (_amiga_grind_end - _amiga_grind_start) / 4\n"
    "_amiga_grind_start:\n"
  );

  fprintf(out,
    "  ;; a3 points to custom chips.\n"
    "  movea.l #0xdff000, a3\n");

  return 0;
}

int Amiga::amiga_setPalette_II()
{
  fprintf(out, "  ;; amiga_setPalette_II()\n");
  fprintf(out, "  lea (COLOR00, a3), a2\n");
  fprintf(out, "  lsl.w #1, d%d\n", reg - 2);
  fprintf(out, "  move.w d%d, (0, a2, d%d)\n", reg - 1, reg - 2);

  reg -= 2;

  return 0;
}

int Amiga::amiga_disableMultitasking()
{
  fprintf(out,
    "  ;; amiga_disableMultitasking()\n"
    "  movea.l (ExecBase), a2\n"
    "  jsr (Forbid,a2)\n");

  return 0;
}

int Amiga::amiga_enableMultitasking()
{
  fprintf(out,
    "  ;; amiga_enableMultitasking()\n"
    "  movea.l (ExecBase), a2\n"
    "  jsr (Permit,a2)\n");

  return 0;
}

int Amiga::amiga_disableInterrupts()
{
  fprintf(out,
    "  ;; amiga_disableInterrupts()\n"
    "  movea.l (ExecBase), a2\n"
    "  jsr (Disable,a2)\n");

  return 0;
}

int Amiga::amiga_enableInterrupts()
{
  fprintf(out,
    "  ;; amiga_enableInterrupts()\n"
    "  movea.l (ExecBase), a2\n"
    "  jsr (Enable,a2)\n");

  return 0;
}

