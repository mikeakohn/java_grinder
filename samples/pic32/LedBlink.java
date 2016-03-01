
import net.mikekohn.java_grinder.IOPort5;
import net.mikekohn.java_grinder.IOPort6;

public class LedBlink
{
  public static void pause()
  {
    int n;

    //for (n = 1000000; n > 0; n--);
    for (n = 0; n < 1000000; n++);
  }

  public static int sub(int a, int b)
  {
    return a - b;
  }

  public static void main(String args[])
  {
    int n;

    //n = sub(1,2);

    IOPort5.setPinsAsOutput(0xff);
    IOPort6.setPinsAsOutput(0xff);

    //IOPort5.setPinsValue(0x01);
    //IOPort6.setPinsValue(0x40);

    while(true)
    {
      IOPort5.setPinsValue(0x01);
      IOPort6.setPinsValue(0x00);
      pause();
      //for (n = 1000000; n > 0; n--);

      IOPort5.setPinsValue(0x00);
      IOPort6.setPinsValue(0x40);
      pause();
      //for (n = 1000000; n > 0; n--);
    }
  }
}


