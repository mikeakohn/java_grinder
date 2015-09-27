
import net.mikekohn.java_grinder.SegaGenesis;

public class Box
{
  public static void run()
  {
    int i;
    int x,y,r;

    SegaGenesis.initBitmap();

    SegaGenesis.setPalettePointer(0);
    SegaGenesis.setPaletteColor(0x000);
    SegaGenesis.setPaletteColor(0xeee);

    r = 0;
    x = 160; y = 112;

    for (i = 0; i < 90; i++)
    {
      while(!SegaGenesis.inVerticalBlank());
      while(SegaGenesis.inVerticalBlank());

      Common.drawLine(160, 112, x, y, 0);

      x = ((Common.cos[r] << 4) >> 6) + 160;
      y = ((Common.sin[r] << 4) >> 6) + 112;

      Common.drawLine(160, 112, x, y, 1);

      r++;
      if (r == 90) { r = 0; }
    }
  }
}

