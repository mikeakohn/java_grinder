
import net.mikekohn.java_grinder.SN76489;
import net.mikekohn.java_grinder.TI99;
import net.mikekohn.java_grinder.TMS9918A;

public class ColecoDemo
{
  static byte[] patterns =
  {
    (byte) 0b11111111,
    (byte) 0b11000011,
    (byte) 0b11000011,
    (byte) 0b11000011,
    (byte) 0b11000011,
    (byte) 0b11000011,
    (byte) 0b11000011,
    (byte) 0b11111111,

    (byte) 0b00000000,
    (byte) 0b00000000,
    (byte) 0b00000000,
    (byte) 0b00000000,
    (byte) 0b11111111,
    (byte) 0b11111111,
    (byte) 0b11111111,
    (byte) 0b11111111,
  };

/*
  static byte[] sprite_dx;
  static byte[] sprite_dy;
  static int[] sprite_x;
  static int[] sprite_y;

  static short[] tones =
  {
    // 0      1      2      3      4      5      6      7      8      9     10
    // C     C#      D     D#      E      F     F#      G     G#      A     A#
    0x60d, 0xa0c, 0xe0b, 0x40b, 0xa0a, 0x00a, 0x709, 0xf08, 0x708, 0xf07, 0x807,
    //11     12     13     14     15      16     17
    // B,     C,    C#,     D,    D#       E      F
    0x107, 0xb06, 0x506, 0xf05, 0xa05,  0x505, 0x005,
  };

  static byte[] song_1 =
  {
    2, 9, 9,  2, 2, 0,
    2, 9, 2,  2, 2, 0,
    2, 9, 14,  2, 2, 0,
    2, 9, 13,  2, 2, 0,
    2, 8, 9,  2, 2, 0,
    2, 8, 12,  2, 2, 0,
    2, 8, 8,  2, 2, 0,
    2, 8, 8,  2, 2, 0,

    2, 9, 9,  2, 2, 0,
    2, 9, 2,  2, 2, 0,
    2, 9, 14,  2, 2, 0,
    2, 9, 13,  2, 2, 0,
    2, 10, 9,  2, 2, 0,
    2, 10, 12,  2, 2, 0,
    2, 10, 8,  2, 2, 0,
    2, 10, 8,  2, 2, 0,
  };

  static byte[] song_2 =
  {
    2, 9, 6,  0, 0, 0,
    9, 0, 0,  0, 15, 15,
    16, 0, 14,  0, 15, 0,
    14, 0, 0,  0, 15, 15,

    2, 9, 6,  0, 0, 0,
    9, 0, 0,  0, 15, 15,
    16, 0, 14,  0, 15, 0,
    14, 0, 0,  0, 15, 15,

    2, 9, 6,  0, 0, 0,
    9, 0, 0,  0, 15, 15,
    16, 0, 14,  0, 15, 0,
    14, 0, 0,  0, 15, 15,

    2, 9, 6,  0, 0, 0,
    9, 0, 0,  0, 15, 15,
    16, 0, 14,  0, 15, 0,
    14, 0, 0,  0, 15, 15,

    0, 7, 4,  0, 0, 0,
    7, 0, 0,  0, 15, 15,
    14, 0, 16,  0, 15, 0,
    12, 0, 0,  0, 15, 15,

    0, 7, 4,  0, 0, 0,
    7, 0, 0,  0, 15, 15,
    14, 0, 16,  0, 15, 0,
    12, 0, 0,  0, 15, 15,

    0, 7, 4,  0, 0, 0,
    7, 0, 0,  0, 15, 15,
    14, 0, 16,  0, 15, 0,
    12, 0, 0,  0, 15, 15,

    0, 7, 4,  0, 0, 0,
    7, 0, 0,  0, 15, 15,
    14, 0, 16,  0, 15, 0,
    12, 0, 0,  0, 15, 15,
  };

  static int song_ptr;

  static byte[] sprite_j =
  {
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,  -32,  112,  112,   56,   28,   15,
     -4,   -4,  112,  112,  112,  112,  112,  112,
    112,  112,  112,  112,  112,  -32,  -64,  -128,
  };

  static byte[] sprite_a =
  {
     15,   31,   60,  120,  120,  -16,  -16,  -16,
     -1,   -1,  -16,  -16,  -16,  -16,  -16,  -16,
   -128,  -64,  -32,  -16,  -16,  120,  120,  120,
     -8,   -8,  120,  120,  120,  120,  120,  120,
  };

  static byte[] sprite_v =
  {
    -16,  -16,  -16,  -16,  -16,  -16,  -16,  -16,
    -16,  -16,  -16,  120,  120,   60,   63,   15,
    120,  120,  120,  120,  120,  120,  120,  120,
    120,  120,  120,  -16,  -16,  -32,  -32,  -128,
  };

  static public void drawMandelbrot()
  {
    final int DEC_PLACE = 4;
    int x, y;
    //int rs = (-2 << DEC_PLACE), re = (1 << DEC_PLACE);
    //int is = (-1 << DEC_PLACE), ie = (1 << DEC_PLACE);
    int rs, is;
    int zi, zr;
    int tr, ti;
    int zr2, zi2;
    //int dx = 0x0019, dy = 0x0015;  according to debugger, 2 lines below work
    //int dx = (re - rs) / 32;
    //int dy = (ie - is) / 24;
    //int rs_save = rs;
    int count;

    TMS9918A.setCursor(0, 0);

    for (y = 0; y < 24; y++)
    {
      is = (((2 << DEC_PLACE) * y) / 24) - (1 << DEC_PLACE);
      //rs = rs_save;

      for (x = 0; x < 32; x++)
      {
        rs = (((3 << DEC_PLACE) * x) / 32) - (2 << DEC_PLACE);

        zr = 0;
        zi = 0;

        for (count = 0; count < 16; count++)
        {
          zr2 = (zr * zr) >> DEC_PLACE;
          zi2 = (zi * zi) >> DEC_PLACE;

          if (zr2 + zi2 > (4 << DEC_PLACE)) { break; }

          tr = zr2 - zi2;
          ti = 2 * ((zr * zi) >> DEC_PLACE);

          zr = tr + rs;
          zi = ti + is;
        }

        //TMS9918A.printChar((char)((count >> 2) + '0'));
        //TMS9918A.printChar((char)(count << 4));
        TMS9918A.plot(x, y, (char)(count << 4));

        //rs += dx;
      }

      //is += dy;
    }
  }

  static public void playSong1()
  {
    SN76489.setSoundFreq(0, tones[song_1[song_ptr++]]);
    SN76489.setSoundFreq(1, tones[song_1[song_ptr++]]);
    SN76489.setSoundFreq(2, tones[song_1[song_ptr++]]);
    SN76489.setSoundVolume(0, song_1[song_ptr++]);
    SN76489.setSoundVolume(1, song_1[song_ptr++]);
    SN76489.setSoundVolume(2, song_1[song_ptr++]);

    if (song_ptr == song_1.length) { song_ptr = 0; }
  }

  static public void playSong2()
  {
    SN76489.setSoundFreq(0, tones[song_2[song_ptr++]]);
    SN76489.setSoundFreq(1, tones[song_2[song_ptr++]]);
    SN76489.setSoundFreq(2, tones[song_2[song_ptr++]]);
    SN76489.setSoundVolume(0, song_2[song_ptr++]);
    SN76489.setSoundVolume(1, song_2[song_ptr++]);
    SN76489.setSoundVolume(2, song_2[song_ptr++]);

    if (song_ptr == song_2.length) { song_ptr = 0; }
  }

  static public void soundOff()
  {
    SN76489.setSoundVolume(0, 15);
    SN76489.setSoundVolume(1, 15);
    SN76489.setSoundVolume(2, 15);
  }

  static public void delay()
  {
    int a;
    for (a = 0; a < 32767; a++);
  }

  static public void delayLong()
  {
    int a;
    for (a = 0; a < 2; a++) { delay(); }
  }

  static public void delayShort()
  {
    int a;
    for (a = 0; a < 10000; a++);
  }

  static public void delayShortTwo()
  {
    int a;
    for (a = 0; a < 2000; a++);
  }

  static public void playTone(int tone)
  {
    SN76489.setSoundFreq(0, tones[tone]);
    SN76489.setSoundVolume(0, 0);
    delayShort();
    SN76489.setSoundVolume(0, 15);
    delayShortTwo();
  }

  static public void scrollcolors()
  {
    int a,b;

    for (a = 1; a < 12; a++)
    {
      for (b = 0; b < 4; b++)
      {
        TMS9918A.setSpriteColor(b, a + b + 1);
      }
      delayShort();
    }
  }

  static public void hideSprites()
  {
    int a;

    for (a = 0; a < 4; a++)
    {
      TMS9918A.setSpriteColor(a, a + 1);
      TMS9918A.setSpritePos(a, 0xff, 0xd0);
    }
  }

  static public void drawBox(int x0, int y0, int x1, int y1, int color)
  {
    int x,y;

    for (x = x0; x <= x1; x++)
    {
      TMS9918A.plot(x, y0, color);
      TMS9918A.plot(x, y1, color);
    }

    for (y = y0; y <= y1; y++)
    {
      TMS9918A.plot(x0, y, color);
      TMS9918A.plot(x1, y, color);
    }
  }

  static public void drawBoxFill(int x0, int y0, int x1, int y1, int color)
  {
    int x,y;

    for (y = y0; y <= y1; y++)
    {
      for (x = x0; x <= x1; x++)
      {
        TMS9918A.plot(x, y, color);
      }
    }
  }

  static public void clearScreenSlow()
  {
    int x,y;

    for (x = 0; x < 12; x++)
    {
      drawBox(x, x, 31 - x, 23 - x, 0);
      delayShortTwo();
    }

    TMS9918A.clearScreen();
  }

  static public void showSunMessage()
  {
    TMS9918A.setCursor(10, 8);
    TMS9918A.print("IN MEMORY OF");
    TMS9918A.setCursor(8, 12);
    TMS9918A.print("SUN MICROSYSTEMS");
    TMS9918A.setCursor(11, 16);
    TMS9918A.print("1982-2010");
    delayLong();
    TMS9918A.clearScreen();
  }

  static public void spritesInit()
  {
    int a,x;

    TMS9918A.setSpriteSize(TMS9918A.SPRITE_SIZE_16X16_SMALL);
    TMS9918A.setSpriteImage(0, sprite_j);
    TMS9918A.setSpriteImage(1, sprite_a);
    TMS9918A.setSpriteImage(2, sprite_v);
    TMS9918A.setSpriteImage(3, sprite_a);
    hideSprites();

    x = 0x45;
    for (a = 0; a < 4; a++)
    {
      sprite_x[a] = x;
      sprite_y[a] = 0x40;
      sprite_dx[a] = 5;
      sprite_dy[a] = 5;
      x += 0x20;
    }
  }

  static public void spritesDisplay()
  {
    TMS9918A.setSpritePos(0, 0x45, 0x40);
    playTone(0);
    TMS9918A.setSpritePos(1, 0x65, 0x40);
    playTone(4);
    TMS9918A.setSpritePos(2, 0x85, 0x40);
    playTone(7);
    TMS9918A.setSpritePos(3, 0xa5, 0x40);
    playTone(9);

    TMS9918A.setCursor(13, 12);
    TMS9918A.print("GRINDER");

    SN76489.setSoundFreq(0, tones[0]);
    SN76489.setSoundVolume(0, 0);
    SN76489.setSoundFreq(1, tones[4]);
    SN76489.setSoundVolume(1, 0);
    SN76489.setSoundFreq(2, tones[7]);
    SN76489.setSoundVolume(2, 0);

    delay();

    soundOff();

    //scrollcolors();

    TMS9918A.clearScreen();
    hideSprites();
  }

  static public void spritesDisplayFast()
  {
    int a,x;

    x = 0x45;
    for (a = 0; a < 4; a++)
    {
      TMS9918A.setSpritePos(a, x, 0x40);
      x += 0x20;
    }
  }

  static public void spritesBounce()
  {
    int a,c;

    c = sprite_x[0] & 0xf;

    for (a = 0; a < 4; a++)
    {
      TMS9918A.setSpritePos(a, sprite_x[a], sprite_y[a]);
      TMS9918A.setSpriteColor(a, c);
      c++;

      sprite_x[a] += sprite_dx[a];
      sprite_y[a] += sprite_dy[a];

      if (sprite_x[a] <= 16) { sprite_dx[a] = 5; }
      if (sprite_y[a] <= 16) { sprite_dy[a] = 5; }
      if (sprite_x[a] >= 0xd0) { sprite_dx[a] = -5; }
      if (sprite_y[a] >= 0xa8) { sprite_dy[a] = -5; }
    }
  }

  static public void animateBox()
  {
    byte dx,dy;
    byte x,y;
    int count;

    song_ptr = 0;

    dx = 1; dy = 1;
    x = 0; y = 0;
    count = 0;

    while (true)
    {
      TMS9918A.clearScreen();
      drawBoxFill(x, y, x + 4, y + 4, (x & 0xf) << 4);

      x += dx;
      y += dy;

      if (x >= 27) { dx = -1; }
      if (y >= 19) { dy = -1; }
      if (x == 0) { dx = 1; }
      if (y == 0) { dy = 1; }

      delayShortTwo();

      if ((count & 0x1) == 0)
      {
        playSong1();
      }

      count++;
      if (count > 100) { break; }

      spritesBounce();
    }

    soundOff();
  }

  static public void animateBoxes()
  {
    byte dx,dy;
    byte x,y;
    int count;

    song_ptr = 0;

    dx = 1; dy = 1;
    x = 0; y = 0;
    count = 0;

    while (true)
    {
      drawBoxFill(x, y, x + 4, y + 4, (x & 0xf) << 4);

      x += dx;
      y += dy;

      if (x >= 27) { dx = -1; }
      if (y >= 19) { dy = -1; }
      if (x == 0) { dx = 1; }
      if (y == 0) { dy = 1; }

      if (count == 200)
      {
        TMS9918A.setSpriteSize(TMS9918A.SPRITE_SIZE_16X16_BIG);
      }

      if ((count & 0xf) == 0)
      {
        playSong2();
      }

      count++;
      if (count > 1600) { break; }

      spritesBounce();
    }

    soundOff();
  }
*/

  static public void main(String[] args)
  {
/*
    sprite_dx = new byte[4];
    sprite_dy = new byte[4];
    sprite_x = new int[4];
    sprite_y = new int[4];

    showSunMessage();
    spritesInit();
    spritesDisplay();

    TMS9918A.setColors();
    drawMandelbrot();

    delay();

    clearScreenSlow();

    spritesDisplayFast();

    animateBox();
    animateBoxes();

    clearScreenSlow();
    //spritesInit();

    TMS9918A.setSpritePos(0, 0x45, 0x40);
    TMS9918A.setSpritePos(1, 0x65, 0x40);
    TMS9918A.setSpritePos(2, 0x85, 0x40);
    TMS9918A.setSpritePos(3, 0xa5, 0x40);

    scrollcolors();
    //delay();
    //TMS9918A.setSpriteSize(TMS9918A.SPRITE_SIZE_16X16_BIG);
    //scrollcolors();
*/
    TMS9918A.initDisplay();
    TMS9918A.clearScreen();
    TMS9918A.setPattern(0, patterns);
    TMS9918A.setTextBackdropColor(0xf7);
    TMS9918A.setColor(0, (TMS9918A.COLOR_DARK_GREEN << 4) | TMS9918A.COLOR_TRANSPARENT);
    TMS9918A.setColor(1, (TMS9918A.COLOR_DARK_RED << 4) | TMS9918A.COLOR_DARK_YELLOW);
    TMS9918A.setColor(8, (TMS9918A.COLOR_MEDIUM_GREEN << 4) | TMS9918A.COLOR_MEDIUM_RED);
    TMS9918A.setCursor(4, 0);
    TMS9918A.printChar((char)0x43);
    TMS9918A.printChar((char)1);
    TMS9918A.printChar((char)2);
    TMS9918A.printChar((char)2);
    TMS9918A.printChar((char)2);
    TMS9918A.printChar((char)2);
    TMS9918A.plot(10, 10, 0x43);

    while (true);
  }
}

