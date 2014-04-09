
import net.mikekohn.java_grinder.TI84C;

public class TI84CDemo
{
  static String hello = "Hello World";

  static public void main(String args[])
  {
    TI84C.clearScreen();
    TI84C.putc('H');
    TI84C.putc('e');
    TI84C.putc('l');
    TI84C.putc('l');
    TI84C.putc('o');
    //TI84C.print(hello);
    TI84C.drawLine(1, 2, 50, 50, TI84C.OPERATION_DRAW_PIXEL);

    //while(true);
  }
}


