import net.mikekohn.java_grinder.Atari2600;

public class Atari2600JavaDemo
{
  public static void main()
  {
    int n;

    while(true)
    {
      // 3 lines of vsync
      Atari2600.vblankOn();
      Atari2600.vsyncOn();

      // This should be faster than a loop
      Atari2600.waitHsync();
      Atari2600.waitHsync();
      Atari2600.waitHsync();

      Atari2600.vsyncOff();

      // 37 lines of blank
      for (n = 0; n < 37; n++)
      {
        Atari2600.waitHsync();
      }

      Atari2600.vblankOff();

      // Draw on screen
      Atari2600.setColorBackground(0x80 | 0x70 | 0x04);

      for (n = 0; n < 100; n++)
      {
        Atari2600.waitHsync();
      }

      Atari2600.setColorBackground(0x80 | 0x70 | 0x02);

      for (n = 0; n < 192; n++)
      {
        Atari2600.waitHsync();
      }

      // Overscan
      Atari2600.vblankOn();
      for (n = 0; n < 30; n++)
      {
        Atari2600.waitHsync();
      }

    }
  }
}

