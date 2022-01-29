
public class Testing
{
  static public int add_nums(int a, int b)
  {
    int n;
    for (n = 0; n < 10; n++)
    {
      a = a + b;
    }

    return a;
  }

  static public int sub_nums(int a, int b)
  {
    int n;
    for (n = 0; n < 10; n++)
    {
      a = a - 2;
      a = a - b;
    }

    return a;
  }

  static public void main(String[] args)
  {
  int c;

    for (c = 0; c < 10; c++)
    {
      //System.out.println(c);
    } 

    while(true);
  }
}


