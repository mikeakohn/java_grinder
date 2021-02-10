
import net.mikekohn.java_grinder.Intellivision;

public class IntellivisionDemo
{
  static int a = 0;

  public static void main(String args[])
  {
    Intellivision.plot((45 << 3) | Intellivision.COLOR_WHITE, 0);
    Intellivision.plot((41 << 3) | Intellivision.COLOR_WHITE, 1);
    Intellivision.plot((43 << 3) | Intellivision.COLOR_WHITE, 2);
    Intellivision.plot((37 << 3) | Intellivision.COLOR_WHITE, 3);

    Intellivision.plot((43 << 3) | Intellivision.COLOR_WHITE, 5);
    Intellivision.plot((47 << 3) | Intellivision.COLOR_WHITE, 6);
    Intellivision.plot((40 << 3) | Intellivision.COLOR_WHITE, 7);
    Intellivision.plot((46 << 3) | Intellivision.COLOR_WHITE, 8);

    Intellivision.plot((1 << 3) | Intellivision.COLOR_WHITE, 9);

    while (true)
    {
      a = a + 1;
    }
  }
}

