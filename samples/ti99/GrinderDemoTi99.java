
import net.mikekohn.java_grinder.TI99;

public class GrinderDemoTi99
{
  static byte[] sprite_j =
  {
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,  -32,  112,  112,   56,   28,   15,
     -4,   -4,  112,  112,  112,  112,  112,  112,
    112,  112,  112,  112,  112,  -32,  -64,  -128,
  };

  static byte[] sprite_a =
  {
     15,   60,   60,  120,  120,  -16,  -16,   -1,
     -1,  -16,  -16,  -16,  -16,  -16,  -16,  -16,
   -128,  -32,  -32,  -16,  -16,  120,  120,   -8,
     -8,  120,  120,  120,  120,  120,  120,  120,
  };

  static byte[] sprite_v =
  {
    -16,  -16,  -16,  -16,  -16,  -16,  -16,  -16,
    -16,  -16,  -16,  120,  120,   60,   60,   15,
    120,  120,  120,  120,  120,  120,  120,  120,
    120,  120,  120,  -16,  -16,  -32,  -32,  -128,
  };

  static public void drawMandelbrot()
  {
    final int DEC_PLACE = 4;
    int x,y;
    //int rs = (-2 << DEC_PLACE), re = (1 << DEC_PLACE);
    //int is = (-1 << DEC_PLACE), ie = (1 << DEC_PLACE);
    int rs,is;
    int zi,zr;
    int tr,ti;
    int zr2,zi2;
    //int dx = 0x0019, dy = 0x0015;  according to debugger, 2 lines below work
    //int dx = (re - rs) / 32;
    //int dy = (ie - is) / 24;
    //int rs_save = rs;
    int count;

    TI99.setCursor(0, 0);

    for (y = 0; y < 24; y++)
    {
      is = (((2 << DEC_PLACE) * y) / 24) - (1 << DEC_PLACE);
      //rs = rs_save;

      for (x = 0; x < 32; x++)
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

        //TI99.printChar((char)((count >> 2) + '0'));
        //TI99.printChar((char)(count << 4));
        TI99.plot(x, y, (char)(count << 4));

        //rs += dx;
      }

      //is += dy;
    }
  }

  static public int testMethod(int a, int b)
  {
    int c;

    c = a + b;

    return c;
  }

  static public void main(String args[])
  {
    int a;

    TI99.setCursor(10, 5);
    TI99.print("JAVA");
    TI99.setSpriteSize(TI99.SPRITE_SIZE_16X16_SMALL);

    //TI99.setSpriteVisible(0, true);
    TI99.setSpriteImage(0, sprite_j);
    TI99.setSpriteImage(1, sprite_a);
    TI99.setSpriteImage(2, sprite_v);
    TI99.setSpriteImage(3, sprite_a);
    TI99.setSpriteColor(0, 2);
    TI99.setSpriteColor(1, 3);
    TI99.setSpriteColor(2, 4);
    TI99.setSpriteColor(3, 5);
    TI99.setSpritePos(0, 0x20, 0x40);
    TI99.setSpritePos(1, 0x40, 0x40);
    TI99.setSpritePos(2, 0x60, 0x40);
    TI99.setSpritePos(3, 0x80, 0x40);

    for (a = 0; a < 32767; a++);

    TI99.setColors();
    drawMandelbrot();

    TI99.setSoundFreq(0, 0x60d);
    TI99.setSoundVolume(0, 0);
    TI99.setSoundFreq(1, 0xa0a);
    TI99.setSoundVolume(1, 0);
    TI99.setSoundFreq(2, 0xf08);
    TI99.setSoundVolume(2, 0);

    for (a = 0; a < 32767; a++);

    TI99.setSoundVolume(0, 15);
    TI99.setSoundVolume(1, 15);
    TI99.setSoundVolume(2, 15);

    TI99.clearScreen();

    for (a = 0; a < 32767; a++);
    TI99.setSpriteSize(TI99.SPRITE_SIZE_16X16_BIG);

    while(true);
  }
}

