import net.mikekohn.java_grinder.Atari2600;
import net.mikekohn.java_grinder.Memory;

public class SpaceRevengeTitle
{
  public static byte pf_left0[] =
  {
(byte)0x00, (byte)0xc0, (byte)0xe0, (byte)0xe0, (byte)0x60, (byte)0x00, (byte)0x00, (byte)0xe0, (byte)0x00, (byte)0xe0, (byte)0xe0, (byte)0xe0, (byte)0x00, (byte)0xe0, (byte)0x60, (byte)0xc0
  };

  public static byte pf_left1[] =
  {
    (byte)0x00, (byte)0xe0, (byte)0xf1, (byte)0xf1, (byte)0x31, (byte)0x31, (byte)0x31, (byte)0xfd, (byte)0x00, (byte)0xcc, (byte)0xec, (byte)0xec, (byte)0x6c, (byte)0xcf, (byte)0x0c, (byte)0xef
  };

  public static byte pf_left2[] =
  {
    (byte)0x00, (byte)0x3f, (byte)0x7f, (byte)0x7f, (byte)0x61, (byte)0x61, (byte)0x61, (byte)0x61, (byte)0x00, (byte)0x30, (byte)0x30, (byte)0x30, (byte)0x30, (byte)0xf3, (byte)0x66, (byte)0xc3
  };

  public static byte pf_right0[] =
  {
    (byte)0x00, (byte)0x30, (byte)0x30, (byte)0x30, (byte)0xb0, (byte)0xf0, (byte)0xf0, (byte)0xf0, (byte)0x00, (byte)0xc0, (byte)0xc0, (byte)0xc0, (byte)0xc0, (byte)0xf0, (byte)0x60, (byte)0x30
  };

  public static byte pf_right1[] =
  {
    (byte)0x00, (byte)0x3d, (byte)0x7d, (byte)0xfd, (byte)0xed, (byte)0xcd, (byte)0x8d, (byte)0x0d, (byte)0x00, (byte)0x1f, (byte)0x3f, (byte)0x7f, (byte)0x70, (byte)0x60, (byte)0x30, (byte)0x1f
  };

  public static byte pf_right2[] =
  {
    (byte)0x00, (byte)0x71, (byte)0x79, (byte)0x3d, (byte)0x1f, (byte)0x1f, (byte)0x39, (byte)0x71, (byte)0x00, (byte)0x7e, (byte)0x7e, (byte)0x7e, (byte)0x06, (byte)0x7e, (byte)0x06, (byte)0x7e
  };

  public static void main()
  {
    Atari2600.setTitlePos(0);
    Atari2600.setTitleColor(0x2e);
  
    while(true)
    {
      Atari2600.startVblank();

      // set horizontal positions
      Atari2600.setPlayer0Position((byte)0, (byte)100);
      Atari2600.setPlayer1Position((byte)0, (byte)100);
      Atari2600.setMissile0Position((byte)0, (byte)100);
      Atari2600.setMissile1Position((byte)0, (byte)100);
      Atari2600.setBallPosition((byte)0, (byte)100);

      Atari2600.waitVblank();
      Atari2600.drawTitleScreen();
      Atari2600.startOverscan();

      Atari2600.waitOverscan();
    }
  }
}

