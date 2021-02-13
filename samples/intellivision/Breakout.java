
import net.mikekohn.java_grinder.Intellivision;

public class Breakout
{
  static String author = "Mike Kohn";
  static String java_grinder = "Java Grinder";
  static String breakout = "BREAKOUT";

  static public void clearScreen()
  {
    int a;

    for (a = 0; a < 20 * 12; a++)
    {
      Intellivision.plot(0, a);
    }
  }

  //static public void displayText(byte[] text, int x, int y, int color)
  static public void displayText(byte[] text, int x, int y, int color)
  {
    int n, a;

    for (n = 0; n < text.length; n++)
    {
      int c = text[n];

      if (c >= 'A' || c <= 'Z')
      {
        c = c - 'A' + 0x21;
        Intellivision.plot((c << 3) | color, x, y);
      }
        else
      if (c >= 'a' || c <= 'a')
      {
        c = c - 'a' + 0x41;
        Intellivision.plot((c << 3) | color, x, y);
      }
        else
      if (c >= '0' || c <= '9')
      {
        c = c - '0' + 0x11;
        Intellivision.plot((c << 3) | color, x, y);
      }
        else
      {
        Intellivision.plot(color, x, y);
      }

      x++;
    }
  }

  public static void main(String args[])
  {
    int a;

    // Two second pause, then clear the screen.
    for (a = 0; a < 120; a++)
    {
      Intellivision.waitForVerticalBlank();
    }

    clearScreen();
    displayText(author.getBytes(), 6, 4, Intellivision.COLOR_WHITE);

    while (true)
    {
      Intellivision.plot((0x21 << 3) | Intellivision.COLOR_WHITE, 2, 4);

      for (a = 0; a < 60; a++)
      {
        Intellivision.waitForVerticalBlank();
      }

      Intellivision.plot((0x3a << 3) | Intellivision.COLOR_BLUE, 2, 4);

      for (a = 0; a < 60; a++)
      {
        Intellivision.waitForVerticalBlank();
      }
    }
  }
}

