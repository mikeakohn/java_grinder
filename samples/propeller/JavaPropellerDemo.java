
import net.mikekohn.java_grinder.CPU;
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

    count = CPU.getCycleCount();
 
    while(true)
    {
      count = Propeller.waitCount(count, 1000);
    }
  }
}

