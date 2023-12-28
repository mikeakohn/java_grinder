
import net.mikekohn.java_grinder.IOPort0;

public class Conditionals
{
  public static void main(String[] args)
  {
    int a, b;
    int counter = 0;

    IOPort0.setPinsValue(0x01);

    a = 5;
    b = 1000;

    // 1
    if (a < b)
    {
      counter++;
    }

    // 2
    if (a <= b)
    {
      counter++;
    }

    a = 1000;

    // 3
    if (a <= b)
    {
      counter++;
    }

    // 4
    if (a >= b)
    {
      counter++;
    }

    a = 2000;

    // 5
    if (a > b)
    {
      counter++;
    }

    // 6
    if (a >= b)
    {
      counter++;
    }

    a = -5;
    b = 1000;

    // 7
    if (a < b)
    {
      counter++;
    }

    // 8
    if (a <= b)
    {
      counter++;
    }

    a = -5;
    b = -4;

    // 9
    if (a < b)
    {
      counter++;
    }

    // 10 
    if (b > a)
    {
      counter++;
    }

    // 11 
    if (b >= a)
    {
      counter++;
    }

    if (counter != 11)
    {
      IOPort0.setPinsValue(0x00);
    }

    while (true)
    {
    }
  }
}

