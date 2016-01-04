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

  // 11, 14, 19, 23. 29
  public static byte bass_line[] = 
  {
    29, 23, 19, 14, 11, 14, 19, 23, 29, 23, 19, 14, 11, 11, 11, 11,
    29, 23, 19, 14, 11, 14, 19, 23, 29, 23, 19, 14, 11, 11, 11, 11,
    29, 23, 19, 14, 11, 14, 19, 23, 29, 23, 19, 14, 11, 11, 11, 11,
    11, 11, 14, 0, 19, 19, 23, 0, 11, 11, 14, 0, 19, 19, 23, 0,
    29, 23, 19, 14, 11, 14, 19, 23, 29, 23, 19, 14, 11, 11, 11, 11,
    29, 23, 19, 14, 11, 14, 19, 23, 29, 23, 19, 14, 11, 11, 11, 11,
    11, 11, 14, 0, 19, 19, 23, 0, 11, 11, 14, 0, 19, 19, 23, 0,
    11, 11, 14, 0, 19, 19, 23, 0, 11, 11, 14, 0, 19, 19, 23, 0,
  };

  // 22, 25, 28
  public static byte lead_line[] = 
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    22, 22, 25, 0, 22, 22, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    22, 22, 25, 0, 22, 22, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    28, 0, 28, 0, 22, 0, 22, 0, 25, 0, 28, 0, 25, 0, 22, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    28, 22, 25, 0, 28, 22, 25, 0, 28, 22, 25, 0, 28, 28, 28, 0,
    28, 22, 25, 0, 28, 22, 25, 0, 28, 22, 25, 0, 25, 25, 25, 0,
    28, 22, 25, 0, 28, 22, 25, 0, 28, 22, 25, 0, 28, 25, 22, 0,
  };

  public static void main()
  {
    int timer = 0;
    int note = 0;
    int temp0, temp1;
    int color = 48;

    Atari2600.setTitlePos(96);
  
    Atari2600.setAudioControl0((byte)6);
    Atari2600.setAudioControl1((byte)4);
    
    Atari2600.setAudioVolume0((byte)0);
    Atari2600.setAudioVolume1((byte)0);

    Atari2600.setAudioFrequency0((byte)0);
    Atari2600.setAudioFrequency1((byte)0);

    while(true)
    {
      Atari2600.startVblank();
      Atari2600.setTitleColor((byte)color);

      if (Atari2600.isSwitchResetDown())
      {
        while(Atari2600.isSwitchResetDown())
        {
          // wait for release
        }

        Atari2600.setBank((byte)1);
      }

      timer++;
      if(timer > 8)
      {
        timer = 0;
        temp0 = bass_line[note];

	if(temp0 == 0)
          Atari2600.setAudioVolume0((byte)0);
        else
          Atari2600.setAudioVolume0((byte)15);

        temp1 = lead_line[note];

        if(temp1 == 0)
          Atari2600.setAudioVolume1((byte)0);
        else
          Atari2600.setAudioVolume1((byte)15);

        if(temp1 != 0)
        {
          color += 16;
//          if(color > 255)
//            color = 48;
        }

        Atari2600.setAudioFrequency0((byte)temp0);
        Atari2600.setAudioFrequency1((byte)temp1);

        note++;
        note &= 127;
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

