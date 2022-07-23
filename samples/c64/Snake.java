import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.Math;
import net.mikekohn.java_grinder.c64.*;

public class Snake
{
  // addresses
  static final int char_ram = 0xc800;
  static final int sprite_ram = 0xf000;
  static final int sprite_pointer = 0xc400 + 1016;

  // score is kept by adding strings, as 16-bit integers
  // don't have enough range
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

  // animation-related
  static int anim_spider_colors[] = { 1, 15, 12, 11 };
  static int anim_spider_timer = 0;
  static int anim_spider_status = 0;

  static int anim_ship_colors[] = { 1, 15, 12, 11 };
  static int anim_ship_timer = 0;
  static int anim_ship_status = 0;

  static int anim_snake_timer = 0;
  static int anim_snake_status = 0;

  // sprite definitions
  // props to spritemate: https://github.com/Esshahn/spritemate
  static final int sprite_ship[] =
  {
    0b00011000, 0b00000000, 0b00000000,
    0b00011000, 0b00000000, 0b00000000,
    0b00011000, 0b00000000, 0b00000000,
    0b10111101, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000,
    0b11100111, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
  };

  static final int sprite_shot[] =
  {
    0b00011000, 0b00000000, 0b00000000,
    0b00011000, 0b00000000, 0b00000000,
    0b00011000, 0b00000000, 0b00000000,
    0b00011000, 0b00000000, 0b00000000,
    0b00011000, 0b00000000, 0b00000000,
    0b00011000, 0b00000000, 0b00000000,
    0b00011000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000,
/*
    8, 0, 0, 8, 0, 0, 8, 0, 0, 8, 0, 0,
    8, 0, 0, 8, 0, 0, 8, 0, 0, 8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0,
*/
  };

  static final int sprite_spider1[] =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
    0x00, 0x02, 0x00, 0x20, 0x08, 0x28, 0x20, 0x08,
    0x28, 0x20, 0x08, 0xaa, 0x20, 0x02, 0xaa, 0x80,
    0x00, 0xaa, 0x02, 0x00, 0xaa, 0x08, 0x0a, 0x69,
    0xa0, 0x20, 0x69, 0x00, 0x80, 0xaa, 0x00, 0x02,
    0xaa, 0x80, 0x02, 0x28, 0x80, 0x02, 0x28, 0x80,
    0x08, 0x00, 0x20, 0x08, 0x00, 0x20, 0x08, 0x00,
    0x20, 0x08, 0x00, 0x80, 0x00, 0x00, 0x80, 0x84,
  };

  static final int sprite_spider2[] =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
    0x80, 0x08, 0x00, 0x80, 0x08, 0x28, 0x20, 0x08,
    0x28, 0x20, 0x08, 0xaa, 0x20, 0x02, 0xaa, 0x80,
    0x80, 0xaa, 0x00, 0x20, 0xaa, 0x00, 0x0a, 0x69,
    0xa0, 0x00, 0x69, 0x08, 0x00, 0xaa, 0x02, 0x02,
    0xaa, 0x80, 0x02, 0x28, 0x80, 0x02, 0x28, 0x80,
    0x08, 0x00, 0x20, 0x08, 0x00, 0x20, 0x08, 0x00,
    0x20, 0x02, 0x00, 0x20, 0x02, 0x00, 0x00, 0x84,
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

  public static void addMushrooms()
  {
    int x, y;

    for(x = 0; x < 40; x++)
      VIC.text_plot(x, 0, 32, 0);

    for(y = 24; y > 0; y--)
    {
      for(x = 0; x < 40; x++)
        VIC.text_plot(x, y, VIC.text_read(x, y - 1), 5);
    }

    for(y = 0; y < 16; y++)
    {
      int tx = rnd() % 40;
      int ty = rnd() % 17 + 1;

      VIC.text_plot(tx, ty, 35, 1);
      VIC.text_copy();

      for(x = 0; x < 12; x++)
      {
        SID.voice1_adsr(0x8888);
        SID.voice1_frequency(x << 9);
        SID.voice1_waveform(33);
//        wait(40);
        VIC.text_plot(tx, ty, 35 - (x >> 2), 1);
        VIC.text_copy();
        SID.voice1_waveform(32);
      }

      VIC.text_plot(tx, ty, 35, 5);
      VIC.text_copy();
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

      VIC.text_plot(x + i, y, temp, color);
    }
  }

  public static void printScore()
  {
    for (int i = 0; i < 8; i++)
      VIC.text_plot(16 + i, 0, score[i] + 48, 3);
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

  public static void wait(int time)
  {
    for(int i = 0; i < time; i++)
    {
      // wait
    }
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

  public static void animSpider()
  {
    if(anim_spider_status == 0)
    {
      if(anim_spider_timer == 0)
      {
        // start attack/decay
        //                srad 
        SID.voice3_adsr(0xf900);
        SID.voice3_frequency(1000);
        SID.voice3_waveform(129);
      }

      VIC.sprite4color((byte)(anim_spider_colors[(anim_spider_timer >> 2) & 3]));
      anim_spider_timer++;
 
      if(anim_spider_timer > 13)
      {
        // start decay/release
        SID.voice3_waveform(128);
        anim_spider_timer = 0;
        anim_spider_status = 1;
        VIC.sprite4color(4);
      }
    }
  }

  public static void animShip()
  {
    if(anim_ship_status == 0)
    {
      if(anim_ship_timer == 0)
      {
        // start attack/decay
        //                srad 
        SID.voice3_adsr(0x8d00);
        SID.voice3_frequency(2000);
        SID.voice3_waveform(129);
      }

      anim_ship_timer++;
 
      if(anim_ship_timer > 13)
      {
        // start decay/release
        SID.voice3_waveform(128);
        anim_ship_timer = 0;
        anim_ship_status = 1;
        VIC.sprite0color(0);
      }
    }
  }

  public static void animSnake()
  {
    if(anim_snake_status == 0)
    {
      if(anim_snake_timer == 0)
      {
        // start attack/decay
        //                srad 
        SID.voice3_adsr(0xe400);
        SID.voice3_frequency(2000);
        SID.voice3_waveform(33);
      }

      anim_snake_timer++;
 
      if(anim_snake_timer > 1)
      {
        // start decay/release
        SID.voice3_waveform(32);
        anim_snake_timer = 0;
        anim_snake_status = 1;
      }
    }
  }

  public static void playTitleMusic()
  {
    int c;

    c = getFreq(title_song1.charAt(music_pos));

    if(c != 0)
    {
      SID.voice1_adsr(0xa91a);
      SID.voice1_frequency(getFreq(title_song1.charAt(music_pos)));
      SID.voice1_pulse_width(1536 + pulse1);
      SID.voice1_waveform(33);
    }
    else
    {
      SID.voice1_waveform(32);
    }

    c = getFreq(title_song2.charAt(music_pos));

    if(c != 0)
    {
      SID.voice2_adsr(0xa91a);
      SID.voice2_frequency(getFreq(title_song2.charAt(music_pos)) << 1);
      SID.voice2_pulse_width(1536 + pulse2);
      SID.voice2_waveform(65);
    }
    else
    {
      SID.voice2_waveform(64);
    }

    c = getFreq(title_song3.charAt(music_pos));

    if(c != 0)
    {
      SID.voice3_adsr(0xa91a);
      SID.voice3_frequency(getFreq(title_song3.charAt(music_pos)) << 2);
      SID.voice3_pulse_width(1536 + pulse3);
      SID.voice3_waveform(65);
    }
    else
    {
      SID.voice1_waveform(64);
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
      SID.voice1_adsr(0xa914);
      SID.voice1_frequency(getFreq(end_song1.charAt(music_pos)));
      SID.voice1_waveform(65);
      SID.voice2_adsr(0xa914);
      SID.voice2_frequency(getFreq(end_song2.charAt(music_pos)) << 1);
      SID.voice2_waveform(65);
    }
    else
    {
      SID.voice1_waveform(64);
      SID.voice2_waveform(64);
    }

    music_pos++;

    if(music_pos >= end_song1.length())
      music_pos = 0;
  }

  public static void playNextLevelMusic()
  {
    int c = 0;
    int time = 0;

    SID.voice3_adsr(0x0000);
    pulse1 = 0x800;
    pulse2 = 0x600;
    music_pos = 0;

    while(true)
    {
      c = getFreq(next_level_song1.charAt(music_pos));

      if(c > 0)
      {
        SID.voice1_adsr(0xa949);
        SID.voice1_frequency(getFreq(next_level_song1.charAt(music_pos)));
        SID.voice1_waveform(65);
        SID.voice2_adsr(0xa949);
        SID.voice2_frequency(getFreq(next_level_song2.charAt(music_pos)) << 1);
        SID.voice2_waveform(65);
      }
      else
      {
        SID.voice1_waveform(64);
        SID.voice2_waveform(64);
      }

      time++;
      music_pos++;

      if(music_pos >= next_level_song1.length())
      {
        SID.voice1_waveform(64);
        SID.voice2_waveform(64);
        break;
      }

      wait(300);
    }

    wait(2000);
  }

  public static void title()
  {
    int time = 0;
    int colors[] = { 10, 7, 13, 14, 4, 14, 13, 7 };

    VIC.text_clear(32);
    VIC.sprite_enable(16);
    VIC.sprite_priority(0);
    VIC.sprite_multicolor_enable(255);
    VIC.sprite_multicolor0(1);
    VIC.sprite_multicolor1(12);
    VIC.sprite4pos(24 + 160 - 24, 50 + 124);
    VIC.sprite_expandx(16);
    VIC.sprite_expandy(16);

    printString(9, 9, 12, "Made with Java_Grinder");
    printString(13, 13, 4, "Kill the Spider");
    printString(13, 23, 14, "Joystick Port 2");

    //                srad 
    SID.filter_cutoff(1500);
    SID.filter_resonance(0x7c);
    SID.volume(31);

    pulse1 = 0x800;
    pulse2 = 0x800;
    pulse3 = 0x800;
    music_pos = 0;

    VIC.border(11);
    VIC.background(0);
    VIC.write_control2(VIC.read_control2() & 239);

    while(true)
    {
      final int joy = 255 - (Memory.read8(0xdc00) + 128);

      if((joy & 16) == 16)
        break;

      playTitleMusic();
      wait(300);
      time++;

      final int c = colors[time & 7];

      printString(9, 3, c,  "=== =   =  ==  = = ===");
      printString(9, 4, c,  "=   ==  = =  = = = =  ");
      printString(9, 5, c,  "=== = = = ==== ==  ===");
      printString(9, 6, c,  "  = =  == =  = = = =  ");
      printString(9, 7, c,  "=== =   = =  = = = ===");

      VIC.sprite4color(c);

      if((time & 15) >= 8)
        Memory.write8(sprite_pointer + 4, (byte)194);
      else
        Memory.write8(sprite_pointer + 4, (byte)195);

      VIC.sprite_multicolor0(colors[(time + 1) & 7]);

      VIC.text_copy();
    }

    VIC.sprite_enable(0);
    VIC.sprite4pos(0, 0);
    VIC.sprite_expandx(0);
    VIC.sprite_expandy(0);
    SID.volume(0);
  }

  public static void gameOver()
  {
    int i = 0;
    int joy = 0;
    int play_music = 64;

    int colors1[] = { 7, 10, 8, 2, 9, 2, 8, 10 }; 
    int colors2[] = { 13, 3, 14, 4, 6, 4, 14, 3 };

    SID.filter_resonance(0);
    SID.volume(15);

    anim_ship_status = 0;

    while(true)
    {
      animShip();

      // game over
      if(anim_ship_timer == 0)
        break;

      i++;

      VIC.border(colors1[(i >> 1) & 7]);

      wait(500);
    }

    VIC.border(2);
    wait(1000);
    VIC.write_control2(VIC.read_control2() & 239);

    VIC.sprite_enable(0);
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
      VIC.text_copy();

      joy = 255 - (Memory.read8(0xdc00) + 128);

      if((joy & 16) == 16)
      {
        while(true)
        {
          joy = 255 - (Memory.read8(0xdc00) + 128);

          if((joy & 16) == 0)
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
      wait(300);
    }
  }

  public static void snake()
  {
    int i = 0;
    int j = 0;
    int shipx = 180;
    int shipy = 208;
    int accelx = 0;
    int accely = 0;
    int time = 0;
    int snake_length = 8;
    int snake_count = snake_length;
    int spiderx = rnd() % 320 + 24;
    int spidery = 0;
    int spider_timer = 0;
    int button_pressed = 0;
    int shotx = 0;
    int shoty = 0;
    int shotstatus = 0;
    int shotfreq = 0;
    int joy = 0;
    int temp = 0;
    int tx = 0;
    int ty = 0;

    byte snakex[] = new byte[12];
    byte snakey[] = new byte[12];
    byte snakedir[] = new byte[12];
    byte snakestatus[] = new byte[12];

    resetScore();
    anim_ship_status = 1;
    anim_spider_status = 1;
    anim_snake_status = 1;

    VIC.sprite_enable(63);
    VIC.sprite_priority(0);
    VIC.sprite_multicolor_enable(16);
    VIC.sprite_multicolor0(1);
    VIC.sprite_multicolor1(12);
    VIC.sprite0color(1);
    VIC.sprite1color(7);
    VIC.sprite4color(4);
    VIC.border(11);

    VIC.background(0);
    VIC.multi1((byte)1);
    VIC.multi2((byte)7);
    VIC.write_control2(VIC.read_control2() | 16);

    // mushrooms
    VIC.text_clear((byte)32);

    for(i = 0; i < 50; i++)
    {
      VIC.text_plot(rnd() % 40, rnd() % 17 + 1, 35, 5);
      VIC.text_copy();
    }

    // initialize
    for(i = 0; i < 12; i++)
    {
      snakex[i] = (byte)i;
      snakey[i] = 1;
      snakedir[i] = 1;

      if(i < snake_length)
        snakestatus[i] = 1;
      else
        snakestatus[i] = 0;
    }

    shotx = 0;
    shoty = 0;
    shotstatus= 0;

    printScore();

    pulse1 = 0x800;
    pulse2 = 0x800;
    pulse3 = 0x800;
    SID.voice1_adsr(0x8411);
    SID.voice2_adsr(0x8411);
    SID.voice3_adsr(0x8411);
    SID.voice1_waveform(128);
    SID.voice2_waveform(128);
    SID.voice3_waveform(128);
    SID.filter_cutoff(0);
    SID.filter_resonance(0);
    SID.volume(15);

    while(true)
    {
      // move ship
      joy = 255 - (Memory.read8(0xdc00) + 128);

      if((joy & 8) == 8) accelx += 1;
      if((joy & 4) == 4) accelx -= 1;
      if((joy & 2) == 2) accely += 1;
      if((joy & 1) == 1) accely -= 1;

      if(accelx > 3) accelx = 3;
      if(accelx < -3) accelx = -3;
      if(accely > 3) accely = 3;
      if(accely < -3) accely = -3;

      if((time & 1) == 1)
      {
        if(accelx > 0) accelx--;
        if(accelx < 0) accelx++;
        if(accely > 0) accely--;
        if(accely < 0) accely++;
      }

      if(shipx > spiderx) spiderx++;
      if(shipx < spiderx) spiderx--;

      // ship move/collision
      tx = shipx + accelx;
      ty = shipy + accely;

      temp = VIC.text_read((tx - 20) >> 3, (ty - 46) >> 3);

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
      //  if(ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1)

      if(anim_spider_status == 1)
      {
        VIC.sprite4pos(spiderx, spidery);

        if(shipx < spiderx + 23 && shipx + 7 > spiderx &&
           shipy < spidery + 20 && shipy + 7 > spidery)
        {
          return;
        }

        if(spider_timer > 0)
        {
          spider_timer--;
        }
        else
        {
          spidery++;

          if(spidery > 250)
          {
            spidery = 0;
          }
        }
      }

      // shoot
      if((joy & 16) == 16)
      {
        if(button_pressed == 0 || shotstatus == 0)
        {
          shotx = shipx;
          shoty = shipy;
          shotstatus = 1;
          shotfreq = 1500;
        }

        button_pressed = 1;
      }
      else
      {
        button_pressed = 0;
      }

      // move shots
      if(shotstatus == 1)
      {
        shoty -= 8;
        shotfreq -= 40;

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
        SID.voice1_adsr(0x8411);
        SID.voice1_frequency(shotfreq);
//        SID.voice1_pulse_width(1536 + pulse1);
        SID.voice1_waveform(129);
      }
      else
      {
        SID.voice1_waveform(128);
      }

      // check shots
      if(shotstatus == 1)
      {
        tx = shotx + 4;
        ty = shoty;

        // shot hit spider
        if(anim_spider_status == 1)
        {
          if(tx < spiderx + 23 && tx + 1 > spiderx &&
             ty < spidery + 20 && ty + 7 > spidery)
          {
            spiderx = rnd() % 320 + 24;
            spidery = 0;
            spider_timer = rnd() % 400 + 1;
            shotx = 0;
            shoty = 0;
            shotstatus = 0;
            VIC.sprite1pos(shotx, shoty);
            anim_spider_status = 0;
            increaseScore(thousand);
            continue;
          }
        }

        tx = ((shotx - 20) >> 3);
        ty = ((shoty - 50) >> 3);
        temp = VIC.text_read(tx, ty);

        if(temp >= 33 && temp <= 35)
        {
          // hit mushroom
          temp--;

          if(temp < 32)
            temp = 32;

          VIC.text_plot(tx, ty, temp, 5);
          shotstatus = 0;
          shotx = 0;
          increaseScore(ten);
        }
        else if(temp != 32)
        {
          for(i = 0; i < 12; i++)
          {
            // check snake
            final int sx = snakex[i];
            final int sy = snakey[i];

            if((snakestatus[i] == 1) && (sx == tx) && (sy == ty))
            {
              // hit snake
              anim_snake_status = 0;
              shotstatus = 0;
              shotx = 0;
              VIC.text_plot(sx, sy, 35, 5);
              snakestatus[i] = 0;
              snake_count--;
              increaseScore(hundred);

              if(snake_count <= 0)
              {
                // next level
                snake_length += 2;

                if(snake_length > 12)
                  snake_length = 12;

                snake_count = snake_length;

                // reset snake
                for(i = 0; i < 12; i++)
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
                for(i = 0; i < 12; i++)
                  if(snakestatus[i] == 1)
                    VIC.text_plot(snakex[i], snakey[i], 32, 5);

                VIC.text_copy();

                // play music
                VIC.sprite1pos(0, 0);
                playNextLevelMusic();

                // add mushrooms
                addMushrooms();
                VIC.text_plot((shipx - 20) >> 3, (shipy - 46) >> 3, 32, 5);
              }
            }
          }
        }
      }

      // update timer
      time++;

      // move snake
      if((time & 1) == 1)
      {
        for(i = 0; i < 12; i++)
          if(snakestatus[i] == 1)
            VIC.text_plot(snakex[i], snakey[i], 32, 10);

        for(i = 0; i < 12; i++)
        {
          int sdir = snakedir[i];
          int sx = snakex[i] + sdir;
          int sy = snakey[i];

          if(sx < 0 || sx > 39 || VIC.text_read(sx, sy) != 32)
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
            VIC.text_plot(sx, sy, d, 10);

          snakex[i] = (byte)sx;
        }

        if((time & 7) >= 4)
          Memory.write8(sprite_pointer + 4, (byte)194);
        else
          Memory.write8(sprite_pointer + 4, (byte)195);

        animSpider();
        animSnake();
        VIC.text_copy();
      }
    }
  }

  public static void main()
  {
    int i, temp;

    VIC.make_text_table();
    VIC.make_color_table();

    // copy sprites
    clearSprite(sprite_ram);
    clearSprite(sprite_ram + 64);
    clearSprite(sprite_ram + 128);
    clearSprite(sprite_ram + 192);
    clearSprite(sprite_ram + 256);

    for(i = 0; i < 63; i++)
    {
      Memory.write8(sprite_ram + i, (byte)sprite_ship[i]);
      Memory.write8(sprite_ram + 64 + i, (byte)sprite_shot[i]);
      Memory.write8(sprite_ram + 128 + i, (byte)sprite_spider1[i]);
      Memory.write8(sprite_ram + 192 + i, (byte)sprite_spider2[i]);
    }

    Memory.write8(sprite_pointer + 0, (byte)192);
    Memory.write8(sprite_pointer + 1, (byte)193);
    Memory.write8(sprite_pointer + 2, (byte)193);
    Memory.write8(sprite_pointer + 3, (byte)193);
    Memory.write8(sprite_pointer + 4, (byte)194);
    Memory.write8(sprite_pointer + 5, (byte)196);

    // copy chars
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

/*
    Memory.write8(0xd021, (byte)0);
    Memory.write8(0xd022, (byte)1);
    Memory.write8(0xd023, (byte)1);
    Memory.write8(0xd024, (byte)1);
*/

    // reset SID
    SID.clear();
/*
    for(i = 0; i <= 28; i++)
      Memory.write8(0xd400 + i, (byte)0);
*/
/*
    // max test
    if(Math.max(1000, -1000) == 1000)
      printString(0, 0, 1, "max success");
    else
      printString(0, 0, 1, "max fail");
      
    if(Math.min(1000, -1000) == -1000)
      printString(0, 2, 1, "min success");
    else
      printString(0, 2, 1, "min fail");
      
    if(Math.abs(-32000) == 32000)
      printString(0, 4, 1, "abs success");
    else
      printString(0, 4, 1, "abs fail");

    VIC.text_copy();
*/      

    while(true)
    {
//      VIC.border(0);
//      VIC.border(4);

      title();
      snake();
      gameOver();

    }
  }
}

