
import net.mikekohn.java_grinder.Intellivision;

public class IntellivisionDemo
{
  static int a = 0;

  public static void main(String args[])
  {
    Intellivision.plot((45 << 3) | 1, 0);
    Intellivision.plot((41 << 3) | 1, 1);
    Intellivision.plot((43 << 3) | 1, 2);
    Intellivision.plot((37 << 3) | 1, 3);

    Intellivision.plot((43 << 3) | 1, 5);
    Intellivision.plot((47 << 3) | 1, 6);
    Intellivision.plot((40 << 3) | 1, 7);
    Intellivision.plot((46 << 3) | 1, 8);

    Intellivision.plot((1 << 3) | 1, 9);

    while (true)
    {
      a = a + 1;
    }
  }
}

