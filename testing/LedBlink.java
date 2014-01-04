
import net.mikekohn.java_grinder.IOPort0;

public class LedBlink
{
  static public void main(String args[])
  {
    int n;

    IOPort0.setPinsAsOutput(0x3);

    while(true)
    {
      IOPort0.setPinHigh(2);
      IOPort0.setPinLow(1);
      for (n = 0; n < 16384; n++);
      IOPort0.setPinHigh(1);
      IOPort0.setPinLow(2);
      for (n = 0; n < 16384; n++);
    }
  }
}


