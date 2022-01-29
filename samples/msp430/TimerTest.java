import net.mikekohn.java_grinder.Timer;
import net.mikekohn.java_grinder.TimerListener;
import net.mikekohn.java_grinder.IOPort0;

// Make LED's blink on the MSP430 Launchpad using timer interrupt.

public class TimerTest implements TimerListener
{
  static int led_value = 0x40;

  static public void main(String[] args)
  {
    IOPort0.setPinsAsOutput(0x41);
    Timer.setInterval(65000, 4);
    Timer.setListener(true);

    while(true);
  }

  public void timerInterrupt()
  {
    IOPort0.setPinsValue(led_value);
    led_value ^= 0x41;
  }
}


