
import net.mikekohn.java_grinder.Memory;

public class MethodCall
{
  static public void main(String args[])
  {
    int n;
    int a,b;

    a = 7;
    b = 11;
    n = add_nums(a,b) + 1;

    nothing(n,n);

    while(true);
  }

  static public int add_nums(int a, int b)
  {
    int n;

    n = a + b;

    return n;
  }

  static public void nothing(int a, int b)
  {
  }
}


