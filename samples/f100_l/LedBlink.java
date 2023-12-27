
import net.mikekohn.java_grinder.IOPort0;

public class LedBlink
{
  static int counter = 1;

  public static void main(String[] args)
  {
    int b;

    while (true)
    {
      IOPort0.setPinsValue(0x01);
      for (counter = 0; counter < 10000; counter++);

      IOPort0.setPinsValue(0x00);
      for (b = 0; b < 10000; b++);
    }
  }
}

