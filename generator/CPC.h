/**
*  Java Grinder
*  Author: Michael Kohn
*   Email: mike@mikekohn.net
*     Web: http://www.mikekohn.net/
* License: GPL
*
* Copyright 2014-2016 by Michael Kohn
*
* CPC support by SRS  - https://github.com/deringenieur71
*
*/

#ifndef _CPC_H
#define _CPC_H

#include "Z80.h"

class CPC : public Z80
{
public:
PC();
virtual ~CPC();
virtual int open(const char *filename);
virtual int cpc_beep();
virtual int cpc_setTxtPen_I();
virtual int cpc_setTxtPaper_I();
virtual int cpc_setGraPen_I();
virtual int cpc_setGraPaper_I();
virtual int cpc_setBorderColor_I();
virtual int cpc_cls();
virtual int cpc_screen_I();
virtual int cpc_putChar_C();
virtual int cpc_setCursor_II();
virtual int cpc_plot_III();
virtual int cpc_draw_III();
virtual char cpc_readChar();

virtual int cpc_setTxtPen_I(int c);
virtual int cpc_setTxtPaper_I(int c);
virtual int cpc_setGraPen_I(int c);
virtual int cpc_setGraPaper_I(int c);
virtual int cpc_setBorderColor_I(int c);
virtual int cpc_screen_I(int c);
virtual int cpc_putChar_C(char c);


private:
//void insert_ldirvv(void);

//uint32_t need_plot_lores : 1;
};

#endif

