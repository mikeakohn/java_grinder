/**
*  Java Grinder
*  Author: Michael Kohn
*   Email: mike@mikekohn.net
*     Web: http://www.mikekohn.net/
* License: GPLv3
*
* Copyright 2014-2018 by Michael Kohn
*
* CPC support started by Carsten Dost  - https://github.com/deringenieur71
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/cpc.h"

#define CHECK_FUNC(funct,sig) \
if (strcmp(#funct#sig, method_name) == 0) \
{ \
return generator->cpc_##funct##sig(); \
}

#define CHECK_FUNC_CONST(funct,sig) \
if (strcmp(#funct#sig, method_name) == 0) \
{ \
return generator->cpc_##funct##sig(const_val); \
}

#define CHECK_FUNC_CONST_2(funct,sig) \
if (strcmp(#funct#sig, function) == 0) \
{ \
return generator->cpc_##funct##sig(const_val1, const_val2); \
}

int cpc(JavaClass *java_class, Generator *generator, const char *method_name)
{
  // call with variable
  CHECK_FUNC(beep,)
  CHECK_FUNC(setTxtPen,_I) 
  CHECK_FUNC(setTxtPaper,_I)
  CHECK_FUNC(setGraPen,_I)
  CHECK_FUNC(setGraPaper,_I)
  CHECK_FUNC(setBorderColor,_I)
  CHECK_FUNC(cls,)
  CHECK_FUNC(screen,_I)
  CHECK_FUNC(putChar,_C)
  CHECK_FUNC(setCursor,_II)
  CHECK_FUNC(plot,_III)
  CHECK_FUNC(draw,_III)
  CHECK_FUNC(readChar,)
  CHECK_FUNC(getVMEM,_ICC)
  CHECK_FUNC(putSpriteMode0,_IIII)
  CHECK_FUNC(printI,_I)
  CHECK_FUNC(getTime,)
  CHECK_FUNC(VSync,)
    
  return -1;
}

int cpc(JavaClass *java_class, Generator *generator, const char *method_name, int const_val)
{
  // call with one constant
  CHECK_FUNC_CONST(setTxtPen,_I) 
  CHECK_FUNC_CONST(setBorderColor,_I)
  CHECK_FUNC_CONST(setTxtPaper,_I)
  CHECK_FUNC_CONST(setGraPen,_I)
  CHECK_FUNC_CONST(setGraPaper,_I)
  CHECK_FUNC_CONST(screen,_I)
  CHECK_FUNC_CONST(putChar,_C)

  return -1;
}

int cpc(JavaClass *java_class, Generator *generator, const char *function, int const_val1, int const_val2)
{
   // call with two constants
  //CHECK_FUNC_CONST_2(someFunction,_II)
  return -1;
}

