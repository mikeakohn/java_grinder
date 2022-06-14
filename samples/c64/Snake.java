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

  // title colors
  static int title_colors[] = { 11, 5, 12, 13, 15, 7, 15, 13, 12, 5 };

  // random number seed
  static int seed = 12345;

  // music related
  static int pulse1 = 0;
  static int pulse2 = 0;
  static int pulse3 = 0;
  static int pos1 = 0;
  static int pos2 = 0;
  static int pos3 = 0;
  
  static int freq[] =
  {
    0x0115, 0x0125, 0x0137, 0x0149, 0x015d, 0x0172,
    0x0188, 0x019f, 0x01b8, 0x01d2, 0x01ee, 0x020b
  };

  static String keys = "q2w3er5t6y7ui9o0p";


  static String song1 =
    "o   o   o   o   7 7 7   9 9 9   "
  + "oooooooooooooooo77777777yyyyrree";

  static String song2 =
    "wry7yrwry7yrwry7wry7yrwry7yrwry7";

  static String song3 =
    "wwrryy77wwrrrree";

  static final int sprite_ship[] =
  {
    0b00011000, 0b00000000, 0b00000000,
    0b00011000, 0b00000000, 0b00000000,
    0b00011000, 0b00000000, 0b00000000,
    0b11011011, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000,
    0b11100111, 0b00000000, 0b00000000,
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
    0b00011000, 0b00000000, 0b00000000,
  };

  static final int sprite_spider1[] =
  {
    0b00010000, 0b00010000, 0b00000000,
    0b00010000, 0b00001000, 0b00000000,
    0b00010000, 0b00001000, 0b00000000,
    0b00010001, 0b10001000, 0b00000000,
    0b10001011, 0b11010000, 0b00000000,
    0b01001111, 0b11110000, 0b00000000,
    0b00100111, 0b11100111, 0b00000000,
    0b00011111, 0b11111000, 0b00000000,
    0b00000111, 0b11100000, 0b00000000,
    0b00011111, 0b11111000, 0b00000000,
    0b11100111, 0b11100100, 0b00000000,
    0b00001011, 0b11010011, 0b00000000,
    0b00001001, 0b01010000, 0b00000000,
    0b00010000, 0b00001000, 0b00000000,
    0b00010000, 0b00001000, 0b00000000,
    0b00001000, 0b00001000, 0b00000000,
  };

  static final int sprite_spider2[] =
  {
    0b00001000, 0b00001000, 0b00000000,
    0b00010000, 0b00001000, 0b00000000,
    0b00010000, 0b00001000, 0b00000000,
    0b00010001, 0b10001000, 0b00000000,
    0b00001011, 0b11010001, 0b00000000,
    0b00001111, 0b11110010, 0b00000000,
    0b11100111, 0b11100100, 0b00000000,
    0b00011111, 0b11111000, 0b00000000,
    0b00000111, 0b11100000, 0b00000000,
    0b00011111, 0b11111000, 0b00000000,
    0b00100111, 0b11100111, 0b00000000,
    0b11001011, 0b11010000, 0b00000000,
    0b00001010, 0b10010000, 0b00000000,
    0b00010000, 0b00001000, 0b00000000,
    0b00010000, 0b00001000, 0b00000000,
    0b00001000, 0b00001000, 0b00000000,
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
    0b01111111,
    0b01111111,
    0b01111111,
    0b01111111,
    0b11111110,
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
    0b11111110,
    0b01111111,
    0b01111111,
    0b01111111,
    0b01111111,
    0b11111110,
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
      VIC.text_plot(16 + i, 0, score[i] + 48, 12);
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


  public static void playMusic()
  {
    int c;

    c = getFreq(song1.charAt(pos1));

    if(c > 0)
    {
      SID.voice1_frequency(getFreq(song1.charAt(pos1)));
      SID.voice1_pulse_width(1536 + pulse1);
      SID.voice1_waveform(33);
    }

    c = getFreq(song2.charAt(pos2));

    if(c > 0)
    {
      SID.voice2_frequency(getFreq(song2.charAt(pos2)) << 1);
      SID.voice2_pulse_width(1536 + pulse2);
      SID.voice2_waveform(65);
    }

    c = getFreq(song3.charAt(pos3));

    if(c > 0)
    {
      SID.voice3_frequency(getFreq(song3.charAt(pos3)) << 2);
      SID.voice3_pulse_width(1536 + pulse3);
      SID.voice3_waveform(65);
    }

    pos1++;

    if(pos1 >= song1.length())
      pos1 = 0;

    pos2++;

    if(pos2 >= song2.length())
      pos2 = 0;

    pos3++;

    if(pos3 >= song3.length())
      pos3 = 0;

    pulse1 += 256;
    pulse1 &= 511;
    pulse2 += 256;
    pulse2 &= 511;
    pulse3 += 256;
    pulse2 &= 511;
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
    SID.voice1_adsr(0x8f1a);
    SID.voice2_adsr(0x8f1a);
    SID.voice3_adsr(0x8f1a);
    SID.filter_cutoff(1500);
    SID.filter_resonance(0x7c);
    SID.volume(31);

    while(true)
    {
      final int joy = 255 - (Memory.read8(0xdc01) + 128);

      if((joy & 16) == 16)
        break;

      playMusic();
      wait(400);
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

  public static int collision(int ax1, int ay1, int ax2, int ay2,
                              int bx1, int by1, int bx2, int by2)
  {
    if(ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1)
      return 1;

    return 0;
  }

  public static void gameOver()
  {
    int joy = 0;

    VIC.sprite_enable(0);
    printString(15, 9, 3, "Game Over");
    VIC.text_copy();
    wait(10000);
    printString(7, 11, 12, "Press Button to Play Again");
    VIC.text_copy();
    wait(10000);

    while(true)
    {
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
    int t0 = 0;
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    int tempx = 0;
    int tempy = 0;
    int moved = 0;

    byte[] snakex = new byte[16];
    byte[] snakey = new byte[16];
    byte[] snakedir = new byte[16];
    byte[] snakestatus = new byte[16];
    int[] shotx = new int[2];
    int[] shoty = new int[2];
    int[] shotstatus = new int[2];
    int[] shotfreq = new int[2];

    // sprites
    clearSprite(sprite_ram);
    clearSprite(sprite_ram + 64);
    clearSprite(sprite_ram + 128);
    clearSprite(sprite_ram + 192);
    clearSprite(sprite_ram + 256);

    for(i = 0; i < 24; i++)
    {
      Memory.write8(sprite_ram + i, (byte)sprite_ship[i]);
      Memory.write8(sprite_ram + 64 + i, (byte)sprite_shot[i]);
    }

    for(i = 0; i < 48; i++)
    {
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
    VIC.sprite0color(1);
    VIC.sprite1color(15);
    VIC.sprite2color(15);
    VIC.sprite3color(15);
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

    SID.voice1_adsr(0x4111);
    SID.voice2_adsr(0x4111);
    SID.voice3_adsr(0x1444);
    SID.voice1_waveform(128);
    SID.voice2_waveform(128);
    SID.voice3_waveform(32);
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

      VIC.sprite4pos(spiderx, spidery);

      // ship move/collision
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
        else if(dx1 != dx2 && dy1 == dy2)
        {
          if(t0 == 32 && t1 == 32)
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
          if(accelx > 0) accelx = 1;
          if(accelx < 0) accelx = -1;
          if(accely > 0) accely = 1;
          if(accely < 0) accely = -1;
        }
      }

      if(shipx < 24) shipx = 24;
      if(shipx > 336) shipx = 336;
      if(shipy < 194) shipy = 194;
      if(shipy > 242) shipy = 242;

      VIC.sprite0pos(shipx, shipy);

      // ship hit snake
      if( (t0 >= 160 && t0 <= 163) ||
          (t1 >= 160 && t1 <= 163) ||
          (t2 >= 160 && t2 <= 163) ||
          (t3 >= 160 && t3 <= 163) )
      {
        VIC.border(2);
        return;
      }

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
              shotfreq[i] = 1000;
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

      // ship hit spider
      if(collision(shipx, shipy, shipx + 7, shipy + 7,
                   spiderx, spidery, spiderx + 15, spidery + 15) == 1)
      {
        VIC.border(2);
        return;
      }

      for(j = 0; j < 2; j++)
      {
        if(shotstatus[j] == 1)
        {
          tx = shotx[j] + 4;
          ty = shoty[j];

          // shot hit spider
          if(collision(tx, ty, tx + 1, ty + 7,
                       spiderx, spidery, spiderx + 15, spidery + 15) == 1)
          {
            spiderx = rnd() % 320 + 24;
            spidery = 0;
            shotx[j] = 0;
            shoty[j] = 0;
            shotstatus[j] = 0;
            VIC.sprite1pos(shotx[0], shoty[0]);
            VIC.sprite2pos(shotx[1], shoty[1]);
            increaseScore(thousand);
            continue;
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
            VIC.text_plot(snakex[i], snakey[i], 32, 7);

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
            VIC.text_plot(sx, sy, d, 7);

          snakex[i] = (byte)sx;

          if((time & 7) >= 4)
            Memory.write8(sprite_pointer + 4, (byte)194);
          else
            Memory.write8(sprite_pointer + 4, (byte)195);
        }

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

    VIC.background(0);
    VIC.border(11);

    while(true)
    {
      VIC.background(0);
      VIC.border(11);

      title();
      snake();
      gameOver();
    }
  }
}

