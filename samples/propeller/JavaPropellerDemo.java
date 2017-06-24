import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.IOPort0;
import net.mikekohn.java_grinder.Propeller;

public class JavaPropellerDemo
{
  static int count,delay;
  static int led;
  static int n;

  static public void main(String args[])
  {
    led = 1 << 16;

    IOPort0.setPinsAsOutput(0xff << 16);
    IOPort0.setPinsHigh(1 << 21);
    IOPort0.setPinsHigh(1 << 18);

    count = CPU.getCycleCount() + 0xffff;

    while(true)
    {
      for (n = 7; n != 0; n--)
      {
        for (delay = 0xffff; delay != 0; delay--);
        IOPort0.setPinsValue(led);
        led = led << 1;
      }

      for (n = 7; n != 0; n--)
      {
        for (delay = 0xffff; delay != 0; delay--);
        IOPort0.setPinsValue(led);
        led = led >> 1;
      }
    }
  }
}

