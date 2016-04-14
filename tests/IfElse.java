
// result=2

public class IfElse
{
  static public int addLessThan5(int a)
  {
    int b;

    if ((a & 1) != 0)
    {
      b = 1;
    }
      else
    if ((a & 2) != 0)
    {
      b = 2;
    }
      else
    {
      b = 3;
    }

    return b;
  }

  static public void main(String args[])
  {
    addLessThan5(2);
  }
}


