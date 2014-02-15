
public class Arrays
{
  static public final short[] numbers = { 1, 5, 2, 0, 2, -1 };
  static public final byte[] mike = { 1, 2, 3 };
  static public int blah = 5;
  static public int another = 3;

  static public int calc_array()
  {
    int total = 0;
    int n;

    for (n = 0; n < numbers.length; n++)
    {
      total = total + numbers[n];
    }

    return total;
  }

  static public void main(String args[])
  {
    int a;

    a = calc_array();
    blah = a;
    another = blah + 1;

    //System.out.println(a);

    while(true);
  }
}


