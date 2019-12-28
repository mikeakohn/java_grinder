
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;
import net.mikekohn.java_grinder.Memory;

public class Stars
{
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
          Display.plot(x, y, 0);
        }

        stars[n + 0] = stars_init[n + 0];
        stars[n + 1] = stars_init[n + 1];
      }

      // Draw a trail of stars
      int x = stars[n], y = stars[n + 1];

      for (int c = 1; c <= 4; c++)
      {
        Display.plot(x, y, c);
        x -= stars[n + 2];
        y -= stars[n + 3];
      }
    }
  }

  public static void run(Copper copper, Blitter blitter)
  {
    int state = 0;
    int delay = 200;
    int text_color = 0;
    int a,n;

    Display.clear();
    Display.setDisplay(copper, 4);

    for (n = 0; n < 16; n++)
    {
      Display.setPalette(copper, n, colors[n]);
    }

    // Set up a blitter copy of 64x56 pixels.
    blitter.setModuloA(0);
    blitter.setModuloDestination(32);
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

    // Main part of method to animate the stars
    for (a = 0; a < 1000; a++)
    {
      if (delay == 0)
      {
        delay = 150;
      }
        else
      {
        delay--;
      }

      moveStars(stars);

      blitter.waitBusy();
      blitter.setSourceA(Memory.addressOf(ImageCommodoreLogo.bitplane_0));
      blitter.setDestination(Memory.addressOf(Display.bitplanes));
      blitter.runFill(4, 56);

      blitter.waitBusy();
      blitter.setSourceA(Memory.addressOf(ImageCommodoreLogo.bitplane_1));
      blitter.setDestination(Memory.addressOf(Display.bitplanes) + 8000);
      blitter.runFill(4, 56);

      while (!Amiga.inVerticalBlank());
    }
  }

  public static short[] colors =
  {
    0x000, 0xeee, 0x888, 0x444,
    0x000, 0x000, 0x000, 0x000,
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

