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

  static int model_points[] =
  {
    -32, -32, -32, 0,
     32, -32, -32, 0,
     32,  32, -32, 0,
    -32,  32, -32, 0,
    -32, -32,  32, 0,
     32, -32,  32, 0,
     32,  32,  32, 0,
    -32,  32,  32, 0,

    -16, -16, -16, 0,
     16, -16, -16, 0,
     16,  16, -16, 0,
    -16,  16, -16, 0,
    -16, -16,  16, 0,
     16, -16,  16, 0,
     16,  16,  16, 0,
    -16,  16,  16, 0,
  };

  static int model_edges[] =
  {
    // top
    0, 1, 0x11,
    1, 2, 0x11,
    2, 3, 0x11,
    3, 0, 0x11,

    // bottom
    4, 5, 0x33,
    5, 6, 0x33,
    6, 7, 0x33,
    7, 4, 0x33,

    // sides
    0, 4, 0x22,
    1, 5, 0x22,
    2, 6, 0x22,
    3, 7, 0x22,

    // top
    8, 9, 0x77,
    9, 10, 0x77,
    10, 11, 0x77,
    11, 8, 0x77,

    // bottom
    12, 13, 0x55,
    13, 14, 0x55,
    14, 15, 0x55,
    15, 12, 0x55,

    // sides
    8, 12, 0x66,
    9, 13, 0x66,
    10, 14, 0x66,
    11, 15, 0x66
  };

  // wave table
  static byte square_wave[] =
  {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  };

  // 0 = mask
  // 1 - 10 colors
  // 11 - 15 grays
  static int palette[] =
  {
    0xf0f, 0xf00, 0xf80, 0xff0, 0x0f0, 0x0ff, 0x08f, 0x00f,
    0x80f, 0xf0f, 0xf08, 0x000, 0x444, 0x888, 0xccc, 0xfff
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
    AppleIIgs.hiresClear(0xbb);

    for(x = 0; x < 500; x++)
    {
      int c = AppleIIgs.rnd() % 10 + 1;
      AppleIIgs.hiresLine(AppleIIgs.rnd() % 160,
                          AppleIIgs.rnd() % 200,
                          AppleIIgs.rnd() % 160,
                          AppleIIgs.rnd() % 200,
                          c | (c << 4));
    }
  }

  public static void model()
  {
    int i, j;
    int model_buf[] = new int[model_points.length];

    // clear front buffer
    AppleIIgs.hiresEnable(0xe1);
    AppleIIgs.hiresClear(0xbb);

    // this is the back buffer
    AppleIIgs.hiresEnable(0x02);
    AppleIIgs.hiresClear(0xbb);

    while(true)
    {
      for(j = 0; j < 256; j += 2)
      {
        AppleIIgs.hiresClear(0xbb);

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

        for(i = 0; i < model_points.length; i += 4)
        {
          int x = model_points[i + 0];
          int y = model_points[i + 1];
          int z = model_points[i + 2];

          model_buf[i + 0] = ((x * xx) >> 6) - ((y * xy) >> 6) - ((z * xz) >> 6);
          model_buf[i + 1] = ((x * yx) >> 6) + ((y * yy) >> 6) - ((z * yz) >> 6);
          // model_buf[i + 2] = ((x * zx) >> 6) + ((y * zy) >> 6) + ((z * zz) >> 6);
        }

        for(i = 0; i < model_edges.length; i += 3)
        {
          int temp0 = model_edges[i + 0] << 2;
          int temp1 = model_edges[i + 1] << 2;
          int color = model_edges[i + 2];

          int x1 = model_buf[temp0];
          int y1 = model_buf[temp0 + 1];
          int x2 = model_buf[temp1];
          int y2 = model_buf[temp1 + 1];

          x1 >>= 1;
          x2 >>= 1;
          x1 += 80;
          y1 += 100;
          x2 += 80;
          y2 += 100;

          AppleIIgs.hiresLine(x1, y1, x2, y2, color);
        }

        AppleIIgs.hiresUpdate(160 * 46, 160 * 154);
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
          AppleIIgs.hiresPlot(xx, yy, 0xbb);
          AppleIIgs.hiresPlot(xx, 199 - yy, 0xbb);

          continue;
        }

        rec = (x - 80 * 16) >> 4;
        rec += recen;

        re = rec;
        im = imc;

        re2 = (re * re) >> 4;
        im2 = (im * im) >> 4;

        for(i = 1; i < 11; i++)
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
  }

  public static void main()
  {
    int i;

    // play an annoying test tone
    AppleIIgs.loadWaveTable(square_wave, 0);
    AppleIIgs.enableOscillators(3);
    AppleIIgs.setMasterVolume(15);
    AppleIIgs.setWaveTable(0, 0, 0);
    AppleIIgs.setSoundVolume(0, 255);
    AppleIIgs.setSoundFrequency(0, 1000);
    AppleIIgs.setWaveTable(1, 0, 0);
    AppleIIgs.setSoundVolume(1, 255);
    AppleIIgs.setSoundFrequency(1, 1200);
    AppleIIgs.setWaveTable(2, 0, 0);
    AppleIIgs.setSoundVolume(2, 255);
    AppleIIgs.setSoundFrequency(2, 1300);

    // set color palette
    AppleIIgs.hiresPalette(0, palette);

    // set screen rows to use palette
    for(i = 0; i <= 199; i++) 
      AppleIIgs.hiresSetRow(i, 0);

    // lines demo
    lines();
    wait(3000);

    // mandelbrot demo
    mandel();
    wait(3000);

    // 3D model demo
    model();

    // loop forever
    while(true)
    {
    }
  }
}

