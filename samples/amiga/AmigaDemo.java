
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
    Copper copper = new Copper(20);

    Amiga.disableMultitasking();
    Amiga.disableInterrupts();

    copper.appendSetColor(0, 0x00f);
    copper.appendWait(0, 100);
    copper.appendSetColor(0, 0xf00);
    copper.appendWait(0, 150);
    copper.appendSetColor(0, 0x0f0);
    copper.appendEnd();
    copper.run();

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

