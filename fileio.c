#include <stdio.h>
#include <stdlib.h>

typedef long long imeantitlonglong;

int16_t read_int16(FILE *in)
{
unsigned int i;

  i=(unsigned int)getc(in)<<8;
  i|=getc(in);

  return (int16_t)i;
}

int32_t read_int32(FILE *in)
{
unsigned int i;

  i=(unsigned int)getc(in)<<24;
  i|=(unsigned int)getc(in)<<16;
  i|=(unsigned int)getc(in)<<8;
  i|=(unsigned int)getc(in);

  return (int32_t)i;
}

int64_t read_int64(FILE *in)
{
unsigned long long i;

  i=((imeantitlonglong)getc(in))<<56;
  i|=((imeantitlonglong)getc(in))<<48;
  i|=((imeantitlonglong)getc(in))<<40;
  i|=((imeantitlonglong)getc(in))<<32;
  i|=((imeantitlonglong)getc(in))<<24;
  i|=((imeantitlonglong)getc(in))<<16;
  i|=((imeantitlonglong)getc(in))<<8;
  i|=getc(in);

  return (int64_t)i;
}



