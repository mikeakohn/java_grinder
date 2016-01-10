import net.mikekohn.java_grinder.Atari2600;
import net.mikekohn.java_grinder.Memory;

public class SpaceRevengeTitle
{
  public static byte pf_left0[] =
  {
    -64, 96, -64, 0, -32, -32, -32, 0, 
    -32, 96, 96, -32, 96, 96, 96, 0, 
  };

  public static byte pf_left1[] =
  {
    -17, 12, -49, 108, -20, -20, -52, 0, 
    -98, -40, -34, -104, -34, -34, -34, 0, 
  };

  public static byte pf_left2[] =
  {
    -61, 102, -13, 48, 48, 48, 48, 0, 
    -37, -37, -37, -37, -33, -50, -60, 0, 
  };

  public static byte pf_right0[] =
  {
    48, 96, -16, -64, -64, -64, -64, 0, 
    -80, -128, -80, -128, -80, -80, -80, 0, 
  };

  public static byte pf_right1[] =
  {
    31, 48, 96, 112, 127, 63, 31, 0, 
    -109, -41, 86, 86, 118, 119, 51, 0, 
  };

  public static byte pf_right2[] =
  {
    126, 6, 126, 6, 126, 126, 126, 0, 
    121, 27, 120, 27, 122, 123, 121, 0, 
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

  public static byte logo0[] =
  {
    (byte)0b00001100,
    (byte)0b00001010,
    (byte)0b00001100,
    (byte)0b00001010,
    (byte)0b00001110,
    (byte)0b00001100,
    (byte)0b00000000,
    (byte)0b11101110,
    (byte)0b01001010,
    (byte)0b01001010,
    (byte)0b01001010,
    (byte)0b11001110,
    (byte)0b11001110,
    (byte)0b00000000,
    (byte)0b00011101,
    (byte)0b11000101,
    (byte)0b10011101,
    (byte)0b10010001,
    (byte)0b11011101,
    (byte)0b00011101,
    (byte)0b00000000,
    (byte)0b10101010,
  };

  public static byte logo1[] =
  {
    (byte)0b10100000,
    (byte)0b10100000,
    (byte)0b11100000,
    (byte)0b00100000,
    (byte)0b11100000,
    (byte)0b11100000,
    (byte)0b00000000,
    (byte)0b11100110,
    (byte)0b10000101,
    (byte)0b11100101,
    (byte)0b10000101,
    (byte)0b11100111,
    (byte)0b11100110,
    (byte)0b00000000,
    (byte)0b11010111,
    (byte)0b01010100,
    (byte)0b01010111,
    (byte)0b01010101,
    (byte)0b11010111,
    (byte)0b11010111,
    (byte)0b00000000,
    (byte)0b10101010,
  };

  public static void main()
  {
    int timer = 0;
    int note = 0;
    int last = 0;
    int temp0, temp1;
    int color = 48;
    int xpos = 0;
    int ypos = 0;
    int title_pos = 64;
    int inc = 1;
  
    Atari2600.setAudioControl0((byte)6);
    Atari2600.setAudioControl1((byte)4);
    
    Atari2600.setAudioVolume0((byte)0);
    Atari2600.setAudioVolume1((byte)0);

    Atari2600.setAudioFrequency0((byte)0);
    Atari2600.setAudioFrequency1((byte)0);

    Atari2600.setPlayer0Sprite(logo0);
    Atari2600.setPlayer1Sprite(logo1);
    Atari2600.setColorPlayer0(0x0e);
    Atari2600.setColorPlayer1(0x0e);

    while(true)
    {
      Atari2600.startVblank();
      Atari2600.setTitleColor((byte)color);
      Atari2600.clearMotionRegisters();

      if (Atari2600.isSwitchResetDown())
      {
        while(Atari2600.isSwitchResetDown())
        {
          // wait for release
        }

        Atari2600.setBank((byte)1);
      }

      timer++;
      if(timer > 7)
      {
        Atari2600.setTitlePos(title_pos);
        title_pos += inc;
        if(title_pos < 32 || title_pos > 92)
          inc = 0 - inc;

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

        if(temp1 != last)
          color += 8;

        last = temp1;

        Atari2600.setAudioFrequency0((byte)temp0);
        Atari2600.setAudioFrequency1((byte)temp1);

        note++;
        note &= 127;
      }

      Atari2600.setPlayer0Position((byte)64, (byte)4);
      Atari2600.setPlayer1Position((byte)71, (byte)4);
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

