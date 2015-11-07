
import net.mikekohn.java_grinder.SegaGenesis;

public class ClosingPage
{
  static public void run()
  {
    int a, b, offset;

    SegaGenesis.clearText();
    SegaGenesis.setCursor(14, 12);
    SegaGenesis.print("JAVA[GRINDER");

    offset = 0;

    while(true)
    {
      while(SegaGenesis.inVerticalBlank());

      for (b = 0; b < 11 * 8 + (offset >> 2); b++)
      {
        SegaGenesis.waitHorizontalBlank();
      }

      for (a = 0; a < 0xf; a++)
      {
        SegaGenesis.setPalettePointer(49);
        SegaGenesis.setPaletteColor((a << 8) | (a << 4) | a);
        SegaGenesis.waitHorizontalBlank();
      }

      SegaGenesis.waitVerticalBlank();

      offset++;
      if (offset == 48) { offset = 0; }
    }
  }
}

