
import net.mikekohn.java_grinder.SegaGenesis;
import net.mikekohn.java_grinder.Memory;

public class Stars
{
/*
  public static short[] palette =
  {
    0x000, 0x04e, 0x044, 0x040, 0x440, 0x880, 0x808, 0xe0e,
    0xe4e, 0x02e, 0x88e, 0x444, 0xe40, 0xee0, 0x400, 0x000,
  };
*/

  public static int run()
  {
    byte[] stars = Memory.allocStackBytes(10);
    int n,total = 0;

    stars[0] = 5;
    stars[1] = 3;
    stars[2] = 1;
    stars[3] = 7;

    for (n = 0; n < 4; n++) { total += stars[n]; }

    return total;
  }
}

