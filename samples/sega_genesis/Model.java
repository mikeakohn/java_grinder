
import net.mikekohn.java_grinder.SegaGenesis;
import net.mikekohn.java_grinder.Memory;

public class Model
{
  static short model_points[] =
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

  static short model_edges[] =
  {
    // top
    0, 1, 1,
    1, 2, 1,
    2, 3, 1,
    3, 0, 1,

    // bottom
    4, 5, 3,
    5, 6, 3,
    6, 7, 3,
    7, 4, 3,

    // sides
    0, 4, 2,
    1, 5, 2,
    2, 6, 2,
    3, 7, 2,

    // top
    8, 9, 7,
    9, 10, 7,
    10, 11, 7,
    11, 8, 7,

    // bottom
    12, 13, 5,
    13, 14, 5,
    14, 15, 5,
    15, 12, 5,

    // sides
    8, 12, 6,
    9, 13, 6,
    10, 14, 6,
    11, 15, 6
  };

  // 0 = mask
  // 1 - 10 colors
  // 11 - 15 grays
  static short palette[] =
  {
    0x000, 0xf00, 0xf80, 0xff0, 0x0f0, 0x0ff, 0x08f, 0x00f,
    0x80f, 0xf0f, 0xf08, 0x000, 0x444, 0x888, 0xccc, 0xfff
  };

  public static void run()
  {
    int i, j;
    int r = 0;
    short model_buf[] = Memory.allocStackShorts(model_points.length);
    short last_buf[] = Memory.allocStackShorts(model_points.length);

    for(i = 0; i < model_points.length; i++)
    {
      model_buf[i] = 0;
      last_buf[i] = 0;
    }

    SegaGenesis.initBitmap();
    SegaGenesis.setPalettePointer(0);
    SegaGenesis.setPaletteColors(palette);

    for(j = 0; j < 130; j++)
    {
/*
      while(!SegaGenesis.inVerticalBlank());

      //SegaGenesis.clearBitmap();
      for (int n = (40 * 7) + 13; n < (40 * 20) + 13; n += 40)
      {
        SegaGenesis.setPlotAddress(n << 5);
        SegaGenesis.clearPatterns(15);
      }
*/
      int sin = (int)Common.sin[r];
      int cos = (int)Common.cos[r];

      int sinsin = (sin * sin) >> 6;
      int coscos = (cos * cos) >> 6;
      int sincos = (sin * cos) >> 6;

      int xx = coscos;
      int xy = sincos;
      int xz = sin;

      int yx = sincos - ((sincos * sin) >> 6);
      int yy = coscos + ((sinsin * sin) >> 6);
      int yz = sincos;

      // short zx = sinsin + ((sincos * cos) >> 6);
      // short zy = sincos - ((sinsin * cos) >> 6);
      // short zz = coscos;

      for(i = 0; i < model_points.length; i += 4)
      {
        int x = model_points[i + 0];
        int y = model_points[i + 1];
        int z = model_points[i + 2];

        model_buf[i + 0] = (short)(((x * xx) >> 6) - ((y * xy) >> 6) - ((z * xz) >> 6));
        model_buf[i + 1] = (short)(((x * yx) >> 6) + ((y * yy) >> 6) - ((z * yz) >> 6));
        // model_buf[i + 2] = (short)(((x * zx) >> 6) + ((y * zy) >> 6) + ((z * zz) >> 6));
      }

      for(i = 0; i < model_edges.length; i += 3)
      {
        int temp0 = model_edges[i + 0] << 2;
        int temp1 = model_edges[i + 1] << 2;

        Common.drawLine(last_buf[temp0] + 160, last_buf[temp0 + 1] + 112,
                        last_buf[temp1] + 160, last_buf[temp1 + 1] + 112,
                        11);
        Common.drawLine(model_buf[temp0] + 160, model_buf[temp0 + 1] + 112,
                        model_buf[temp1] + 160, model_buf[temp1 + 1] + 112,
                        model_edges[i + 2]);
      }

      for(i = 0; i < model_points.length; i++)
        last_buf[i] = model_buf[i];

      r++;
      if (r == 90) { r = 0; }

//      while(SegaGenesis.inVerticalBlank());

    }
  }
}

