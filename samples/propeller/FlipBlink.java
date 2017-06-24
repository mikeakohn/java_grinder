
import net.mikekohn.java_grinder.IOPort0;

public class FlipBlink
{
  static public void main(String args[])
  {
    int i;

    IOPort0.setPinsAsOutput(0x1f | (1 << 26) | (1 << 27));
    IOPort0.setPinsHigh(1 << 26);

    while(true)
    {
      for (i = 0; i < 300000; i++);
      IOPort0.setPinsLow(1 << 26);
      IOPort0.setPinsHigh(1 << 27);

      for (i = 0; i < 300000; i++);
      IOPort0.setPinsHigh(1 << 26);
      IOPort0.setPinsLow(1 << 27);
    }
  }
}

