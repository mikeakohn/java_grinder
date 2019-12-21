
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;

public class Display
{
  static public byte[] bitplanes;
  static public int bitplane_count;

  static public void init()
  {
    // Allocate memory for up to 6 bit planes (around 48k).
    bitplanes = new byte[8000 * 6];

    Amiga.setPlayfieldScroll(0, 0);
    Amiga.setPlayfieldPriority(4, 4, false);
    Amiga.setBitplaneModuloEven(0);
    Amiga.setBitplaneModuloOdd(0);
    Amiga.setDisplayWindowStart(129, 44);
    Amiga.setDisplayWindowStop(193, 244);
    Amiga.setDisplayBitplaneStart(56);
    Amiga.setDisplayBitplaneStop(208);
  }

  static public void setDisplay(Copper copper, int bitplane_count)
  {
    int n, p = 0;

    Display.bitplane_count = bitplane_count;

    Amiga.setVideoMode(
      (bitplane_count << 12) |
      Amiga.VIDEO_MODE_COLOR);

    copper.stop();
    copper.resetIndex();

    p = Memory.addressOf(bitplanes);

    for (n = 0; n < bitplane_count; n++)
    {
      copper.appendSetBitplane(n, p);

      p += 8000;
    }

    for (n = 0; n < 1 << bitplane_count; n++)
    {
      copper.appendSetColor(n, 0);
    }

    copper.appendEnd();
    copper.run();
  }

  static public void setPalette(Copper copper, int index, int color)
  {

    int[] copper_list = copper.getArrayAsInt();

    copper_list[bitplane_count + index] = (0x180 << 16) | color;
/*
    int copper_index = copper.index;

    copper.setIndex(bitplane_count);
    copper.appendSetColor(index, color);
    copper.setIndex(copper_index);
*/
  }

  static public void waitForVerticalBlank()
  {
    while (Amiga.inVerticalBlank() == false);
    while (Amiga.inVerticalBlank() == true);
  }

  static public void waitForVericalBlanks(int count)
  {
    int n;

    for (n = 0; n < count; n++)
    {
      waitForVerticalBlank();
    }
  }
}

