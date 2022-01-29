
// result=10

public class StaticByteArrays
{
  static byte[] array = { 1, 2, 3, 4 };

  static public int add_nums()
  {
    int total = 0;
    int n;

    for (n = 0; n < array.length; n++)
    {
      total += array[n];
    }

    return total;
  }

  static public void main(String[] args)
  {
    add_nums();
  }
}


