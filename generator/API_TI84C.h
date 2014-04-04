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

#ifndef _API_TI84C_H
#define _API_TI84C_H

class API_TI84C
{
public:
  // TI-84 plus C Color Calculator
  virtual int centerPutS() { return -1; }
  virtual int clearRect() { return -1; }
  virtual int dispHL() { return -1; }
  virtual int fillRect() { return -1; }
  virtual int iLine() { return -1; }
  virtual int iPoint() { return -1; }
  virtual int putS() { return -1; }
  virtual int setPenBGWhite() { return -1; }
};

#endif

