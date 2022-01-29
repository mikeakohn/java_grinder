
// result=21

public class Statics
{
  static int integer = 50;
  static short short_integer = 25;
  static byte byte_integer = 1;
  static byte byte_integer_neg = -50;

  static public int get_number()
  {
    return integer + short_integer + byte_integer + byte_integer_neg;
  }

  static public void main(String[] args)
  {
    short_integer = 20;
    get_number();
  }
}


