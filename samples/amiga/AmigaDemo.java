
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;

public class AmigaDemo
{
  static int[] sprite =
  {
    0x00000a0a,
    0x00000a0a,
    0x00000a0a,
    0x00000a05,
    0x00000a05,
    0x00000a05,
  };

  static public void main(String args[])
  {
    byte[] bitplane_1 = new byte[16000];
    byte[] blitter_a = new byte[128];
    Copper copper = new Copper(20);
    Blitter blitter = new Blitter();

    for (int i = 0; i < 16000; i++) { bitplane_1[i] = 0x0f; }

    Amiga.disableMultitasking();
    //Amiga.disableInterrupts();

    copper.appendSetBitplane(0, bitplane_1);
    copper.appendSetColor(0, 0x00f);
    copper.appendSetColor(1, 0xff0);
    copper.appendWait(0, 100);
    copper.appendSetColor(0, 0xf00);
    copper.appendWait(0, 150);
    copper.appendSetColor(0, 0x0f0);
    copper.appendEnd();
    copper.run();

    Amiga.setVideoMode(
      //Amiga.VIDEO_MODE_HIRES |
      Amiga.VIDEO_MODE_BITPLANE_COUNT_1 |
      Amiga.VIDEO_MODE_COLOR);

    Amiga.setPlayfieldScroll(0, 0);
    Amiga.setPlayfieldPriority(4, 4, false);
    Amiga.setBitplaneModuloEven(0);
    Amiga.setBitplaneModuloOdd(0);
    Amiga.setDisplayWindowStart(129, 44);
    Amiga.setDisplayWindowStop(193, 244);
    Amiga.setDisplayBitplaneStart(56);
    Amiga.setDisplayBitplaneStop(208);

    Amiga.setDMA(
      Amiga.DMA_SPRITE |
      Amiga.DMA_BLITTER |
      Amiga.DMA_COPPER |
      Amiga.DMA_BITPLANE);

    Memory.clearArray(blitter_a);
    blitter.setSourceA(blitter_a);
    //blitter.runFill(10, 10);

    while(true)
    {
      Amiga.setSpriteImage(0, sprite);
      Amiga.setSpritePosition(0, 100, 100, 105);

      Amiga.setPalette(16, 0xfff);
      Amiga.setPalette(17, 0xfff);
      Amiga.setPalette(18, 0xfff);
      Amiga.setPalette(19, 0xfff);

      //Amiga.setPalette(0, 0xf0f);
      //Amiga.setPalette(1, 0x000);
    }
  }
}

