import net.mikekohn.java_grinder.IOPort4;
import net.mikekohn.java_grinder.W65C265SXB;

public class BlinkLED
{
  public static void main()
  {
    int i;

    W65C265SXB.putChar('\n');
    W65C265SXB.putInt(-32767);
    W65C265SXB.putChar('\n');
    W65C265SXB.putInt(367);
    W65C265SXB.putChar('\n');
    W65C265SXB.putInt(12345);
    W65C265SXB.putChar('\n');

    W65C265SXB.putChar('\n');
    IOPort4.setPinsAsOutput(255);

    while(true)
    {
      W65C265SXB.putString("on\n");
      IOPort4.setPinHigh(0);
      
      for(i = 0; i < 30000; i++)
      {
        // delay
      }

      W65C265SXB.putString("off\n");
      IOPort4.setPinLow(0);
      
      for(i = 0; i < 30000; i++)
      {
        // delay
      }
    }
  }
}

