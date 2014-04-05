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

int TI84C::clearRect()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld b,c\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  call ClearRect\n");

  return 0;
}

int TI84C::drawHL()
{
  fprintf(out, "  call CenterPutS\n");

  return 0;
}

int TI84C::drawLine()
{
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  pop ix\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld b,l\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  ld hl,ix\n");
  fprintf(out, "  call ILine\n");

  return 0;
}

int TI84C::drawPoint()
{
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld a,l\n");
  fprintf(out, "  call IPoint\n");

  return 0;
}

int TI84C::drawString()
{
  fprintf(out, "  call PutS\n");

  return 0;
}

int TI84C::drawStringCenter()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  call CenterPutS\n");

  return 0;
}

int TI84C::fillRect()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  pop de\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  ld b,c\n");
  fprintf(out, "  pop bc\n");
  fprintf(out, "  call FillRect\n");

  return 0;
}

int TI84C::setCursorX()
{
  fprintf(out, "  pop af\n");
  fprintf(out, "  ld (curCol), a\n");

  return 0;
}

int TI84C::setCursorY()
{
  fprintf(out, "  pop af\n");
  fprintf(out, "  ld (curRow), a\n");

  return 0;
}

int TI84C::setDrawBGColor()
{
  fprintf(out, "  pop af\n");
  fprintf(out, "  ld (penBGColor), a\n");

  return 0;
}

int TI84C::setDrawBGWhite()
{
  fprintf(out, "  call SetPenBG_White\n");

  return 0;
}


int TI84C::setDrawColor()
{
  fprintf(out, "  pop af\n");
  fprintf(out, "  ld (penFGColor), a\n");

  return 0;
}

int TI84C::setTextBGColor()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (curBGColor), hl\n");

  return 0;
}

int TI84C::setTextColor()
{
  fprintf(out, "  pop hl\n");
  fprintf(out, "  ld (curFGColor), hl\n");

  return 0;
}


