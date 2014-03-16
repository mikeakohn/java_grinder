import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.C64.*;

public class CommodoreDemo
{
  static byte mandel_colors[] = { 6, 4, 14, 3, 13, 10, 7, 1, 7, 10, 13, 3 };
  static int text[] = { 10, 1, 22, 1, 100, 7, 18, 9, 14, 4, 5, 18 };
  static int balloon_height[] = { 0, 0, 0, 0, 2, 2, 5, 5, 7, 7, 7, 7, 5, 5, 2, 2 };
  static int screen_row[] =
  {
    1024 + 40 * 0,
    1024 + 40 * 1,
    1024 + 40 * 2,
    1024 + 40 * 3,
    1024 + 40 * 4,
    1024 + 40 * 5,
    1024 + 40 * 6,
    1024 + 40 * 7,
    1024 + 40 * 8,
    1024 + 40 * 9,
    1024 + 40 * 10,
    1024 + 40 * 11,
    1024 + 40 * 12,
    1024 + 40 * 13,
    1024 + 40 * 14,
    1024 + 40 * 15,
    1024 + 40 * 16,
    1024 + 40 * 17,
    1024 + 40 * 18,
    1024 + 40 * 19,
    1024 + 40 * 20,
    1024 + 40 * 21,
    1024 + 40 * 22,
    1024 + 40 * 23,
    1024 + 40 * 24
  };

  static int color_row[] = 
  {
    55296 + 40 * 0,
    55296 + 40 * 1,
    55296 + 40 * 2,
    55296 + 40 * 3,
    55296 + 40 * 4,
    55296 + 40 * 5,
    55296 + 40 * 6,
    55296 + 40 * 7,
    55296 + 40 * 8,
    55296 + 40 * 9,
    55296 + 40 * 10,
    55296 + 40 * 11,
    55296 + 40 * 12,
    55296 + 40 * 13,
    55296 + 40 * 14,
    55296 + 40 * 15,
    55296 + 40 * 16,
    55296 + 40 * 17,
    55296 + 40 * 18,
    55296 + 40 * 19,
    55296 + 40 * 20,
    55296 + 40 * 21,
    55296 + 40 * 22,
    55296 + 40 * 23,
    55296 + 40 * 24
  };

  static int sprite1[] =
  {
    0, 127, 0, 1, 255, 192, 3, 255, 224, 3, 231, 224,
    7, 217, 240, 7, 223, 240, 7, 217, 240, 3, 231, 224,
    3, 255, 224, 3, 255, 224, 2, 255, 160, 1, 127, 64,
    1, 62, 64, 0, 156, 128, 0, 156, 128, 0, 73, 0, 0, 73, 0,
    0, 62, 0, 0, 62, 0, 0, 62, 0, 0, 28, 0
  };

  static int sprite2[] =
  {
    0b00000000, 0b11111111, 0b00000000,
    0b00000111, 0b00000000, 0b11100000,
    0b00001000, 0b00000000, 0b00010000,
    0b00010000, 0b00000000, 0b00001000,
    0b00100000, 0b00000000, 0b00000100,
    0b01000001, 0b11111111, 0b11100010,
    0b01000000, 0b00001111, 0b00000010,
    0b10000000, 0b00001111, 0b00000001,
    0b10000000, 0b00001111, 0b00000001,
    0b10000000, 0b00001111, 0b00000001,
    0b10000000, 0b00001111, 0b00000001,
    0b10000000, 0b00001111, 0b00000001,
    0b10000000, 0b00001111, 0b00000001,
    0b10000011, 0b00001111, 0b00000001,
    0b01000011, 0b00001111, 0b00000010,
    0b01000001, 0b10011110, 0b00000010,
    0b00100000, 0b11111100, 0b00000100,
    0b00010000, 0b00000000, 0b00001000,
    0b00001000, 0b00000000, 0b00010000,
    0b00000111, 0b00000000, 0b11100000,
    0b00000000, 0b11111111, 0b00000000
  };

  public static void wait(int time)
  {
    int i;

    for(i = 0; i < time; i++);
  }

  public static void wipe()
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
        Memory.write8(55296 + i + 40 * top, (byte)0);
        Memory.write8(1024 + i + 40 * top, (byte)32);
      }
      for(i = top; i <= bottom; i++)
      {
        Memory.write8(55296 + right + 40 * i, (byte)0);
        Memory.write8(1024 + right + 40 * i, (byte)32);
      }
      for(i = right; i >= left; i--)
      {
        Memory.write8(55296 + i + 40 * bottom, (byte)0);
        Memory.write8(1024 + i + 40 * bottom, (byte)32);
      }
      for(i = bottom; i >= top; i--)
      {
        Memory.write8(55296 + left + 40 * i, (byte)0);
        Memory.write8(1024 + left + 40 * i, (byte)32);
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

    for(i = 55296; i < 56296; i++)
      Memory.write8(i, (byte)0);
    for(i = 1024; i < 2024; i++)
      Memory.write8(i, (byte)160);

    for(i = 0; i < text.length; i++)
    {
      Memory.write8(1024 + 40 * 12 + 19 + i, (byte)text[i]);
      Memory.write8(55296 + 40 * 12 + 19 + i, (byte)1);
    }

    int loc1 = 55296;
    int loc2 = 56256;
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

          im = (re * im) >> 4;
          im <<= 1;
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

        xx++;
      }
      yy++;
      loc1 += 40;
      loc2 -= 40;
    }

    wait(10000);
  }

  public static void sprites()
  {
    int i, j;

    VIC.background(6);
    VIC.border(0);
    for(i = 880; i < 1000; i++)
    {
      Memory.write8(1024 + i, (byte)160);
      Memory.write8(55296 + i, (byte)5);
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

    VIC.sprite_enable(255);
    Memory.write8(2040, (byte)13);
    Memory.write8(2041, (byte)13);
    Memory.write8(2042, (byte)13);
    Memory.write8(2043, (byte)13);
    Memory.write8(2044, (byte)13);
    Memory.write8(2045, (byte)13);
    Memory.write8(2046, (byte)13);
    Memory.write8(2047, (byte)13);

    VIC.sprite0color(2);
    VIC.sprite1color(8);
    VIC.sprite2color(7);
    VIC.sprite3color(13);
    VIC.sprite4color(14);
    VIC.sprite5color(3);
    VIC.sprite6color(4);
    VIC.sprite7color(1);

    for(i = 0; i < 63; i++)
      Memory.write8(832 + i, (byte)sprite1[i]);

    VIC.sprite_expandx(255);
    VIC.sprite_expandy(255);

    for(i = 0; i < 512; i++)
    {
      VIC.wait_raster(251);
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

    VIC.sprite_enable(0);
    VIC.background(0);

    for(i = 0; i < 19; i++)
    {
      VIC.wait_raster(251);
      Memory.write8(1904 + i, (byte)32);
      Memory.write8(1943 - i, (byte)32);
      Memory.write8(1944 + i, (byte)32);
      Memory.write8(1983 - i, (byte)32);
      Memory.write8(1984 + i, (byte)32);
      Memory.write8(2023 - i, (byte)32);
    }

    for(i = 21; i >= 0; i--)
    {
      VIC.wait_raster(251);
      Memory.write8(55296 + 19 + 40 * i, (byte)5);
      Memory.write8(55296 + 20 + 40 * i, (byte)5);
      Memory.write8(1024 + 19 + 40 * i, (byte)160);
      Memory.write8(1024 + 20 + 40 * i, (byte)160);
    }

    for(i = 18; i >= 0; i--)
    {
      for(j = 0; j < 1000; j += 40)
      {
        Memory.write8(55296 + j + i, (byte)5);
        Memory.write8(55335 + j - i, (byte)5);
        Memory.write8(1024 + j + i, (byte)160);
        Memory.write8(1063 + j - i, (byte)160);
      }
    }
  }

  public static void green()
  {
    int i, j;

    wait(5000);

    for(i = 0; i < 63; i++)
      Memory.write8(832 + i, (byte)sprite2[i]);

    //VIC.sprite_expandx(0);
    //VIC.sprite_expandy(0);
    VIC.sprite0pos(32, 58);

    VIC.sprite0color(5);
    VIC.sprite_enable(1);
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
    }

    wait(5000);

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

  public static void main()
  {
    mandel();
    wipe();
    sprites();
    green();

    while(true);
  }
}

