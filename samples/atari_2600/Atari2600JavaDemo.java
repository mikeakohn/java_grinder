import net.mikekohn.java_grinder.Atari2600;

public class Atari2600JavaDemo
{
  public static void main()
  {
    int n;

    while(true)
    {
      Atari2600.startVblank();
      Atari2600.waitVblank();

      // Draw on screen
      Atari2600.setColorBackground(0x80 | 0x70 | 0x04);
      //Atari2600.setColorBackground(0x70 | 0x04);

      for (n = 0; n < 100; n++)
      {
        Atari2600.waitHsync();
      }

      Atari2600.setColorBackground(0x80 | 0x70 | 0x02);
      //Atari2600.setColorBackground(0x70 | 0x02);

      for (n = 0; n < 92; n++)
      {
        Atari2600.waitHsync();
      }

      Atari2600.startOverscan();
      Atari2600.waitOverscan();
    }
  }
}

