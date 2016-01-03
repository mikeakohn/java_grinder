import net.mikekohn.java_grinder.Atari2600;
import net.mikekohn.java_grinder.Memory;

public class SpaceRevengeTitle
{
  public static byte pf_left0[] =
  {
    0, 96, 96, 96, -32, 96, 96, -32, 
    0, -32, -32, -32, 0, -64, 96, -64, 
  };

  public static byte pf_left1[] =
  {
    0, -34, -34, -34, -104, -34, -40, -98, 
    0, -52, -20, -20, 108, -49, 12, -17, 
  };

  public static byte pf_left2[] =
  {
    0, -60, -50, -33, -37, -37, -37, -37, 
    0, 48, 48, 48, 48, -13, 102, -61, 
  };

  public static byte pf_right0[] =
  {
    0, -80, -80, -80, -128, -80, -128, -80, 
    0, -64, -64, -64, -64, -16, 96, 48, 
  };

  public static byte pf_right1[] =
  {
    0, 51, 119, 118, 86, 86, -41, -109, 
    0, 31, 63, 127, 112, 96, 48, 31, 
  };

  public static byte pf_right2[] =
  {
    0, 121, 123, 122, 27, 120, 27, 121, 
    0, 126, 126, 126, 6, 126, 6, 126, 
  };

  public static void main()
  {
    int switches;

    Atari2600.setTitlePos(0);
    Atari2600.setTitleColor(0x2e);
  
    while(true)
    {
      Atari2600.startVblank();

      switches = Memory.read8(0x282);

      if ((switches & Atari2600.SWITCH_RESET) == 0)
      {
        Atari2600.setBank((byte)1);
      }

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

