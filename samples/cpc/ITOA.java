import net.mikekohn.java_grinder.CPC;

public class ITOA
{
  public static String convert(int x, int base)
  {
    boolean negative = false;
    String s = "";
    if (x == 0) return "0";
    negative = (x < 0);
    if (negative) x = -1 * x;
    while (x != 0) {
      s = (x % base) + s; // add char to front of s
      x = x / base; // integer division gives quotient
    }
    if (negative)
      s = "-" + s;
    return s;
  } // convert

  public static void main(String[] args) // test
  {
  int x = 16;
  int i; 
  char c;
  String message;
    
  CPC.screen(1);
  
  int base = 2;
  message = convert(x, base);
    
  for (i = 0; i < message.length(); i++)
  {
    c = message.charAt(i);
    CPC.putChar(c);
  }
  c=CPC.readChar();
    
  base = 10;
  message = convert(x, base);
  for (i = 0; i < message.length(); i++)
  {
    c = message.charAt(i);
    CPC.putChar(c);
  }
   
  }
} // end class itoa
