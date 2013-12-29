/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2013 by Michael Kohn
 *
 */

#ifndef _MSP430_H
#define _MSP430_H

struct msp430_t
{
  FILE *out;
};

int msp430_init(struct msp430_t *msp430);
void msp430_close(struct msp430_t *msp430);

#endif

