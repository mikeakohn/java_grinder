
import net.mikekohn.java_grinder.TRS80Coco;

public class Mandelbrot
{
  static int[] color_table =
  {
    0x80, 0x8f, 0x9f, 0xaf, 0xbf, 0xcf, 0xdf, 0xef, 0xff
  };

  static public void run()
  {
    // Time to try a Mandelbrot
    final int DEC_PLACE = 4;
    int cursor = 1024;
    int rs,is;
    int zi,zr;
    int tr,ti;
    int zr2,zi2;
    int count;
    int color;
    int x,y;

    for (y = 0; y < 16; y++)
    {
      // (y1 - y0) / 16 * y
      is = (((2 << DEC_PLACE) * y) >> 4) - (1 << DEC_PLACE);

      for (x = 0; x < 32; x++)
      {
        // (x1 - x0) / 32 * x
        rs = (((3 << DEC_PLACE) * x) >> 5) - (2 << DEC_PLACE);

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

        count >>= 1;

        color = color_table[count];
        TRS80Coco.setText(cursor, color);

        cursor++;
      }
    }
  }
}

