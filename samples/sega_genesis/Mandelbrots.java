
import net.mikekohn.java_grinder.SegaGenesis;

public class Mandelbrots
{
  public static void draw()
  {
    final int DEC_PLACE = 10;
    int x,y;
    int rs,is;
    int zi,zr;
    int tr,ti;
    int zr2,zi2;
    int count;
    final int r0 = (-2 << DEC_PLACE);
    final int i0 = (-1 << DEC_PLACE);
    final int r1 = (1 << DEC_PLACE);
    final int i1 = (1 << DEC_PLACE);
    int dx = (r1 - r0) / 320;
    int dy = (i1 - i0) / 224;

    is = i0;

    for (y = 0; y < 224; y++)
    {
      rs = r0;

      for (x = 0; x < 320; x++)
      {
        zr = 0;
        zi = 0;

        for (count = 0; count < 16; count++)
        {
          zr2 = (zr * zr) >> DEC_PLACE;
          zi2 = (zi * zi) >> DEC_PLACE;

          if (zr2 + zi2 > (4 << DEC_PLACE)) { break; }

          tr = zr2 - zi2;
          ti = 2 * ((zr * zi) >> DEC_PLACE);

          zr = tr + rs;
          zi = ti + is;
        }

        if (count == 16) { count = 15; }

        if (x > 8 && y > 8) // FIXME
        {
          SegaGenesis.plot(x, y, count);
        }

        rs += dx;
      }

      is += dy;
    }
  }
}

