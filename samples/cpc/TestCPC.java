import net.mikekohn.java_grinder.CPC;

public class TestCPC
{
  public static void main(String[] args)
  {
    String message = "Hello CPC from JAVA";
    int zahl = 6128;
    
    int i,j;
    char c;
    
    CPC.screen(1);
    CPC.setTxtPen(3);
    
    for (i = 0; i < message.length(); i++)
    {
      c = message.charAt(i);
      CPC.putChar(c);
    }
    
    c = '\n';
    CPC.putChar(c);
    CPC.putChar(c);
    CPC.setTxtPen(2);
    
    CPC.printI(zahl);
         
    // Wait for Keypress and exit to BASIC
    c=CPC.readChar();
    
    return;
  }
}

