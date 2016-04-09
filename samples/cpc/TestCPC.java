
import net.mikekohn.java_grinder.CPC;

public class TestCPC
{
  public static void main(String args[])
  {
    int i;
 
    CPC.setTxtPen(2);

    String message = "Hello CPC from JAVA";

    CPC.screen(1);


    for (i = 0; i < message.length(); i++)
    {
      char c = message.charAt(i);

      CPC.putChar(c);
    }

    while (true);
  }
}

