import net.mikekohn.java_grinder.AppleIIgs;
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.CPU;

public class AppleIIgsJavaDemo
{
// sprite test
//  static byte sprite[] = { 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  static int palette1[] =
  {
    0x0ff,
    0x0ee,
    0x0dd,
    0x0cc,
    0x0bb,
    0x0aa,
    0x099,
    0x088,
    0x077,
    0x066,
    0x055,
    0x044,
    0x033,
    0x022,
    0x011,
    0x000
  };

  static int palette2[] =
  {
    0xf0f,
    0xe0e,
    0xd0d,
    0xc0c,
    0xb0b,
    0xa0a,
    0x909,
    0x808,
    0x707,
    0x606,
    0x505,
    0x404,
    0x303,
    0x202,
    0x101,
    0x000
  };

  static int palette3[] =
  {
    0xff0,
    0xee0,
    0xdd0,
    0xcc0,
    0xbb0,
    0xaa0,
    0x990,
    0x880,
    0x770,
    0x660,
    0x550,
    0x440,
    0x330,
    0x220,
    0x110,
    0x000
  };

  static int palette4[] =
  {
    0xfff,
    0xeee,
    0xddd,
    0xccc,
    0xbbb,
    0xaaa,
    0x999,
    0x888,
    0x777,
    0x666,
    0x555,
    0x444,
    0x333,
    0x222,
    0x111,
    0x000
  };

  public static void wait(int time)
  {
    int i;

    for(i = 0; i < time; i++)
    {
    }
  }

  public static void rect(int x1, int y1, int x2, int y2, int color)
  {
    int x, y, address;

    for(x = x1; x <= x2; x++)
    {
      address = 0x2000 + x + 160 * y1;
      AppleIIgs.hiresPlot(address, color);
      address = 0x2000 + x + 160 * y2;
      AppleIIgs.hiresPlot(address, color);
    }

    for(y = y1; y <= y2; y++)
    {
      address = 0x2000 + x1 + 160 * y;
      AppleIIgs.hiresPlot(address, color);
      address = 0x2000 + x2 + 160 * y;
      AppleIIgs.hiresPlot(address, color);
    }
  }

  public static void rectfill(int x1, int y1, int x2, int y2, int color)
  {
    if(x1 < 0)
      x1 = 0;

    if(x2 > 159)
      x2 = 159;

    if(y1 < 0)
      y1 = 0;

    if(y2 > 199)
      y2 = 199;

    int y, address;

    for(y = y1; y <= y2; y++)
    {
      address = 0x2000 + x1 + 160 * y;
      AppleIIgs.hiresSpan(address, color, (x2 - x1) + 1);
    }
  }

  public static void main()
  {
    int recen = -8;
    int imcen = 0;
    int re, im, re2, im2, rec, imc;
    int address, address2;
    int x, y, i;

    AppleIIgs.hiresEnable();
    AppleIIgs.hiresPalette(0, palette1);
    AppleIIgs.hiresPalette(1, palette2);
    AppleIIgs.hiresPalette(2, palette3);
    AppleIIgs.hiresPalette(3, palette4);

    for(i = 0; i <= 49; i++) 
      AppleIIgs.hiresSetLinePalette(i, 0);

    for(i = 50; i <= 99; i++) 
      AppleIIgs.hiresSetLinePalette(i, 1);

    for(i = 100; i <= 149; i++) 
      AppleIIgs.hiresSetLinePalette(i, 2);

    for(i = 150; i <= 199; i++) 
      AppleIIgs.hiresSetLinePalette(i, 3);

    address = 0x2000;

    for(i = 0; i < 200; i++) 
    {
      AppleIIgs.hiresSpan(address, 255, 160);
      address += 160;
    }

// sprite test
//    AppleIIgs.hiresBlit(sprite, 0x2000, 4, 16);
//    AppleIIgs.hiresBlit(sprite, 0x2000 + 50 + 160 * 50, 4, 16);
//    AppleIIgs.hiresBlit(sprite, 0x2000 + 100 + 160 * 175, 4, 16);
//    while(true);

    address = 0x2000;
    address2 = 0x9c60;
    int yy = 0;

    for(y = 0; y < 100 * 16; y += 16, yy++, address2 -= 320)
    {
      imc = (y - 100 * 16) >> 5;
      imc += imcen;

      int xx = 0;

      for(x = 0; x < 160 * 16; x += 16, xx++, address++, address2++)
      {
        if((xx < 50 || xx > 110 || yy < 50 || yy > 145) ||
           (xx > 78 && xx < 93 && yy > 85 && yy < 115))
        {
          continue;
        }

        rec = (x - 80 * 16) >> 4;
        rec += recen;

        re = rec;
        im = imc;

        re2 = (re * re) >> 4;
        im2 = (im * im) >> 4;

        for(i = 0; i < 15; i++)
        {
          if((re2 + im2) > 4 * 16)
            break;

          im = (re * im) >> 3;
          im += imc;

          re = (re2 - im2) + rec;

          re2 = (re * re) >> 4;
          im2 = (im * im) >> 4;
        }

        AppleIIgs.hiresPlot(address, (i | (i << 4)));
        AppleIIgs.hiresPlot(address2, (i | (i << 4)));
      }
    }

    int x1 = 49;
    int x2 = 111;
    int y1 = 49;
    int y2 = 146;

    for(i = 0; i < 8; i++)
    {
      rect(x1, y1, x2, y2, 0);
      x1--;
      x2++;
      y1--;
      y2++;
    }

    for(i = 0; i < 16; i += 2)
    {
      rect(x1, y1, x2, y2, i | (i << 4));
      x1--;
      x2++;
      y1--;
      y2++;
      rect(x1, y1, x2, y2, i | (i << 4));
      y1--;
      y2++;
    }

    for(address = 0x2000; address < 0x9d00; address++) 
      AppleIIgs.hiresPlot(address, 255 - AppleIIgs.hiresRead(address));

    for(i = 0; i < 32000; i++) 
    {
      AppleIIgs.hiresPlot(0x2000 + i, 255 - AppleIIgs.hiresRead(0x9cff - i));
    }

    for(x = 0; x < 80; x++) 
    {
      int incy = 0;
      for(y = 0; y < 200; y++) 
      {
        AppleIIgs.hiresPlot(0x2000 + x + incy, 255 - AppleIIgs.hiresRead(0x2000 + x + 160 * y));
        incy += 160;
      }
    }

    for(i = 0; i < 8000; i++) 
    {
      AppleIIgs.hiresPlot(0x2000 + i, 255 - AppleIIgs.hiresRead(0x9cff - i));
      AppleIIgs.hiresPlot(0x9cff - i, AppleIIgs.hiresRead(0x2000 + i));
    }

    for(i = 0xff; i >= 0; i -= 0x11)
    {
      rectfill(0, 0, 79, 49, i);
      wait(1000);
    }

    for(i = 0xff; i >= 0x22; i -= 0x11)
    {
      rectfill(80, 100, 159, 149, i);
      wait(1000);
    }

    for(i = 0xff; i >= 0x44; i -= 0x11)
    {
      rectfill(0, 150, 79, 199, i);
      wait(1000);
    }

    for(i = 0xff; i >= 0x66; i -= 0x11)
    {
      rectfill(80, 0, 159, 49, i);
      wait(1000);
    }

    for(i = 0; i <= 0x88; i += 0x11)
    {
      rectfill(0, 100, 79, 149, i);
      wait(1000);
    }

    for(i = 0; i <= 0xaa; i += 0x11)
    {
      rectfill(80, 50, 159, 99, i);
      wait(1000);
    }

    for(i = 0; i <= 0xcc; i += 0x11)
    {
      rectfill(0, 50, 79, 99, i);
      wait(1000);
    }

    for(i = 0; i <= 0xdd; i += 0x11)
    {
      rectfill(80, 150, 159, 199, i);
      wait(1000);
    }

    while(true)
    {
    }
  }
}

