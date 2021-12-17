
import net.mikekohn.java_grinder.NES;

public class NesJavaDemo
{
  static public void main(String args[])
  {
    NES.setBackgroundPalette(0, 0x23);
    NES.setBackgroundPalette(1, 0x09);

    while(true);
  }
}

