
import net.mikekohn.java_grinder.UART;
import net.mikekohn.java_grinder.UART1;
import net.mikekohn.java_grinder.IOPort3;
import net.mikekohn.java_grinder.ADC;

public class CuriosityADC
{
  //static byte[] temp = new byte[8];

  static public void main(String[] args)
  {
    IOPort3.setPinAsOutput(10);
    ADC.setChannel(0);
    ADC.enable();

    UART1.init(UART.BAUD_9600);

    int i = 0;

    while (true)
    {
      sendPrompt();

      int value = ADC.read();
      sendInt(value);

      if ((i & 1) == 0)
      {
        IOPort3.setPinHigh(10);
      }
        else
      {
        IOPort3.setPinLow(10);
      }

      delay();

      i++;
    }
  }

  static public void delay()
  {
    for (int m = 0; m < 100; m++)
    for (int k = 0; k < 32767; k++)
    {
    }
  }

  static public void sendChar(int value)
  {
    while (!UART1.isSendReady());
    UART1.send(value);
  }

  static public void sendString(String value)
  {
    for (int i = 0; i < value.length(); i++)
    {
      sendChar(value.charAt(i));
    }
  }

  static public void sendPrompt()
  {
    sendChar('\r');
    sendChar('\n');
    sendChar('>');
    sendChar(' ');
  }

  static public void sendDigits(int value)
  {
    if (value == 0) { return; }

    int digit = (value % 10) + '0';

    sendDigits(value / 10);
    sendChar(digit);
    //value = value / 10;
  }

  static public void sendInt(int value)
  {
    int length = 0;

    if (value == 0)
    {
      sendChar('0');
      return;
    }

    sendChar('\r');
    sendChar('\n');

    sendDigits(value);

/*
    while (value > 0)
    {
      //int digit = (value % 10) + '0';
      int digit = 'x';
      temp[length++] = (byte)digit;
      value = value / 10;

      if (length == 6) { break; }
    }

    while (length == 0)
    {
      length--;
      sendChar(temp[length]);
    }
*/
  }
}

