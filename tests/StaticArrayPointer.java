
public class StaticArrayPointer
{
  static byte[] array1 = { 1, 2, 3, 4 };
  static byte[] array2 = { 5, 6, 7, 8, 9 };
  static byte[] array;

  static public int reflection(int i) { return i; }

  static public void main(String args[])
  {
    int total = 0;
    int i;

    array = array1;
    for (i = 0; i < array.length; i++)
    {
      total += array[i];
    }

    array = array2;
    for (i = 0; i < array.length; i++)
    {
      total += array[i];
    }

    // force r15 to hold the value of total
    total = reflection(total);
  }
}


