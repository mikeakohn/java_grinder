
import net.mikekohn.java_grinder.IOPort0;

public class LedBlink
{
  static public void main(String args[])
  {
    int n;

    IOPort0.setPinsAsOutput(0x7f);
    IOPort0.setPinsValue(0x01);

    while(true);
  }
}


