
import net.mikekohn.java_grinder.IOPort3;
import net.mikekohn.java_grinder.IOPort5;
import net.mikekohn.java_grinder.IOPort6;
import net.mikekohn.java_grinder.SPI0;

public class Display
{
  static public void main(String args[])
  {
    int n;

    IOPort3.setPinsAsOutput(0x200);
    IOPort5.setPinsAsOutput(0x01);
    IOPort6.setPinsAsOutput(0x40);

    IOPort3.setPinsValue(0x200);
    IOPort5.setPinsValue(0x01);
    IOPort6.setPinsValue(0x40);

    SPI0.init16(SPI0.DIV128, 0);

    IOPort3.setPinsValue(0x000);
    SPI0.send16(0x3fff);
    IOPort3.setPinsValue(0x200);

    while(true);
  }
}


