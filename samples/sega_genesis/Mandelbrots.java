
import net.mikekohn.java_grinder.SegaGenesis;

public class Mandelbrots
{
  public static void draw()
  {
    final int DEC_PLACE = 20;
    int x,y;
    int rs,is;
    int zi,zr;
    int tr,ti;
    int zr2,zi2;
    int count;

    for (y = 0; y < 224; y++)
    {
      is = (((2 << DEC_PLACE) * y) / 24) - (1 << DEC_PLACE);

      for (x = 0; x < 320; x++)
      {
        rs = (((3 << DEC_PLACE) * x) / 32) - (2 << DEC_PLACE);

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

        SegaGenesis.plot(x, y, (char)(count << 4));
      }
    }
  }
}

