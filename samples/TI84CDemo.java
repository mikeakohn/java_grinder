
import net.mikekohn.java_grinder.TI84C;

public class TI84CDemo
{
  static String hello = "Hello World\0";

  static public void main(String args[])
  {
    TI84C.clearScreen();
    TI84C.putc('H');
    TI84C.putc('e');
    TI84C.putc('l');
    TI84C.putc('l');
    TI84C.putc('o');
    //TI84C.print(hello);

    //while(true);
  }
}


