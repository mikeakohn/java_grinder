
public class Test
{
  static int glo = 10;

  public static int add_nums(int a, int b)
  {
    int c;
    c = a + b;

    return c;
  }

  public static int sub_nums(int a, int b)
  {
    int c;
    c = a - b;

    return c;
  }

  public static int mul_nums(int a, int b)
  {
    int c;
    c = a * b;

    return c;
  }

  public static int div_nums(int a, int b)
  {
    int c;
    c = a / b;

    return c;
  }

  public static int mod_nums(int a, int b)
  {
    int c;
    c = a % b;

    return c;
  }

  public static int shift_nums(int a, int b)
  {
    int c;

    c = a << b;

    return c;
  }

  public static int loop_test(int a, int b)
  {
    int c,d;

    d = 0;

    for (c = 0; c < b; c++)
    {
      d = d + a;
    }

    return d;
  }

  public static void put_static(int a)
  {
    glo = a;
  }

  public static int get_static()
  {
    return glo;
  }

  public static int method_call_test(int a, int b)
  {
    return add_nums(a, b) + sub_nums(a, b);
  }
}

