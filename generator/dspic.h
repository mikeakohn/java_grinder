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

#ifndef _DSPIC_H
#define _DSPIC_H

struct dspic_t
{
  FILE *out;
};

int dspic_init(struct dspic_t *dspic);
void dspic_close(struct dspic_t *dspic);

#endif

