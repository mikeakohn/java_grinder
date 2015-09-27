
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
      if (dy < 0) { j = -1; }

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
      if (dx < 0) { i = -1; }

      while (y0 != y1)
      {
        SegaGenesis.plot(x0, y0, c);

        y0 += j;
        count += dx;
        if (count > dy) { count -= dy; x0 += i; }
      }
    }
  }
}

