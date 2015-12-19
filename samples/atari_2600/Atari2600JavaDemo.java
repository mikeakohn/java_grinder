import net.mikekohn.java_grinder.Atari2600;

public class Atari2600JavaDemo
{
  public static void main()
  {
    int n;

    while(true)
    {
      Atari2600.setColorBackground(0x80 | 0x70 | 0x04);

      for (n = 0; n < 100; n++)
      {
        Atari2600.waitHsync();
      }

      Atari2600.setColorBackground(0x80 | 0x70 | 0x02);

      for (n = 0; n < 162; n++)
      {
        Atari2600.waitHsync();
      }

      Atari2600.vsyncOn();
      Atari2600.vblankOn();

      for (n = 0; n < 3; n++)
      {
        Atari2600.waitHsync();
      }

      Atari2600.vsyncOff();

      for (n = 0; n < 34; n++)
      {
        Atari2600.waitHsync();
      }

      Atari2600.vblankOff();
    }
  }
}

