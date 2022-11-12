import net.mikekohn.java_grinder.SXB;
//import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.Memory;

public class BlinkLED
{
  public static void wait(int time)
  {
    int i;

    for(i = 0; i < time; i++)
    {
      // delay;
    }
  }

  public static void main()
  {
    int i;

    SXB.putString("\nWelcome to the LED Blinker!\n");


    while(true)
    {
      Memory.write8(0xdf23, (byte)4);
      SXB.putString("Off\n");
      wait(10000);

/*
      for(i = 0; i < time; i++)
      {
        // delay;
      }
*/

      Memory.write8(0xdf23, (byte)0);
      SXB.putString("On\n");
      wait(10000);
/*
      for(i = 0; i < time; i++)
      {
        // delay;
      }
*/
    }

  }
}

