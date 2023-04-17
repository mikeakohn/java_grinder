/*
 *  C64 sound program for java_grinder, mainly for playing around with the SID.
 *
 *  left/right: move cursor
 *  < or >: move cursor faster
 *  up/down: change voice
 *  control: voice parameter menu
 *  return: play song (again or run/stop to cancel)
 *
 * There is currently a 120-note limit per voice and no way to save except
 * VICE's snapshot feature.
 */

import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.Grinder;
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.Math;
import net.mikekohn.java_grinder.Keyboard;
import net.mikekohn.java_grinder.c64.*;

public class Sound
{
  // piano keys: q2w3er5t6y7ui9o0p@
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

  static public int freq[] =
  {
    268, 284, 301, 318, 337, 358, 379, 401, 425,
    451, 477, 506, 536, 568, 602, 637, 675, 716
  };

  // PETSCII bar graph segments
  public static int segments[] =
  {
    0x64, 0x6f, 0x79, 0x62, 0xf8, 0xf7, 0xe3, 0xa0
  };

  static public String note_keys = "Q2W3ER5T6Y7UI9O0P@                      ";
  static public String hex = "0123456789ABCDEF";
  static int voice = 0;
  static int octave[] = { 2, 3, 4 };
  static int wave[] = { 2, 1, 0 };
  static int waveforms[] = { 16, 32, 64, 128 };
  static int pulse[] = { 0x800, 0x800, 0x800 };
  static int adsr[] = { 0xa444, 0xa444, 0xa444 };
  static int[] song = new int[256 * 3];
  static int pos[] = { 0, 0, 0 };
  static int len[] = { 0, 0, 0 };
  static int last_item = 0;
  static int tempo = 1500;

  static public void printString(int x, int y, String str, int color)
  {
    for (int i = 0; i < str.length(); i++)
    {
      int temp = str.charAt(i);

      if(temp == 124)
        temp = 66;
      else if(temp == 95)
        temp = 100;
      else if(temp >= 64)
        temp -= 64;

      VIC.textPlot(x + i, y, temp, color);
    }
  }

  static public void printChar(int x, int y, int c, int color)
  {
    if(c == 124)
      c = 66;
    else if(c >= 64)
      c -= 64;

    VIC.textPlot(x, y, c, color);
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
      VIC.textPlot(x, y, 48, color);
      return;
    }

    if(num < 0)
    {
      num = -num;
      VIC.textPlot(x, y, 45, color);
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
      VIC.textPlot(x + digits - i, y, (byte)(rem + 48), (byte)color);
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
      VIC.textPlot(x + i, y, 0x43, color);
      VIC.textPlot(x + i, y + h - 1, 0x43, color);
    }

    for(int i = 1; i < h - 1; i++)
    {
      VIC.textPlot(x, y + i, 0x42, color);
      VIC.textPlot(x + w - 1, y + i, 0x42, color);
    }

    VIC.textPlot(x, y, 0x70, color);
    VIC.textPlot(x + w - 1, y, 0x6e, color);
    VIC.textPlot(x, y + h - 1, 0x6d, color);
    VIC.textPlot(x + w - 1, y + h - 1, 0x7d, color);
  }

  public static void paramGraph(int x, int y, int size, int color, int value)
  {
    int xx = x;

    for(int j = 0; j < 16 / size; j++)
    {
      if(j <= value)
      {
        for(int i = 0; i < size - 1; i++)
          VIC.textPlot(xx + i, y, 0xf7, color);

        VIC.textPlot(xx + size - 1, y, 0xd0, color);
      }
      else
      {
        for(int i = 0; i < size; i++)
          VIC.textPlot(xx + i, y, 32, color);
      }

      xx += size;
    }
  }

  public static void barGraph(int x, int y, String str, int color, int value)
  {
    int b1 = 32;
    int b2 = 32;

    if(value >= 8)
    {
      b1 = 0xa0;
      b2 = segments[(value - 8) & 7];
    }
    else
    {
      b1 = segments[value];
    }

    printString(x, y, str, color);
    VIC.textPlot(x + 1, y + 2, b2, color);
    VIC.textPlot(x + 1, y + 3, b1, color);
    drawBorder(x, y + 1, 3, 4, 11);
    printChar(x + 1, y + 5, hex.charAt(value), 12);
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

        if(i < len[v])
          Memory.write8(0xc280 + i, (byte)c);
        else
          Memory.write8(0xc280 + i, (byte)64);
      }
    }
    else if(v == 1)
    {
      for(int i = 0; i < 120; i++)
      {
        c = note_keys.charAt(song[256 + i]);

        if(c >= 64)
          c -= 64;

        if(i < len[v])
          Memory.write8(0xc2f8 + i, (byte)c);
        else
          Memory.write8(0xc2f8 + i, (byte)64);
      }
    }
    else if(v == 2)
    {
      for(int i = 0; i < 120; i++)
      {
        c = note_keys.charAt(song[512 + i]);

        if(c >= 64)
          c -= 64;

        if(i < len[v])
          Memory.write8(0xc370 + i, (byte)c);
        else
          Memory.write8(0xc370 + i, (byte)64);
      }
    }
  }

  public static void playSong()
  {
    int f = 0;
    int longest = Math.max(Math.max(len[0], len[1]), len[2]);

    updateSong(0);
    updateSong(1);
    updateSong(2);

    for(int z = 0; z < longest; z++)
    {
      int k = Keyboard.currentKeyPressed();

      k &= 255;

      if(k == 0 || k == 55)
        break;

      Memory.write8(0xc000 + 40 * 16 + z,
                    (byte)(Memory.read8(0xc000 + 40 * 16 + z) + 128));
      Memory.write8(0xc000 + 40 * 19 + z,
                    (byte)(Memory.read8(0xc000 + 40 * 19 + z) + 128));
      Memory.write8(0xc000 + 40 * 22 + z,
                    (byte)(Memory.read8(0xc000 + 40 * 22 + z) + 128));

      for(int v = 0; v < 3; v++)
      {
        int zz = z % len[v];

        int i = song[(v << 8) + zz];

        if(i == 32)
        {
          if(v == 0)
            SID.waveform1(waveforms[wave[0]]);
          if(v == 1)
            SID.waveform2(waveforms[wave[1]]);
          if(v == 2)
            SID.waveform3(waveforms[wave[2]]);

          wait(1920 - tempo);
          continue;
        }

        f = freq[i] << octave[v];

        if(v == 0)
        {
          SID.adsr1(adsr[0]);
          SID.frequency1(f);
          SID.pulseWidth1(pulse[0]);
          SID.waveform1(waveforms[wave[0]] + 1);
        }
        else if(v == 1)
        {
          SID.adsr2(adsr[1]);
          SID.frequency2(f);
          SID.pulseWidth2(pulse[1]);
          SID.waveform2(waveforms[wave[1]] + 1);
        }
        else if(v == 2)
        {
          SID.adsr3(adsr[2]);
          SID.frequency3(f);
          SID.pulseWidth3(pulse[2]);
          SID.waveform3(waveforms[wave[2]] + 1);
        }

        wait(1920 - tempo);
      }
    }

    SID.waveform1(waveforms[wave[0]]);
    SID.waveform2(waveforms[wave[1]]);
    SID.waveform3(waveforms[wave[2]]);

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
      Memory.write8(0xc000 + 40 * 16 + pos[voice],
                    (byte)(Memory.read8(0xc000 + 40 * 16 + pos[voice]) + r));
    }
    else if(voice == 1)
    {
      Memory.write8(0xc000 + 40 * 19 + pos[voice],
                    (byte)(Memory.read8(0xc000 + 40 * 19 + pos[voice]) + r));

    }
    else if(voice == 2)
    {
      Memory.write8(0xc000 + 40 * 22 + pos[voice],
                    (byte)(Memory.read8(0xc000 + 40 * 22 + pos[voice]) + r));
    }
  }

  public static void drawVoiceParams(int v, int status)
  {
    int x = 0;
    int y = 8;
    int color = 0;

    if(v == 0)
    {
      x = 0;
      color = 10;
    }
    else if(v == 1)
    {
      x = 11;
      color = 7;
    }
    else if(v == 2)
    {
      x = 22;
      color = 14;
    }

    if(status == 0)
      color = 11;

    if(v == 0)
      printString(x + 2, y, "VOICE 1", color);
    else if(v == 1)
      printString(x + 2, y, "VOICE 2", color);
    else if(v == 2)
      printString(x + 2, y, "VOICE 3", color);

    drawBorder(x, y + 1, 11, 6, color);
    printString(x + 1, y + 2, "OCTAVE:", 12);
    printChar(x + 9, y + 2, octave[v] + 0x30, 15);
    printString(x + 1, y + 3, "WAVE:", 12);
    printChar(x + 9, y + 3, wave[v] + 0x30, 15);
    printString(x + 1, y + 4, "PW:", 12);
    printHex(x + 6, y + 4, pulse[v], 15);
    printString(x + 1, y + 5, "ADSR:", 12);
    printHex(x + 6, y + 5, (adsr[v] >>> 8) | ((adsr[v] & 0xff) << 8), 15);
  }

  public static void clearRect(int x, int y, int w, int h)
  {
    for(int yy = 0; yy < h; yy++)
    {
      for(int xx = 0; xx < w; xx++)
      {
        VIC.textPlot(x + xx, y + yy, 32, 0);
      }
    }
  }

  public static void menu()
  {
    for(int i = 40 * 15; i < 40 * 25 + 39; i++)
      Memory.write8(0xc000 + i, (byte)32);

    drawBorder(0, 15, 40, 10, 15);
    printString(2, 16, "OCTAVE", 12);
    printString(2, 17, "WAVEFORM", 12);
    printString(2, 18, "PULSE WIDTH", 12);
    printString(2, 19, "ATTACK", 12);
    printString(2, 20, "DECAY", 12);
    printString(2, 21, "SUSTAIN", 12);
    printString(2, 22, "RELEASE", 12);
    printString(2, 23, "TEMPO", 12);

    int item = last_item;
    int redraw = 1;
    int temp = 0;

    while((Keyboard.currentKeyPressed() & 255) != 255)
    {
      // wait for key release
    }

    while(true)
    {
      int f = 0;
      int note_playing = 0;
      int k = Keyboard.currentKeyPressed();
      int shift_key = k >> 8;

      VIC.textPlot(1, 16 + item, '>', 4);

      // mask off shift status
      k &= 255;

      if(k == 47)
      {
        // 'cursor left/right,'
        // get shift status again (otherwise glitch)
        k = Keyboard.currentKeyPressed();
        shift_key = k >> 8;
        k &= 255;

        if(item == 0)
        {
          if(shift_key == 0)
          {
            octave[voice]++;
            if(octave[voice] > 6) { octave[voice] = 6; }
          }
          else
          {
            octave[voice]--;
            if(octave[voice] < 0) { octave[voice] = 0; }
          }

          paramGraph(16, 16, 2, 15, octave[voice]);
          wait(500);
        }
        else if(item == 1)
        {
          if(shift_key == 0)
          {
            wave[voice]++;
            if(wave[voice] > 3) { wave[voice] = 3; }
          }
          else
          {
            wave[voice]--;
            if(wave[voice] < 0) { wave[voice] = 0; }
          }

          paramGraph(16, 17, 4, 15, wave[voice]);
          wait(750);
        }
        else if(item == 2)
        {
          if(shift_key == 0)
          {
            pulse[voice] += 256;
            if(pulse[voice] > 3840) { pulse[voice] = 3840; }
          }
          else
          {
            pulse[voice] -= 256;
            if(pulse[voice] < 0) { pulse[voice] = 0; }
          }

          paramGraph(16, 18, 1, 15, pulse[voice] / 256);
        }
        else if(item == 3)
        {
          temp = (adsr[voice] >>> 4) & 15;

          if(shift_key == 0)
          {
            temp++;
            if(temp > 15) { temp = 15; }
          }
          else
          {
            temp--;
            if(temp < 0) { temp = 0; }
          }

          adsr[voice] &= 0xff0f;
          adsr[voice] |= temp << 4; 

          paramGraph(16, 19, 1, 15, (adsr[voice] >>> 4) & 0xf);
        }
        else if(item == 4)
        {
          temp = adsr[voice] & 15;

          if(shift_key == 0)
          {
            temp++;
            if(temp > 15) { temp = 15; }
          }
          else
          {
            temp--;
            if(temp < 0) { temp = 0; }
          }

          adsr[voice] &= 0xfff0;
          adsr[voice] |= temp; 

          paramGraph(16, 20, 1, 15, adsr[voice] & 0xf);
        }
        else if(item == 5)
        {
          temp = (adsr[voice] >>> 12) & 15;

          if(shift_key == 0)
          {
            temp++;
            if(temp > 15) { temp = 15; }
          }
          else
          {
            temp--;
            if(temp < 0) { temp = 0; }
          }

          adsr[voice] &= 0x0fff;
          adsr[voice] |= temp << 12; 

          paramGraph(16, 21, 1, 15, (adsr[voice] >>> 12) & 0xf);
        }
        else if(item == 6)
        {
          temp = (adsr[voice] >>> 8) & 15;

          if(shift_key == 0)
          {
            temp++;
            if(temp > 15) { temp = 15; }
          }
          else
          {
            temp--;
            if(temp < 0) { temp = 0; }
          }

          adsr[voice] &= 0xf0ff;
          adsr[voice] |= temp << 8; 

          paramGraph(16, 22, 1, 15, (adsr[voice] >>> 8) & 0xf);
        }
        else if(item == 7)
        {
          if(shift_key == 0)
          {
            tempo += 128;
            if(tempo > 1920) { tempo = 1920; }
          }
          else
          {
            tempo -= 128;
            if(tempo < 0) { tempo = 0; }
          }

          paramGraph(16, 23, 1, 15, tempo / 128);
        }

        drawVoiceParams(voice, 1);
      }
      else if(k == 7)
      {
        // 'cursor up/down,'
        // get shift status again (otherwise glitch)
        k = Keyboard.currentKeyPressed();
        shift_key = k >> 8;
        k &= 255;

        VIC.textPlot(1, 16 + item, ' ', 4);

        if(shift_key == 0)
        {
          item++;

          if(item > 7)
            item = 7;
        }
        else
        {
          item--;

          if(item < 0)
            item = 0;
        }

        VIC.textPlot(1, 16 + item, '>', 4);
        wait(1000);
      }
      else if(k == 0 || k == 40)
      {
        clearRect(0, 15, 40, 10);

        for(int i = 0; i < 120; i++)
        {
          Memory.write8(0xd800 + 40 * 16 + i, (byte)10);
          Memory.write8(0xd800 + 40 * 19 + i, (byte)7);
          Memory.write8(0xd800 + 40 * 22 + i, (byte)14);
        }

        updateSong(0);
        updateSong(1);
        updateSong(2);
        drawCursor(1);
        last_item = item;
        return;
      }

      if(redraw == 1)
      {
        for(int i = 0; i < 3; i++)
        {
          drawVoiceParams(voice, 1);
          paramGraph(16, 16, 2, 15, octave[voice]);
          paramGraph(16, 17, 4, 15, wave[voice]);
          paramGraph(16, 18, 1, 15, pulse[voice] / 256);
          paramGraph(16, 19, 1, 15, (adsr[voice] >>> 4) & 0xf);
          paramGraph(16, 20, 1, 15, adsr[voice] & 0xf);
          paramGraph(16, 21, 1, 15, (adsr[voice] >>> 12) & 0xf);
          paramGraph(16, 22, 1, 15, (adsr[voice] >>> 8) & 0xf);
          paramGraph(16, 23, 1, 15, tempo / 128);
        }

        redraw = 0;
      }
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
      VIC.textPlot(x + i, y + 1, keyboard[i], 15);
      VIC.textPlot(x + i, y + 2, keyboard[i], 15);
      VIC.textPlot(x + i, y + 3, keyboard[i + 14], 15);
      VIC.textPlot(x + i, y + 4, keyboard[i + 14], 15);
    }

    for(int i = 0; i < 7; i++)
    {
      VIC.textPlot(x + i + 14, y + 1, keyboard[i + 0], 15);
      VIC.textPlot(x + i + 14, y + 2, keyboard[i + 0], 15);
      VIC.textPlot(x + i + 14, y + 3, keyboard[i + 14], 15);
      VIC.textPlot(x + i + 14, y + 4, keyboard[i + 14], 15);
    }

    printString(23, 0, "SID_GRINDER", 4); 
    printString(23, 2, "UP/DN: VOICE", 3); 
    printString(23, 3, "LT/RT: POSITION", 3); 
    printString(23, 4, "CTRL: PARAMS", 3); 
    printString(23, 5, "RETURN: PLAY", 3); 
    printString(23, 6, "RUN/STOP: CANCEL", 3); 

    drawBorder(x - 1, y - 1, 23, 8, 11);

    drawVoiceParams(0, 1);
    drawVoiceParams(1, 0);
    drawVoiceParams(2, 0);

    //barGraph(33, 8, "VOL", 3, 12);

    SID.clear();
    SID.volume(15);
    SID.filterCutoff(0);
    SID.filterResonance(0);

    int last_key = 0;

    for(int i = 0; i < 768; i++)
      song[i] = 32;

    drawCursor(0);

    while(true)
    {
      int f = 0;
      int note_playing = 0;
      int k = Keyboard.currentKeyPressed();
      int shift_key = k >> 8;

      // mask off shift status
      k &= 255;

      if(k == last_key)
        continue;

      if(k == 40)
      {
        // 'm' (menu)
        menu();
        last_key = k;
      }
      else if(k == 55)
      {
        // return (play song)
        playSong();
        last_key = k;
      }
      else if(k == 47)
      {
        // 'cursor left/right,' (move cursor)
        // get shift status again (otherwise glitch)
        k = Keyboard.currentKeyPressed();
        shift_key = k >> 8;
        k &= 255;

        drawCursor(1);

        if(shift_key == 0)
        {
          pos[voice]++;

          if(pos[voice] > len[voice])
            pos[voice] = len[voice];
        }
        else
        {
          pos[voice]--;

          if(pos[voice] < 0)
            pos[voice] = 0;
        }

        drawCursor(0);
        wait(1000);
      }
      else if(k == 26)
      {
        // ','
        drawCursor(1);
        pos[voice] += 4;

        if(pos[voice] > len[voice])
          pos[voice] = len[voice];

        drawCursor(0);
        wait(1000);
      }
      else if(k == 2)
      {
        // '.'
        drawCursor(1);
        pos[voice] -= 4;

        if(pos[voice] < 0)
          pos[voice] = 0;

        drawCursor(0);
        wait(1000);
      }
      else if(k == 7)
      {
        // cursor up/down (cycle voice)
        // get shift status again (otherwise glitch)
        k = Keyboard.currentKeyPressed();
        shift_key = k >> 8;
        k &= 255;

        if(shift_key == 1)
        {
          voice--;

          if(voice < 0)
            voice = 2;
        }
        else
        {
          voice++;

          if(voice > 2)
            voice = 0;
        }

        drawVoiceParams(0, 0);
        drawVoiceParams(1, 0);
        drawVoiceParams(2, 0);

        drawVoiceParams(voice, 1);

        updateSong(voice);

        drawCursor(0);
        last_key = k;
      }
      else if(k == 63)
      {
        // 'ins/del' (backspace, shifted inserts)
        if(shift_key == 0 && pos[voice] > 0)
        {
          pos[voice]--;

          if(pos[voice] < 0)
            pos[voice] = 0;

          for(int i = pos[voice]; i < 119; i++)
            song[(voice << 8) + i] = song[(voice << 8) + i + 1];

          song[(voice << 8) + 119] = 32;

          len[voice]--;

          if(len[voice] < 0)
            len[voice] = 0;
        }
        else if(shift_key == 1)
        {
          for(int i = 119; i > pos[voice]; i--)
            song[(voice << 8) + i] = song[(voice << 8) + i - 1];

          song[(voice << 8) + pos[voice]] = 32;

          len[voice]++;

          if(len[voice] > 120)
            len[voice] = 120;
        }
          
        updateSong(voice);
        drawCursor(0);
      }
      else if(k >= 0 && k <= 63)
      {
        int i = piano_keys[k];

        if(i != -1)
        {
          if(i != 32)
          {
            f = freq[i] << octave[voice];

            SID.adsr1(adsr[voice]);
            SID.frequency1(f);
            SID.pulseWidth1(pulse[voice]);
            SID.waveform1(waveforms[wave[voice]] + 1);
          }

          song[(voice << 8) + pos[voice]] = i;

          pos[voice]++;

          if(pos[voice] > 119)
            pos[voice] = 119;

          if(pos[voice] > len[voice])
            len[voice] = pos[voice];

          updateSong(voice);
          drawCursor(0);
          last_key = k;
        }
      }
      else if(k == 255)
      {
        SID.waveform1(waveforms[wave[voice]]);
        last_key = 0;
      }
    }
  }

  public static void main()
  {
    // Grinder.largeJavaStack();
 
    VIC.makeTextTable();
    VIC.makeColorTable();
    VIC.copyUppercase();

    VIC.background(0);
    VIC.border(11);
    VIC.textClear(32);

    for(int i = 0; i < 120; i++)
    {
      Memory.write8(0xc000 + 40 * 16 + i, (byte)64);
      Memory.write8(0xc000 + 40 * 19 + i, (byte)64);
      Memory.write8(0xc000 + 40 * 22 + i, (byte)64);
      Memory.write8(0xd800 + 40 * 16 + i, (byte)10);
      Memory.write8(0xd800 + 40 * 19 + i, (byte)7);
      Memory.write8(0xd800 + 40 * 22 + i, (byte)14);
    }

    piano();
  }
}

