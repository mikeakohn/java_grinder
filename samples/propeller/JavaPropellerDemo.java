
import net.mikekohn.java_grinder.Propeller;

public class JavaPropellerDemo
{
  static int n;

  static public void main(String args[])
  {
    for (n = 5; n != 0; n--)
    {
      Propeller.stopCog(n);
    }

    while(true);
  }
}

