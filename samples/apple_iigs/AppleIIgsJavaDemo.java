import net.mikekohn.java_grinder.AppleIIgs;
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.CPU;

public class AppleIIgsJavaDemo
{
  static int sine_table[] =
  {
    0, 1, 3, 4, 6, 7, 9, 10, 
    12, 14, 15, 17, 18, 20, 21, 23, 
    24, 25, 27, 28, 30, 31, 32, 34, 
    35, 36, 38, 39, 40, 41, 42, 44, 
    45, 46, 47, 48, 49, 50, 51, 52, 
    53, 54, 54, 55, 56, 57, 57, 58, 
    59, 59, 60, 60, 61, 61, 62, 62, 
    62, 63, 63, 63, 63, 63, 63, 63, 
    64, 63, 63, 63, 63, 63, 63, 63, 
    62, 62, 62, 61, 61, 60, 60, 59, 
    59, 58, 57, 57, 56, 55, 54, 54, 
    53, 52, 51, 50, 49, 48, 47, 46, 
    45, 44, 42, 41, 40, 39, 38, 36, 
    35, 34, 32, 31, 30, 28, 27, 25, 
    24, 23, 21, 20, 18, 17, 15, 14, 
    12, 10, 9, 7, 6, 4, 3, 1, 
    0, -1, -3, -4, -6, -7, -9, -10, 
    -12, -14, -15, -17, -18, -20, -21, -23, 
    -24, -25, -27, -28, -30, -31, -32, -34, 
    -35, -36, -38, -39, -40, -41, -42, -44, 
    -45, -46, -47, -48, -49, -50, -51, -52, 
    -53, -54, -54, -55, -56, -57, -57, -58, 
    -59, -59, -60, -60, -61, -61, -62, -62, 
    -62, -63, -63, -63, -63, -63, -63, -63, 
    -64, -63, -63, -63, -63, -63, -63, -63, 
    -62, -62, -62, -61, -61, -60, -60, -59, 
    -59, -58, -57, -57, -56, -55, -54, -54, 
    -53, -52, -51, -50, -49, -48, -47, -46, 
    -45, -44, -42, -41, -40, -39, -38, -36, 
    -35, -34, -32, -31, -30, -28, -27, -25, 
    -24, -23, -21, -20, -18, -17, -15, -14, 
    -12, -10, -9, -7, -6, -4, -3, -1 
  };

  static int box_points[] =
  {
    -32, -32, -32, 0,
     32, -32, -32, 0,
     32,  32, -32, 0,
    -32,  32, -32, 0,
    -32, -32,  32, 0,
     32, -32,  32, 0,
     32,  32,  32, 0,
    -32,  32,  32, 0,
  };

  static int box_edges[] =
  {
    // top
    0, 1,
    1, 2,
    2, 3,
    3, 0,

    // bottom
    4, 5,
    5, 6,
    6, 7,
    7, 4,

    // sides
    0, 4,
    1, 5,
    2, 6,
    3, 7
  };

/*
  static byte sprite[] =
  {
    (byte)0x00, (byte)0x88, (byte)0x88, (byte)0x00,
    (byte)0x88, (byte)0x66, (byte)0x66, (byte)0x88,
    (byte)0x88, (byte)0x66, (byte)0x66, (byte)0x88,
    (byte)0x00, (byte)0x88, (byte)0x88, (byte)0x00
  };
*/

  static int palette_gray[] =
  {
    0xfff, 0xeee, 0xddd, 0xccc, 0xbbb, 0xaaa, 0x999, 0x888,
    0x777, 0x666, 0x555, 0x444, 0x333, 0x222, 0x111, 0x000
  };
 
  static int palette_color[] =
  {
    0xfff, 0xff0, 0xf60, 0xd00, 0xf09, 0x409, 0x00c, 0x09f,
    0x0a0, 0x060, 0x630, 0x963, 0xbbb, 0x888, 0x444, 0x000
  };

  public static void wait(int time)
  {
    int i;

    for(i = 0; i < time; i++) { }
    for(i = 0; i < time; i++) { }
    for(i = 0; i < time; i++) { }
    for(i = 0; i < time; i++) { }
    for(i = 0; i < time; i++) { }
    for(i = 0; i < time; i++) { }
    for(i = 0; i < time; i++) { }
    for(i = 0; i < time; i++) { }
    for(i = 0; i < time; i++) { }
    for(i = 0; i < time; i++) { }
  }

  public static void rect(int x1, int y1, int x2, int y2, int color)
  {
    int x, y;

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

    int x, y, z, c;
    int i, j;
    int x1, y1, z1;
    int x2, y2, z2;
    int nx, ny, nz;
    int sin, cos;
    int box_buf[] = new int[24];

    AppleIIgs.hiresEnable(0xe1);
    AppleIIgs.hiresPalette(0, palette_gray);
    AppleIIgs.hiresPalette(1, palette_color);

    for(i = 0; i <= 199; i++) 
      AppleIIgs.hiresSetRow(i, 1);

    AppleIIgs.hiresClear(0xff);

    for(x = 0; x < 500; x++)
    {
      c = AppleIIgs.rnd() & 15;
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

    x1 = 49;
    x2 = 111;
    y1 = 49;
    y2 = 146;

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

    for(i = 0; i <= 199; i++) 
      AppleIIgs.hiresSetRow(i, 0);

    wait(5000);

    for(i = 0xff; i >= 0; i -= 0x22)
      rectfill(0, 0, 79, 49, i);

    for(i = 0xff; i >= 0x22; i -= 0x22)
      rectfill(80, 100, 159, 149, i);

    for(i = 0xff; i >= 0x44; i -= 0x22)
      rectfill(0, 150, 79, 199, i);

    for(i = 0xff; i >= 0x66; i -= 0x22)
      rectfill(80, 0, 159, 49, i);

    for(i = 0; i <= 0x88; i += 0x22)
      rectfill(0, 100, 79, 149, i);

    for(i = 0; i <= 0xaa; i += 0x22)
      rectfill(80, 50, 159, 99, i);

    for(i = 0; i <= 0xcc; i += 0x22)
      rectfill(0, 50, 79, 99, i);

    for(i = 0; i <= 0xdd; i += 0x22)
      rectfill(80, 150, 159, 199, i);

/*
    for(x = 0; x < 5000; x++)
    {
      c = AppleIIgs.rnd() & 15;
      AppleIIgs.hiresSprite(sprite, AppleIIgs.rnd() % 157,
                                    AppleIIgs.rnd() % 197,
                                    4, 16);
    }
*/

    AppleIIgs.hiresClear(0xff);

    for(i = 0; i <= 199; i++) 
      AppleIIgs.hiresSetRow(i, 1);

    AppleIIgs.hiresEnable(0x02);

    while(true)
    {
      c = 0;

      for(j = 0; j < 256; j++)
      {
        sin = sine_table[j];
        cos = sine_table[(j + 64) & 255];
        AppleIIgs.hiresClear(0xff);

        for(i = 0; i < 32; i += 4)
        {
          x = box_points[i];
          y = box_points[i + 1];
          z = box_points[i + 2];

          // x rotate
          ny = ((y * cos) >> 6) - ((z * sin) >> 6);
          nz = ((z * cos) >> 6) + ((y * sin) >> 6);
          y = ny;
          z = nz;

          // y rotate
          nx = ((x * cos) >> 6) - ((z * sin) >> 6);
          // nz = ((z * cos) >> 6) + ((x * sin) >> 6);
          x = nx;
          // z = nz;

          // z rotate
          nx = ((x * cos) >> 6) - ((y * sin) >> 6);
          ny = ((y * cos) >> 6) + ((x * sin) >> 6);
          // x = nx;
          // y = ny;

          box_buf[i] = nx;
          box_buf[i + 1] = ny;
          // box_buf[i + 2] = nz;
        }

        for(i = 0; i < 24; i += 2)
        {
          x1 = box_buf[(box_edges[i] << 2)];
          y1 = box_buf[(box_edges[i] << 2) + 1];
          x2 = box_buf[(box_edges[i + 1] << 2)];
          y2 = box_buf[(box_edges[i + 1] << 2) + 1];

          x1 >>= 1;
          x2 >>= 1;
          x1 += 80;
          y1 += 100;
          x2 += 80;
          y2 += 100;

          AppleIIgs.hiresLine(x1, y1, x2, y2, c | (c << 4));
        }

        AppleIIgs.hiresUpdate();

        c++;
        c &= 0xf;
      }
    }
  }
}

