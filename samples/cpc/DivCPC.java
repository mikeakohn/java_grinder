import net.mikekohn.java_grinder.CPC;
// result=67

public class DivCPC
{
  static public int div_nums(int a, int b)
  {
    return a / b;
  }

  static public void main(String[] args)
  {
    int help;
    char c;
    
    String mes = "We divide 27000 / 4000 and we get \n";
    
    for (help=0;help<mes.length();help++)
    {
      c = mes.charAt(help);
      CPC.putChar(c);
    }
         
    CPC.printI(div_nums(27000, 400));
    
    c=CPC.readChar();
    
  }
}


