
import net.mikekohn.java_grinder.Intellivision;

public class IntellivisionDemo
{
  static int a = 0;

  public static void main(String[] args)
  {
    Intellivision.setColorStack(0, Intellivision.COLOR_DARK_GREEN);
    Intellivision.setColorStack(1, Intellivision.COLOR_BLACK);
    Intellivision.setColorStack(2, Intellivision.COLOR_TAN);
    Intellivision.setColorStack(3, Intellivision.COLOR_PURPLE);

    Intellivision.plot((45 << 3) | Intellivision.COLOR_WHITE | Intellivision.COLOR_STACK_ADVANCE, 0);
    Intellivision.plot((41 << 3) | Intellivision.COLOR_WHITE | Intellivision.COLOR_STACK_ADVANCE, 1);
    Intellivision.plot((43 << 3) | Intellivision.COLOR_WHITE | Intellivision.COLOR_STACK_ADVANCE, 2);
    Intellivision.plot((37 << 3) | Intellivision.COLOR_WHITE | Intellivision.COLOR_STACK_ADVANCE, 3);

    Intellivision.plot((43 << 3) | Intellivision.COLOR_WHITE, 5);
    Intellivision.plot((47 << 3) | Intellivision.COLOR_WHITE, 6);
    Intellivision.plot((40 << 3) | Intellivision.COLOR_WHITE, 7);
    Intellivision.plot((46 << 3) | Intellivision.COLOR_WHITE, 8);

    Intellivision.plot((1 << 3) | Intellivision.COLOR_WHITE, 9);

    while (true)
    {
      Intellivision.plot((0x21 << 3) | Intellivision.COLOR_WHITE, 40);

      for (a = 0; a < 60; a++)
      {
        Intellivision.waitForVerticalBlank();
      }

      Intellivision.plot((0x3a << 3) | Intellivision.COLOR_BLUE, 40);

      for (a = 0; a < 60; a++)
      {
        Intellivision.waitForVerticalBlank();
      }
    }
  }
}

