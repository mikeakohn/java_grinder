
import net.mikekohn.java_grinder.IOPort6;
import net.mikekohn.java_grinder.IOPort7;

public class LedBlink
{
  static public void main(String args[])
  {
    int n;

    IOPort6.setPinsAsOutput(0x00);
    IOPort7.setPinsAsOutput(0x00);
    IOPort6.setPinsValue(0x01);
    IOPort7.setPinsValue(0x40);

    while(true);
  }
}


