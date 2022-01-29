
import net.mikekohn.java_grinder.Memory;

public class MemoryTest
{
  static public void main(String[] args)
  {
    Memory.write16(0x200, (short)10);
    Memory.read16(0x200);
    Memory.write8(0x400, (byte)5);
    Memory.read8(0x400);

    while(true);
  }
}


