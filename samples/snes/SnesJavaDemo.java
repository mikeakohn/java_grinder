
import net.mikekohn.java_grinder.SNES;

public class SnesJavaDemo
{
  static public void main(String[] args)
  {
    // Color is 0bbbbbgg gggrrrrr, in this case b=32, g=0, r=0.
    SNES.setBackgroundColor(0xe800);

    while(true);
  }
}

