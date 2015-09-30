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

  static int palette[] =
  {
    0xf0f, 0x000, 0xf00, 0xf80, 0xff0, 0x0f0, 0x0f8, 0x0ff,
    0x08f, 0x00f, 0xf0f, 0xf08, 0x444, 0x888, 0xccc, 0xfff
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

  public static void lines()
  {
    int x;

    AppleIIgs.hiresEnable(0xe1);
    AppleIIgs.hiresClear(0x11);

    for(x = 0; x < 500; x++)
    {
      int c = AppleIIgs.rnd() & 15;
      AppleIIgs.hiresLine(AppleIIgs.rnd() % 160,
                          AppleIIgs.rnd() % 200,
                          AppleIIgs.rnd() % 160,
                          AppleIIgs.rnd() % 200,
                          c | (c << 4));
    }
  }

  public static void box()
  {
    int i, j, c;
    int box_buf[] = new int[box_edges.length];

    // clear front buffer
    AppleIIgs.hiresEnable(0xe1);
    AppleIIgs.hiresClear(0x11);

    // this is the back buffer
    AppleIIgs.hiresEnable(0x02);
    AppleIIgs.hiresClear(0x11);

    while(true)
    {
      c = 0;

      for(j = 0; j < 256; j += 2)
      {
        AppleIIgs.hiresClear(0x11);

        int sin = sine_table[j];
        int cos = sine_table[(j + 64) & 255];

        int sinsin = (sin * sin) >> 6;
        int coscos = (cos * cos) >> 6;
        int sincos = (sin * cos) >> 6;

        int xx = coscos;
        int xy = sincos;
        int xz = sin;

        int yx = sincos - ((sincos * sin) >> 6);
        int yy = coscos + ((sinsin * sin) >> 6);
        int yz = sincos;

        // int zx = sinsin + ((sincos * cos) >> 6);
        // int zy = sincos - ((sinsin * cos) >> 6);
        // int zz = coscos;

        for(i = 0; i < box_points.length; i += 4)
        {
          int x = box_points[i + 0];
          int y = box_points[i + 1];
          int z = box_points[i + 2];

          box_buf[i + 0] = ((x * xx) >> 6) - ((y * xy) >> 6) - ((z * xz) >> 6);
          box_buf[i + 1] = ((x * yx) >> 6) + ((y * yy) >> 6) - ((z * yz) >> 6);
          // box_buf[i + 2] = ((x * zx) >> 6) + ((y * zy) >> 6) + ((z * zz) >> 6);
        }

        for(i = 0; i < box_edges.length; i += 2)
        {
          int temp0 = box_edges[i + 0] << 2;
          int temp1 = box_edges[i + 1] << 2;

          int x1 = box_buf[temp0];
          int y1 = box_buf[temp0 + 1];
          int x2 = box_buf[temp1];
          int y2 = box_buf[temp1 + 1];

          x1 >>= 1;
          x2 >>= 1;
          x1 += 80;
          y1 += 100;
          x2 += 80;
          y2 += 100;

          AppleIIgs.hiresLine(x1, y1, x2, y2, c | (c << 4));
        }

        AppleIIgs.hiresUpdate(160 * 46, 160 * 154);

        c++;
        c &= 0xf;
      }
    }
  }

  public static void mandel()
  {
    int recen = -8;
    int imcen = 0;
    int re, im, re2, im2, rec, imc;
    int x, y, xx, yy, i;

    AppleIIgs.hiresEnable(0xe1);

    yy = 0;

    for(y = 0; y < 100 * 16; y += 16, yy++)
    {
      imc = (y - 100 * 16) >> 5;
      imc += imcen;

      xx = 0;

      for(x = 0; x < 160 * 16; x += 16, xx++)
      {
        if(xx < 50 || xx > 110 || yy < 50 || yy > 145)
          continue;

        if(xx > 78 && xx < 93 && yy > 85 && yy < 115)
        {
          AppleIIgs.hiresPlot(xx, yy, (1 | (1 << 4)));
          AppleIIgs.hiresPlot(xx, 199 - yy, (1 | (1 << 4)));

          continue;
        }

        rec = (x - 80 * 16) >> 4;
        rec += recen;

        re = rec;
        im = imc;

        re2 = (re * re) >> 4;
        im2 = (im * im) >> 4;

        for(i = 1; i < 15; i++)
        {
          if((re2 + im2) > 4 * 16)
            break;

          im = (re * im) >> 3;
          im += imc;

          re = (re2 - im2) + rec;

          re2 = (re * re) >> 4;
          im2 = (im * im) >> 4;
        }

        i = 16 - i;
        AppleIIgs.hiresPlot(xx, yy, (i | (i << 4)));
        AppleIIgs.hiresPlot(xx, 199 - yy, (i | (i << 4)));
      }
    }
  }

  public static void main()
  {
    int i;

    // set color palette
    AppleIIgs.hiresPalette(0, palette);

    // set screen rows to use palette
    for(i = 0; i <= 199; i++) 
      AppleIIgs.hiresSetRow(i, 0);

    // lines demo
    lines();
    wait(2000);

    // mandelbrot demo
    mandel();
    wait(2000);

    // 3D box demo
    box();

    // loop forever
    while(true)
    {
    }
  }
}

