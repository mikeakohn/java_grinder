
import net.mikekohn.java_grinder.SegaGenesis;

public class SegaGenesisJavaDemo
{
  static public void main(String args[])
  {
    int a,b;

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

