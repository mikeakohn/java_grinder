
import net.mikekohn.java_grinder.SegaGenesis;

public class Common
{
  static public void wait(int frames)
  {
    int a;

    // 60 frames should be 1 second.
    for (a = 0; a < frames; a++)
    {
      while(!SegaGenesis.inVerticalBlank());
      while(SegaGenesis.inVerticalBlank());
    }
  }
}

