
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

  static public void setDoublePlayfield()
  {
    Amiga.setVideoMode(
      (bitplane_count << 12) |
      Amiga.VIDEO_MODE_DOUBLE_PLAYFIELD |
      Amiga.VIDEO_MODE_COLOR);
  }

  static public void setPalette(Copper copper, int index, int color)
  {

    int[] copper_list = copper.getArrayAsInt();
    int palette = 0x180 + (index << 1);

    index = (bitplane_count << 1) + index;

    copper_list[index] = (palette << 16) | color;
  }

  static public void drawLine(int x0, int y0, int x1, int y1, int color)
  {
    int count;
    int dx,dy;
    int i,j;

    if (x0 == x1)
    { 
      if (y1 < y0) { j = -1; } else { j = 1; }
      
      while (y0 <= y1)
      { 
        Display.plot(x0, y0, color);
        y0 += j;
      }

      return;
    }

    if (y0 == y1)
    {
      if (x1 < x0) { i = -1; } else { i = 1; }

      while (x0 <= x1)
      {
        Display.plot(x0, y0, color);
        x0 += i;
      }

      return;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    i = dx;
    j = dy;

    if (i < 0) { i = -i; }
    if (j < 0) { j = -j; }

    count = 0;

    if (i > j)
    {
      // If the change in X is bigger, move the line horizontal

      i = 1; j = 1;

      if (dx < 0) { dx = -dx; i = -1; }
      if (dy < 0) { dy = -dy; j = -1; }

      count = dx >> 1;

      while (x0 != x1)
      {
        Display.plot(x0, y0, color);

        x0 += i;
        count += dy;
        if (count > dx) { count -= dx; y0 += j; }
      }
    }
      else
    {
      // If the change in Y is bigger, move the line vertical

      i = 1; j = 1;

      if (dy < 0) { dy = -dy; j = -1; }
      if (dx < 0) { dx = -dx; i = -1; }

      count = dy >> 1;

      while (y0 != y1)
      {
        Display.plot(x0, y0, color);

        y0 += j;
        count += dx;
        if (count > dy) { count -= dy; x0 += i; }
      }
    }
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

  static public void plotEven(int x, int y, int color)
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

      index += 16000;
    }
  }

  static public void plotOdd(int x, int y, int color)
  {
    int index;
    int n, mask, bit;

    index = (y * (320 / 8)) + (x >> 3);

    index += 8000;

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

      index += 16000;
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

