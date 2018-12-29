/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2018 by Michael Kohn
 *
 * CPC support started by Carsten Dost - https://github.com/deringenieur71
 *
 */

#ifndef _API_CPC_H
#define _API_CPC_H

class API_CPC
{
public:
  // CPC with STACK
  
  virtual int cpc_beep() { return -1; }
  virtual int cpc_setTxtPen_I() { return -1; }
  virtual int cpc_setTxtPaper_I() { return -1; }
  virtual int cpc_setGraPen_I() { return -1; }
  virtual int cpc_setGraPaper_I() { return -1; }
  virtual int cpc_setBorderColor_I() { return -1; }
  virtual int cpc_cls() { return -1; }
  virtual int cpc_screen_I() { return -1; }
  virtual int cpc_putChar_C() { return -1; }
  virtual int cpc_setCursor_II() { return -1; }
  virtual int cpc_plot_III() { return -1; }
  virtual int cpc_draw_III() { return -1; }
  virtual char cpc_readChar() { return -1; }
  virtual int cpc_getVMEM_ICC() {return -1; }
  virtual int cpc_putSpriteMode0_IIII() { return -1; }
  virtual int cpc_printI_I() { return -1; }
  virtual int cpc_getTime() { return -1; }
  virtual int cpc_VSync() { return -1; }
  
  // CPC with CONST
  virtual int cpc_setTxtPen_I(int c) { return -1; }
  virtual int cpc_setTxtPaper_I(int c) { return -1; }
  virtual int cpc_setGraPen_I(int c) { return -1; }
  virtual int cpc_setGraPaper_I(int c) { return -1; }
  virtual int cpc_setBorderColor_I(int c) { return -1; }
  virtual int cpc_screen_I(int c) { return -1; }
  virtual int cpc_putChar_C(char c) { return -1; }
  
};
#endif
