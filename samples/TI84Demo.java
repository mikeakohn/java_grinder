
import net.mikekohn.java_grinder.TI84;

public class TI84Demo
{
  static String hello = "Hello World";

  static public void main(String args[])
  {
    int n;

    TI84.clearScreen();
    TI84.putc('H');
    TI84.putc('e');
    TI84.putc('l');
    TI84.putc('l');
    TI84.putc('o');
    //TI84.print(hello);

    for (n = 2; n < 50; n++)
    {
       TI84.drawLine(1, 2, n, 50, TI84.OPERATION_DRAW_PIXEL);
       TI84.drawLine(1, 2, n, 50, TI84.OPERATION_ERASE_PIXEL);
    }

    TI84.drawLine(1, 2, 50, 50, TI84.OPERATION_DRAW_PIXEL);

    //while(true);
  }
}


