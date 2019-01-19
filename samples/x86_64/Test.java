
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

  public static int long_method(int a, int b, int c, int d, int e, int f)
  {
    return a + b + c + d + e + f;
  }

  public static int method_call_long_test(int a)
  {
    return long_method(a, 1, 2, 3, 4, 5);
  }

  public static int add_array(byte[] a)
  {
    int i,len;
    int total = 0;

    len = a.length;

    for (i = 0; i < len; i++)
    {
      total += a[i];
    }

    return total;
  }

  public static void fill_array(byte[] a)
  {
    int i,len;

    len = a.length;

    for (i = 0; i < len; i++)
    {
      a[i] = (byte)(10 + i);
    }
  }

  public static int add_array(short[] a)
  {
    int i,len;
    int total = 0;

    len = a.length;

    for (i = 0; i < len; i++)
    {
      total += a[i];
    }

    return total;
  }

  public static void fill_array(short[] a)
  {
    int i,len;

    len = a.length;

    for (i = 0; i < len; i++)
    {
      a[i] = (short)(100 + i);
    }
  }

  public static int add_array(int[] a)
  {
    int i,len;
    int total = 0;

    len = a.length;

    for (i = 0; i < len; i++)
    {
      total += a[i];
    }

    return total;
  }

  public static void fill_array(int[] a)
  {
    int i,len;

    len = a.length;

    for (i = 0; i < len; i++)
    {
      a[i] = 1000 + i;
    }
  }
}

