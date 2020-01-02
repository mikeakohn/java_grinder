
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;
import net.mikekohn.java_grinder.Memory;

public class ImageCodedUsing
{
  static final int width = 112;
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
    return (4000 - (13 * 40)) + (20 - 2);
  }

  static public byte[] bitplane_0 =
  {
    15, -128, 0, 56, 0, 3, -128, 0,
    0, 0, 6, 0, 0, 0, 15, -128,
    0, 56, 0, 3, -128, 0, 0, 0,
    6, 0, 0, 0, 24, -64, 0, 24,
    0, 1, -128, 0, 0, 0, 0, 0,
    0, 0, 24, -64, 0, 24, 0, 1,
    -128, 0, 0, 0, 0, 0, 0, 0,
    48, 3, -31, -40, 62, 29, -128, 3,
    24, 63, 14, 7, -31, -20, 48, 3,
    -31, -40, 62, 29, -128, 3, 24, 63,
    14, 7, -31, -20, 48, 6, 51, 56,
    99, 51, -128, 3, 24, 96, 6, 6,
    51, 24, 48, 6, 51, 56, 99, 51,
    -128, 3, 24, 96, 6, 6, 51, 24,
    48, 6, 51, 24, 127, 49, -128, 3,
    24, 62, 6, 6, 51, 24, 48, 6,
    51, 24, 127, 49, -128, 3, 24, 62,
    6, 6, 51, 24, 24, -58, 51, 24,
    96, 49, -128, 3, 24, 3, 6, 6,
    49, -16, 24, -58, 51, 24, 96, 49,
    -128, 3, 24, 3, 6, 6, 49, -16,
    15, -125, -31, -20, 62, 30, -64, 1,
    -20, 126, 15, 6, 48, 24, 15, -125,
    -31, -20, 62, 30, -64, 1, -20, 126,
    15, 6, 48, 24, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    3, -16, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, -16,
  };
}

