
import net.mikekohn.java_grinder.SegaGenesis;

public class SegaGenesisJavaDemo
{
  static public void main(String args[])
  {
    SegaGenesis.setPalettePointer(0);
    SegaGenesis.setPaletteColor(0x444);
    SegaGenesis.setPaletteColor(0xe00);

    while(true);
  }
}

