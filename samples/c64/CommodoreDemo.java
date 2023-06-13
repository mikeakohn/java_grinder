// Original C64 demo for java_grinder. This was written before there
// was text support in the API.

import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.Grinder;
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.c64.*;

public class CommodoreDemo
{
  static final int screen_ram = 0xc000;
  static final int color_ram = 0xd800;
  static final int hires_ram = 0xe000;
  static final int sprite_ram = screen_ram + 1024;
  static final int sprite_pointer = screen_ram + 1016;

  static int pixel_table[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
  static byte mandel_colors[] = { 6, 4, 14, 3, 13, 10, 7, 1, 7, 10, 13, 3 };
  static int text[] = { 10, 1, 22, 1, 100, 7, 18, 9, 14, 4, 5, 18 };
  static int text2[] = { 1, 22, 1, 32, 3, 15, 13, 16, 9, 12, 5, 18 };
  static int balloon_height[] = { 0, 0, 0, 0, 2, 2, 5, 5, 7, 7, 7, 7, 5, 5, 2, 2 };

  static int screen_row[] =
  {
    screen_ram + 40 * 0,
    screen_ram + 40 * 1,
    screen_ram + 40 * 2,
    screen_ram + 40 * 3,
    screen_ram + 40 * 4,
    screen_ram + 40 * 5,
    screen_ram + 40 * 6,
    screen_ram + 40 * 7,
    screen_ram + 40 * 8,
    screen_ram + 40 * 9,
    screen_ram + 40 * 10,
    screen_ram + 40 * 11,
    screen_ram + 40 * 12,
    screen_ram + 40 * 13,
    screen_ram + 40 * 14,
    screen_ram + 40 * 15,
    screen_ram + 40 * 16,
    screen_ram + 40 * 17,
    screen_ram + 40 * 18,
    screen_ram + 40 * 19,
    screen_ram + 40 * 20,
    screen_ram + 40 * 21,
    screen_ram + 40 * 22,
    screen_ram + 40 * 23,
    screen_ram + 40 * 24
  };

  static int color_row[] = 
  {
    color_ram + 40 * 0,
    color_ram + 40 * 1,
    color_ram + 40 * 2,
    color_ram + 40 * 3,
    color_ram + 40 * 4,
    color_ram + 40 * 5,
    color_ram + 40 * 6,
    color_ram + 40 * 7,
    color_ram + 40 * 8,
    color_ram + 40 * 9,
    color_ram + 40 * 10,
    color_ram + 40 * 11,
    color_ram + 40 * 12,
    color_ram + 40 * 13,
    color_ram + 40 * 14,
    color_ram + 40 * 15,
    color_ram + 40 * 16,
    color_ram + 40 * 17,
    color_ram + 40 * 18,
    color_ram + 40 * 19,
    color_ram + 40 * 20,
    color_ram + 40 * 21,
    color_ram + 40 * 22,
    color_ram + 40 * 23,
    color_ram + 40 * 24
  };

  static int java_sprite[] =
  {
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 3, 
    0, 0, 15, 0, 0, 30, 0, 0, 
    56, 0, 0, 112, 0, 0, 240, 0, 
    0, 224, 0, 0, 240, 0, 0, 240, 
    0, 0, 120, 0, 0, 28, 0, 0, 
    6, 0, 0, 3, 0, 0, 0, 0,

    1, 0, 0, 3, 0, 0, 6, 0, 
    0, 30, 0, 0, 60, 0, 0, 248, 
    0, 0, 240, 0, 0, 192, 224, 0, 
    135, 128, 0, 30, 0, 0, 60, 0, 
    0, 120, 0, 0, 240, 0, 0, 240, 
    0, 0, 120, 0, 0, 120, 0, 0, 
    60, 0, 0, 62, 0, 0, 30, 0, 
    0, 30, 0, 0, 28, 0, 0, 0,

    0, 248, 0, 3, 240, 0, 1, 255, 
    255, 0, 31, 255, 0, 0, 0, 0, 
    31, 224, 0, 15, 255, 0, 1, 255, 
    0, 0, 0, 0, 0, 0, 0, 7, 
    255, 0, 3, 255, 15, 128, 31, 62, 
    0, 0, 127, 128, 0, 31, 255, 255, 
    0, 255, 255, 0, 15, 255, 0, 3, 
    255, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 240, 0, 12, 60, 255, 240, 
    30, 192, 0, 30, 0, 0, 62, 0, 
    56, 124, 255, 192, 240, 248, 1, 192, 
    0, 3, 0, 0, 0, 0, 255, 224, 
    0, 255, 128, 0, 248, 0, 0, 0, 
    0, 112, 0, 255, 192, 255, 252, 8, 
    248, 7, 248, 255, 255, 192, 254, 0, 
    0, 0, 0, 0, 0, 0, 0, 0,
  };

  public static int pulse = 1024;
  public static int note[] = { 1403, 1486, 1574, 1668, 1767, 1872, 1984, 2102, 2227, 2359, 2500, 2648, 2806, 2973, 3149, 3337, 3535, 3745, 3968, 4204, 4454, 4719, 5000, 5297 };

  public static int song1pos = 0;
  public static int song2pos = 0;
  public static int song3pos = 0;

  public static int song1[] = { 0, 0, 4, 0, -1, 0, 4, 0 };

  public static int song2[] =
  {
    0, -1, 0, 3, 0, -1, 0, 7, 0, -1, 0, 5, 0, -1, 0, 0,
    0, -1, 0, 3, 0, -1, 0, 7, 0, -1, 0, 5, 0, -1, 0, 0,
    0, -1, 0, 8, 0, -1, 0, 12, 0, -1, 0, 10, 0, -1, 0, 0,
    0, -1, 0, 3, 0, -1, 0, 7, 0, -1, 0, 5, 0, -1, 0, 0
  };

  public static int song3[] =
  {
    0, 0, 0, 0, 7, 7, 7, 7, 3, 3, 7, 7, 5, 5, 3, 3,
    0, 0, 0, 0, 7, 7, 7, 7, 3, 3, 7, 7, 5, 5, 3, 3,
    5, 5, 5, 5, 12, 12, 12, 12, 8, 8, 12, 12, 10, 10, 8, 8,
    0, 0, 0, 0, 7, 7, 7, 7, 3, 3, 7, 7, 5, 5, 3, 3
  };

  public static void wait(int time)
  {
    int i;

    for(i = 0; i < time; i++);
  }

  public static void initMusic()
  {
    //          srad 
    SID.adsr1(0x0004);
    SID.adsr2(0x0009);
    SID.adsr3(0x0060);

    SID.pulseWidth1(2048);
    SID.pulseWidth2(2048);
    SID.pulseWidth3(2048);
  }

  public static void playMusic()
  {
    int i;

    i = note[song1[song1pos]];

    if(i != -1)
    {
      SID.frequency1(note[song1[song1pos]]);
      SID.waveform1(128);
      SID.waveform1(129);
    }

    i = note[song2[song2pos]];

    if(i != -1)
    {
      SID.frequency2(note[song2[song2pos]] << 1);
      SID.waveform2(64);
      SID.waveform2(65);
    }

    i = note[song3[song3pos]];

    if(i != -1)
    {
      SID.frequency3(note[song3[song3pos]] << 2);
      SID.waveform3(32);
      SID.waveform3(33);
    }

    song1pos++;

    if(song1pos >= song1.length)
      song1pos = 0;

    song2pos++;

    if(song2pos >= song2.length)
      song2pos = 0;

    song3pos++;

    if(song3pos >= song3.length)
      song3pos = 0;

    SID.pulseWidth2(pulse);
    pulse += 16;
  }

  public static void transition()
  {
    int left = 0;
    int right = 39;
    int top = 0;
    int bottom = 24;
    int i;

    while(top <= 13)
    {
      for(i = left; i <= right; i++)
      {
        Memory.write8(color_row[top] + i, (byte)0);
        Memory.write8(screen_row[top] + i, (byte)32);
      }

      for(i = top; i <= bottom; i++)
      {
        Memory.write8(color_row[i] + right, (byte)0);
        Memory.write8(screen_row[i] + right, (byte)32);
      }

      for(i = right; i >= left; i--)
      {
        Memory.write8(color_row[bottom] + i, (byte)0);
        Memory.write8(screen_row[bottom] + i, (byte)32);
      }

      for(i = bottom; i >= top; i--)
      {
        Memory.write8(color_row[i] + left, (byte)0);
        Memory.write8(screen_row[i] + left, (byte)32);
      }

      left++;

      if(left > 39)
        left = 39;

      right--;

      if(right < 0)
        right = 0;

      top++;

      if(top > 24)
        top = 24;

      bottom--;

      if(bottom < 0)
        bottom = 0;
    }
  }

  public static void mandel()
  {
    int x, y, i;

    int recen = -8;
    int imcen = 0;
    int re, im, re2, im2, rec, imc;

    VIC.background(0);
    VIC.border(0);

    VIC.textClear(160);
    VIC.colorRamClear(0);

    for(i = 0; i < text.length; i++)
    {
      Memory.write8(color_ram + 40 * 12 + 19 + i, (byte)1);
      Memory.write8(screen_ram + 40 * 12 + 19 + i, (byte)text[i]);
    }

    SID.adsr1(0xf000);
    SID.adsr2(0xf000);
    SID.adsr3(0xf000);

    int loc1 = color_ram;
    int loc2 = color_ram + 960;
    int yy = 0;

    for(y = 0; y < 13 * 16; y += 16)
    {
      imc = (y - 13 * 16) >> 4;
      imc += imcen;

      int xx = 0;
      for(x = 0; x < 40 * 16; x += 16)
      {
        if(xx >= 20 && xx <= 33 && yy >= 5 && yy <= 19)
        {
          xx++;
          continue;
        }

        rec = (x - 20 * 16) >> 4;
        rec += recen;

        re = rec;
        im = imc;

        re2 = (re * re) >> 4;
        im2 = (im * im) >> 4;

        for(i = 0; i < 12; i++)
        {
          if((re2 + im2) > 4 * 16)
            break;

          im = (re * im) >> 3;
          im += imc;

          re = (re2 - im2) + rec;

          re2 = (re * re) >> 4;
          im2 = (im * im) >> 4;
        }

        if(i < 12)
        {
          byte c = mandel_colors[i];

          Memory.write8(loc1 + xx, c);
          Memory.write8(loc2 + xx, c);
        }

        if((xx & 7) == 7)
        {
          SID.frequency1(note[re2 & 15]);
          SID.waveform1(32);
          SID.waveform1(33);
          SID.frequency2(note[im2 & 15]);
          SID.waveform2(32);
          SID.waveform2(33);
        }

        xx++;
      }

      yy++;
      loc1 += 40;
      loc2 -= 40;
      SID.frequency3(note[yy & 15]);
      SID.waveform3(16);
      SID.waveform3(17);
    }

    wait(10000);
  }

  public static void sprites()
  {
    byte balloon[] = Memory.preloadByteArray("sprite_balloon.bin");
    int i, j;

    VIC.background(6);
    VIC.border(0);
    for(i = 880; i < 1000; i++)
    {
      Memory.write8(color_ram + i, (byte)5);
      Memory.write8(screen_ram + i, (byte)160);
    }

    int x0 = 0;
    int x1 = 0;
    int x2 = 0;
    int x3 = 0;
    int x4 = 0;
    int x5 = 0;
    int x6 = 0;
    int x7 = 0;

    int y0 = 55;
    int y1 = 65;
    int y2 = 75;
    int y3 = 85;
    int y4 = 95;
    int y5 = 105;
    int y6 = 115;
    int y7 = 125;

    VIC.spriteEnable(255);
    Memory.write8(sprite_pointer + 0, (byte)16);
    Memory.write8(sprite_pointer + 1, (byte)16);
    Memory.write8(sprite_pointer + 2, (byte)16);
    Memory.write8(sprite_pointer + 3, (byte)16);
    Memory.write8(sprite_pointer + 4, (byte)16);
    Memory.write8(sprite_pointer + 5, (byte)16);
    Memory.write8(sprite_pointer + 6, (byte)16);
    Memory.write8(sprite_pointer + 7, (byte)16);

    VIC.sprite0color(10);
    VIC.sprite1color(8);
    VIC.sprite2color(7);
    VIC.sprite3color(13);
    VIC.sprite4color(3);
    VIC.sprite5color(14);
    VIC.sprite6color(4);
    VIC.sprite7color(1);

    for(i = 0; i < 63; i++)
      Memory.write8(sprite_ram + i, (byte)balloon[i]);

    VIC.spriteExpandX(255);
    VIC.spriteExpandY(255);

    for(i = 0; i < 512; i++)
    {
      if((i & 7) == 7)
        playMusic();

      VIC.sprite0pos(x0, y0 + balloon_height[(x0 >> 3) & 15]);
      VIC.sprite1pos(x1, y1 + balloon_height[(x1 >> 3) & 15]);
      VIC.sprite2pos(x2, y2 + balloon_height[(x2 >> 4) & 15]);
      VIC.sprite3pos(x3, y3 + balloon_height[(x3 >> 4) & 15]);
      VIC.sprite4pos(x4, y4 + balloon_height[(x4 >> 3) & 15]);
      VIC.sprite5pos(x5, y5 + balloon_height[(x5 >> 3) & 15]);
      VIC.sprite6pos(x6, y6 + balloon_height[(x6 >> 4) & 15]);
      VIC.sprite7pos(x7, y7 + balloon_height[(x7 >> 4) & 15]);

      x0 -= 1;
      x0 &= 0x1ff;
      x1 += 1;
      x1 &= 0x1ff;
      x2 -= 2;
      x2 &= 0x1ff;
      x3 += 2;
      x3 &= 0x1ff;
      x4 -= 3;
      x4 &= 0x1ff;
      x5 += 3;
      x5 &= 0x1ff;
      x6 -= 4;
      x6 &= 0x1ff;
      x7 += 4;
      x7 &= 0x1ff;

      if((x0 & 3) == 3)
      {
        y0++;

        if(y0 > 160)
          y0 = 160;

        y1++;

        if(y1 > 160)
          y1 = 160;

        y2++;

        if(y2 > 160)
          y2 = 160;

        y3++;

        if(y3 > 160)
          y3 = 160;

        y4++;

        if(y4 > 160)
          y4 = 160;

        y5++;

        if(y5 > 160)
          y5 = 160;

        y6++;

        if(y6 > 160)
          y6 = 160;

        y7++;

        if(y7 > 160)
          y7 = 160;
      }
    }

    VIC.spriteEnable(0);
    VIC.background(0);

    int vol = 15;

    for(i = 0; i < 19; i++)
    {
      Memory.write8(1904 + i, (byte)32);
      Memory.write8(1943 - i, (byte)32);
      Memory.write8(1944 + i, (byte)32);
      Memory.write8(1983 - i, (byte)32);
      Memory.write8(1984 + i, (byte)32);
      Memory.write8(2023 - i, (byte)32);

      vol--;

      if(vol < 0)
        vol = 0;

      SID.volume(vol);
    }

    for(i = 21; i >= 0; i--)
    {
      Memory.write8(color_row[i] + 19, (byte)5);
      Memory.write8(color_row[i] + 20, (byte)5);
      Memory.write8(screen_row[i] + 19, (byte)160);
      Memory.write8(screen_row[i] + 20, (byte)160);
    }

    for(i = 18; i >= 0; i--)
    {
      for(j = 0; j < 1000; j += 40)
      {
        Memory.write8(color_ram + j + i, (byte)5);
        Memory.write8(color_ram + 39 + j - i, (byte)5);
        Memory.write8(screen_ram + j + i, (byte)160);
        Memory.write8(screen_ram + 39 + j - i, (byte)160);
      }
    }
  }

  public static void green()
  {
    byte circlej[] = Memory.preloadByteArray("sprite_circlej.bin");
    int i, j;
    int pulse1 = 1024;
    int pulse2 = 2048;
    int pulse3 = 3072;

    wait(5000);

    //          srad 
    SID.adsr1(0xf000);
    SID.adsr2(0xf000);
    SID.adsr3(0xf000);
    SID.frequency1(1024);
    SID.frequency2(2048);
    SID.frequency3(4096);
    SID.pulseWidth1(1024);
    SID.pulseWidth2(1024);
    SID.pulseWidth3(1024);
    SID.waveform1(64);
    SID.waveform1(65);
    SID.waveform2(64);
    SID.waveform2(65);
    SID.waveform3(64);
    SID.waveform3(65);
    SID.volume(15);

    for(i = 0; i < 63; i++)
      Memory.write8(sprite_ram + i, (byte)circlej[i]);

    VIC.sprite0pos(32, 58);

    VIC.sprite0color(5);
    VIC.spriteEnable(1);
    wait(500);
    VIC.sprite0color(12);
    wait(500);
    VIC.sprite0color(13);
    wait(500);
    VIC.sprite0color(15);
    wait(500);
    VIC.sprite0color(7);
    wait(500);
    VIC.sprite0color(1);

    wait(5000);

    for(i = 38; i >= 7; i--)
    {
      int k = i;

      for(j = 24; j >= 0; j--)
      {
        Memory.write8(screen_row[j] + k, (byte)105);
        Memory.write8(screen_row[j] + k + 1, (byte)32);

        k++;

        if(k > 38)
          break;
      }

      pulse1 += 16;
      pulse2 += 16;
      pulse3 += 16;

      SID.pulseWidth1(pulse1);
      SID.pulseWidth2(pulse2);
      SID.pulseWidth3(pulse3);
    }

    wait(5000);

    for(i = 32; i <= 175; i++)
    {
      VIC.sprite0pos(i, i + 26);
      wait(50);
      pulse1 -= 16;
      pulse2 -= 16;
      pulse3 -= 16;
      SID.pulseWidth1(pulse1);
      SID.pulseWidth2(pulse2);
      SID.pulseWidth3(pulse3);
    }

    wait(4000);

    int vol = 15;

    for(i = 0; i < text2.length; i++)
    {
      Memory.write8(color_row[21] + i + 25, (byte)12);
      Memory.write8(screen_row[21] + i + 25, (byte)text2[i]);
      wait(400);
      Memory.write8(color_row[21] + i + 25, (byte)13);
      wait(400);
      Memory.write8(color_row[21] + i + 25, (byte)15);
      wait(400);
      Memory.write8(color_row[21] + i + 25, (byte)7);
      wait(400);
      Memory.write8(color_row[21] + i + 25, (byte)1);

      vol--;
      SID.volume(vol);
    }

    SID.volume(0);

    wait(10000);

    VIC.spriteEnable(0);

    for(i = 38; i >= 8; i--)
    {
      int k = i;
      for(j = 24; j >= 0; j--)
      {
        Memory.write8(screen_row[j] + k, (byte)233);
        Memory.write8(screen_row[j] + k + 1, (byte)160);
        Memory.write8(color_row[j] + k, (byte)4);
        Memory.write8(color_row[j] + k + 1, (byte)4);

        k++;

        if(k > 38)
          break;
      }
    }
  }

  // parody of a Java advertisement from back in the day
  public static void billion()
  {
    byte logo1[] = Memory.preloadByteArray("hires_logo_1.bin");
    byte logo2[] = Memory.preloadByteArray("hires_logo_2.bin");

    VIC.background(1);
    VIC.border(0);

    int i;

    for(i = screen_ram; i < screen_ram + 160; i++)
      Memory.write8(i, (byte)1);

    for(i = screen_ram + 160; i < screen_ram + 840; i++)
      Memory.write8(i, (byte)17);

    for(i = screen_ram + 840; i < screen_ram + 960; i++)
      Memory.write8(i, (byte)18);

    for(i = screen_ram + 960; i < screen_ram + 1000; i++)
      Memory.write8(i, (byte)34);

    VIC.hiresEnable(0);
    VIC.hiresClear(0);

    for(i = 0; i < logo1.length; i++)
      Memory.write8(hires_ram + 320 + i, (byte)logo1[i]);

    for(i = 0; i < 240; i++)
    {
      Memory.write8(hires_ram + 7080 + i, (byte)logo2[i]);
      Memory.write8(hires_ram + 7080 + 320 + i, (byte)logo2[i + 240]);
    }

    VIC.spriteEnable(15);
    VIC.spriteExpandX(15);
    VIC.spriteExpandY(15);
    VIC.sprite0color(10);
    VIC.sprite1color(10);
    VIC.sprite2color(14);
    VIC.sprite3color(14);

    for(i = 0; i < java_sprite.length; i++)
      Memory.write8(sprite_ram + i, (byte)java_sprite[i]);

    Memory.write8(sprite_pointer + 0, (byte)16);
    Memory.write8(sprite_pointer + 1, (byte)17);
    Memory.write8(sprite_pointer + 2, (byte)18);
    Memory.write8(sprite_pointer + 3, (byte)19);

    VIC.sprite0pos(136, 112);
    VIC.sprite1pos(136 + 48, 112);
    VIC.sprite2pos(136, 112 + 42);
    VIC.sprite3pos(136 + 48, 112 + 42);

    wait(20000);

    VIC.background(0);
    VIC.textClear(160);
    VIC.colorRamClear(1);
    VIC.spriteEnable(0);
    VIC.textEnable(0);
  }

  // hires demo, adapted from 
  public static void yinYang()
  {
    VIC.background(5);
    VIC.border(0);

    VIC.textClear(16);
    VIC.colorRamClear(0);

    VIC.hiresEnable(0);
    VIC.hiresClear(0);

    int j, k;
    int temp1 = 0;
    int temp2 = 0;

    for(j = 1; j <= 81; j++)
    {
      for(k = 65; k <= 129; k++)
      {
        //temp1 = (13 * (k - 65)) / 21;
        temp1 = (13 * (k - 65)) >> 4;
        temp1 *= temp1;

        temp2 = (41 - j);
        temp2 *= temp2;

        if((temp1 + temp2) > 1332 )
        {
          // this makes a checkerboard background
          if(((k & 1) ^ (j & 1)) == 1)
          {
            VIC.hiresPlot(95 + (129 - k), 60 + (82 - j), 1);
            VIC.hiresPlot(95 + k, 60 + j, 1);
          }

          continue;
        }

        temp2 = (23 - j);
        temp2 *= temp2;
        temp2 += temp1;

        if(temp2 < 30 )
        {
          VIC.hiresPlot(95 + k, 60 + j, 1);
          continue;
        }

        if(temp2 < 342 )
        {
          VIC.hiresPlot(95 + (129 - k), 60 + (82 - j), 1);
          continue;
        }

        temp2 = (59 - j);
        temp2 *= temp2;
        temp2 += temp1;

        if(temp2 < 30 )
        {
          VIC.hiresPlot(95 + (129 - k), 60 + (82 - j), 1);
          continue;
        }

        if(temp2 < 342 )
        {
          VIC.hiresPlot(95 + k, 60 + j, 1);
          continue;
        }

        VIC.hiresPlot(95 + k, 60 + j, 1);
      }
    }
  }

  public static void main()
  {
    // Grinder.largeJavaStack;

    VIC.makeHiresTables();
    VIC.copyUppercase();

    VIC.textClear(16);
    VIC.background(0);
    VIC.colorRamClear(3);

    wait(10000);

    billion();
    transition();
    initMusic();
    SID.volume(15);
    mandel();
    transition();
    sprites();
    green();
    transition();
    SID.volume(0);
    yinYang();
    while(true);
  }
}

