
import net.mikekohn.java_grinder.SPI0;

public class SPITest
{
  static public void main(String args[])
  {
    int n;

    SPI0.init(SPI0.DIV128, 0);
    SPI0.send('A');
    n = SPI0.read();

    while(true);
  }
}


