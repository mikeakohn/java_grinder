/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 * CPC support by Carsten Dost - https://github.com/deringenieur71
 *
 */

#ifndef _API_CPC_H
#define _API_CPC_H

class API_CPC
{
public:
  // CPC
  virtual int cpc_getFunction_IC() { return -1; }
};

#endif

