import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.c64.*;

public class Snake
{
  // addresses
  static final int char_ram = 0xc800;
  static final int sprite_ram = 0xf000;
  static final int sprite_pointer = 0xc400 + 1016;

  // string for score
  static int score[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  static final int ten[] = { 0, 0, 0, 0, 0, 0, 1, 0 };
  static final int hundred[] = { 0, 0, 0, 0, 0, 1, 0, 0 };
  static final int thousand[] = { 0, 0, 0, 0, 1, 0, 0, 0 };

  // colors
  static int title_colors[] = { 11, 12, 15, 12, 11, 12, 15, 12, 11, 11 };
  static int spider_exp_colors[] = { 1, 15, 12, 11 };

  // random number seed
  static int seed = 12345;

  // music related
  static int pulse1 = 0;
  static int pulse2 = 0;
  static int pulse3 = 0;
  static int music_pos1 = 0;
  static int music_pos2 = 0;
  static int music_pos3 = 0;
  
  static int freq[] =
  {
    0x0115, 0x0125, 0x0137, 0x0149, 0x015d, 0x0172,
    0x0188, 0x019f, 0x01b8, 0x01d2, 0x01ee, 0x020b
  };

  static String keys = "q2w3er5t6y7ui9o0p";

  static String title_song1 =
    "o   o   o   o   7 7 7   9 9 9   "
  + "oooooooooooooooo77777777yyyyrree";

  static String title_song2 =
    "wry7yrwry7yrwry7wry7yrwry7yrwry7";

  static String title_song3 =
    "wwrryy77wwrrrree";

  static String end_song1 =
    "q 0 0 0 q 0 0 0 "
  + "3 0 0 0 3 0 0 0 ";

  static String end_song2 =
    "q 3 r 5 t 5 r 3 "
  + "q 3 r 5 t 5 r 3 ";

  static String end_song3 =
    "i o i 0 i o i 0 "
  + "i o i 0 i o i 0 ";

  static int spider_exp_timer = 0;
  static int spider_exp_status = 0;

  static final int sprite_ship[] =
  {
    0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x04, 0x00,
    0x00, 0x04, 0x00, 0x00, 0x44, 0x40, 0x00, 0x44,
    0x40, 0x00, 0x59, 0x40, 0x00, 0x59, 0x40, 0x00,
    0x59, 0x40, 0x00, 0x55, 0x40, 0x00, 0x44, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83,
  };

  static final int sprite_shot[] =
  {
    0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x08, 0x00,
    0x00, 0x04, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08,
    0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87,
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

  static final int char_block[] =
  {
    0b01111110,
    0b11111111,
    0b11111110,
    0b11111111,
    0b11111110,
    0b11111111,
    0b11111110,
    0b01010100
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
    0b11111110,
    0b10111111,
    0b10111111,
    0b10111111,
    0b10111111,
    0b11111110,
    0b11001100
  };

  static final int char_segment2[] =
  {
    0b00110011,
    0b10111111,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b10111111,
    0b00110011
  };

  static final int char_segment3[] =
  {
    0b00110011,
    0b11111110,
    0b10111111,
    0b10111111,
    0b10111111,
    0b10111111,
    0b11111110,
    0b00110011
  };

  static final int char_segment4[] =
  {
    0b11001100,
    0b10111111,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b10111111,
    0b11001100
  };

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

  public static void expSpider()
  {
    if(spider_exp_status == 0)
    {
      if(spider_exp_timer == 0)
      {
        // start attack/decay
        //                srad 
        SID.voice3_adsr(0xb900);
        SID.voice3_frequency(1000);
        SID.voice3_waveform(129);
      }

      VIC.sprite4color((byte)(spider_exp_colors[(spider_exp_timer >> 2) & 3]));
      spider_exp_timer++;
 
      if(spider_exp_timer > 13)
      {
        // start decay/release
        SID.voice3_waveform(128);
        spider_exp_timer = 0;
        spider_exp_status = 1;
        VIC.sprite4color(4);
      }
    }
  }

  public static void playTitleMusic()
  {
    int c;

    c = getFreq(title_song1.charAt(music_pos1));

    if(c != 0)
    {
      SID.voice1_frequency(getFreq(title_song1.charAt(music_pos1)));
      SID.voice1_pulse_width(1536 + pulse1);
      SID.voice1_waveform(33);
    }
    else
    {
      SID.voice1_waveform(32);
    }

    c = getFreq(title_song2.charAt(music_pos2));

    if(c != 0)
    {
      SID.voice2_frequency(getFreq(title_song2.charAt(music_pos2)) << 1);
      SID.voice2_pulse_width(1536 + pulse2);
      SID.voice2_waveform(65);
    }
    else
    {
      SID.voice1_waveform(64);
    }

    c = getFreq(title_song3.charAt(music_pos3));

    if(c != 0)
    {
      SID.voice3_frequency(getFreq(title_song3.charAt(music_pos3)) << 2);
      SID.voice3_pulse_width(1536 + pulse3);
      SID.voice3_waveform(65);
    }
    else
    {
      SID.voice1_waveform(64);
    }

    music_pos1++;

    if(music_pos1 >= title_song1.length())
      music_pos1 = 0;

    music_pos2++;

    if(music_pos2 >= title_song2.length())
      music_pos2 = 0;

    music_pos3++;

    if(music_pos3 >= title_song3.length())
      music_pos3 = 0;

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

    c = getFreq(end_song1.charAt(music_pos1));

    if(c > 0)
    {
      SID.voice1_frequency(getFreq(end_song1.charAt(music_pos1)));
 //     SID.voice1_pulse_width(1536 + pulse1);
      SID.voice1_waveform(17);
    }
    else
    {
      SID.voice1_waveform(16);
    }

    c = getFreq(end_song2.charAt(music_pos2));

    if(c > 0)
    {
      SID.voice2_frequency(getFreq(end_song2.charAt(music_pos2)) << 1);
//      SID.voice2_pulse_width(1536 + pulse2);
      SID.voice2_waveform(17);
    }
    else
    {
      SID.voice2_waveform(16);
    }

    c = getFreq(end_song3.charAt(music_pos3));

    if(c > 0)
    {
      SID.voice3_frequency(getFreq(end_song3.charAt(music_pos3)) << 2);
//      SID.voice3_pulse_width(1536 + pulse3);
      SID.voice3_waveform(17);
    }
    else
    {
      SID.voice3_waveform(16);
    }

    music_pos1++;

    if(music_pos1 >= end_song1.length())
      music_pos1 = 0;

    music_pos2++;

    if(music_pos2 >= end_song2.length())
      music_pos2 = 0;

    music_pos3++;

    if(music_pos3 >= end_song3.length())
      music_pos3 = 0;

/*
    pulse1 += 256;
    pulse1 &= 511;
    pulse2 += 256;
    pulse2 &= 511;
    pulse3 += 256;
    pulse2 &= 511;
*/
  }

  public static void title()
  {
    int time = 0;

    VIC.text_clear(32);
    printString(19, 12, 11, "by");
    printString(14, 13, 12, "Joe Davisson");
    printString(9, 15, 5, "Made with Java_Grinder");
    printString(12, 23, 12, "Joystick Port 1");

    //                srad 
    SID.voice1_adsr(0xaf1a);
    SID.voice2_adsr(0xaf1a);
    SID.voice3_adsr(0xaf1a);
    SID.filter_cutoff(1500);
    SID.filter_resonance(0x7c);
    SID.volume(31);

    pulse1 = 0;
    pulse2 = 0;
    pulse3 = 0;
    music_pos1 = 0;
    music_pos2 = 0;
    music_pos3 = 0;

    while(true)
    {
      final int joy = 255 - (Memory.read8(0xdc01) + 128);

      if((joy & 16) == 16)
        break;

      playTitleMusic();
      wait(300);
      time++;

      final int c = title_colors[time % 10];

      printString(9, 5, c,  "=== =   =  ==  = = ===");
      printString(9, 6, c,  "=   ==  = =  = = = =  ");
      printString(9, 7, c,  "=== = = = ==== ==  ===");
      printString(9, 8, c,  "  = =  == =  = = = =  ");
      printString(9, 9, c,  "=== =   = =  = = = ===");

      VIC.text_copy();
    }

    SID.volume(0);
  }

  public static void gameOver()
  {
    int i = 0;
    int joy = 0;

    int colors[] = { 9, 2, 11, 8, 12, 10, 15, 7, 15, 10, 12, 8, 11, 2 }; 

    VIC.border(7);
    wait(1000);
    VIC.border(10);
    wait(1000);
    VIC.border(8);
    wait(1000);
    VIC.border(2);
    wait(2000);
    VIC.sprite_enable(0);
    VIC.sprite0pos(0, 0);
    VIC.sprite1pos(0, 0);
    VIC.sprite2pos(0, 0);
    VIC.sprite3pos(0, 0);
    VIC.sprite4pos(0, 0);
    VIC.sprite5pos(0, 0);
    VIC.sprite6pos(0, 0);
    VIC.sprite7pos(0, 0);
    
    pulse1 = 0;
    pulse2 = 0;
    pulse3 = 0;
    music_pos1 = 0;
    music_pos2 = 0;
    music_pos3 = 0;

    while(true)
    {
      final int c = colors[i];

      printString(14, 11, c,  "           ");
      printString(14, 12, c,  " Game Over ");
      printString(14, 13, c, "           ");
      VIC.text_copy();
      i++;

      if(i > 13)
        i = 0;

      joy = 255 - (Memory.read8(0xdc01) + 128);

      if((joy & 16) == 16)
      {
        while(true)
        {
          joy = 255 - (Memory.read8(0xdc01) + 128);

          if((joy & 16) == 0)
            return;
        }
      }

      playEndMusic();
      wait(200);
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
    int snake_length = 6;
    int snake_count = snake_length;
    int spiderx = rnd() % 320 + 24;
    int spidery = 0;
    int button_pressed = 0;
    int shots_active = 0;
    int shot_time = 0;
    int joy = 0;
    int temp = 0;
    int tx = 0;
    int ty = 0;
    int dx = 0;
    int dy = 0;
    int tempx = 0;
    int tempy = 0;

    byte[] snakex = new byte[16];
    byte[] snakey = new byte[16];
    byte[] snakedir = new byte[16];
    byte[] snakestatus = new byte[16];
    int[] shotx = new int[2];
    int[] shoty = new int[2];
    int[] shotstatus = new int[2];
    int[] shotfreq = new int[2];

    resetScore();
    spider_exp_status = 1;

    // sprites
    clearSprite(sprite_ram);
    clearSprite(sprite_ram + 64);
    clearSprite(sprite_ram + 128);
    clearSprite(sprite_ram + 192);
    clearSprite(sprite_ram + 256);

/*
    for(i = 0; i < 24; i++)
    {
      Memory.write8(sprite_ram + i, (byte)sprite_ship[i]);
      Memory.write8(sprite_ram + 64 + i, (byte)sprite_shot[i]);
    }
*/

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

    VIC.sprite_enable(63);
    VIC.sprite_priority(0);
    VIC.sprite_multicolor_enable(255);
    VIC.sprite_multicolor0(1);
    VIC.sprite_multicolor1(12);
    VIC.sprite0color(14);
    VIC.sprite1color(10);
    VIC.sprite2color(10);
    VIC.sprite3color(10);
    VIC.sprite4color(4);

    // mushrooms
    VIC.text_clear((byte)32);

    for(i = 0; i < 50; i++)
    {
      VIC.text_plot(rnd() % 40, rnd() % 17 + 1, 35, 5);
      VIC.text_copy();
    }

    // initialize
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

    for(i = 0; i < 2; i++)
    {
      shotx[i] = 0;
      shoty[i] = 0;
      shotstatus[i] = 0;
    }

    printScore();

    SID.voice1_adsr(0x8411);
    SID.voice2_adsr(0x8411);
    SID.voice3_adsr(0xa411);
    SID.voice1_waveform(128);
    SID.voice2_waveform(128);
    SID.voice3_waveform(128);
    SID.filter_cutoff(0);
    SID.filter_resonance(0);
    SID.volume(15);

    while(true)
    {
      // move ship
      joy = 255 - (Memory.read8(0xdc01) + 128);

      if((joy & 8) == 8) accelx += 2;
      if((joy & 4) == 4) accelx -= 2;
      if((joy & 2) == 2) accely += 2;
      if((joy & 1) == 1) accely -= 2;

      if(accelx > 4) accelx = 4;
      if(accelx < -4) accelx = -4;
      if(accely > 4) accely = 4;
      if(accely < -4) accely = -4;

      if(accelx > 0) accelx--;
      if(accelx < 0) accelx++;
      if(accely > 0) accely--;
      if(accely < 0) accely++;

      if(shipx > spiderx) spiderx++;
      if(shipx < spiderx) spiderx--;

      spidery++;
      if(spidery > 250)
        spidery = 0;

      // ship move/collision
      tempx = shipx + accelx;
      tempy = shipy + accely;

      final int dx1 = (tempx - 18) >> 3;
      final int dy1 = (tempy - 44) >> 3;
//      final int dx1 = (tempx - 20) >> 3;
//      final int dy1 = (tempy - 46) >> 3;

      temp = VIC.text_read(dx1, dy1);

      if(temp == 32)
      {
        shipx = tempx;
        shipy = tempy;
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
/*
      moved = 0;

      for(i = 0; i < 3; i++)
      {
        tempx = shipx;
        tempy = shipy;

        if(i == 0)
        {
          tempx += accelx;
          tempy += accely;
        }
        else if(i == 1)
        {
          tempx += accelx;
        }
        else if(i == 2)
        {
          tempy += accely;
        }

        final int dx1 = (tempx - 24) >> 3;
        final int dy1 = (tempy - 50) >> 3;
        final int dx2 = ((tempx + 7) - 24) >> 3;
        final int dy2 = ((tempy + 7) - 50) >> 3;

        t0 = VIC.text_read(dx1, dy1);
        t1 = VIC.text_read(dx2, dy1);
        t2 = VIC.text_read(dx1, dy2);
        t3 = VIC.text_read(dx2, dy2);

        if(dx1 != dx2 && dy1 != dy2)
        {
          if(t0 == 32 && t1 == 32 && t2 == 32 && t3 == 32)
          {
            moved = 1;
          }
        }
        else if(dx1 == dx2 && dy1 != dy2)
        {
          if(t0 == 32 && t2 == 32)
          {
            moved = 1;
          }
        }
        else if(dx1 != dx2 && dy1 == dy2)
        {
          if(t0 == 32 && t1 == 32)
          {
            moved = 1;
          }
        }
        else if(dx1 == dx2 && dy1 == dy2)
        {
          if(t0 == 32)
          {
            moved = 1;
          }
        }

        if(moved == 1)
        {
          shipx = tempx;
          shipy = tempy;
          break;
        }
        else
        {
          // ship hit snake
          if( (t0 >= 160 && t0 <= 163) ||
              (t1 >= 160 && t1 <= 163) ||
              (t2 >= 160 && t2 <= 163) ||
              (t3 >= 160 && t3 <= 163) )
          {
            return;
          }

          if(accelx > 0) accelx = 1;
          if(accelx < 0) accelx = -1;
          if(accely > 0) accely = 1;
          if(accely < 0) accely = -1;
        }
      }
*/

      if(shipx < 24) shipx = 24;
      if(shipx > 336) shipx = 336;
//      if(shipy < 194) shipy = 194;
      if(shipy > 242) shipy = 242;

      VIC.sprite0pos(shipx, shipy);

      // ship hit spider
  //  if(ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1)

    if(spider_exp_status == 1)
    {
      VIC.sprite4pos(spiderx, spidery);

      if(shipx < spiderx + 15 && shipx + 7 > spiderx &&
         shipy < spidery + 15 && shipy + 7 > spidery)
      {
        return;
      }
    }

/*
      if(collision(shipx, shipy, shipx + 7, shipy + 7,
                   spiderx, spidery, spiderx + 15, spidery + 15) == 1)
      {
        return;
      }
*/
      // shoot
      if((joy & 16) == 16)
      {
        if(button_pressed == 0 || shots_active == 0 || shot_time == 0)
        {
          for(i = 0; i < 2; i++)
          {
            if(shotstatus[i] == 0)
            {
              shotx[i] = shipx;
              shoty[i] = shipy;
              shotstatus[i] = 1;
              shotfreq[i] = 1500;
              shots_active++;
              shot_time = 0;
              break;
            }
          }
        }

        shot_time++;

        if(shot_time >= 6)
          shot_time = 0;

        button_pressed = 1;
      }
      else
      {
        button_pressed = 0;
      }

      // move shots
      for(i = 0; i < 2; i++)
      {
        if(shotstatus[i] == 1)
        {
          shoty[i] -= 8;
          shotfreq[i] -= 40;

          if(shoty[i] < 42)
          {
            shotx[i] = 0;
            shoty[i] = 0;
            shotstatus[i] = 0;
            shots_active--;
          }
        }
      }

      VIC.sprite1pos(shotx[0], shoty[0]);
      VIC.sprite2pos(shotx[1], shoty[1]);

      if(shotstatus[0] == 1)
      {
        SID.voice1_frequency(shotfreq[0]);
//        SID.voice1_pulse_width(1536 + pulse1);
        SID.voice1_waveform(129);
      }
      else
      {
        SID.voice1_waveform(128);
      }

      if(shotstatus[1] == 1)
      {
        SID.voice2_frequency(shotfreq[1]);
//        SID.voice2_pulse_width(1536 + pulse1);
        SID.voice2_waveform(129);
      }
      else
      {
        SID.voice2_waveform(128);
      }

      for(j = 0; j < 2; j++)
      {
        if(shotstatus[j] == 1)
        {
          tx = shotx[j] + 4;
          ty = shoty[j];

          // shot hit spider
          if(spider_exp_status == 1)
          {
            if(tx < spiderx + 15 && tx + 1 > spiderx &&
               ty < spidery + 15 && ty + 7 > spidery)
            {
              spiderx = rnd() % 320 + 24;
              spidery = 0;
              shotx[j] = 0;
              shoty[j] = 0;
              shotstatus[j] = 0;
              VIC.sprite1pos(shotx[0], shoty[0]);
              VIC.sprite2pos(shotx[1], shoty[1]);
              spider_exp_status = 0;
              increaseScore(thousand);
              continue;
            }
          }

          dx = ((shotx[j] - 20) >> 3);
          dy = ((shoty[j] - 50) >> 3);
          temp = VIC.text_read(dx, dy);

          if(temp >= 33 && temp <= 35)
          {
            // hit mushroom
            temp--;

            if(temp < 32)
              temp = 32;

            VIC.text_plot(dx, dy, temp, 5);
            shotstatus[j] = 0;
            shots_active--;
            shotx[j] = 0;
            shot_time = 0;
            increaseScore(ten);
          }
          else if(temp != 32)
          {
            for(i = 0; i < snake_length; i++)
            {
              // check snake
              final int sx = snakex[i];
              final int sy = snakey[i];

              if((snakestatus[i] == 1) && (sx == dx) && (sy == dy))
              {
                // hit snake
                shotstatus[j] = 0;
                shots_active--;
                shotx[j] = 0;
                VIC.text_plot(sx, sy, 35, 5);
                snakestatus[i] = 0;
                snake_count--;
                increaseScore(hundred);
                shot_time = 0;

                if(snake_count <= 0)
                {
                  // next level
                  snake_length += 2;

                  if(snake_length > 16)
                    snake_length = 16;

                  snake_count = snake_length;

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

                  for(i = 0; i < 10; i++)
                  {
                    VIC.text_plot(rnd() % 40, rnd() % 17 + 1, 35, 5);
                    wait(500);
                    VIC.text_copy();
                  }
                }

                break;
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
        for(i = 0; i < 16; i++)
          if(snakestatus[i] == 1)
            VIC.text_plot(snakex[i], snakey[i], 32, 10);

        for(i = 0; i < 16; i++)
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

          if((time & 7) >= 4)
            Memory.write8(sprite_pointer + 4, (byte)194);
          else
            Memory.write8(sprite_pointer + 4, (byte)195);
        }

        expSpider();
        VIC.text_copy();
      }
    }
  }

  public static void main()
  {
    int i, temp;

    VIC.make_text_table();
    VIC.make_color_table();

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

    VIC.write_control2(VIC.read_control2() | 16);

    VIC.background(0);
    VIC.multi1((byte)1);
    VIC.multi2((byte)7);

/*
    Memory.write8(0xd021, (byte)0);
    Memory.write8(0xd022, (byte)1);
    Memory.write8(0xd023, (byte)1);
    Memory.write8(0xd024, (byte)1);
*/

    while(true)
    {
      VIC.border(11);
      title();
      VIC.border(11);
      snake();
      gameOver();
    }
  }
}

