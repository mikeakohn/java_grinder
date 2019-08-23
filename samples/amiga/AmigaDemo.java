
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
    //int[] copper_code = new int[20];
    //Copper copper = new Copper(20);

    Amiga.disableMultitasking();
    Amiga.disableInterrupts();

    //Copper.setWait(copper_code, 0, 10, 10);
    //copper.appendWait(10, 10);
    //copper.appendInstruction(0x12349876);

    //copper.appendSetColor(0, 0x000);
    //copper.appendWait(0, 50);
    //copper.appendSetColor(0, 0xf00);
    //copper.appendWait(0, 70);
    //copper.appendSetColor(0, 0x0f0);
    //copper.appendEnd();

    while(true)
    {
      Amiga.setSpriteImage(0, sprite);
      Amiga.setSpritePosition(0, 100, 100, 105);

      Amiga.setPalette(16, 0xfff);
      Amiga.setPalette(17, 0xfff);
      Amiga.setPalette(18, 0xfff);
      Amiga.setPalette(19, 0xfff);

      Amiga.setPalette(0, 0xf0f);
      Amiga.setPalette(1, 0x000);
    }
  }
}

