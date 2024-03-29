// C64 demo game for java_grinder.

import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.Grinder;
import net.mikekohn.java_grinder.Joystick;
import net.mikekohn.java_grinder.Keyboard;
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.Math;
import net.mikekohn.java_grinder.Timer;
import net.mikekohn.java_grinder.TimerListener;
import net.mikekohn.java_grinder.c64.*;

public class Snake implements TimerListener
{
  // addresses
  static final int char_ram = 0xc800;
  static final int sprite_ram = 0xf000;
  static final int sprite_pointer1 = 0xc000 + 1016;
  static final int sprite_pointer2 = 0xc400 + 1016;

  // score is kept by adding these strings
  static int score[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  static final int ten[] = { 0, 0, 0, 0, 0, 0, 1, 0 };
  static final int hundred[] = { 0, 0, 0, 0, 0, 1, 0, 0 };
  static final int thousand[] = { 0, 0, 0, 0, 1, 0, 0, 0 };

  // random number seed
  static int seed = 12345;

  // music related
  static int pulse1 = 0x800;
  static int pulse2 = 0x800;
  static int pulse3 = 0x800;
  static int music_pos = 0;

  // one octave of note frequencies
  static int freq[] =
  {
    0x0115, 0x0125, 0x0137, 0x0149, 0x015d, 0x0172,
    0x0188, 0x019f, 0x01b8, 0x01d2, 0x01ee, 0x020b
  };

  // these correspond to piano keys
  static String keys = "q2w3er5t6y7ui9o0p[";

  // music, spaces rest
  static String title_song1 =
    "o   o   o   o   7 7 7   9 9 9   "
  + "oooooooooooooooo77777777yyyyrree";

  static String title_song2 =
    "wry7yrwry7yrwry7wry7yrwry7yrwry7"
  + "wry7yrwry7yrwry7wry7yrwry7yrwry7";

  static String title_song3 =
    "wwrryy77wwrrrreewwrryy77wwrrrree"
  + "wwrryy77wwrrrreewwrryy77wwrrrree";

  static String end_song1 =
    "qqqqrrrrqqqqrrrr";

  static String end_song2 =
    "qq33rr55tt55rr33";

  static String next_level_song1 =
    "rrrrttttyyyyiiii[[[[";

  static String next_level_song2 =
    "r   t   y   i i9o0p[";

  // sound/animation related
  static int spider_move_timer = 0;
  static int spider_exp_colors[] = { 1, 15, 12, 11 };
  static int spider_exp_timer = 0;
  static int spider_timer = 1;
  static int spider_type = 1;

  static int ship_exp_colors[] = { 1, 15, 12, 11 };
  static int ship_exp_timer = 0;

  static int snake_exp_timer = 0;

  // sprite definitions
  static final byte sprite_ship[] =
  {
    (byte)0x18, (byte)0x00, (byte)0x00, (byte)0x18, (byte)0x00, (byte)0x00, (byte)0x18, (byte)0x00,
    (byte)0x00, (byte)0xbd, (byte)0x00, (byte)0x00, (byte)0xff, (byte)0x00, (byte)0x00, (byte)0xff,
    (byte)0x00, (byte)0x00, (byte)0xff, (byte)0x00, (byte)0x00, (byte)0xe7, (byte)0x00, (byte)0x00,
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x84
  };

  static final byte sprite_shot[] =
  {
    (byte)0x18, (byte)0x00, (byte)0x00, (byte)0x18, (byte)0x00, (byte)0x00, (byte)0x18, (byte)0x00,
    (byte)0x00, (byte)0x18, (byte)0x00, (byte)0x00, (byte)0x18, (byte)0x00, (byte)0x00, (byte)0x18,
    (byte)0x00, (byte)0x00, (byte)0x18, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x84
  };

  static final byte sprite_spider1[] =
  {
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x02, (byte)0x00,
    (byte)0x00, (byte)0x02, (byte)0x00, (byte)0x20, (byte)0x08, (byte)0x28, (byte)0x20, (byte)0x08,
    (byte)0x28, (byte)0x20, (byte)0x08, (byte)0xaa, (byte)0x20, (byte)0x02, (byte)0xaa, (byte)0x80,
    (byte)0x00, (byte)0xaa, (byte)0x02, (byte)0x00, (byte)0xaa, (byte)0x08, (byte)0x0a, (byte)0x69,
    (byte)0xa0, (byte)0x20, (byte)0x69, (byte)0x00, (byte)0x80, (byte)0xaa, (byte)0x00, (byte)0x02,
    (byte)0xaa, (byte)0x80, (byte)0x02, (byte)0x28, (byte)0x80, (byte)0x02, (byte)0x28, (byte)0x80,
    (byte)0x08, (byte)0x00, (byte)0x20, (byte)0x08, (byte)0x00, (byte)0x20, (byte)0x08, (byte)0x00,
    (byte)0x20, (byte)0x08, (byte)0x00, (byte)0x80, (byte)0x00, (byte)0x00, (byte)0x80, (byte)0x84,
  };

  static final byte sprite_spider2[] =
  {
    (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x80, (byte)0x00, (byte)0x00,
    (byte)0x80, (byte)0x08, (byte)0x00, (byte)0x80, (byte)0x08, (byte)0x28, (byte)0x20, (byte)0x08,
    (byte)0x28, (byte)0x20, (byte)0x08, (byte)0xaa, (byte)0x20, (byte)0x02, (byte)0xaa, (byte)0x80,
    (byte)0x80, (byte)0xaa, (byte)0x00, (byte)0x20, (byte)0xaa, (byte)0x00, (byte)0x0a, (byte)0x69,
    (byte)0xa0, (byte)0x00, (byte)0x69, (byte)0x08, (byte)0x00, (byte)0xaa, (byte)0x02, (byte)0x02,
    (byte)0xaa, (byte)0x80, (byte)0x02, (byte)0x28, (byte)0x80, (byte)0x02, (byte)0x28, (byte)0x80,
    (byte)0x08, (byte)0x00, (byte)0x20, (byte)0x08, (byte)0x00, (byte)0x20, (byte)0x08, (byte)0x00,
    (byte)0x20, (byte)0x02, (byte)0x00, (byte)0x20, (byte)0x02, (byte)0x00, (byte)0x00, (byte)0x84,
  };

  // character definitions
  static final int char_block[] =
  {
    0b01111110,
    0b11111111,
    0b11111110,
    0b11111111,
    0b11111110,
    0b11111111,
    0b11111110,
    0b01010100,
  };

  static final int char_mushroom1[] =
  {
    0b00111000,
    0b01111100,
    0b11111110,
    0b11111110,
    0b00111000,
    0b00111000,
    0b01111100,
    0b00000000
  };

  static final int char_mushroom2[] =
  {
    0b00111000,
    0b01111100,
    0b11111110,
    0b11101110,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
  };

  static final int char_mushroom3[] =
  {
    0b00111000,
    0b01111100,
    0b11000110,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
  };

  static final int char_segment1[] =
  {
    0b11001100,
    0b11111101,
    0b10111111,
    0b10111111,
    0b10111111,
    0b10111111,
    0b11111101,
    0b11001100
  };

  static final int char_segment2[] =
  {
    0b00110011,
    0b01111111,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b01111111,
    0b00110011
  };

  static final int char_segment3[] =
  {
    0b00110011,
    0b11111101,
    0b10111111,
    0b10111111,
    0b10111111,
    0b10111111,
    0b11111101,
    0b00110011
  };

  static final int char_segment4[] =
  {
    0b11001100,
    0b01111111,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b01111111,
    0b11001100
  };

  public static void wait(int delay)
  {
    for(int i = 0; i < delay; i++)
    {
      // wait
    }
  }

  public static void addMushrooms()
  {
    int x, y;

    for(x = 0; x < 40; x++)
      VIC.textPlot(x, 0, 32, 0);

    VIC.textScrollDown();

/*
    for(y = 24; y > 0; y--)
    {
      for(x = 0; x < 40; x++)
        VIC.textPlot(x, y, VIC.textRead(x, y - 1), 5);
    }
*/

    for(y = 0; y < 16; y++)
    {
      int tx = rnd() % 40;
      int ty = rnd() % 17 + 1;

      VIC.textPlot(tx, ty, 35, 1);
      wait(100);

      for(x = 0; x < 12; x++)
      {
        SID.adsr1(0x8888);
        SID.frequency1(x << 9);
        SID.waveform1(33);
        VIC.textPlot(tx, ty, 35 - (x >> 2), 1);
        wait(100);
        SID.waveform1(32);
      }

      VIC.textPlot(tx, ty, 35, 5);
    }

    wait(1000);
    printScore();
  }

  public static void clearSprite(int address)
  {
    for(int i = 0; i < 64; i++)
      Memory.write8(address + i, (byte)0);
  }

  public static int rnd()
  {
    seed ^= seed << 9;
    seed ^= seed >> 5;
    seed ^= seed << 3;

    return seed;
  }

  public static void printString(int x, int y, int color, String str)
  {
    for (int i = 0; i < str.length(); i++)
    {
      int temp = str.charAt(i);

      if(temp == 95)
        temp = 100; // fix underscore
      else if(temp >= 96)
        temp -= 96;

      VIC.textPlot(x + i, y, temp, color);
    }
  }

  public static void printScore()
  {
    for (int i = 0; i < 8; i++)
      VIC.textPlot(16 + i, 0, score[i] + 48, 3);
  }

  public static void resetScore()
  {
    for(int i = 0; i < 8; i++)
      score[i] = 0;
   }

  public static void increaseScore(int[] amount)
  {
    int carry = 0;

    for(int i = 7; i >= 0; i--)
    {
      int temp = score[i] + amount[i] + carry;

      score[i] = temp % 10;
      carry = temp / 10;
    }

    printScore();
  }

  public static int getFreq(int c)
  {
    for(int i = 0; i < keys.length(); i++)
    {
      int temp = keys.charAt(i);

      if(temp == 32)
      {
        return 0;
      }
      else if(temp == c)
      {
        if(i < 12)
          return freq[i] << 2;
        else
          return freq[i - 12] << 3;
      }
    }

    return 0;
  }

  // play sounds/animations
  public static void updateSpiderMove()
  {
    if(spider_move_timer < 0)
      return;

  if(spider_type == 1)
  {
    if(spider_move_timer == 0)
    {
      // start attack/decay
      //                srad 
      SID.adsr3(0x6333);
      SID.waveform3(33);
    }

    SID.frequency3(2000 - (spider_move_timer << 8));
    spider_move_timer++;

    if(spider_move_timer > 4)
    {
      // start decay/release
      SID.waveform3(32);

      if(spider_move_timer > 6)
        spider_move_timer = -1;
    }

  }
  else
  {
    if(spider_move_timer == 0)
    {
      // start attack/decay
      //                srad 
      SID.adsr3(0x8555);
      SID.waveform3(65);
    }

    SID.frequency3(4000 - (spider_move_timer << 8));
    SID.pulseWidth3(0x800 + (spider_move_timer << 4));
    spider_move_timer++;

    if(spider_move_timer > 2)
    {
      // start decay/release
      SID.waveform3(64);
      SID.pulseWidth3(0x800);

      if(spider_move_timer > 4)
        spider_move_timer = -1;
    }
  }

  }

  public static void updateSpiderExp()
  {
    if(spider_exp_timer < 0)
      return;

    if(spider_exp_timer == 0)
    {
      spider_move_timer = -1;
      SID.waveform3(32);

      // start attack/decay
      //                srad 
      SID.adsr3(0xf900);
      SID.frequency3(1000);
      SID.waveform3(129);
    }

    VIC.sprite4color((byte)(spider_exp_colors[(spider_exp_timer >> 2) & 3]));
    spider_exp_timer++;

    if(spider_exp_timer > 13)
    {
      // start decay/release
      SID.waveform3(128);
      VIC.sprite4pos(0, 0);

      spider_timer = rnd() % 400 + 1;
      spider_type = rnd() % 2 + 1;

      if(spider_type == 1)
        VIC.sprite4color(4);
      else
        VIC.sprite4color(8);

      spider_exp_timer = -1;
    }
  }

  public static void updateShipExp()
  {
    if(ship_exp_timer < 0)
      return;

    if(ship_exp_timer == 0)
    {
      // start attack/decay
      //                srad 
      SID.adsr3(0x8d00);
      SID.frequency3(2000);
      SID.waveform3(129);
    }

    ship_exp_timer++;
 
    if(ship_exp_timer > 13)
    {
      // start decay/release
      SID.waveform3(128);
      ship_exp_timer = -1;
      VIC.sprite0color(0);
    }
  }

  public static void updateSnakeExp()
  {
    if(snake_exp_timer < 0)
      return;

    if(snake_exp_timer == 0)
    {
      // start attack/decay
      //                srad 
      SID.adsr3(0xe400);
      SID.frequency3(2000);
      SID.waveform3(33);
    }

    snake_exp_timer++;
 
    if(snake_exp_timer > 1)
    {
      // start decay/release
      SID.waveform3(32);
      snake_exp_timer = -1;
    }
  }

  public static void playTitleMusic()
  {
    int c;

    c = getFreq(title_song1.charAt(music_pos));

    if(c != 0)
    {
      SID.adsr1(0xa91a);
      SID.frequency1(getFreq(title_song1.charAt(music_pos)));
      SID.pulseWidth1(1536 + pulse1);
      SID.waveform1(33);
    }
    else
    {
      SID.waveform1(32);
    }

    c = getFreq(title_song2.charAt(music_pos));

    if(c != 0)
    {
      SID.adsr2(0xa91a);
      SID.frequency2(getFreq(title_song2.charAt(music_pos)) << 1);
      SID.pulseWidth2(1536 + pulse2);
      SID.waveform2(65);
    }
    else
    {
      SID.waveform2(64);
    }

    c = getFreq(title_song3.charAt(music_pos));

    if(c != 0)
    {
      SID.adsr3(0xa91a);
      SID.frequency3(getFreq(title_song3.charAt(music_pos)) << 2);
      SID.pulseWidth3(1536 + pulse3);
      SID.waveform3(65);
    }
    else
    {
      SID.waveform3(64);
    }

    music_pos++;

    if(music_pos >= title_song1.length())
      music_pos = 0;

    pulse1 += 256;
    pulse1 &= 511;
    pulse2 += 256;
    pulse2 &= 511;
    pulse3 += 256;
    pulse2 &= 511;
  }

  public static void playEndMusic()
  {
    int c;

    c = getFreq(end_song1.charAt(music_pos));

    if(c > 0)
    {
      SID.adsr1(0xa914);
      SID.frequency1(getFreq(end_song1.charAt(music_pos)));
      SID.waveform1(65);
      SID.adsr2(0xa914);
      SID.frequency2(getFreq(end_song2.charAt(music_pos)) << 1);
      SID.waveform2(65);
    }
    else
    {
      SID.waveform1(64);
      SID.waveform2(64);
    }

    music_pos++;

    if(music_pos >= end_song1.length())
      music_pos = 0;
  }

  public static void playNextLevelMusic()
  {
    int c = 0;

    SID.adsr3(0x0000);
    pulse1 = 0x800;
    pulse2 = 0x600;
    music_pos = 0;

    while(true)
    {
      c = getFreq(next_level_song1.charAt(music_pos));

      if(c > 0)
      {
        SID.adsr1(0xa949);
        SID.frequency1(getFreq(next_level_song1.charAt(music_pos)));
        SID.waveform1(65);
        SID.adsr2(0xa949);
        SID.frequency2(getFreq(next_level_song2.charAt(music_pos)) << 1);
        SID.waveform2(65);
      }
      else
      {
        SID.waveform1(64);
        SID.waveform2(64);
      }

      music_pos++;

      if(music_pos >= next_level_song1.length())
      {
        SID.waveform1(64);
        SID.waveform2(64);
        break;
      }

      wait(400);
    }

    wait(2000);
  }

  public static void title()
  {
    int colors[] = { 10, 7, 13, 14, 4, 14, 13, 7 };

    VIC.textClear(32);
    VIC.spriteEnable(16);
    VIC.spritePriority(0);
    VIC.spriteMulticolorEnable(255);
    VIC.spriteMulticolor0(1);
    VIC.spriteMulticolor1(12);
    VIC.sprite4pos(24 + 160 - 24, 50 + 124);
    VIC.spriteExpandX(16);
    VIC.spriteExpandY(16);

    printString(9, 9, 12, "Made with Java_Grinder");
    printString(13, 13, 4, "Kill the Spider");
    printString(13, 23, 14, "Joystick Port 2");

    //                srad 
    SID.filterCutoff(1500);
    SID.filterResonance(0x7c);
    SID.volume(31);

    pulse1 = 0x800;
    pulse2 = 0x840;
    pulse3 = 0x880;
    music_pos = 0;

    VIC.border(11);
    VIC.background(0);
    VIC.writeControl2(VIC.readControl2() & 239);

    int time = 0;

    while(true)
    {
      if(Joystick.isButtonDown_0(1))
        break;

      playTitleMusic();
      wait(400);

      final int c = colors[time & 7];

      printString(9, 3, c,  "=== =   =  ==  = = ===");
      printString(9, 4, c,  "=   ==  = =  = = = =  ");
      printString(9, 5, c,  "=== = = = ==== ==  ===");
      printString(9, 6, c,  "  = =  == =  = = = =  ");
      printString(9, 7, c,  "=== =   = =  = = = ===");

      VIC.sprite4color(c);

      if((time & 15) >= 8)
      {
        Memory.write8(sprite_pointer1 + 4, (byte)194);
        Memory.write8(sprite_pointer2 + 4, (byte)194);
      }
      else
      {
        Memory.write8(sprite_pointer1 + 4, (byte)195);
        Memory.write8(sprite_pointer2 + 4, (byte)195);
      }

      VIC.spriteMulticolor0(colors[(time + 1) & 7]);
      time++;
/*
for(int i = 0; i < 4; i++)
{
  VIC.textScrollLeft();
  wait(200);
}

for(int i = 0; i < 4; i++)
{
  VIC.textScrollUp();
  wait(200);
}

for(int i = 0; i < 4; i++)
{
  VIC.textScrollRight();
  wait(200);
}

for(int i = 0; i < 4; i++)
{
  VIC.textScrollDown();
  wait(200);
}
*/

//      while(time == 0)
//      {
//        // wait for next timer update
//      }
    }

    VIC.spriteEnable(0);
    VIC.sprite4pos(0, 0);
    VIC.spriteExpandX(0);
    VIC.spriteExpandY(0);
    SID.volume(0);
  }

  public static void gameOver()
  {
    int i = 0;
    int play_music = 64;

    int colors1[] = { 7, 10, 8, 2, 9, 2, 8, 10 }; 
    int colors2[] = { 13, 3, 14, 4, 6, 4, 14, 3 };

    SID.filterResonance(0);
    SID.volume(15);

    ship_exp_timer = 0;

    while(true)
    {
      updateShipExp();

      // game over
      if(ship_exp_timer == -1)
        break;

      i++;
      VIC.border(colors1[(i >> 1) & 7]);
      wait(200);
    }

    VIC.border(2);
    wait(1000);
    VIC.writeControl2(VIC.readControl2() & 239);
    VIC.spriteEnable(0);
    VIC.sprite0pos(0, 0);
    VIC.sprite1pos(0, 0);
    VIC.sprite2pos(0, 0);
    VIC.sprite3pos(0, 0);
    VIC.sprite4pos(0, 0);
    VIC.sprite5pos(0, 0);
    VIC.sprite6pos(0, 0);
    VIC.sprite7pos(0, 0);
    
    pulse1 = 0x800;
    pulse2 = 0x800;
    pulse3 = 0x800;
    music_pos = 0;

    while(true)
    {
      printString(14, 11, 0,  "           ");
      printString(14, 12, colors2[i & 7],  " Game Over ");
      printString(14, 13, 0, "           ");

      if(Joystick.isButtonDown_0(1))
      {
        while(true)
        {
          if(!Joystick.isButtonDown_0(1))
            return;
        }
      }

      if(play_music > 0)
      {
        play_music--;
        SID.volume(play_music >> 2);
        playEndMusic();
      }

      i++;
      wait(350);
    }
  }

  public static void snake()
  {
    int i = 0;
    int shipx = 180;
    int shipy = 208;
    int accelx = 0;
    int accely = 0;
    int snake_length = 8;
    int snake_count = snake_length;
    int spiderx = rnd() % 320 + 24;
    int spidery = 0;
    int shotx = 0;
    int shoty = 0;
    int shotstatus = 0;
    int shotfreq = 0;
    int temp = 0;
    int tx = 0;
    int ty = 0;
    int time = 0;

    byte snakex[] = new byte[16];
    byte snakey[] = new byte[16];
    byte snakedir[] = new byte[16];
    byte snakestatus[] = new byte[16];

    // reset animations
    spider_type = 1;
    spider_move_timer = -1;
    spider_exp_timer = -1;
    ship_exp_timer = -1;
    snake_exp_timer = -1;

    // setup graphics
    VIC.spriteEnable(31);
    VIC.spritePriority(0);
    VIC.spriteMulticolorEnable(48);
    VIC.spriteMulticolor0(1);
    VIC.spriteMulticolor1(12);
    VIC.sprite0color(1);
    VIC.sprite1color(7);
    VIC.sprite4color(4);
    VIC.sprite5color(3);
    VIC.border(11);
    VIC.background(0);
    VIC.multicolor1((byte)1);
    VIC.multicolor2((byte)7);
    VIC.writeControl2(VIC.readControl2() | 16);

    // setup sound
    pulse1 = 0x800;
    pulse2 = 0x800;
    pulse3 = 0x800;
    SID.adsr1(0xf411);
    SID.adsr2(0xf411);
    SID.adsr3(0xf411);
    SID.waveform1(128);
    SID.waveform2(128);
    SID.waveform3(128);
    SID.filterCutoff(0);
    SID.filterResonance(0);
    SID.volume(15);

    // init mushrooms
    VIC.textClear(32);

    for(i = 0; i < 50; i++)
    {
      VIC.textPlot(rnd() % 40, rnd() % 17 + 1, 35, 5);
      wait(200);
    }

    // init snake
    for(i = 0; i < 16; i++)
    {
      snakex[i] = (byte)i;
      snakey[i] = 1;
      snakedir[i] = 1;

      if(i < snake_length)
        snakestatus[i] = 1;
      else
        snakestatus[i] = 0;
    }

    // init shot
    shotx = 0;
    shoty = 0;
    shotstatus = 0;

    // init score
    resetScore();
    printScore();

    while(true)
    {
      // move ship
      if((time % 2) == 0)
      {
        if(Joystick.isRight(1)) accelx++;
        if(Joystick.isLeft(1)) accelx--;
        if(Joystick.isDown(1)) accely++;
        if(Joystick.isUp(1)) accely--;
      }

      if((time % 3) == 0)
      {
        if(accelx > 0) accelx--;
        if(accelx < 0) accelx++;
        if(accely > 0) accely--;
        if(accely < 0) accely++;
      }

      if(accelx > 3) accelx = 3;
      if(accelx < -3) accelx = -3;
      if(accely > 3) accely = 3;
      if(accely < -3) accely = -3;

      if(shipx > spiderx) spiderx++;
      if(shipx < spiderx) spiderx--;

      // ship move/collision
      tx = shipx + accelx;
      ty = shipy + accely;

      temp = VIC.textRead((tx - 20) >> 3, (ty - 46) >> 3);

      if(temp == 32)
      {
        shipx = tx;
        shipy = ty;
      }
      else
      {
        // ship hit snake
        if(temp >= 160 && temp <= 163)
          return;

        if(accelx > 0) accelx = 1;
        if(accelx < 0) accelx = -1;
        if(accely > 0) accely = 1;
        if(accely < 0) accely = -1;
      }

      if(shipx < 24) shipx = 24;
      if(shipx > 336) shipx = 336;
      if(shipy < 194) shipy = 194;
      if(shipy > 242) shipy = 242;

      VIC.sprite0pos(shipx, shipy);

      // ship hit spider
      if(spider_exp_timer == -1)
      {
        VIC.sprite4pos(spiderx, spidery);

        if(shipx < spiderx + 23 && shipx + 7 > spiderx &&
           shipy < spidery + 20 && shipy + 7 > spidery)
        {
          return;
        }

        // move spider
        if(spider_timer > 0)
        {
          spider_timer--;

          if(spider_timer == 0)
            spider_move_timer = 0;
        }
        else
        {
          spidery += spider_type;

          if(spider_move_timer == -1)
            spider_move_timer = 0;

          if(spidery > 250)
          {
            spidery = 0;
            VIC.sprite4pos(spiderx, spidery);
            spider_move_timer = -1;
          }
        }
      }

      // shoot
      if(Joystick.isButtonDown_0(1))
      {
        if(shotstatus == 0)
        {
          shotx = shipx;
          shoty = shipy;
          shotstatus = 1;
          shotfreq = 1500;
        }
      }

      // move shot
      if(shotstatus == 1)
      {
        shoty -= 4;
        shotfreq -= 20;

        if(shoty < 42)
        {
          shotx = 0;
          shoty = 0;
          shotstatus = 0;
        }
      }

      VIC.sprite1pos(shotx, shoty);

      if(shotstatus == 1)
      {
        SID.adsr1(0x8411);
        SID.frequency1(shotfreq);
        SID.waveform1(129);
      }
      else
      {
        SID.waveform1(128);
      }

      // check shot
      if(shotstatus == 1)
      {
        tx = shotx + 4;
        ty = shoty;

        // shot hit spider
        if(spider_exp_timer == -1)
        {
          if(tx < spiderx + 23 && tx + 1 > spiderx &&
             ty < spidery + 20 && ty + 7 > spidery)
          {
            spiderx = rnd() % 320 + 24;
            spidery = 0;
            shotx = 0;
            shoty = 0;
            shotstatus = 0;
            VIC.sprite1pos(shotx, shoty);
            spider_exp_timer = 0;
            increaseScore(thousand);
            continue;
          }
        }

        tx = ((shotx - 20) >> 3);
        ty = ((shoty - 50) >> 3);
        temp = VIC.textRead(tx, ty);

        if(temp >= 33 && temp <= 35)
        {
          // shot hit mushroom
          temp--;

          if(temp < 32)
            temp = 32;

          VIC.textPlot(tx, ty, temp, 5);
          shotstatus = 0;
          shotx = 0;
          increaseScore(ten);
        }
        else if(temp != 32)
        {
          for(i = 0; i < 16; i++)
          {
            // check snake
            final int sx = snakex[i];
            final int sy = snakey[i];

            if((snakestatus[i] == 1) && (sx == tx) && (sy == ty))
            {
              // hit snake
              snake_exp_timer = 0;
              shotstatus = 0;
              shotx = 0;
              VIC.textPlot(sx, sy, 35, 5);
              snakestatus[i] = 0;
              snake_count--;
              increaseScore(hundred);

              if(snake_count <= 0)
              {
                // next level
                snake_length += 2;

                if(snake_length > 16)
                  snake_length = 16;

                snake_count = snake_length;

                // reset snake
                for(i = 0; i < 16; i++)
                {
                  snakex[i] = (byte)i;
                  snakey[i] = 1;
                  snakedir[i] = 1;

                  if(i < snake_length)
                    snakestatus[i] = 1;
                  else
                    snakestatus[i] = 0;
                }

                // clear snake
                for(i = 0; i < 16; i++)
                  if(snakestatus[i] == 1)
                    VIC.textPlot(snakex[i], snakey[i], 32, 5);

                // play music
                VIC.sprite1pos(0, 0);
                playNextLevelMusic();

                // add mushrooms
                addMushrooms();

                // clear character under ship
                VIC.textPlot((shipx - 20) >> 3, (shipy - 46) >> 3, 32, 5);
              }
            }
          }
        }
      }

      // move snake
      if((time & 3) == 3)
      {
        for(i = 0; i < 16; i++)
          if(snakestatus[i] == 1)
            VIC.textPlot(snakex[i], snakey[i], 32, 10);

        for(i = 0; i < 16; i++)
        {
          int sdir = snakedir[i];
          int sx = snakex[i] + sdir;
          int sy = snakey[i];

          if(sx < 0 || sx > 39 || VIC.textRead(sx, sy) != 32)
          {
            sdir = -sdir;
            sx += sdir;
            sy++;

            if(sy > 24)
              sy = 1;

            snakey[i] = (byte)sy;
            snakedir[i] = (byte)sdir;
          }

          int d = sdir >= 0 ? 160 : 161;

          if((time & 3) == 3)
            d += 2;

          if(snakestatus[i] == 1)
            VIC.textPlot(sx, sy, d, 10);

          snakex[i] = (byte)sx;
        }

        // animate spider
        if((time & 31) >= 16)
        {
          Memory.write8(sprite_pointer1 + 4, (byte)194);
          Memory.write8(sprite_pointer2 + 4, (byte)194);
        }
        else
        {
          Memory.write8(sprite_pointer1 + 4, (byte)195);
          Memory.write8(sprite_pointer2 + 4, (byte)195);
        }

        // update animations
        updateSpiderMove();
        updateSpiderExp();
        updateSnakeExp();
      }

      time++;
    }
  }

  public static void main()
  {
    int i, temp;

//    Grinder.largeJavaStack();

    // set up screen
    VIC.textClear(32);
    VIC.textEnable(0);
    VIC.makeTextTable();
    VIC.makeColorTable();
    VIC.copyLowercase();

    // copy sprite data
    clearSprite(sprite_ram);
    clearSprite(sprite_ram + 64);
    clearSprite(sprite_ram + 128);
    clearSprite(sprite_ram + 192);
    clearSprite(sprite_ram + 256);

    VIC.copyDataFromArray(sprite_ship, sprite_ram + 0, 63);
    VIC.copyDataFromArray(sprite_shot, sprite_ram + 64, 63);
    VIC.copyDataFromArray(sprite_spider1, sprite_ram + 128, 63);
    VIC.copyDataFromArray(sprite_spider2, sprite_ram + 192, 63);


    Memory.write8(sprite_pointer1 + 0, (byte)192);
    Memory.write8(sprite_pointer2 + 0, (byte)192);
    Memory.write8(sprite_pointer1 + 1, (byte)193);
    Memory.write8(sprite_pointer2 + 1, (byte)193);
    Memory.write8(sprite_pointer1 + 2, (byte)193);
    Memory.write8(sprite_pointer2 + 2, (byte)193);
    Memory.write8(sprite_pointer1 + 3, (byte)193);
    Memory.write8(sprite_pointer2 + 3, (byte)193);
    Memory.write8(sprite_pointer1 + 4, (byte)194);
    Memory.write8(sprite_pointer2 + 4, (byte)194);

    // copy character data
    for(i = 0; i < 8; i++)
    {
      Memory.write8(char_ram + 8 * 61 + i, (byte)char_block[i]);
      Memory.write8(char_ram + 8 * 35 + i, (byte)char_mushroom1[i]);
      Memory.write8(char_ram + 8 * 34 + i, (byte)char_mushroom2[i]);
      Memory.write8(char_ram + 8 * 33 + i, (byte)char_mushroom3[i]);
      Memory.write8(char_ram + 8 * 160 + i, (byte)char_segment1[i]);
      Memory.write8(char_ram + 8 * 161 + i, (byte)char_segment2[i]);
      Memory.write8(char_ram + 8 * 162 + i, (byte)char_segment3[i]);
      Memory.write8(char_ram + 8 * 163 + i, (byte)char_segment4[i]);
    }

    // reset SID
    SID.clear();

    while(true)
    {
      Timer.setListener(false);
      Timer.setInterval(14, 30000);
      Timer.setListener(true);
      title();
      Timer.setListener(false);
      snake();
      gameOver();
    }
  }

  public void timerInterrupt()
  {
    CPU.asm("inc 53280\n");
  }
}

