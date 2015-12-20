import net.mikekohn.java_grinder.Atari2600;

public class Atari2600JavaDemo
{
  public static void main()
  {
    while(true)
    {
      Atari2600.startVblank();
      Atari2600.waitVblank();

      // Draw on screen
      Atari2600.setColorBackground(0x80 | 0x70 | 0x04);
      Atari2600.waitHsync(100);

      Atari2600.setColorBackground(0x80 | 0x70 | 0x02);
      Atari2600.waitHsync(92);

      Atari2600.startOverscan();
      Atari2600.waitOverscan();
    }
  }
}

