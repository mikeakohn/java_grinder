
import net.mikekohn.java_grinder.IOPort0;

public class FlipBlink
{
  static public int sub(int a, int b)
  {
     a = a - 1;
     return a - b;
  }

  static public void main(String[] args)
  {
    int n;
    int i;

    IOPort0.setPinsAsOutput(0x1f | (1 << 26) | (1 << 27));
    IOPort0.setPinsHigh(1 << 26);

    // This is pointless except for checking to make sure function
    // calls work.
    n = sub(300000, 1);

    while(true)
    {
      for (i = 0; i < n; i++);
      IOPort0.setPinsLow(1 << 26);
      IOPort0.setPinsHigh(1 << 27);

      for (i = 0; i < n; i++);
      IOPort0.setPinsHigh(1 << 26);
      IOPort0.setPinsLow(1 << 27);
    }
  }
}

