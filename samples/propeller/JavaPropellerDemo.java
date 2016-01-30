
import net.mikekohn.java_grinder.Propeller;

public class JavaPropellerDemo
{
  static public void main(String args[])
  {
    int n;

    for (n = 0; n < 5; n++)
    {
      Propeller.stopCog(n);
    }

    while(true);
  }
}

