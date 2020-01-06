
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;
import net.mikekohn.java_grinder.Memory;

public class Stars
{
  private static int logo_offset_0;
  private static int logo_offset_1;
  private static int logo_offset_2;

  public static void moveStars(short[] stars)
  {
    int n;

    for (n = 0; n < stars.length; n += 4)
    {
      stars[n + 0] += stars[n + 2];
      stars[n + 1] += stars[n + 3];

      // If star is beyond boundary, erase it and reset it.
      if (stars[n + 0] <= 0 || stars[n + 0] >= 320 ||
          stars[n + 1] <= 0 || stars[n + 1] >= 200)
      {
        int x = stars[n], y = stars[n + 1];

        for (int c = 1; c <= 4; c++)
        {
          x -= stars[n + 2];
          y -= stars[n + 3];
          Display.plotEven(x, y, 0);
        }

        stars[n + 0] = stars_init[n + 0];
        stars[n + 1] = stars_init[n + 1];
      }

      // Draw a trail of stars
      int x = stars[n], y = stars[n + 1];

      for (int c = 1; c <= 4; c++)
      {
        Display.plotEven(x, y, c);
        x -= stars[n + 2];
        y -= stars[n + 3];
      }
    }
  }

  public static void clearLogo(Blitter blitter)
  {
    int offset;

    blitter.enableChannels(Blitter.MASK_ENABLE_D);
    blitter.setDataRegisterA((char)0);
    blitter.setModuloDestination(26);

    offset = 8000 + (4000 - (28 * 40)) + (20 - 6);

    blitter.waitBusy();
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + offset);
    blitter.runFill(7, 68);

    offset += 16000;

    blitter.waitBusy();
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + offset);
    blitter.runFill(7, 68);

    offset += 16000;

    blitter.waitBusy();
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + offset);
    blitter.runFill(7, 68);

    // Since this only modifies memory and not Amiga registers, this can
    // be set after runFill();
    blitter.enableChannels(Blitter.MASK_ENABLE_A | Blitter.MASK_ENABLE_D);
  }

  public static void clearCommodoreLogo(Blitter blitter)
  {
    int offset;

    blitter.enableChannels(Blitter.MASK_ENABLE_D);
    blitter.setDataRegisterA((char)0);
    blitter.setModuloDestination(39);
    blitter.setShiftA(0);

    blitter.waitBusy();
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + logo_offset_0 - 2);
    blitter.runFill(1, 56);

    blitter.enableChannels(Blitter.MASK_ENABLE_A | Blitter.MASK_ENABLE_D);
    blitter.setModuloDestination(30);
  }

  public static void showJavaGrinder(Blitter blitter)
  {
    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageJavaGrinder.bitplane_0));
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + logo_offset_0);
    blitter.runFill(7, 68);

    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageJavaGrinder.bitplane_1));
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + logo_offset_1);
    blitter.runFill(7, 68);

    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageJavaGrinder.bitplane_2));
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + logo_offset_2);
    blitter.runFill(7, 68);
  }

  public static void showAmigaLogo(Blitter blitter)
  {
    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageAmigaLogo8.bitplane_0));
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + logo_offset_0);
    blitter.runFill(7, 37);

    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageAmigaLogo8.bitplane_1));
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + logo_offset_1);
    blitter.runFill(7, 37);

    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageAmigaLogo8.bitplane_2));
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + logo_offset_2);
    blitter.runFill(7, 37);
  }

  public static void showCommodoreLogo(Blitter blitter)
  {
    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageCommodoreLogo.bitplane_0));
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + logo_offset_0);
    blitter.runFill(5, 56);

    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageCommodoreLogo.bitplane_1));
    blitter.setDestination(Memory.addressOf(Display.bitplanes) + logo_offset_1);
    blitter.runFill(5, 56);
  }

  public static void run(Copper copper, Blitter blitter)
  {
    int n, frame;

    Display.clear();
    Display.setDisplay(copper, 6);
    Display.setDoublePlayfield();
    Amiga.setPlayfieldPriority(0, 0, true);

    for (n = 0; n < colors.length; n++)
    {
      Display.setPalette(copper, n, colors[n]);
    }

    blitter.setModuloA(0);
    blitter.setModuloDestination(31);
    blitter.setShiftA(0);
    blitter.setShiftB(0);
    blitter.setLogicalFunction(0xf0);
    blitter.enableChannels(Blitter.MASK_ENABLE_A | Blitter.MASK_ENABLE_D);
    blitter.setChannelAMasks(0xffff, 0xffff);
    blitter.setAsFillMode(Blitter.FILL_OPTIONS_NONE);
    blitter.setDataRegisterA((char)0);
    blitter.setDataRegisterB((char)0);
    blitter.setDataRegisterC((char)0);

    short[] stars = Memory.allocStackShorts(stars_init.length);

    // Copy stars to RAM
    for (n = 0; n < stars.length; n++)
    {
      stars[n] = stars_init[n];
    }

    SongAmigaGrind.play();

    // Main part of method to animate the stars
    for (frame = 0; frame < 290; frame++)
    {
      if (frame == 37)
      {
        ImageJavaGrinder.init(copper, blitter);

        logo_offset_0 = ImageJavaGrinder.getCenter();
        logo_offset_1 = logo_offset_0 + 16000;
        logo_offset_2 = logo_offset_1 + 16000;

        showJavaGrinder(blitter);
      }
        else
      if (frame == 87)
      {
        clearLogo(blitter);
      }
        else
      if (frame == 133)
      {
        ImageAmigaLogo8.init(copper, blitter);

        logo_offset_0 = ImageAmigaLogo8.getCenter();
        logo_offset_1 = logo_offset_0 + 16000;
        logo_offset_2 = logo_offset_1 + 16000;

        showAmigaLogo(blitter);
      }
        else
      if (frame == 183)
      {
        clearLogo(blitter);
      }
        else
      if (frame == 233)
      {
        ImageCommodoreLogo.init(copper, blitter);

        logo_offset_0 = ImageCommodoreLogo.getCenter();
        logo_offset_1 = logo_offset_0 + 16000;
        logo_offset_2 = logo_offset_1 + 16000;

        showCommodoreLogo(blitter);
      }

      moveStars(stars);

      while (!Amiga.inVerticalBlank());
    }

    byte[] bitplanes = Display.bitplanes;

    for (n = 0; n < 8000; n++)
    {
      bitplanes[n] = 0;
      bitplanes[n + 16000] = 0;
    }

    n = 0;

    for (frame = 0; frame < 60; frame++)
    {
      while (!Amiga.inVerticalBlank());

      if (n == 0) { clearCommodoreLogo(blitter); }

      blitter.setShiftA(n);

      showCommodoreLogo(blitter);

      n = n + 2;

      if (n == 16)
      {
        n = 0;

        logo_offset_0 += 2;
        logo_offset_1 = logo_offset_0 + 16000;
        logo_offset_2 = logo_offset_1 + 16000;
      }
    }
  }

  public static short[] colors =
  {
    // Playfield even colors (0 to 7).
    0x000, 0xeee, 0x888, 0x444,
    0x000, 0x000, 0x000, 0x000,
    // Playfield odd colors (16 to 23).
    //0x000, 0xfff, 0xfff, 0xfff,
  };

  public static short[] stars_init =
  {
    // X0,  Y0,  DX,  DY   (160,100) is center
      140, 100,  -1,  -1,
      180,  80,   1,  -1,
      170,  120,  1,   2,
      190,  130,  2,   1,
      150,  130, -2,   1,
      150,  135, -2,   2,
      150,  90,  -1,  -2,
      170,  91,   1,  -2,
      165,  100,  2,   0,
      155,  100, -1,   0,
      140, 100,  -2,  -2,
      180,  80,   2,  -2,
      170,  120,  2,   3,
      190,  130,  3,   2,
      150,  130, -3,   2,
      150,  135, -3,   3,
      150,  90,  -2,  -3,
      170,  91,   2,  -3,
      165,  95,  3,   1,
      155,  100, -2,   1,
  };
}

