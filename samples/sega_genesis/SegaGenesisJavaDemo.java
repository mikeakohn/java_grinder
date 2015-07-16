
import net.mikekohn.java_grinder.SegaGenesis;

public class SegaGenesisJavaDemo
{
  static public void main(String args[])
  {
    int a,b;

    SegaGenesis.setPaletteColors(ImageJavaGrinder.palette);
    SegaGenesis.setPatternTable(ImageJavaGrinder.pattern);
    SegaGenesis.setImageData(ImageJavaGrinder.image);
    for (a = 0; a < 100000; a++);

    SegaGenesis.loadZ80(PlayTitleSample.z80_code);

    for (a = 0; a < 500000; a++);

    SegaGenesis.pauseZ80();

    SegaGenesis.setPaletteColors(ImageMike.palette);
    SegaGenesis.setPatternTable(ImageMike.pattern);
    SegaGenesis.setImageData(ImageMike.image);

    for (a = 0; a < 500000; a++);

    SegaGenesis.initBitmap();
    SegaGenesis.clearBitmap();
    SegaGenesis.loadFonts();

    SegaGenesis.setCursor(0, 7);
    SegaGenesis.print("mike kohn");
    SegaGenesis.setCursor(0, 0);
    SegaGenesis.print("java grinder");


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

