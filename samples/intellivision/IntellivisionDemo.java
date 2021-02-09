
import net.mikekohn.java_grinder.Intellivision;

public class IntellivisionDemo
{
  static int a = 0;

  public static void main(String args[])
  {
    Intellivision.plot(45, 0);
    Intellivision.plot(41, 0);
    Intellivision.plot(43, 0);
    Intellivision.plot(37, 0);

    while (true)
    {
      a = a + 1;
    }
  }
}

