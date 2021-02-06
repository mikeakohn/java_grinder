
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;

/*
 ---------- BMP Info Header ----------
            size: 108
           width: 112
          height: 37

Total Colors: 8
*/

public class ImageAmigaLogo8
{
  static public void init(Copper copper, Blitter blitter)
  { 
    int n;
    
    for (n = 8; n < palette.length + 8; n++)
    { 
      Display.setPalette(copper, n, palette[n - 8]);
    }
    
    // 320 / 8 = 40, 40 - (width / 8) = 26.
    blitter.setModuloDestination(26);
  }
  
  static public int getCenter()
  {
    return 8000 + (4000 - (28 * 40)) + (20 - 6);
  }

  static public short[] palette =
  {
    0x000, 0x654, 0xdf2, 0x999, 0xeee, 0xea0, 0x29e, 0x15c,
  };

  static public byte[] bitplane_0 =
  {
    0, 0, 0, 0, 125, -16, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, -16, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1,
    -1, -16, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, -1, -32,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 3, -1, -64, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, -1, -64, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 7,
    -1, -128, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 15, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 31, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 31, -2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 63,
    -4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 127, -4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -57, 8, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -122, 16, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 4,
    16, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 8, 32, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 24, 68, 0, 0, 0,
    0, 0, 0, 0, 0, 32, 0, 0,
    4, 16, 78, 1, -2, 3, -5, -2,
    1, -1, 0, 112, 0, 0, 8, 32,
    -110, 1, 2, 4, 10, 3, 14, 57,
    0, -112, 0, 0, 0, 97, 32, 0,
    66, 8, 33, 4, 48, -55, 1, 0,
    0, 0, 16, 64, 0, 0, 2, 0,
    1, 8, -62, 8, 2, 8, 0, 0,
    32, -126, 64, 0, -128, 16, 1, 9,
    4, 10, 2, 8, 125, -16, 1, -124,
    -127, 0, -128, 32, 64, 10, 8, 6,
    4, 8, 127, -8, 65, 1, 33, 0,
    0, 8, 66, 4, 16, 0, 8, 8,
    43, -4, -126, 8, 97, 1, 64, 80,
    66, 20, 0, 0, 3, 8, 32, 0,
    6, 18, 33, 0, 64, -112, 0, 24,
    32, 0, 17, 8, 0, 3, 4, 4,
    -95, 2, 0, 48, -124, 8, 3, 6,
    37, 8, 15, -2, 8, 33, -31, 2,
    -127, 80, -124, 8, 0, 1, 7, 8,
    15, -4, 16, 72, 1, 0, -128, 0,
    -124, 40, 3, 3, 64, 8, 7, -8,
    16, 19, -31, 4, 32, -128, 0, 40,
    65, 4, -97, 0, 6, 56, 32, -124,
    33, 5, 33, 33, 8, 8, 1, 1,
    33, 0, 2, 16, 0, 32, 33, 1,
    32, 33, 8, 64, 32, 9, 1, 0,
    1, 0, 0, 72, 32, 8, 34, 1,
    0, 68, 32, 10, 65, 0, 1, 0,
    3, -124, 96, -15, -92, -63, -80, 98,
    26, 12, 99, 6, 0, -128, 6, 2,
    -128, 64, 97, 0, 64, 17, -128, 16,
    20, 0, 0, 0, 1, -4, 127, -1,
    -112, -1, -65, -32, 127, -17, -29, -2,
    0, 125, -16, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
  };

  static public byte[] bitplane_1 =
  {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 56, -16, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 121, -32, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -5,
    -32, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, -9, -64, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 3, -25, -128, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    3, -17, -114, 0, -4, 3, -15, 14,
    0, -2, 0, 112, 0, 0, 7, -33,
    18, 1, 2, 4, 2, 2, 6, 57,
    0, -112, 0, 0, 15, -98, 0, 0,
    66, 0, 33, 0, 48, -119, 0, 0,
    0, 0, 15, -66, 0, 0, 2, 0,
    1, 8, 66, 8, 0, 0, 0, 0,
    31, 124, 64, 0, -128, 16, 1, 8,
    0, 10, 2, 0, 125, -16, 62, 120,
    -128, 0, -128, 0, 64, 0, 0, 4,
    4, 0, 127, -8, 62, -8, 32, 0,
    0, 8, 64, 0, 16, 0, 0, 8,
    63, -4, 125, -16, 96, 1, 64, 80,
    2, 20, 0, 0, 3, 8, 63, 124,
    -7, -30, 33, 0, 0, 0, 0, 0,
    32, 0, 17, 8, 31, -2, -5, -32,
    33, 0, 0, 0, -128, 8, 0, 0,
    1, 8, 15, -65, -9, -63, -31, 2,
    -127, 80, -124, 8, 0, 1, 7, 8,
    7, -33, -17, -120, 1, 0, 0, 0,
    4, 40, 3, 2, 64, 8, 7, -1,
    -17, -125, -31, 0, 32, 0, 0, 8,
    64, 0, -97, 0, 2, 47, -33, 4,
    1, 5, 33, 1, 0, 8, 1, 0,
    33, 0, 0, -1, -1, 32, 1, 0,
    32, 33, 8, 64, 0, 9, 0, 0,
    1, -1, -2, 72, 0, 0, 32, 0,
    0, 68, 32, 2, 64, 0, 0, -1,
    -4, -124, 32, -111, -92, 65, 16, 66,
    18, 4, 97, 4, 0, 127, -8, 2,
    0, 64, 32, 0, 64, 0, -128, 16,
    20, 0, 0, 127, -7, -120, 96, -128,
    0, -63, 48, 64, 63, -60, 35, 4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
  };

  static public byte[] bitplane_2 =
  {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -3, -16, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -5, -32, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, -9, -64,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 3, -9, -64, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, -17, -128, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 7,
    -33, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 15, -33, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 15, -66, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 31, 124, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 63,
    -4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 62, -8, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 69, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -122, 16, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 24, 64, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    12, 0, -4, 3, -15, -4, 1, -58,
    0, 96, 0, 0, 0, 97, 30, 0,
    60, 7, -64, -8, 15, 6, 0, -16,
    0, 0, 16, 0, 62, 0, 124, 15,
    -64, -16, 60, 6, 1, -16, 0, 0,
    0, 0, 62, 0, 126, 15, -64, -16,
    -8, 4, 1, -16, 125, -16, 1, -124,
    126, 0, 126, 31, -127, -15, -16, 0,
    3, -16, 127, -8, 64, 0, -34, 0,
    -2, 55, -127, -13, -32, 0, 7, -16,
    63, -4, 0, 1, -98, 0, -66, 47,
    -127, -29, -32, 0, 12, -16, 63, 124,
    6, 17, -98, 1, -66, 111, -125, -25,
    -64, 0, 12, -16, 31, -3, 4, 3,
    30, 1, -66, -49, 3, -25, -64, 0,
    24, -16, 0, 0, 0, 6, 30, 1,
    62, -113, 3, -25, -61, -2, 56, -16,
    0, 0, 16, 71, -2, 3, 63, -97,
    3, -57, -64, -4, 63, -16, 0, 0,
    16, 12, 30, 3, 31, 31, 7, -57,
    -128, -8, 96, -8, 1, -64, 0, 24,
    30, 2, 30, 30, 7, -57, -64, -8,
    -64, -8, 1, 0, 0, 24, 30, 6,
    30, 30, 7, -121, -63, -16, -64, -8,
    0, 0, 0, 48, 31, 6, 28, 62,
    15, -125, -63, -15, -128, -8, 0, 0,
    0, 120, 31, 14, 24, 62, 15, -127,
    -31, -13, -128, -8, 0, 0, 1, -4,
    127, -65, -104, -1, -65, -32, 127, -17,
    -29, -2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
  };

/*
  static public void show(Copper copper)
  {
    copper.stop();
    copper.resetIndex();
    copper.appendSetBitplane(0, bitplane_0);
    copper.appendSetBitplane(1, bitplane_1);
    copper.appendSetBitplane(2, bitplane_2);
    copper.appendSetColor(0, palette[0]);
    copper.appendSetColor(1, palette[1]);
    copper.appendSetColor(2, palette[2]);
    copper.appendSetColor(3, palette[3]);
    copper.appendSetColor(4, palette[4]);
    copper.appendSetColor(5, palette[5]);
    copper.appendSetColor(6, palette[6]);
    copper.appendSetColor(7, palette[7]);
    copper.appendEnd();
    copper.run();
  }
*/
}
