import net.mikekohn.java_grinder.CPC;
// result=67

public class DivCPC
{
  static public int div_nums(int a, int b)
  {
    return a / b;
  }

  static public void main(String args[])
  {
    int help;
    
    help = div_nums(27000, 400);
    
    char c = (char) help; 
    
    CPC.putChar(c); //should be a "C" 
    
    c=CPC.readChar();
    
  }
}


