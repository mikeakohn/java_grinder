
import net.mikekohn.java_grinder.IOPort1;
import net.mikekohn.java_grinder.IOPort2;

public class SleepyBeeDemo
{
  static public int subNums(int a, int b)
  {
    int c = a - b;

    return c;
  }

  static public void delay()
  {
    int t = subNums(22000, 2000);

    for (int n = 0; n < t; n++);
  }

  static public void main(String[] args)
  {
    int n;

    // SPI for LCD is P1.0 (CLK), P1.1 (MOSI), P1.5 (CS)
    // Doing a software SPI here because the LCD expects the data
    // to be in reverse order (LSb first instead of MSb first).
    IOPort1.setPinsAsOutput(0x00);

    // RGB LED is P2.0, P2.1, P2.2.
    // EFM_DISP_ENABLE for LCD display is P2.6.
    IOPort2.setPinsAsOutput(0x47);
    IOPort2.setPinHigh(6);

    while (true)
    {
      IOPort2.setPinHigh(0);
      IOPort2.setPinLow(1);
      IOPort2.setPinHigh(2);

      delay();

      IOPort2.setPinLow(0);
      IOPort2.setPinHigh(1);
      IOPort2.setPinHigh(2);

      delay();
    }
  }
}

