// demo game for java_grinder
// "space revenge" - by Joe Davisson

import net.mikekohn.java_grinder.Atari2600;
import net.mikekohn.java_grinder.Joystick;
import net.mikekohn.java_grinder.Memory;

public class SpaceRevenge
{
  public static byte ship0[] =
  {
    (byte)0b00011000,
    (byte)0b00011000,
    (byte)0b10111101,
    (byte)0b11100111,
    (byte)0b11111111,
    (byte)0b10011001,
    (byte)0b00000000,
  };

  public static byte ship1[] =
  {
    (byte)0b10011001,
    (byte)0b11111111,
    (byte)0b11100111,
    (byte)0b10111101,
    (byte)0b00011000,
    (byte)0b00011000,
    (byte)0b00000000,
  };

  public static byte shot[] =
  {
    (byte)0b11111111,
    (byte)0b11111111,
    (byte)0b11111111,
    (byte)0b00000000,
  };

  public static byte background[] =
  {
    (byte)0b11110000, (byte)0b11111111, (byte)0b11111111,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b11110000, (byte)0b11000000, (byte)0b11000000,
    (byte)0b11110000, (byte)0b11000000, (byte)0b11000000,
    (byte)0b01110000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b01110000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b01110000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b01110000, (byte)0b00111100, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00111100, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00111100, (byte)0b00111100,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00111100,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00111100,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00111100,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00111100,
    (byte)0b00010000, (byte)0b00111100, (byte)0b00111100,
    (byte)0b00010000, (byte)0b00111100, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00111100, (byte)0b00000000,
    (byte)0b00010000, (byte)0b11111111, (byte)0b00000000,
    (byte)0b00010000, (byte)0b11111111, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b00010000, (byte)0b00000000, (byte)0b00000000,
    (byte)0b11110000, (byte)0b11111111, (byte)0b11111111,
  };

  public static byte nothing[] =
  {
    (byte)0
  };

  public static byte game_params[] =
  {
    (byte)0b010000,
    (byte)0b010010,
    (byte)0b010110,
    (byte)0b010010,
    (byte)0b010001,
    (byte)0b010011,
    (byte)0b100101,
    (byte)0b110111,
  };

  public static byte offset[] =
  {
    (byte)0,
    (byte)32,
    (byte)56,
    (byte)32,
    (byte)16,
    (byte)32,
    (byte)9,
    (byte)24,
  };

  public static void main()
  {
    final int left = 7;
    final int right = 120;
    final int ship0_y = 51;
    final int ship1_y = 2;

    int ship0_x = 0;
    int shot0_y = 0;
    int ship1_x = 0;
    int shot1_y = 0;
    int shot1_offset = 0;
    int rnd = 127;
    int frame = 0;
    int ship0_hit = 0;
    int ship1_hit = 0;
    int score0 = 1;
    int score1 = 11;
    int mode = 0;
    int dir = 0;
    int game = 0;
    int switches = 0;

    Memory.write8(0x04, (byte)0b010000);

    // reflect playfield
    Memory.write8(0x0a, (byte)1);

    // set graphics pointers
    Atari2600.setPlayer0Sprite(ship0);
    Atari2600.setPlayer1Sprite(ship1);
    Atari2600.setMissile0Sprite(shot);
    Atari2600.setMissile1Sprite(shot);
    Atari2600.setBallSprite(nothing);
    Atari2600.setPlayfieldData(background);
    Atari2600.setPlayfieldLength((byte)background.length);

    while(true)
    {
      // won't execute
      if (dir == 32)
        Atari2600.setBank((byte)0);

      Atari2600.clearMotionRegisters();
      Atari2600.clearCollisionLatches();
      switches = Atari2600.getSwitches();

      if (mode == 0)
      {
        // initial screen, waiting for reset
        Atari2600.startVblank();

        Atari2600.setMissile0Position((byte)0, (byte)100);
        Atari2600.setMissile1Position((byte)0, (byte)100);
        Atari2600.setColorPlayfield(0x02);
        Atari2600.setColorPlayer0(0x8);
        Atari2600.setColorPlayer1(0x8);

        ship0_x = 63;
        shot0_y = 100;
        ship1_x = 65;
        shot1_y = 100;

        // score must be set each frame
        Atari2600.setScore0((byte)score0);
        Atari2600.setScore1((byte)score1);

        // choose game type
        if (((frame & 31) == 31) && ((switches & 2) == 0))
        {
          game++;
          game &= 7;
          score0 = game + 1;
          score1 = 11;
        }

        Memory.write8(0x05, (byte)game_params[game]);

        // start game
        if ((switches & 1) == 0)
        {
          score0 = 0;
          score1 = 0;
//joe
          shot1_offset = (3 << (game_params[game] >> 4)) >> 1;
          //shot1_offset = (2 << game_params[game] >> 4);
          Atari2600.setColorPlayfield(0x06);
          Atari2600.setColorPlayer0(0x88);
          Atari2600.setColorPlayer1(0x38);
          mode = 1;
        }

        // set horizontal positions
        Atari2600.setPlayer0Position((byte)ship0_x, (byte)ship0_y);
        Atari2600.setPlayer1Position((byte)(ship1_x - (offset[game] >> 1)),
                                     (byte)ship1_y);
        Atari2600.waitVblank();
        Atari2600.drawScreen();
        Atari2600.startOverscan();
        Atari2600.waitOverscan();
      }
      else
      {
        // game
        Atari2600.startVblank();

        // score must be set each frame
        Atari2600.setScore0((byte)score0);
        Atari2600.setScore1((byte)score1);

        if ((ship0_hit == 0) && (ship1_hit == 0))
        {
          // move player
          if (Joystick.isRight(0))
          {
            if (ship0_x < (right - 4))
            {
              ship0_x++;
            }
          }
          else if (Joystick.isLeft(0))
          {
            if (ship0_x > (left + 4))
            {
              ship0_x--;
            }
          }

          // fire shot
          if (Joystick.isButtonDown_0(0) && shot0_y == 100)
          {
            shot0_y = ship0_y;
            Atari2600.setAudioControl0((byte)0b0100);
            Atari2600.setAudioVolume0((byte)15);
          }

          // move enemy
          if ((frame & 3) == 3)
          {
            dir = (ship0_x - (ship1_x + (offset[game] >> 1))) >> 2;
            if ((dir < 0) || (ship1_x <= (right - offset[game])))
            {
              ship1_x += dir;
              if (ship1_x < left)
                ship1_x = left;
            }
          }
        }
        else
        {
          // animate players when hit
          if (ship0_hit > 0)
          {
            Atari2600.setColorPlayer0(0x88 + ship0_hit);
            ship0_hit--;
            if (ship0_hit == 0)
              Atari2600.setAudioVolume1((byte)0);
          }

          if (ship1_hit > 0)
          {
            Atari2600.setColorPlayer1(0x38 + ship1_hit);
            ship1_hit--;
            if (ship1_hit == 0)
              Atari2600.setAudioVolume0((byte)0);
          }
        }

        // set horizontal positions
        Atari2600.setPlayer0Position((byte)ship0_x, (byte)ship0_y);
        Atari2600.setPlayer1Position((byte)ship1_x, (byte)ship1_y);
        Atari2600.setMissile0Position((byte)(ship0_x + 3), (byte)shot0_y);
        Atari2600.setMissile1Position((byte)(ship1_x + shot1_offset),
                                      (byte)shot1_y);

        Atari2600.waitVblank();
        Atari2600.drawScreen();
        Atari2600.startOverscan();

        // playfield hit
        if (Atari2600.isCollisionMissile0Playfield())
        {
          shot0_y = 100;
          Atari2600.setAudioVolume0((byte)0);
        }

        // player hit
        if (Atari2600.isCollisionMissile1Player0())
        {
          ship0_hit = 40;
          shot0_y = 100;
          shot1_y = 100;
          Atari2600.setAudioControl1((byte)0b1000);
          Atari2600.setAudioFrequency1((byte)22);
          Atari2600.setAudioVolume0((byte)0);
          Atari2600.setAudioVolume1((byte)15);
          score1++;
        }

        // enemy hit
        if (Atari2600.isCollisionMissile0Player1())
        {
          ship1_hit = 40;
          shot0_y = 100;
          shot1_y = 100;
          Atari2600.setAudioControl0((byte)0b1000);
          Atari2600.setAudioFrequency0((byte)16);
          Atari2600.setAudioVolume0((byte)15);
          Atari2600.setAudioVolume1((byte)0);
          score0++;
        }

        if ((frame & 1) == 1)
        {
          // move player shot
          if (shot0_y < 100)
          {
            Atari2600.setAudioFrequency0((byte)((shot0_y >> 1) + 2));
            shot0_y--;
            if (shot0_y < ship1_y)
            {
              shot0_y = 100;
              Atari2600.setAudioVolume0((byte)0);
            }
          }

          // move enemy shot
          if (shot1_y < 100)
          {
            Atari2600.setAudioFrequency1((byte)(shot1_y >> 1));
            shot1_y++;
            if (game > 2)
              shot1_y++;
            if (game > 5)
              shot1_y++;
            if (shot1_y > ship0_y + 4)
            {
              shot1_y = 100;
              Atari2600.setAudioVolume1((byte)0);
            }
          }
        }

        // enemy shoots at random
        rnd -= 77;
        rnd &= 127;

        if (((rnd ^ ship0_x) < ship1_y) && (shot1_y == 100))
        {
          shot1_y = ship1_y + 4;
          Atari2600.setAudioControl1((byte)0b0001);
          Atari2600.setAudioVolume1((byte)15);
        }

        // end game if max score reached or switches pressed
        if ((score0 >= 10) || (score1 >= 10) | ((switches & 2) == 0))
        {
          Atari2600.setAudioVolume0((byte)0);
          Atari2600.setAudioVolume1((byte)0);
          mode = 0;
        }

        Atari2600.waitOverscan();
      }

      frame++;
    }
  }
}

