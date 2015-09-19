import net.mikekohn.java_grinder.AppleIIgs;
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.CPU;

public class AppleIIgsJavaDemo
{
  static byte sprite[] =
  {
    (byte)0x00, (byte)0x88, (byte)0x88, (byte)0x00,
    (byte)0x88, (byte)0x66, (byte)0x66, (byte)0x88,
    (byte)0x88, (byte)0x66, (byte)0x66, (byte)0x88,
    (byte)0x00, (byte)0x88, (byte)0x88, (byte)0x00
  };

  static int palette1[] =
  {
    0xfff,
    0xff0,
    0xf60,
    0xd00,
    0xf09,
    0x409,
    0x00c,
    0x09f,
    0x0a0,
    0x060,
    0x630,
    0x963,
    0xbbb,
    0x888,
    0x444,
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
      AppleIIgs.hiresPlot(x, y1, color);
      AppleIIgs.hiresPlot(x, y2, color);
    }

    for(y = y1; y <= y2; y++)
    {
      AppleIIgs.hiresPlot(x1, y, color);
      AppleIIgs.hiresPlot(x2, y, color);
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

    int y;

    for(y = y1; y <= y2; y++)
      AppleIIgs.hiresSpan(x1, y, (x2 - x1) + 1, color);
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

    for(i = 0; i <= 199; i++) 
      AppleIIgs.hiresSetRow(i, 0);

    for(i = 0; i < 200; i++) 
      AppleIIgs.hiresSpan(0, i, 160, 255);

    for(x = 0; x < 1000; x++)
    {
      int c = AppleIIgs.rnd() & 15;
      AppleIIgs.hiresLine(AppleIIgs.rnd() % 160,
                          AppleIIgs.rnd() % 200,
                          AppleIIgs.rnd() % 160,
                          AppleIIgs.rnd() % 200,
                          c | (c << 4));
    }

    int yy = 0;

    for(y = 0; y < 100 * 16; y += 16, yy++)
    {
      imc = (y - 100 * 16) >> 5;
      imc += imcen;

      int xx = 0;

      for(x = 0; x < 160 * 16; x += 16, xx++)
      {
        if(xx < 50 || xx > 110 || yy < 50 || yy > 145)
          continue;

        if(xx > 78 && xx < 93 && yy > 85 && yy < 115)
        {
          AppleIIgs.hiresPlot(xx, yy, (15 | (15 << 4)));
          AppleIIgs.hiresPlot(xx, 199 - yy, (15 | (15 << 4)));

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

        AppleIIgs.hiresPlot(xx, yy, (i | (i << 4)));
        AppleIIgs.hiresPlot(xx, 199 - yy, (i | (i << 4)));
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

    for(i = 0xff; i >= 0; i -= 0x11)
      rectfill(0, 0, 79, 49, i);

    for(i = 0xff; i >= 0x22; i -= 0x11)
      rectfill(80, 100, 159, 149, i);

    for(i = 0xff; i >= 0x44; i -= 0x11)
      rectfill(0, 150, 79, 199, i);

    for(i = 0xff; i >= 0x66; i -= 0x11)
      rectfill(80, 0, 159, 49, i);

    for(i = 0; i <= 0x88; i += 0x11)
      rectfill(0, 100, 79, 149, i);

    for(i = 0; i <= 0xaa; i += 0x11)
      rectfill(80, 50, 159, 99, i);

    for(i = 0; i <= 0xcc; i += 0x11)
      rectfill(0, 50, 79, 99, i);

    for(i = 0; i <= 0xdd; i += 0x11)
      rectfill(80, 150, 159, 199, i);

    for(x = 0; x < 10000; x++)
    {
      int c = AppleIIgs.rnd() & 15;
      AppleIIgs.hiresSprite(sprite, AppleIIgs.rnd() % 157,
                                    AppleIIgs.rnd() % 197,
                                    4, 16);
    }
    AppleIIgs.hiresSprite(sprite, 0, 0, 4, 16);
    AppleIIgs.hiresSprite(sprite, 100, 100, 4, 16);
    AppleIIgs.hiresSprite(sprite, 120, 160, 4, 16);

    while(true)
    {
    }
  }
}

