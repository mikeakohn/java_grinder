
// result=640

public class LoopTest
{
  static public int loop_up()
  {
     int total = 0;
     int n;

     for (n = 0; n < 640; n++)
     {
       total += 1;
     }

     return total;
  }

  static public int loop_down()
  {
     int total = 0;
     int n;

     for (n = 640; n > 0; n--)
     {
       total += 1;
     }

     return total;
  }

  static public void main(String args[])
  {
     //loop_up();
     loop_down();
  }
}


