
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
      for (counter = 0; counter < 30000; counter++);

      IOPort0.setPinsValue(0x00);
      for (b = 0; b < 30000; b++);

      b = getValue(1000, 5000);

      IOPort0.setPinsValue(0x01);
      delay(b);

      b = getValue(1000, 5000);

      IOPort0.setPinsValue(0x00);
      delay(b);
    }
  }
}

