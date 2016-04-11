
import net.mikekohn.java_grinder.CPC;

public class TestCPC
{
  public static void main(String args[])
  {
    String message = "Hello CPC from JAVA";
    int i,j;
    char c;
    CPC.screen(1);
    CPC.setTxtPen(3);
    for (i = 0; i < message.length(); i++)
    {
      c = message.charAt(i);
      CPC.putChar(c);
    }
    // Wait for Keypress and exit to BASIC
    c=CPC.readChar();
  }
}

