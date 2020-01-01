
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;

public class AmigaDemo
{
/*
  static char[] sprite_old =
  {
    0x5555, 0x5900,
    0x00f0, 0x000f,
    0x00f0, 0x0a0f,
    0x00f0, 0x000f,
    0x00f0, 0x000f,
    0x00f0, 0x000f,
  };
*/

  static public void main(String args[])
  {
    //char[] sprite = new char[12];
    //char[] sprite_blank = new char[4];
    Copper copper = new Copper(200);
    Blitter blitter = new Blitter();

    Display.init();

    //sprite_blank[0] = 0;
    //sprite_blank[1] = 0;

    //for (int i = 0; i < 8000; i++) { bitplane_1[i] = 0x0f; }
    //for (int i = 0; i < 12; i += 2) { sprite[i] = 0x0f; sprite[i + 1] = 0xf0; }

    //sprite[0] = 0x5555;
    //sprite[1] = 0x5900;

    Amiga.disableMultitasking();
    //Amiga.disableInterrupts();

/*
    //copper.appendInstruction(Copper.MOVE_BPLCON0 | 0x1200);
    copper.appendSetBitplane(0, bitplane_1);
    copper.appendSetSprite(0, sprite_old);
    copper.appendSetSprite(1, sprite_blank);
    copper.appendSetSprite(2, sprite_blank);
    copper.appendSetSprite(3, sprite_blank);
    copper.appendSetSprite(4, sprite_blank);
    copper.appendSetSprite(5, sprite_blank);
    copper.appendSetSprite(6, sprite_blank);
    copper.appendSetSprite(7, sprite_blank);
    //copper.appendInstruction(Copper.MOVE_SPR0POS | 0x5555);
    //copper.appendInstruction(Copper.MOVE_SPR0CTL | 0x5900);
    copper.appendSetColor(0, 0x00f);
    copper.appendSetColor(1, 0x700);
    copper.appendWait(0, 100);
    copper.appendSetColor(0, 0xf00);
    copper.appendWait(0, 150);
    copper.appendSetColor(0, 0x0f0);
    copper.appendEnd();
    copper.run();
*/

    // Setup 320x200 display.
    Amiga.setVideoMode(
      //Amiga.VIDEO_MODE_HIRES |
      Amiga.VIDEO_MODE_BITPLANE_COUNT_1 |
      Amiga.VIDEO_MODE_COLOR);

    Display.init();

/*
    Amiga.setPlayfieldScroll(0, 0);
    Amiga.setPlayfieldPriority(4, 4, false);
    Amiga.setBitplaneModuloEven(0);
    Amiga.setBitplaneModuloOdd(0);
    Amiga.setDisplayWindowStart(129, 44);
    Amiga.setDisplayWindowStop(193, 244);
    Amiga.setDisplayBitplaneStart(56);
    Amiga.setDisplayBitplaneStop(208);
*/

    Amiga.setDMA(
      Amiga.DMA_BLITTER |
      Amiga.DMA_COPPER |
      Amiga.DMA_BITPLANE);

    Amiga.clearDMA(Amiga.DMA_SPRITE);

    // Show JavaGrinder Billion devices logo.
    Image3Billion.show(copper);
    Display.waitForVericalBlanks(80);

    // After a pause, play Bach's Invention 13.
    SongBach.play();
    Display.waitForVericalBlanks(260);
    Amiga.clearDMA(Amiga.DMA_AUDIO_0);

    Display.clear();
    Display.waitForVericalBlanks(5);

    Stars.run(copper, blitter);
    Mandelbrot.run(copper);

    while (true) { }
  }
}

