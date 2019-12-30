
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.amiga.Copper;
import net.mikekohn.java_grinder.amiga.Amiga;

public class Image3Billion
{
  static public short[] palette =
  {
    0x000, 0x322, 0x665, 0x944, 0xdcd, 0xfff, 0xa98, 0x98d,
  };

  static public byte[] bitplanes = null;

  static public void show(Copper copper)
  {
    int n;
    bitplanes = Memory.preloadByteArray("Image3Billion.bin");

    Amiga.setVideoMode(
      Amiga.VIDEO_MODE_BITPLANE_COUNT_3 |
      Amiga.VIDEO_MODE_COLOR);

    copper.stop();
    copper.resetIndex();
    copper.appendSetBitplane(0, Memory.addressOf(bitplanes) + 0);
    copper.appendSetBitplane(1, Memory.addressOf(bitplanes) + 8000);
    copper.appendSetBitplane(2, Memory.addressOf(bitplanes) + 16000);

    for (n = 0; n < palette.length; n++)
    {
      copper.appendSetColor(n, palette[n]);
    }

    copper.appendEnd();
    copper.run();
  }
}

