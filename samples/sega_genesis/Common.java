
import net.mikekohn.java_grinder.SegaGenesis;

public class Common
{
  static public void wait(int frames)
  {
    int a;

    // 60 frames should be 1 second.
    for (a = 0; a < frames; a++)
    {
      while(!SegaGenesis.inVerticalBlank());
      while(SegaGenesis.inVerticalBlank());
    }
  }

  static public void drawLine(int x0, int y0, int x1, int y1, int c)
  {
    int count;
    int dx,dy;
    int i,j;

    if (x0 == x1)
    {
      if (y1 < y0) { j = -1; } else { j = 1; }

      while (y0 != y1)
      {
        SegaGenesis.plot(x0, y0, c);
        y0 += j;
      }

      return;
    }

    if (y0 == y1)
    {
      if (x1 < x0) { i = -1; } else { i = 1; }

      while (x0 != x1)
      {
        SegaGenesis.plot(x0, y0, c);
        x0 += i;
      }

      return;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    i = dx;
    j = dy;

    if (i < 0) { i = -i; }
    if (j < 0) { j = -j; }

    count = 0;

    if (i > j)
    {
      // If the change in X is bigger, move the line horizontal

      i = 1; j = 1;

      if (dx < 0) { dx = -dx; i = -1; }
      if (dy < 0) { dy = -dy; j = -1; }

      count = dx >> 1;

      while (x0 != x1)
      {
        SegaGenesis.plot(x0, y0, c);

        x0 += i;
        count += dy;
        if (count > dx) { count -= dx; y0 += j; }
      }
    }
      else
    {
      // If the change in Y is bigger, move the line vertical

      i = 1; j = 1;

      if (dy < 0) { dy = -dy; j = -1; }
      if (dx < 0) { dx = -dx; i = -1; }

      count = dy >> 1;

      while (y0 != y1)
      {
        SegaGenesis.plot(x0, y0, c);

        y0 += j;
        count += dx;
        if (count > dy) { count -= dy; x0 += i; }
      }
    }
  }

  static short[] cos =
  {
      64,  63,  63,  62,  61,  60,  58,  56,  54,  51,
      49,  46,  42,  39,  35,  32,  28,  24,  19,  15,
      11,   6,   2,  -3,  -7, -12, -16, -20, -24, -28,
     -32, -36, -40, -43, -46, -49, -52, -55, -57, -59,
     -61, -62, -63, -64, -64, -64, -64, -64, -63, -62,
     -61, -59, -57, -55, -52, -50, -47, -43, -40, -36,
     -33, -29, -25, -20, -16, -12,  -7,  -3,   2,   6,
      10,  15,  19,  23,  27,  31,  35,  39,  42,  45,
      48,  51,  54,  56,  58,  60,  61,  62,  63,  63,
  };

  static short[] sin =
  {
       0,   4,   8,  13,  17,  21,  26,  30,  33,  37,
      41,  44,  47,  50,  53,  55,  57,  59,  60,  62,
      63,  63,  63,  63,  63,  63,  62,  60,  59,  57,
      55,  53,  50,  47,  44,  41,  37,  33,  30,  26,
      21,  17,  13,   9,   4,   0,  -5,  -9, -14, -18,
     -22, -26, -30, -34, -38, -42, -45, -48, -51, -53,
     -56, -58, -60, -61, -63, -64, -64, -64, -64, -64,
     -64, -63, -61, -60, -58, -56, -54, -51, -48, -45,
     -42, -38, -35, -31, -27, -23, -18, -14, -10,  -5,
  };
}

