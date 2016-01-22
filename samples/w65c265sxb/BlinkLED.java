import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.W65C265SXB;

public class BlinkLED
{
  public static void main()
  {
    int i;

    W65C265SXB.putChar('\n');
    Memory.write8(0xdf24, (byte)1);

    while(true)
    {
      W65C265SXB.putString("on\n");
      Memory.write8(0xdf20, (byte)1);
      
      for(i = 0; i < 30000; i++)
      {
        // wait
      }

      W65C265SXB.putString("off\n");
      Memory.write8(0xdf20, (byte)0);
      
      for(i = 0; i < 30000; i++)
      {
        // wait
      }
    }
  }
}

