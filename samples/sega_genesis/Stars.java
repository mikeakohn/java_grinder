
import net.mikekohn.java_grinder.SegaGenesis;
import net.mikekohn.java_grinder.Memory;

public class Stars
{
  public static short[] palette =
  {
    0x000, 0xeee, 0xaaa, 0x888,
  };

  public static short[] stars_init =
  {
    // X0,  Y0,  DX,  DY   (160,112) is center
      140, 110,  -1,  -1,
      180,  80,   1,  -1,
  };

  public static void run()
  {
    int a,n;

    short[] stars = Memory.allocStackShorts(stars_init.length);

    // Need white and a few shades darker of pixels
    SegaGenesis.setPaletteColors(palette);

    // Copy stars to RAM
    for (n = 0; n < stars.length; n++)
    {
      stars[n] = stars_init[n];
    }

    for (a = 0; a < 300; a++)
    {
      while(!SegaGenesis.inVerticalBlank());

      for (n = 0; n < stars.length; n += 4)
      {
        SegaGenesis.plot(stars[n], stars[n + 1], 0);

        stars[n] += stars[n + 2];
        stars[n + 1] += stars[n + 3];

        if (stars[n] <= 0 || stars[n] >= 320 ||
            stars[n + 1] <= 0 || stars[n + 1] >= 224) 
        {
          stars[n] = stars_init[n];
          stars[n + 1] = stars_init[n + 1];
        }

        SegaGenesis.plot(stars[n], stars[n + 1], 1);
      }

      while(SegaGenesis.inVerticalBlank());
    }
  }
}

