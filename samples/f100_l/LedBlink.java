
import net.mikekohn.java_grinder.IOPort0;

public class LedBlink
{
  static int counter = 1;

  public static void delay(int value)
  {
    for (int b = 0; b < value; b++);
  }

  public static int getValue(int a, int b)
  {
    return a + b;
  }

  public static void main(String[] args)
  {
    int b;

    while (true)
    {
      IOPort0.setPinsValue(0x01);
      for (counter = 0; counter < 3000; counter++);

      IOPort0.setPinsValue(0x00);
      for (b = 0; b < 3000; b++);

      b = getValue(10000, 20000);

      delay(b);
      IOPort0.setPinsValue(0x01);

      b = getValue(10000, 10000);

      delay(b);
      IOPort0.setPinsValue(0x00);
    }
  }
}

