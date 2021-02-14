
import net.mikekohn.java_grinder.Intellivision;

public class Breakout
{
  static String author = " MIKE KOHN";
  static String java_grinder = " JAVA GRINDER";
  static String breakout = "BREAKOUT";

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

  public static void main(String args[])
  {
    int n, x;

    pause(120);

    clearScreen();
    Intellivision.setVideoMode(Intellivision.VIDEO_MODE_FOREGROUND_BACKGROUND);

    pause(60);

    scrollText(author, 5);
    scrollText(java_grinder, 3);

    while (true);
  }
}

