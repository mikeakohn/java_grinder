
import net.mikekohn.java_grinder.IOPort0;

public class LedBlink
{
  static public void main(String args[])
  {
    int n;

    IOPort0.setPinsAsOutput(0x3);

    while(true)
    {
      //setLeds(2,0);
      IOPort0.setPinsValue(2);
      //for (n = 0; n < 16738; n++);
      delay();
      //setLeds(1,0);
      IOPort0.setPinsValue(1);
      //for (n = 0; n < 16738; n++);
      delay();
    }
  }

  static public void setLeds(int n, int a)
  {
    IOPort0.setPinsValue(n);
  }

  // Since the default speed on the dsPIC I'm using is so much faster than
  // the MSP430 I'm using, this delay is for the dsPIC.
  static public void delay()
  {
    int a,b;

    for (b = 0; b < 10; b++)
    {
      for (a = 0; a < 16384; a++);
    }
  }
}


