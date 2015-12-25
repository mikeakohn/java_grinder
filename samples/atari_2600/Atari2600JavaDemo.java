import net.mikekohn.java_grinder.Atari2600;

public class Atari2600JavaDemo
{
  public static int bass_note[] =
  {
    31, 29, 26, 23, 19, 17, 15, 14, 11, 9, 8, 7, 5, 4, 3, 2, 1, 0
  };

/*  public static int bass_line[] = 
  {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
  };
*/
  public static void main()
  {
    Atari2600.setColorPlayfield(0x80 | 0x70 | 0x02);
    Atari2600.setPlayfieldMode(Atari2600.PLAYFIELD_REFLECT);

    int n = 100;
    int timer = 0;
    int note = 0;

    Atari2600.setAudioControl0((byte)6);
    Atari2600.setAudioControl1((byte)12);
    
    Atari2600.setAudioVolume0((byte)15);
    Atari2600.setAudioVolume1((byte)0);

    while(true)
    {
      Atari2600.startVblank();

      timer++;
      if(timer > 10)
      {
        timer = 0;
        Atari2600.setAudioFrequency0((byte)(bass_note[note] | 32));
        note++;
        if(note > 15)
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

      n = n + 1;

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

