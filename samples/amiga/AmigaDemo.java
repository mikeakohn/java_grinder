
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;

public class AmigaDemo
{
  static public void main(String args[])
  {
    Copper copper = new Copper(200);
    Blitter blitter = new Blitter();

    Display.init();

    Amiga.disableMultitasking();
    //Amiga.disableInterrupts();

    // Setup 320x200 display.
    Amiga.setVideoMode(
      //Amiga.VIDEO_MODE_HIRES |
      Amiga.VIDEO_MODE_BITPLANE_COUNT_1 |
      Amiga.VIDEO_MODE_COLOR);

    Display.init();

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
    Logo.run(copper, blitter);

    Amiga.clearDMA(Amiga.DMA_AUDIO_0 |
                   Amiga.DMA_SPRITE);

    Mandelbrot.run(copper, blitter);

    while (true) { }
  }
}

