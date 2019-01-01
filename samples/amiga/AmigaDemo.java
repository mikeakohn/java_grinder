
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;

public class AmigaDemo
{
  static public void main(String args[])
  {
    while(true)
    {
      Amiga.setPalette(0, 0xf0f);
    }
  }
}

