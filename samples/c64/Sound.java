import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.Keyboard;
import net.mikekohn.java_grinder.c64.*;

// Music program (work-in-progress)
// --------------------------------
// Chance voice 1-3 (F1, F3, F5)
// Move cursor: , and . (Shift speeds it up)
// Play song: Return, Run/Stop to cancel
// Change octave (V and Shift-V, not reported to screen yet)
//
// There is currently a 120-note limit per voice

public class Sound
{
/*
  static public String keys =
    "~/,NVX~~"
  + "Q~@OUTE~"
  + "~=:KHFS~"
  + " ~.MBCZ~"
  + "2~-0864~" 
  + "~;LJGDA~"
  + "~*PIYRW~"
  + "1~+9753~";
*/

  // q2w3er5t6y7ui9o0p@
  static public int piano_keys[] =
  {
    -1, -1, -1, -1, -1, -1, -1, -1,
    0, -1, 17, 14, 11, 7, 4, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    32, -1, -1, -1, -1, -1, -1, -1,
    1, -1, -1, 15, -1, 8, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, 16, 12, 9, 5, 2, -1,
    -1, -1, -1, 13, 10, 6, 3, -1
  };

  // one octave of note frequencies
  static public int freq[] =
  {
    268, 284, 301, 318, 337, 358, 379, 401, 425,
    451, 477, 506, 536, 568, 602, 637, 675, 716
  };

  static public String note_keys = "Q2W3ER5T6Y7UI9O0P@                      ";
  static public String hex = "0123456789ABCDEF";
  static int voice = 0;
  static int octave[] = { 2, 3, 4 };
  static int wave[] = { 2, 1, 0 };
  static int waveforms[] = { 16, 32, 64, 128 };
  static int pulse[] = { 0x800, 0x1200, 0x600 };
  static int adsr[] = { 0xa444, 0xa444, 0xa444 };
  static int[] song = new int[256 * 3];
  static int pos = 0;
  static int shift_key = 0;

  static public void printString(int x, int y, String str, int color)
  {
    for (int i = 0; i < str.length(); i++)
    {
      int temp = str.charAt(i);

      if(temp == 124)
        temp = 66;
      else if(temp >= 64)
        temp -= 64;

      VIC.text_plot(x + i, y, temp, color);
    }
  }

  static public void printChar(int x, int y, int c, int color)
  {
    if(c == 124)
      c = 66;
    else if(c >= 64)
      c -= 64;

    VIC.text_plot(x, y, c, color);
  }

  static public void printHex(int x, int y, int num, int color)
  {
    printChar(x + 0, y, hex.charAt((num >> 12) & 0xf), color);
    printChar(x + 1, y, hex.charAt((num >> 8) & 0xf), color);
    printChar(x + 2, y, hex.charAt((num >> 4) & 0xf), color);
    printChar(x + 3, y, hex.charAt((num >> 0) & 0xf), color);
  }

  static public void printNum(int x, int y, int num, int color)
  {
    int dec = 0;
    int base = 1;
    int rem = 0;
    int digits = 0;
    int i = 1;

    if(num == 0)
    {
      VIC.text_plot(x, y, 48, color);
      return;
    }

    if(num < 0)
    {
      num = -num;
      VIC.text_plot(x, y, 45, color);
      i--;
    }

    int temp = num;

    while(temp > 0)
    {
      temp /= 10;
      digits++;
    }

    while(num > 0)
    {
      rem = num % 10;
      dec += rem * base;
      num /= 10;
      base *= 2; 
      VIC.text_plot(x + digits - i, y, (byte)(rem + 48), (byte)color);
      i++;
    }
  }

  public static void wait(int delay)
  {
    for(int i = 0; i < delay; i++)
    {
      // wait
    }
  }

  public static void drawBorder(int x, int y, int w, int h, int color)
  {
    for(int i = 1; i < w - 1; i++)
    {
      VIC.text_plot(x + i, y, 0x43, color);
      VIC.text_plot(x + i, y + h - 1, 0x43, color);
    }

    for(int i = 1; i < h - 1; i++)
    {
      VIC.text_plot(x, y + i, 0x42, color);
      VIC.text_plot(x + w - 1, y + i, 0x42, color);
    }

    VIC.text_plot(x, y, 0x70, color);
    VIC.text_plot(x + w - 1, y, 0x6e, color);
    VIC.text_plot(x, y + h - 1, 0x6d, color);
    VIC.text_plot(x + w - 1, y + h - 1, 0x7d, color);
  }

  public static void barGraph(int x, int y, int size, String str, int color)
  {
    int segments[] = { 0x64, 0x6f, 0x79, 0x62, 0xf8, 0xf7, 0xe3, 0xa0 };

    int b1 = 0x20;
    int b2 = 0x20;

    if(size >= 8)
    {
      b1 = 0xa0;
      b2 = segments[(size - 8) & 7];
    }
    else
    {
      b1 = segments[size];
    }

    printString(x, y, str, color);
    VIC.text_plot(x + 1, y + 2, b2, color);
    VIC.text_plot(x + 1, y + 3, b1, color);
    drawBorder(x, y + 1, 3, 4, 11);
    printChar(x + 1, y + 5, hex.charAt(size), 12);
  }

  public static void updateSong(int v)
  {
    int c;

    if(v == 0)
    {
      for(int i = 0; i < 120; i++)
      {
        c = note_keys.charAt(song[i]);

        if(c >= 64)
          c -= 64;

        Memory.write8(0xc000 + 40 * 16 + i, (byte)c);
      }
    }
    else if(v == 1)
    {
      for(int i = 0; i < 120; i++)
      {
        c = note_keys.charAt(song[256 + i]);

        if(c >= 64)
          c -= 64;

        Memory.write8(0xc000 + 40 * 19 + i, (byte)c);
      }
    }
    else if(v == 2)
    {
      for(int i = 0; i < 120; i++)
      {
        c = note_keys.charAt(song[512 + i]);

        if(c >= 64)
          c -= 64;

        Memory.write8(0xc000 + 40 * 22 + i, (byte)c);
      }
    }
  }

  public static void playSong()
  {
    int f = 0;
    int speed = 500;

    updateSong(0);
    updateSong(1);
    updateSong(2);

    for(int z = 0; z < 120; z++)
    {
      int k = Keyboard.currentKeyPressed();

      k &= 255;

      if(k == 0)
        break;

      pos = z;

      for(int v = 0; v < 3; v++)
      {
        Memory.write8(0xc000 + 40 * 16 + pos,
                      (byte)(Memory.read8(0xc000 + 40 * 16 + pos) + 128));
        Memory.write8(0xc000 + 40 * 19 + pos,
                      (byte)(Memory.read8(0xc000 + 40 * 19 + pos) + 128));
        Memory.write8(0xc000 + 40 * 22 + pos,
                      (byte)(Memory.read8(0xc000 + 40 * 22 + pos) + 128));

        int i = song[(v << 8) + z];

        if(i == 32)
        {
          if(v == 0)
            SID.voice1_waveform(waveforms[wave[0]]);
          if(v == 1)
            SID.voice2_waveform(waveforms[wave[1]]);
          if(v == 2)
            SID.voice3_waveform(waveforms[wave[2]]);

          wait(speed);
          continue;
        }

        f = freq[i] << octave[v];

        if(v == 0)
        {
          SID.voice1_adsr(adsr[0]);
          SID.voice1_frequency(f);
          SID.voice1_pulse_width(pulse[0]);
          SID.voice1_waveform(waveforms[wave[0]] + 1);
        }
        else if(v == 1)
        {
          SID.voice2_adsr(adsr[1]);
          SID.voice2_frequency(f);
          SID.voice2_pulse_width(pulse[1]);
          SID.voice2_waveform(waveforms[wave[1]] + 1);
        }
        else if(v == 2)
        {
          SID.voice3_adsr(adsr[2]);
          SID.voice3_frequency(f);
          SID.voice3_pulse_width(pulse[2]);
          SID.voice3_waveform(waveforms[wave[2]] + 1);
        }

        wait(speed);
      }
    }

    SID.voice1_waveform(waveforms[wave[0]]);
    SID.voice2_waveform(waveforms[wave[1]]);
    SID.voice3_waveform(waveforms[wave[2]]);

    updateSong(0);
    updateSong(1);
    updateSong(2);
    drawCursor(1);    
  }

  public static void drawCursor(int reverse)
  {
    int r = -128;

    if(reverse == 1)
      r = 128;

    if(voice == 0)
    {
      Memory.write8(0xc000 + 40 * 16 + pos,
                    (byte)(Memory.read8(0xc000 + 40 * 16 + pos) + r));
    }
    else if(voice == 1)
    {
      Memory.write8(0xc000 + 40 * 19 + pos,
                    (byte)(Memory.read8(0xc000 + 40 * 19 + pos) + r));

    }
    else if(voice == 2)
    {
      Memory.write8(0xc000 + 40 * 22 + pos,
                    (byte)(Memory.read8(0xc000 + 40 * 22 + pos) + r));
    }
  }

  public static void piano()
  {
    int keyboard[] =
    {
      0xa0, 0x20, 0xa0, 0x20, 0xa0, 0xc2, 0xa0, 0x20, 0xa0, 0x20, 0xa0, 0x20, 0xa0, 0xc2,
      0xa0, 0xc2, 0xa0, 0xc2, 0xa0, 0xc2, 0xa0, 0xc2, 0xa0, 0xc2, 0xa0, 0xc2, 0xa0, 0xc2,
      0xa0, 0xc2, 0xa0, 0xc2, 0xa0, 0xc2, 0xa0, 0xc2, 0xa0, 0xc2, 0xa0, 0xc2, 0xa0, 0xc2
    };

    int x = 1;
    int y = 1;

    printString(x, y,     " 2 3   5 6 7   9 0", 12);
    printString(x, y + 5, "Q W E R T Y U I O P @ ", 12);

    for(int i = 0; i < 14; i++)
    {
      VIC.text_plot(x + i, y + 1, keyboard[i], 15);
      VIC.text_plot(x + i, y + 2, keyboard[i], 15);
      VIC.text_plot(x + i, y + 3, keyboard[i + 14], 15);
      VIC.text_plot(x + i, y + 4, keyboard[i + 14], 15);
    }

    for(int i = 0; i < 7; i++)
    {
      VIC.text_plot(x + i + 14, y + 1, keyboard[i + 0], 15);
      VIC.text_plot(x + i + 14, y + 2, keyboard[i + 0], 15);
      VIC.text_plot(x + i + 14, y + 3, keyboard[i + 14], 15);
      VIC.text_plot(x + i + 14, y + 4, keyboard[i + 14], 15);
    }

    drawBorder(x - 1, y - 1, 23, 8, 11);

    x = 0;
    y = 8;
    drawBorder(x, y + 1, 11, 6, 10);
    printString(x + 1, y + 2, "OCTAVE:", 12);
    printChar(x + 9, y + 2, octave[0] + 0x30, 15);
    printString(x + 1, y + 3, "WAVE:", 12);
    printChar(x + 9, y + 3, wave[0] + 0x30, 15);
    printString(x + 1, y + 4, "PW:", 12);
    printHex(x + 6, y + 4, pulse[0], 15);
    printString(x + 1, y + 5, "ADSR:", 12);
    printHex(x + 6, y + 5, (adsr[0] >>> 8) | ((adsr[0] & 0xff) << 8), 15);

    x = 11;
    y = 8;
    drawBorder(x, y + 1, 11, 6, 11);
    printString(x + 1, y + 2, "OCTAVE:", 12);
    printChar(x + 9, y + 2, octave[1] + 0x30, 15);
    printString(x + 1, y + 3, "WAVE:", 12);
    printChar(x + 9, y + 3, wave[1] + 0x30, 15);
    printString(x + 1, y + 4, "PW:", 12);
    printHex(x + 6, y + 4, pulse[1], 15);
    printString(x + 1, y + 5, "ADSR:", 12);
    printHex(x + 6, y + 5, (adsr[1] >>> 8) | ((adsr[1] & 0xff) << 8), 15);

    x = 22;
    y = 8;
    drawBorder(x, y + 1, 11, 6, 11);
    printString(x + 1, y + 2, "OCTAVE:", 12);
    printChar(x + 9, y + 2, octave[2] + 0x30, 15);
    printString(x + 1, y + 3, "WAVE:", 12);
    printChar(x + 9, y + 3, wave[2] + 0x30, 15);
    printString(x + 1, y + 4, "PW:", 12);
    printHex(x + 6, y + 4, pulse[2], 15);
    printString(x + 1, y + 5, "ADSR:", 12);
    printHex(x + 6, y + 5, (adsr[2] >>> 8) | ((adsr[2] & 0xff) << 8), 15);

    SID.clear();
    SID.volume(15);
    SID.filter_cutoff(0);
    SID.filter_resonance(0);

    int last_key = 0;

    for(int i = 0; i < 768; i++)
      song[i] = 0x20;

    drawCursor(0);

    while(true)
    {
      int f = 0;
      int note_playing = 0;
      int k = Keyboard.currentKeyPressed();
      int shift_key = k >> 8;

      k &= 255;

      if(k == last_key)
        continue;

      if(k == 4)
      {
        if(shift_key == 1)
        {
          octave[voice]--;

          if(octave[voice] < 1)
            octave[voice] = 1;
        }
        else
        {
          octave[voice]++;

          if(octave[voice] > 6)
            octave[voice] = 6;
        }

        last_key = k;
      }
      else if(k == 55)
      {
        playSong();
        last_key = k;
      }
      else if(k == 26)
      {
        drawCursor(1);

        if(shift_key == 1)
          pos += 4;
        else
          pos++;

        if(pos > 119)
          pos = 119;

        drawCursor(0);
        wait(1000);
      }
      else if(k == 2)
      {
        drawCursor(1);

        if(shift_key == 1)
          pos -= 4;
        else
          pos--;

        if(pos < 0)
          pos = 0;

        drawCursor(0);
        wait(1000);
      }
      else if(k == 31)
      {
        voice = 0;
        drawBorder(0, 9, 11, 6, 10);
        drawBorder(11, 9, 11, 6, 11);
        drawBorder(22, 9, 11, 6, 11);
        updateSong(0);
        updateSong(1);
        updateSong(2);
        drawCursor(0);
        last_key = k;
      }
      else if(k == 23)
      {
        voice = 1;
        drawBorder(0, 9, 11, 6, 11);
        drawBorder(11, 9, 11, 6, 7);
        drawBorder(22, 9, 11, 6, 11);
        updateSong(0);
        updateSong(1);
        updateSong(2);
        drawCursor(0);
        last_key = k;
      }
      else if(k == 15)
      {
        voice = 2;
        drawBorder(0, 9, 11, 6, 11);
        drawBorder(11, 9, 11, 6, 11);
        drawBorder(22, 9, 11, 6, 14);
        updateSong(0);
        updateSong(1);
        updateSong(2);
        drawCursor(0);
        last_key = k;
      }
      else if(k >= 0 && k <= 63)
      {
        int i = piano_keys[k];

        if(i != -1)
        {
          if(i != 32)
          {
            f = freq[i] << octave[voice];

            SID.voice1_adsr(adsr[voice]);
            SID.voice1_frequency(f);
            SID.voice1_pulse_width(pulse[voice]);
            SID.voice1_waveform(waveforms[wave[voice]] + 1);
          }

          song[(voice << 8) + pos] = i;

          pos++;

          if(pos > 119)
            pos = 119;

          updateSong(voice);
          drawCursor(0);
          last_key = k;
        }
      }
      else if(k == 255)
      {
        SID.voice1_waveform(waveforms[wave[voice]]);
        last_key = 0;
      }
    }
  }

  public static void main()
  {
    VIC.make_text_table();
    VIC.make_color_table();
    VIC.copy_uppercase();

    VIC.background(0);
    VIC.border(11);
    VIC.text_clear(32);

    for(int i = 0; i < 120; i++)
    {
      Memory.write8(0xd800 + 40 * 16 + i, (byte)10);
      Memory.write8(0xd800 + 40 * 19 + i, (byte)7);
      Memory.write8(0xd800 + 40 * 22 + i, (byte)14);
    }

    piano();
  }
}

