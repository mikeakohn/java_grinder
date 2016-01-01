
import net.mikekohn.java_grinder.IOPort5;
import net.mikekohn.java_grinder.IOPort6;

public class LedBlink
{
  static public void main(String args[])
  {
    int n;

    IOPort5.setPinsAsOutput(0xff);
    IOPort6.setPinsAsOutput(0xff);
    //IOPort5.setPinsValue(0x00);
    //IOPort6.setPinsValue(0x00);

    while(true)
    {
      IOPort5.setPinsValue(0x01);
      IOPort6.setPinsValue(0x00);
      for (n = 1000000; n > 0; n--);

      IOPort5.setPinsValue(0x00);
      IOPort6.setPinsValue(0x40);
      for (n = 1000000; n > 0; n--);
    }
  }
}


