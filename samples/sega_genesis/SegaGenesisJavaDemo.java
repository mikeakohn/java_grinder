
import net.mikekohn.java_grinder.SegaGenesis;

public class SegaGenesisJavaDemo
{
  static public void main(String args[])
  {
    int a,b;

    SegaGenesis.setCursor(10, 7);
    SegaGenesis.print("mike kohn");

    while(true)
    {
      for (a = 0; a < 0xf; a++)
      {
        SegaGenesis.setPalettePointer(1);
        SegaGenesis.setPaletteColor((a << 8) | (a << 4) | a);
      }
    }

    //while(true);
  }
}

