
public class StringParam
{
  static String letters = "ABCDE";

  static public int add_letters(String test_string)
  {
    int len;
    int n,a;
    int total = 0;

    len = test_string.length();

    for (n = 0; n < len; n++)
    {
      a = test_string.charAt(n);
      total += a;
    }

    return total;
  }

  static public void main(String args[])
  {
    int a;

    a = add_letters(letters);
  }
}


