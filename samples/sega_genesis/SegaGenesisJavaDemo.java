
import net.mikekohn.java_grinder.SegaGenesis;

public class SegaGenesisJavaDemo
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

  static public void main(String args[])
  {
    int a,b;

    SegaGenesis.setPaletteColors(ImageJavaGrinder.palette);
    SegaGenesis.setPatternTable(ImageJavaGrinder.pattern);
    SegaGenesis.setImageData(ImageJavaGrinder.image);

    SegaGenesis.setPalettePointer(49);
    SegaGenesis.setPaletteColor(0xeee);
    SegaGenesis.loadFonts();
    SegaGenesis.clearText();
    wait(30);

    SegaGenesis.loadZ80(PlayTitleSample.z80_code);
    wait(120);

    SegaGenesis.pauseZ80();

    SegaGenesis.setPaletteColors(NakenLogo.palette);
    SegaGenesis.setPatternTable(NakenLogo.pattern);
    SegaGenesis.setImageData(NakenLogo.image);

    b = 1;
    a = 0;
    for (int n = 0; n < 120; n++)
    {
      while(!SegaGenesis.inVerticalBlank());

      for (int c = 0; c < 90; c++)
      {
        SegaGenesis.setHorizontalScrollB(c, a);
      }

      SegaGenesis.setVerticalScrollB(a);

      a += b;

      if (a >=24) { b = -1; }
      else if (a == 0) { b = 0; }

      while(SegaGenesis.inVerticalBlank());
    }

    for (int c = 0; c < 216; c++)
    {
      SegaGenesis.setHorizontalScrollB(c, 0);
    }

    SegaGenesis.setSpritePosition(0, 10, 20);
    SegaGenesis.setSpritePosition(1, 10, 20);
    SegaGenesis.setSpriteConfig1(0,
      SegaGenesis.SPRITE_CONFIG1_HORIZONTAL_SIZE_2_CELL);

    SegaGenesis.initBitmap();

    //SegaGenesis.clearBitmap();
    //SegaGenesis.setPalettePointer(1);
    //SegaGenesis.setPaletteColor(0xe);
    //SegaGenesis.plot(1, 0, 1);
    //SegaGenesis.plot(8, 8, 1);
    //SegaGenesis.plot(16, 16, 1);
    //SegaGenesis.plot(24, 24, 1);
    //for (a = 0; a < 8; a++) { SegaGenesis.plot(a, a, 1); }
    //wait(120);

    int k = Stars.run();

    SegaGenesis.setCursor(3, 7);
    SegaGenesis.print("TESTING[TEXT");
    wait(60);

    SegaGenesis.loadZ80(PlaySong.z80_code);
    SegaGenesis.setPaletteColors(Mandelbrots.palette);
    Mandelbrots.draw();

    wait(60);

    SegaGenesis.clearBitmap();

    //SegaGenesis.setPlotAddress(0);
    //SegaGenesis.fastPlot(1,2,3,4);

    SegaGenesis.clearText();
    SegaGenesis.setCursor(0, 7);
    SegaGenesis.print("MIKE[KOHN");
    SegaGenesis.setCursor(0, 1);
    SegaGenesis.print("JAVA[GRINDER");

    while(true)
    {
      while(SegaGenesis.inVerticalBlank());

      for (b = 0; b < 4; b++)
      {
        for (a = 0; a < 0xf; a++)
        {
          SegaGenesis.setPalettePointer(49);
          SegaGenesis.setPaletteColor((a << 8) | (a << 4) | a);
          SegaGenesis.waitHorizontalBlank();
        }
      }

      SegaGenesis.waitVerticalBlank();
    }

    //while(true);
  }
}

