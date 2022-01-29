
import net.mikekohn.java_grinder.SPI0;

public class SPITest
{
  static public void main(String[] args)
  {
    int n;

    SPI0.init(SPI0.DIV128, 0);
    n = SPI0.read('A');

    while(true);
  }
}


