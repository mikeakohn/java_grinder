
import net.mikekohn.java_grinder.Intellivision;

public class Breakout
{
  static final int STATE_GAME_OVER = 0;
  static final int STATE_PLAYING = 1;

  static String author = " MIKE KOHN";
  static String java_grinder = " JAVA GRINDER";
  static String game_over = "GAME OVER";

  //static byte[] tones = { 169, 190, 213, 254 };
  static byte[] tones = { 0, -87, -66, -43, -2 };

  static int player_x = 9;
  static int ball_x = 10;
  static int ball_y = 12;
  static int ball_dx = 1;
  static int ball_dy = 1;
  static int count = 0;
  static int state = STATE_GAME_OVER;
  static int bricks_left = 0;
  static int sound_on = 0;

  static void clearScreen()
  {
    int a, color = 0x200;

    for (a = 0; a < 20 * 12; a++)
    {
      Intellivision.plot(0, a);
    }
  }

  static void displayText(byte[] text, int x, int y, int color)
  {
    int n, a;

    for (n = 0; n < text.length; n++)
    {
      int c = text[n] - 32;

      Intellivision.plot((c << 3) | color, x, y);

      x++;

      if (x >= 20) { break; }
    }
  }

  static void scrollText(String text, int x)
  {
    int n, r;

    displayText(text.getBytes(), x, 4, Intellivision.COLOR_TAN);
    pause(60);

    for (n = x; n < 20; n++)
    {
      for (r = n; r < n + text.length(); r++)
      {
        Intellivision.plot(' ', r, 4);
      }

      Intellivision.setHorizontalDelay(0);

      displayText(text.getBytes(), n, 4, Intellivision.COLOR_WHITE);

      for (r = 0; r < 8; r++)
      {
        Intellivision.setHorizontalDelay(r);
        pause(1);
      }
    }

    Intellivision.setHorizontalDelay(0);
  }

  static void pause(int value)
  {
    int n;

    for (n = 0; n < value; n++)
    {
      Intellivision.waitForVerticalBlank();
    }
  }

  static void drawBricks()
  {
    int x, y, c = 1;

    for (y = 1; y < 5; y++)
    {
      for (x = 0; x < 20; x++)
      {
        Intellivision.plot((0x5f << 3) | c, x, y);
      }

      c++;
    }

    bricks_left = 80;
    ball_x = 10;
    ball_y = 12;
  }

  static void drawPlayer()
  {
    Intellivision.plot((0xca << 3) | Intellivision.COLOR_WHITE, player_x, 11);
    Intellivision.plot((0xca << 3) | Intellivision.COLOR_WHITE, player_x + 1, 11);
  }

  static void drawBall()
  {
    int x = ball_x >> 1;
    int y = ball_y >> 1;
    int x0 = ball_x & 1;
    int y0 = ball_y & 1;
    int c = 0xc4 + x0;

    c = c + (y0 << 1);

    Intellivision.plot((c << 3) | Intellivision.COLOR_WHITE, x, y);
  }

  static void checkBrickCollision()
  {
    int x = ball_x >> 1;
    int y = ball_y >> 1;

    if (y < 5)
    {
      if (Intellivision.readDisplay(x, y) != 0)
      {
        Intellivision.setSoundType(0x08);
        Intellivision.setSoundChannelPeriod(0, tones[y] & 0xff);
        Intellivision.setSoundChannelVolume(0, 0x0f);
        sound_on = 15;

        if (ball_dy == -1)
        {
          ball_dy = 1;
        }
          else
        {
          ball_dx = -ball_dx;
        }

        bricks_left--;
      }
    }
      else
    if (bricks_left == 0)
    {
      drawBricks();
    }
  }

  static boolean checkPlayerCollision(int x, int y)
  {
    if (ball_y == 21)
    {
      boolean bounce = false;

      if (x == player_x || x == player_x - 1)
      {
        bounce = true;
        ball_dx = -1;
      }
        else
      if (x == player_x + 1 || x == player_x + 2)
      {
        bounce = true;
        ball_dx = 1;
      }

      if (bounce)
      {
        Intellivision.setSoundType(0x08);
        Intellivision.setSoundChannelPeriod(0, 508);
        Intellivision.setSoundChannelVolume(0, 0x0f);
        sound_on = 15;

        ball_dy = -1;
      }
    }
      else
    if (ball_y == 22)
    {
      Intellivision.setSoundEnvelopePeriod(1000);
      Intellivision.setSoundEnvelopeType(1);
      Intellivision.setSoundType(0);

      Intellivision.setSoundChannelPeriod(0, 5000);
      Intellivision.setSoundChannelVolume(0, 15);

      sound_on = 15;

      setGameOver();
      return true;
    }

    return false;
  }

  static void moveBall()
  {
    count++;

    if ((count & 3) != 0) { return; }

    int x = ball_x >> 1;
    int y = ball_y >> 1;

    Intellivision.plot(0, x, y);

    ball_x += ball_dx;
    ball_y += ball_dy;

    if (ball_dy == 1)
    {
      if (checkPlayerCollision(x, y)) { return; }
    }

    if (ball_x <= 0)  { ball_x =  0; ball_dx =  1; }
    if (ball_x >= 39) { ball_x = 39; ball_dx = -1; }

    if (ball_y <= 2)  { ball_y =  2; ball_dy =  1; }
    if (ball_y >= 23) { ball_y = 23; ball_dy = -1; }

    checkBrickCollision();
  }

  static void movePlayer()
  {
    int hand_controller = Intellivision.getControllerLeft();

    if ((count & 1) != 0) { return; }

    if (hand_controller == 0x0a ||
        hand_controller == 0x60 ||
        hand_controller == 0xc0)
    {
      if (state == STATE_GAME_OVER)
      {
        reset();
      }
    }
      else
    if (hand_controller == 0x08)
    {
      Intellivision.plot(Intellivision.COLOR_WHITE, player_x, 11);
      Intellivision.plot(Intellivision.COLOR_WHITE, player_x + 1, 11);

      player_x -= 1;

      if (player_x == -1) { player_x = 0; }
    }
      else
    if (hand_controller == 0x02)
    {
      Intellivision.plot(Intellivision.COLOR_WHITE, player_x, 11);
      Intellivision.plot(Intellivision.COLOR_WHITE, player_x + 1, 11);

      player_x += 1;

      if (player_x >= 18) { player_x = 18; }
    }
  }

  static void reset()
  {
    for (int n = 6 + (8 * 20); n < 15 + (8 * 20); n++)
    {
      Intellivision.plot(Intellivision.COLOR_WHITE, n);
      drawBricks();

      ball_x = 10;
      ball_y = 12;
    }

    state = STATE_PLAYING;
  }

  static void setGameOver()
  {
    state = STATE_GAME_OVER;
    displayText(game_over.getBytes(), 6, 8, Intellivision.COLOR_GREEN);
  }

  public static void main(String args[])
  {
    int n, x;

    pause(120);

    clearScreen();
    Intellivision.setVideoMode(Intellivision.VIDEO_MODE_FOREGROUND_BACKGROUND);

    pause(60);

    scrollText(author, 5);
    scrollText(java_grinder, 3);

    Intellivision.setColorStack(0, Intellivision.COLOR_BLACK);
    Intellivision.setColorStack(1, Intellivision.COLOR_BLACK);
    Intellivision.setColorStack(2, Intellivision.COLOR_BLACK);
    Intellivision.setColorStack(3, Intellivision.COLOR_BLACK);
    Intellivision.setVideoMode(Intellivision.VIDEO_MODE_COLOR_STACK);

    Intellivision.setSoundEnvelopePeriod(40000);
    Intellivision.setSoundEnvelopeType(0);

    clearScreen();
    drawBricks();
    setGameOver();

    while (true)
    {
      drawPlayer();

      if (state == STATE_PLAYING) { drawBall(); }

      Intellivision.waitForVerticalBlank();

      if (state == STATE_PLAYING) { moveBall(); }
      movePlayer();

      if (sound_on != 0)
      {
        sound_on--;
      }
        else
      {
        Intellivision.setSoundChannelVolume(0, 0);
      }
    }
  }
}

