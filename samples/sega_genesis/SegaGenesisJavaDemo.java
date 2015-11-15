
import net.mikekohn.java_grinder.SegaGenesis;

public class SegaGenesisJavaDemo
{
  static public void main(String args[])
  {
    // Set font screen.
    SegaGenesis.setPalettePointer(49);
    SegaGenesis.setPaletteColor(0xeee);
    SegaGenesis.loadFonts();
    SegaGenesis.clearText();
    Common.wait(30);

    // Run parts of demo.
    ImageJavaGrinder.run();
    NakenLogo.run();

    ClosingPage.run();

    SegaGenesis.loadZ80(PlayTitleSong.z80_code);
    Stars.run();
    Box.run();
    ImageInside.run();
    Sprites.run();
    Model.run();

    Common.wait(60);

    SegaGenesis.loadZ80(PlaySong.z80_code);

    Mandelbrots.run();

    Common.wait(2);

    SegaGenesis.clearBitmap();

    ClosingPage.run();
  }
}

