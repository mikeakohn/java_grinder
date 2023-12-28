
import net.mikekohn.java_grinder.IOPort0;

public class Arrays
{
  static int[] data = { 10000, 20000, 3000, 30000 };

  public static void main(String[] args)
  {
    //     1      2    3
    int counter, next, n;

    while (true)
    {
      for (n = 0; n < data.length; n++)
      {
        next = data[n];

        IOPort0.setPinsValue(0x01);
        for (counter = 0; counter < next; counter++);

        IOPort0.setPinsValue(0x00);
        for (counter = 0; counter < next; counter++);
      }
    }
  }
}

