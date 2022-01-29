
// result=335

public class StringTest
{
  static String letters = "ABCDE";

  static int add_letters()
  {
    int len;
    int n,a;
    int total = 0;

    len = letters.length();

    for (n = 0; n < len; n++)
    {
      a = letters.charAt(n);
      total += a;
    }

    return total;
  }

  static public void main(String[] args)
  {
  int a;

    a = add_letters();
  }
}


