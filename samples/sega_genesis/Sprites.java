
import net.mikekohn.java_grinder.SegaGenesis;

public class Sprites
{
  static final int CENTER_X = 270;
  static final int CENTER_Y = 230;
  static final int SPRITE_LOCATION = 1120;

  public static void spin()
  {
    int t0 = 5;
    int r = 10;
    int x, y, i;
    int dx, dy, dr = 1;

    for (i = 0; i < 1024; i++)
    {
      while(!SegaGenesis.inVerticalBlank());

      dx = ((Common.cos[t0] * r) >> 6);
      dy = ((Common.sin[t0] * r) >> 6);

      //x = CENTER_X + dx;
      //y = CENTER_Y + dy;

      SegaGenesis.setSpritePosition(0, CENTER_X + dx, CENTER_Y + dy);
      SegaGenesis.setSpritePosition(1, CENTER_X - dx, CENTER_Y + dy);
      SegaGenesis.setSpritePosition(2, CENTER_X + dx, CENTER_Y - dy);
      SegaGenesis.setSpritePosition(3, CENTER_X - dx, CENTER_Y - dy);

      while(SegaGenesis.inVerticalBlank());

      t0++;
      if (t0 == 90) { t0 = 0; }

      if ((i & 0x3) == 0) { r += dr; }

      if (r == 90) { dr = -1; }
      if (r == 0) { break; }
    }
  }

  public static void run()
  {
    //SegaGenesis.initBitmap();
    //SegaGenesis.setPalettePointer(0);
    //SegaGenesis.setPaletteColor(0xeee);

    ImageJava.run();

    SegaGenesis.setPaletteColorsAtIndex(16, palette);
    SegaGenesis.setPatternTableAtIndex(SPRITE_LOCATION, pattern);

    SegaGenesis.setSpritePosition(0, CENTER_X, CENTER_Y);
    SegaGenesis.setSpriteConfig1(0,
      SegaGenesis.SPRITE_CONFIG1_HORIZONTAL_SIZE_4_CELL |
      SegaGenesis.SPRITE_CONFIG1_VERTICAL_SIZE_4_CELL);
    SegaGenesis.setSpriteConfig2(0, SegaGenesis.SPRITE_CONFIG2_PALETTE_1 | SPRITE_LOCATION);
    Common.wait(80);

    spin();

    SegaGenesis.setSpriteConfig1(0,
      SegaGenesis.SPRITE_CONFIG1_HORIZONTAL_SIZE_4_CELL |
      SegaGenesis.SPRITE_CONFIG1_VERTICAL_SIZE_4_CELL | 1);

    SegaGenesis.setSpriteConfig1(1,
      SegaGenesis.SPRITE_CONFIG1_HORIZONTAL_SIZE_4_CELL |
      SegaGenesis.SPRITE_CONFIG1_VERTICAL_SIZE_4_CELL | 2);
    SegaGenesis.setSpriteConfig2(1, SegaGenesis.SPRITE_CONFIG2_PALETTE_1 | SPRITE_LOCATION);

    SegaGenesis.setSpriteConfig1(2,
      SegaGenesis.SPRITE_CONFIG1_HORIZONTAL_SIZE_4_CELL |
      SegaGenesis.SPRITE_CONFIG1_VERTICAL_SIZE_4_CELL | 3);
    SegaGenesis.setSpriteConfig2(2, SegaGenesis.SPRITE_CONFIG2_PALETTE_1 | SPRITE_LOCATION);

    SegaGenesis.setSpriteConfig1(3,
      SegaGenesis.SPRITE_CONFIG1_HORIZONTAL_SIZE_4_CELL |
      SegaGenesis.SPRITE_CONFIG1_VERTICAL_SIZE_4_CELL);
    SegaGenesis.setSpriteConfig2(3, SegaGenesis.SPRITE_CONFIG2_PALETTE_1 | SPRITE_LOCATION);

    spin();

    SegaGenesis.setSpriteConfig1(0,
      SegaGenesis.SPRITE_CONFIG1_HORIZONTAL_SIZE_4_CELL |
      SegaGenesis.SPRITE_CONFIG1_VERTICAL_SIZE_4_CELL);

    SegaGenesis.setSpritePosition(0, 0, 0);

    // FIXME - use a different pattern number for sprites
    SegaGenesis.loadFonts();
  }

  public static int[] pattern =
  {
    // Pattern 0
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    // Pattern 1
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0x00000002,
    // Pattern 2
    0x00000023, 0x00000232, 0x00014530, 0x04610537,
    0x65000267, 0x81000507, 0x01628577, 0x01333677,
    // Pattern 3
    0x00133077, 0x03133077, 0x12333777, 0x11233777,
    0x00013777, 0x00025977, 0x00011696, 0x00000231,
    // Pattern 4
    0x00000002, 0x00000002, 0x00000023, 0x00000333,
    0x00001333, 0x00005333, 0x00002333, 0x00023333,
    // Pattern 5
    0x00123333, 0x00533333, 0x02333333, 0x0233386a,
    0x1333b77b, 0x1333c9cb, 0x355adddd, 0x6792dddd,
    // Pattern 6
    0x0774addd, 0x777913aa, 0x77777061, 0x77777779,
    0x77777777, 0x77777777, 0x77777777, 0x77777777,
    // Pattern 7
    0x77777777, 0x77777777, 0x77777777, 0x77777777,
    0x77706646, 0x76232100, 0x12000000, 0x00000000,
    // Pattern 8
    0x50000000, 0x22000000, 0x32000000, 0x33500000,
    0x33330000, 0x33320000, 0x33335000, 0x33332000,
    // Pattern 9
    0x33333400, 0x33333310, 0x33333320, 0xaa333350,
    0xadda3332, 0xdddda333, 0xddddd306, 0xadada399,
    // Pattern 10
    0xddda3677, 0xaa336077, 0x54609777, 0x99997777,
    0x77777777, 0x77777777, 0x77777777, 0x77777777,
    // Pattern 11
    0x77777777, 0x77777777, 0x77777777, 0x97777777,
    0x14669777, 0x01051677, 0x00000110, 0x00000022,
    // Pattern 12
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    // Pattern 13
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x10000000, 0x50000000, 0x58000000,
    // Pattern 14
    0x43200000, 0x03540000, 0x73504200, 0x75200220,
    0x76100054, 0x70200155, 0x77252210, 0x77423500,
    // Pattern 15
    0x77633200, 0x77033330, 0x77033321, 0x77933353,
    0x77923100, 0x77020000, 0x79610000, 0x22800000,
  };

  public static short[] palette =
  {
    0xaaa, 0x888, 0x442, 0x000, 0x444, 0x222, 0x644, 0xeee,
    0x666, 0xccc, 0x006, 0x42a, 0x86a, 0x208, 0x000, 0x000,
  };
}

