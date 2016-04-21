
// result=35

public class ArrayReadWrite
{
  static public int add_nums()
  {
    int[] array = new int[6];
    int total = 0;
    int n;

    array[0] = 1;
    array[1] = -2;
    array[2] = 3;
    array[3] = 10;
    array[4] = 11;
    array[5] = 12;

    for (n = 0; n < array.length; n++)
    {
      total += array[n];
    }

    return total;
  }

  static public void main(String args[])
  {
    add_nums();
  }
}


