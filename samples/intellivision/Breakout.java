
import net.mikekohn.java_grinder.Intellivision;

public class Breakout
{
  static String author = " MIKE KOHN";
  static String java_grinder = " JAVA GRINDER";
  static String breakout = "BREAKOUT";

  static int player_x = 9;
  static int ball_x = 10;
  static int ball_y = 12;
  static int ball_dx = 1;
  static int ball_dy = 1;

  static public void clearScreen()
  {
    int a, color = 0x200;

    for (a = 0; a < 20 * 12; a++)
    {
      Intellivision.plot(0, a);
    }
  }

  static public void displayText(byte[] text, int x, int y, int color)
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

  public static void scrollText(String text, int x)
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

  public static void pause(int value)
  {
    int n;

    for (n = 0; n < value; n++)
    {
      Intellivision.waitForVerticalBlank();
    }
  }

  public static void drawAllBricks()
  {
    int x, y, c = 1;

    for (y = 2; y < 6; y++)
    {
      for (x = 0; x < 20; x++)
      {
        Intellivision.plot((0x5f << 3) | c, x, y);
      }

      c++;
    }
  }

  public static void drawPlayer()
  {
    Intellivision.plot((0xca << 3) | Intellivision.COLOR_WHITE, player_x, 11);
    Intellivision.plot((0xca << 3) | Intellivision.COLOR_WHITE, player_x + 1, 11);
  }

  public static void drawBall()
  {
    int x = ball_x >> 1;
    int y = ball_y >> 1;
    int x0 = ball_x & 1;
    int y0 = ball_y & 1;
    int c = 0xc4 + x0;

    c = c + (y0 << 1);

    Intellivision.plot((c << 3) | Intellivision.COLOR_WHITE, x, y);
  }

  public static void moveBall()
  {
    int x = ball_x >> 1;
    int y = ball_y >> 1;

    Intellivision.plot(Intellivision.COLOR_WHITE, x, y);

    ball_x += ball_dx;
    ball_y += ball_dy;

    if (ball_x <= 0)  { ball_x =  0; ball_dx =  1; }
    if (ball_x >= 39) { ball_x = 39; ball_dx = -1; }

    if (ball_y <= 2)  { ball_y =  2; ball_dy =  1; }
    if (ball_y >= 23) { ball_y = 23; ball_dy = -1; }
  }

  public static void movePlayer()
  {
    int hand_controller = Intellivision.getControllerLeft();

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

      if (player_x == 18) { player_x = 18; }
    }
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

    clearScreen();
    drawAllBricks();

    while (true)
    {
      drawPlayer();
      drawBall();

      Intellivision.waitForVerticalBlank();

      moveBall();
      movePlayer();
    }
  }
}

