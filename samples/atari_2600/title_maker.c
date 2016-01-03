// outputs the java code needed for the title screen
#include <stdio.h>

// define graphics here, 40x16
static const char *graphics =
{
  "  xxxxx xxxxxx    xxxx     xxxxx xxxxxx "
  " xx     xx   xx  xx  xx   xx     xx     "
  "  xxxx  xxxxxx  xxxxxxxx xx      xxxxxx "
  "     xx xx      xx    xx xxx     xx     "
  " xxxxxx xx      xx    xx xxxxxxx xxxxxx "
  " xxxxxx xx      xx    xx  xxxxxx xxxxxx "
  " xxxxx  xx      xx    xx   xxxxx xxxxxx "
  "                                        "
  " xxxx  xxxx xx xx xxxx xx  x  xxx  xxxx "
  " xx xx xx   xx xx xx   xxx x xxxxx xx   "
  " xx xx xxxx xx xx xxxx x x x xx    xxxx "
  " xxxx  xx   xx xx xx   x x x xx xx xx   "
  " xx xx xxxx xxxxx xxxx x xxx xx  x xxxx "
  " xx xx xxxx  xxx  xxxx x xxx xxxxx xxxx "
  " xx xx xxxx   x   xxxx x  xx  xxx  xxxx "
  "                                        "
};

int main()
{
  int x, y, val, count;

  printf("\n");

  // playfield 0
  count = 0;

  printf("  public static byte pf_left0[] =\n  {\n    ");

  for(y = 0; y < 16; y++)
  {
    val = 0;

    for(x = 3; x >= 0; x--)
    {
      if(graphics[x + 40 * (15 - y)] != ' ')
        val |= 1;
      val <<= 1;
    }

    val <<= 3;
    printf("%d, ", (char)val);
    if((count++ & 7) == 7 && y < 15)
      printf("\n    ");
  }

  printf("\n  };\n\n");
  
  // playfield 1
  count = 0;

  printf("  public static byte pf_left1[] =\n  {\n    ");

  for(y = 0; y < 16; y++)
  {
    val = 0;

    for(x = 4; x < 12; x++)
    {
      if(graphics[x + 40 * (15 - y)] != ' ')
        val |= 1;
      val <<= 1;
    }

    val >>= 1;

    printf("%d, ", (char)val);
    if((count++ & 7) == 7 && y < 15)
      printf("\n    ");
  }

  printf("\n  };\n\n");
  
  // playfield 2
  count = 0;

  printf("  public static byte pf_left2[] =\n  {\n    ");

  for(y = 0; y < 16; y++)
  {
    val = 0;

    for(x = 19; x >= 12; x--)
    {
      if(graphics[x + 40 * (15 - y)] != ' ')
        val |= 1;
      val <<= 1;
    }

    val >>= 1;

    printf("%d, ", (char)val);
    if((count++ & 7) == 7 && y < 15)
      printf("\n    ");
  }

  printf("\n  };\n\n");
  
  // playfield 3
  count = 0;

  printf("  public static byte pf_right0[] =\n  {\n    ");

  for(y = 0; y < 16; y++)
  {
    val = 0;

    for(x = 23; x >= 20; x--)
    {
      if(graphics[x + 40 * (15 - y)] != ' ')
        val |= 1;
      val <<= 1;
    }

    val <<= 3;
    printf("%d, ", (char)val);
    if((count++ & 7) == 7 && y < 15)
      printf("\n    ");
  }

  printf("\n  };\n\n");
  
  // playfield 4
  count = 0;

  printf("  public static byte pf_right1[] =\n  {\n    ");

  for(y = 0; y < 16; y++)
  {
    val = 0;

    for(x = 24; x < 32; x++)
    {
      if(graphics[x + 40 * (15 - y)] != ' ')
        val |= 1;
      val <<= 1;
    }

    val >>= 1;

    printf("%d, ", (char)val);
    if((count++ & 7) == 7 && y < 15)
      printf("\n    ");
  }

  printf("\n  };\n\n");
  
  // playfield 5
  count = 0;

  printf("  public static byte pf_right2[] =\n  {\n    ");

  for(y = 0; y < 16; y++)
  {
    val = 0;

    for(x = 39; x >= 32; x--)
    {
      if(graphics[x + 40 * (15 - y)] != ' ')
        val |= 1;
      val <<= 1;
    }

    val >>= 1;

    printf("%d, ", (char)val);
    if((count++ & 7) == 7 && y < 15)
      printf("\n    ");
  }

  printf("\n  };\n\n");
  
  return 0;
}

