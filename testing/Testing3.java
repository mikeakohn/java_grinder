
public class Testing3
{
  static public void main(String args[])
  {
  int c,k;
  double d;
  float f;

    c=100;

    for (k=0; k<10; k++)
    {
      c=c-5;
    }

    System.out.println(c);
    f=(float)c*(float)k;
    System.out.println(f);
    d=f;
    d=d*1.34;
    f=(float)d;
    System.out.println(f);
  }
}


