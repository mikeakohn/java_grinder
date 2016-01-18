
import net.mikekohn.java_grinder.TRS80Coco;

public class JavaTrs80Demo
{
  static public void main(String args[])
  {
    int a,n,x,y;
    int dx,dy;

    //TRS80Coco.setBackgroundColor(5);
    TRS80Coco.setText(1024, 0x8f);
    TRS80Coco.setText(1025, 0x9f);
    TRS80Coco.setText(1026, 0xaf);
    TRS80Coco.setText(1027, 0xbf);
    TRS80Coco.setText(1028, 0xcf);
    TRS80Coco.setText(1029, 0xdf);
    TRS80Coco.setText(1030, 0xef);
    TRS80Coco.setText(1031, 0xff);
    TRS80Coco.setText(1032, 0x80);

    for (a = 1; a < 10; a++)
    {
      TRS80Coco.plot(a, a, TRS80Coco.COLOR_ORANGE);
    }

    dx = 1; dy = 1;
    x = 20; y = 10;

    for (a = 1; a < 50; a++)
    {
      TRS80Coco.setText(y * 32 + x + 1024, TRS80Coco.COLOR_RED);
      for (n = 0; n < 5000; n++);
      TRS80Coco.setText(y * 32 + x + 1024, TRS80Coco.COLOR_GREEN);

      x += dx;
      y += dy;

      if (x <= 20) { dx = 1; }
      if (y <= 10) { dy = 1; }
      if (x >= 31) { dx = -1; }
      if (y >= 15) { dy = -1; }
    }

    // Time to try a Mandelbrot
    final int DEC_PLACE = 4;
    int cursor = 1024;
    int rs,is;
    int zi,zr;
    int tr,ti;
    int zr2,zi2;
    int count;
    int color;

    for (y = 0; y < 16; y++)
    {
      is = (((2 << DEC_PLACE) * y) >> 4) - (1 << DEC_PLACE);

      for (x = 0; x < 32; x++)
      {
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

        // Change this to a lookup table.
        //if (count == 0) { TRS80Coco.setText(cursor, TRS80Coco.COLOR_BLACK); }
        //else { TRS80Coco.setText(cursor, (((count >> 1) + 7) << 4) | 0xf); }
        count >>= 1;

        if (count == 0) { color = TRS80Coco.COLOR_BLACK; }
        else if (count == 1) { color = TRS80Coco.COLOR_GREEN; }
        else if (count == 2) { color = TRS80Coco.COLOR_YELLOW; }
        else if (count == 3) { color = TRS80Coco.COLOR_BLUE; }
        else if (count == 4) { color = TRS80Coco.COLOR_RED; }
        else if (count == 5) { color = TRS80Coco.COLOR_WHITE; }
        else if (count == 6) { color = TRS80Coco.COLOR_CYAN; }
        else if (count == 7) { color = TRS80Coco.COLOR_PURPLE; }
        else { color = TRS80Coco.COLOR_ORANGE; }

        TRS80Coco.setText(cursor, color);

        cursor++;
      }
    }

    while(true);
  }
}

