
public class StringTest
{
  static String string = "Just a test";
  static byte[] mem;

  static public void main(String[] args)
  {
  int len;
  int n;

    len = string.length();

    mem = new byte[len];

    for (n = 0; n < len; n++)
    {
      mem[n] = (byte)string.charAt(n);
    } 

    while(true);
  }
}


