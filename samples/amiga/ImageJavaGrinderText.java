
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;
import net.mikekohn.java_grinder.Memory;

/*
 width: 128
height: 16
*/

public class ImageJavaGrinderText
{
  static final int width = 128;
  static final int height = 16;

  static public void init(Blitter blitter)
  {
    int n;

    // 320 / 8 = 40, 40 - (width / 8) = 26.
    blitter.setModuloDestination(40 - (width / 8));
  }

  public static void show(Blitter blitter)
  {
    int offset = Memory.addressOf(Display.bitplanes);

    offset += getCenter() + 24000;

    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(bitplane_0));
    blitter.setDestination(offset);
    blitter.runFill(width / 16, height);
  }

  static public int getCenter()
  {
    return (4000 + (5 * 40)) + (20 - 4);
  }

  static public byte[] bitplane_0 =
  {
    0, 3, -128, 0, 0, 0, 0, 7,
    -64, 0, 48, 0, 1, -64, 0, 0,
    0, 3, -128, 0, 0, 0, 0, 7,
    -64, 0, 48, 0, 1, -64, 0, 0,
    0, 1, -128, 0, 0, 0, 0, 12,
    96, 0, 0, 0, 0, -64, 0, 0,
    0, 1, -128, 0, 0, 0, 0, 12,
    96, 0, 0, 0, 0, -64, 0, 0,
    0, 1, -113, -122, 24, -8, 0, 24,
    7, 112, 112, 63, 14, -63, -15, -36,
    0, 1, -113, -122, 24, -8, 0, 24,
    7, 112, 112, 63, 14, -63, -15, -36,
    0, 1, -128, -58, 24, 12, 0, 24,
    -29, -104, 48, 49, -103, -61, 24, -26,
    0, 1, -128, -58, 24, 12, 0, 24,
    -29, -104, 48, 49, -103, -61, 24, -26,
    0, 49, -121, -61, 48, 124, 0, 24,
    99, 24, 48, 49, -104, -61, -8, -58,
    0, 49, -121, -61, 48, 124, 0, 24,
    99, 24, 48, 49, -104, -61, -8, -58,
    0, 49, -104, -63, -31, -116, 0, 12,
    99, 0, 48, 49, -104, -61, 0, -64,
    0, 49, -104, -63, -31, -116, 0, 12,
    99, 0, 48, 49, -104, -61, 0, -64,
    0, 31, 15, 96, -64, -10, 0, 7,
    -25, -128, 120, 49, -113, 97, -15, -32,
    0, 31, 15, 96, -64, -10, 0, 7,
    -25, -128, 120, 49, -113, 97, -15, -32,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
  };
}

