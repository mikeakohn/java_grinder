
import net.mikekohn.java_grinder.SegaGenesis;

public class Box
{
  public static void run()
  {
    int i;

    Common.drawLine(10, 10, 30, 30, 1);

    for (i = 0; i < 300; i++)
    {
      while(!SegaGenesis.inVerticalBlank());

      while(SegaGenesis.inVerticalBlank());
    }
  }
}

