
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.SPI0;

public class SPITest
{
  static public void main(String args[])
  {
    Memory.write16(0x200, (short)10);
    Memory.read16(0x200);

    while(true);
  }
}


