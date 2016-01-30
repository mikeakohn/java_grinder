
import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.IOPort0;
import net.mikekohn.java_grinder.Propeller;

public class JavaPropellerDemo
{
  static int count;

  static public void main(String args[])
  {
/*
    for (n = 5; n != 0; n--)
    {
      Propeller.stopCog(n);
    }
*/

    IOPort0.setPinsAsOutput(1 << 21);

    count = CPU.getCycleCount();
 
    while(true)
    {
      IOPort0.setPinsHigh(1 << 21);
      count = Propeller.waitCount(count, 1000);
      IOPort0.setPinsLow(1 << 21);
      count = Propeller.waitCount(count, 1000);
    }
  }
}

