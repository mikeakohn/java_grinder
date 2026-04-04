
import net.mikekohn.java_grinder.IOPort3;
import net.mikekohn.java_grinder.Timer;
import net.mikekohn.java_grinder.TimerListener;

public class CuriosityTimer implements TimerListener
{
  static int interrupt_count = 0;

  static public void main(String[] args)
  {
    int value = 0;

    IOPort3.setPinAsOutput(10);
    IOPort3.setPinLow(10);

    Timer.setInterval(60000, 3);
    Timer.setListener(true);

    while (true);
  }

  public void timerInterrupt()
  {
    // Blink LED in timer interrupt.
    if ((interrupt_count & 1) == 0)
    {
      IOPort3.setPinHigh(10);
    }
      else
    {
      IOPort3.setPinLow(10);
    }

    interrupt_count += 1;
  }
}

