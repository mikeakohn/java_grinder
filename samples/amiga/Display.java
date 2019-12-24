
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;

public class Display
{
  static public byte[] bitplanes;
  static public int bitplane_count;
  static public final int MEMORY_SIZE = 8000 * 6;

  static public void init()
  {
    // Allocate memory for up to 6 bit planes (around 48k).
    bitplanes = new byte[MEMORY_SIZE];

    Amiga.setPlayfieldScroll(0, 0);
    Amiga.setPlayfieldPriority(4, 4, false);
    Amiga.setBitplaneModuloEven(0);
    Amiga.setBitplaneModuloOdd(0);
    Amiga.setDisplayWindowStart(129, 44);
    Amiga.setDisplayWindowStop(193, 244);
    Amiga.setDisplayBitplaneStart(56);
    Amiga.setDisplayBitplaneStop(208);
  }

  static public void clear()
  {
    int n;

    for (n = 0; n < MEMORY_SIZE; n++)
    {
      bitplanes[n] = 0x00;
    }
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
    int palette = 0x180 + (index << 1);

    index = (bitplane_count << 1) + index;

    copper_list[index] = (palette << 16) | color;
  }

  static public void plot(int x, int y, int color)
  {
    int index;
    int n, mask, bit;

    index = (y * (320 / 8)) + (x >> 3);

    bit = 1 << (7 - (x & 0x7));
    mask = bit ^ 0xff;

    for (n = 0; n < bitplane_count; n++)
    {
      if ((color & 1) == 0)
      {
        bitplanes[index] &= (byte)mask;
      }
        else
      {
        bitplanes[index] |= (byte)bit;
      }

      color = color >> 1;

      index += 8000;
    }
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

