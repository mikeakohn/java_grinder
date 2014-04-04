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

#include "TI84C.h"
#include "Z80.h"

TI84C::TI84C()
{
}

TI84C::~TI84C()
{
}

int TI84C::open(const char *filename)
{
  if (Z80::open(filename) != 0) { return -1; }

  // http://wikiti.brandonw.net/index.php?title=84PCSE:OS:Applications

  fprintf(out,
        "  ; Master Field\n"
	"  .db	0x80, 0x0F, 0x00, 0x00, 0x00, 0x00\n"
	"  ; Name\n"
	"  .db	0x80, 0x46, \"My App\"\n"
	"  ; Disable TI splash screen.\n"
	"  .db	0x80, 0x90\n"
	"  ; Revision\n"
	"  .db	0x80, 0x21, 0x00\n"
	"  ; Build\n"
	"  .db	0x80h, 0x31, 0x01\n"
	"  ; Pages\n"
	"  .db	0x80, 0x81, 0x01\n"
	"  ; Signing Key ID\n"
	"  .db	0x80, 0x12, 0x01, 0x0F\n"
	"  ; Date stamp.  Nothing ever checks this.\n"
	"  .db	0x03, 0x22, 0x09, 0x00\n"
	"  ; Date stamp signature.  Since nothing ever checks this.\n"
	"  .db	0x02, 0x00\n"
	"  ; Final field\n"
	"  .db	0x80, 0x70\n"
	"  ; TI just starts execution after the last field.\n"
  );

  return 0;
}

int TI84C::start_init()
{
  // Add any set up items (stack, registers, etc).
  //fprintf(out, ".org ???\n");
  fprintf(out, "start:\n");
  fprintf(out, "  ld SP, 0x8100\n");

  return 0;
}



