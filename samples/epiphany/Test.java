
import net.mikekohn.java_grinder.Parallella;

public class Test
{
  public static void main(String args[])
  {
    int n;

    n = 0;

    while(true)
    {
      // Write the contents of n to shared address 0.
      Parallella.writeSharedRamInt(0, n);
    }
  }
}


