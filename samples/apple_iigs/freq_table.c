// frequency table generator for Apple IIgs sound

#include <stdio.h>

int main()
{
  // bass guitar low E
  double hz = 41.20;

  // frequency depends on number of voices,
  // seems to mess up if greater than 16
  int voices = 16;

  printf("\n");

  int i;
  for(i = 0; i <= 48; i++)
  {
    double num = (131072.0 * hz) / (894886.0 / (voices + 2));
    printf("%d, ", (int)num);
    hz *= 1.059;
  }

  printf("\n");
  
  return 0;
}

