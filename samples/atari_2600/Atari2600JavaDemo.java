import net.mikekohn.java_grinder.Atari2600;

public class Atari2600JavaDemo
{
  public static byte ship[] =
  {
    (byte)0b00011000,
    (byte)0b00011000,
    (byte)0b00011000,
    (byte)0b10111101,
    (byte)0b11100111,
    (byte)0b11100111,
    (byte)0b11111111,
    (byte)0b10011001,
    (byte)0b00000000,
  };

  public static byte shot[] =
  {
    (byte)0b11111111,
    (byte)0b11111111,
    (byte)0b11111111,
    (byte)0b11111111,
    (byte)0b11111111,
    (byte)0b11111111,
    (byte)0b11111111,
    (byte)0b11111111,
    (byte)0b00000000,
  };

  public static void main()
  {
    while(true)
    {
      Atari2600.setPlayer0Sprite(ship);
      Atari2600.setPlayer1Sprite(ship);
      Atari2600.setMissile0Sprite(shot);
      Atari2600.setMissile1Sprite(shot);
      Atari2600.setBallSprite(shot);

      Atari2600.setColorPlayer0(0x3a);
      Atari2600.setColorPlayer1(0x5a);
      Atari2600.setColorPlayfield(0x8a);

      Atari2600.setPlayer0Position((byte)10, (byte)10);
      Atari2600.setPlayer1Position((byte)20, (byte)10);
      Atari2600.setMissile0Position((byte)30, (byte)10);
      Atari2600.setMissile1Position((byte)40, (byte)10);
      Atari2600.setBallPosition((byte)50, (byte)10);

      Atari2600.startVblank();
      Atari2600.waitVblank();
      Atari2600.drawScreen();
      Atari2600.startOverscan();
      Atari2600.waitOverscan();
    }
  }
}

/*
public class Atari2600JavaDemo
{
  public static int bass_waveform[] = 
  {
    8, 6, 6, 6, 8, 6, 6, 6, 8, 6, 6, 6, 8, 6, 8, 6,
    8, 6, 6, 6, 8, 6, 6, 6, 8, 6, 6, 6, 8, 6, 8, 6,
    8, 6, 6, 6, 8, 6, 6, 6, 8, 6, 6, 6, 8, 6, 8, 6,
    8, 6, 6, 6, 8, 6, 6, 6, 8, 6, 6, 6, 8, 6, 8, 6,
  };

  public static int bass_line[] = 
  {
    29, -1, 19, -1, 14, -1, 19, -1, 29, -1, 19, -1, 14, -1, 19, -1,
    23, -1, 11, 11, 14, -1, 11, -1, 23, -1, 11, 11, 14, -1, 11, -1,
    29, -1, 19, -1, 14, -1, 19, -1, 29, -1, 19, -1, 14, -1, 19, -1,
    23, -1, 11, 11, 14, -1, 11, -1, 23, -1, 11, 11, 14, -1, 11, -1,
  };

  public static int lead_line[] = 
  {
    28, 25, 28, -1, 28, 25, 28, -1, 28, 28, -1, -1, 28, 28, -1, -1,
    22, 22, 25, -1, 22, 25, -1, -1, 22, 22, 25, -1, 28, 25, 28, 28,
    28, -1, 25, -1, 25, -1, 28, -1, 28, -1, 25, -1, 25, 28, 28, -1,
    22, 22, 25, -1, 22, 25, -1, -1, 22, 22, 25, -1, 28, 25, 28, 28,
  };

  public static void main()
  {
    Atari2600.setColorPlayfield(0x80 | 0x70 | 0x02);
    Atari2600.setPlayfieldMode(Atari2600.PLAYFIELD_REFLECT);

    int n = 100;
    int timer = 0;
    int note = 0;
    int temp0, temp1;

    Atari2600.setAudioControl0((byte)6);
    Atari2600.setAudioControl1((byte)4);
    
    Atari2600.setAudioVolume0((byte)0);
    Atari2600.setAudioVolume1((byte)0);

    Atari2600.setAudioFrequency0((byte)0);
    Atari2600.setAudioFrequency1((byte)0);

    while(true)
    {
      Atari2600.startVblank();

      timer++;
      if(timer > 8)
      {
        timer = 0;
        temp0 = bass_line[note];

        if(temp0 < 0)
          Atari2600.setAudioVolume0((byte)0);
        else
          Atari2600.setAudioVolume0((byte)15);

        temp1 = lead_line[note];

        if(temp1 < 0)
          Atari2600.setAudioVolume1((byte)0);
        else
          Atari2600.setAudioVolume1((byte)15);

        Atari2600.setAudioControl0((byte)bass_waveform[note]);
        Atari2600.setAudioFrequency0((byte)temp0);
        Atari2600.setAudioFrequency1((byte)temp1);

        note++;
        if(note >= 64)
          note = 0;
      }

      Atari2600.waitVblank();

      // Draw on screen
      //Atari2600.setColorBackground(0x80 | 0x70 | 0x04);
      Atari2600.waitHsync(10);
      Atari2600.setColorBackground(0x80 | 0x70 | 0x02);
      Atari2600.waitHsync(10);
      Atari2600.setColorBackground(0x80 | 0x70 | 0x01);
      Atari2600.waitHsync(10);

      Atari2600.setPlayfield2((byte)0x98);
      Atari2600.setColorPlayfield(n);
      Atari2600.waitHsync(70);

      Atari2600.setPlayfield2((byte)0x8c);
      Atari2600.setColorPlayfield(n);
      Atari2600.waitHsync(25);

      Atari2600.setPlayfield2((byte)0x86);
      Atari2600.setColorPlayfield(n);
      Atari2600.waitHsync(20);

      Atari2600.setPlayfield2((byte)0x83);
      Atari2600.setColorPlayfield(n);
      Atari2600.waitHsync(15);

      Atari2600.setPlayfield2((byte)0x81);
      Atari2600.setPlayfield1((byte)0x01);
      Atari2600.setColorPlayfield(n);
      Atari2600.waitHsync(10);

      Atari2600.setPlayfield2((byte)0x00);
      Atari2600.setPlayfield1((byte)0x00);
      Atari2600.setColorPlayfield(n);
      Atari2600.waitHsync(22);

      Atari2600.startOverscan();

      n++;

      if (Atari2600.isJoystick0Left())
      {
        Atari2600.setColorBackground(0x80 | 0x70 | 0x01);
      }
        else
      if (Atari2600.isJoystick0Right())
      {
        Atari2600.setColorBackground(0x80 | 0x70 | 0x02);
      }
        else
      if (Atari2600.isJoystick0Down())
      {
        Atari2600.setColorBackground(0x80 | 0x74 | 0x02);
      }
        else
      if (Atari2600.isJoystick0Up())
      {
        Atari2600.setColorBackground(0x80 | 0x64 | 0x02);
      }
        else
      if (Atari2600.isJoystick0ButtonDown())
      {
        Atari2600.setColorBackground(0x80 | 0x64 | 0x08);
      }
        else
      {
        Atari2600.setColorBackground(0x80 | 0x70 | 0x04);
      }

      Atari2600.waitOverscan();
    }
  }
}
*/

