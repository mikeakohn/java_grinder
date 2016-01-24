// blinks LED on P40 if button on P41 is pressed

import net.mikekohn.java_grinder.IOPort4;
import net.mikekohn.java_grinder.W65C265SXB;

public class BlinkLED
{
  public static void main()
  {
    int i;
    int temp = 5;

    IOPort4.setPinsAsOutput(0b11111111);
    IOPort4.setPinsValue(0);

    IOPort4.setPinsAsInput(0b00000010);

    W65C265SXB.putString("\nWelcome to the LED Blinker!\n");
    W65C265SXB.putString("(press Q to quit.)\n");

    while(true)
    {
      if(W65C265SXB.getChar() == 'Q')
        break;

      if(IOPort4.getPortInputValue() == 0)
      {
        IOPort4.setPinHigh(0);
      
        for(i = 0; i < 3000; i++)
        {
          // delay
        }

        IOPort4.setPinLow(0);
      
        for(i = 0; i < 3000; i++)
        {
          // delay
        }
      }
    }
  }
}

