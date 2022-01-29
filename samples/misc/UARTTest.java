
import net.mikekohn.java_grinder.UART0;

public class UARTTest
{
  static void main(String[] args)
  {
    int c;

    UART0.init(UART0.BAUD_9600);

    while(true)
    {
      while(!UART0.isDataAvailable());

      c = UART0.read();

      while(!UART0.isSendReady());

      UART0.send(c);
    }
  }
}

