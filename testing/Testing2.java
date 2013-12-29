
public class Testing2
{
  static public void main(String args[])
  {
  int c;
  float k;

    k = 100.0f;

    for (c = 0; c < 1000000000; c++)
    {
      k = k + c;
    }

    System.out.println(k);
    System.out.println(c);
  }
}


