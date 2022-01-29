
import net.mikekohn.java_grinder.TI84;

public class TI84Demo
{
  static String hello = "Hello World";
  static int blah = 7;

  static public void main(String[] args)
  {
    int n,x,y,ptr,pixels;

    //blah = Logo.logo_count;

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

    ptr = 0;
    for (y = 0; y < 64; y++)
    {
      for (x = 0; x < 96 / 8; x++)
      {
        //pixels = Logo.logo[ptr++];
        pixels = 0;

        for (n = 0; n < 8; n++)
        {
          if ((pixels & 0x80) == 1)
          {
            TI84.drawLine(x,y,x,y,TI84.OPERATION_DRAW_PIXEL);
          }
          else
          {
            TI84.drawLine(x,y,x,y,TI84.OPERATION_ERASE_PIXEL);
          }
        }
      }
    }

    //while(true);
  }
}


