
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;
import net.mikekohn.java_grinder.Memory;

public class Logo
{
  public static void showAmigaLogo(Blitter blitter)
  {
    int offset = Memory.addressOf(Display.bitplanes);

    offset += ImageAmigaLogo16.getCenter();

    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageAmigaLogo16.bitplane_0));
    blitter.setDestination(offset);
    blitter.runFill(7, 37);

    offset += 8000;

    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageAmigaLogo16.bitplane_1));
    blitter.setDestination(offset);
    blitter.runFill(7, 37);

    offset += 8000;

    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageAmigaLogo16.bitplane_2));
    blitter.setDestination(offset);
    blitter.runFill(7, 37);

    offset += 8000;

    blitter.waitBusy();
    blitter.setSourceA(Memory.addressOf(ImageAmigaLogo16.bitplane_3));
    blitter.setDestination(offset);
    blitter.runFill(7, 37);
  }

  public static void run(Copper copper, Blitter blitter)
  {
    int n, frame;

    Display.clear();
    Display.setDisplay(copper, 4);
    Amiga.setPlayfieldPriority(0, 0, true);

    int copper_end = copper.getIndex() - 1;

    SpriteJava.init(copper);

    //copper.appendWait(0, 150);
    //copper.appendSetColor(0, 0x003);
    copper.appendEnd();

    // Set index back to where copper end used to be and insert a
    // dummy instruction.
    copper.setIndex(copper_end);
    //copper.appendWait(0, 0);
    //copper.appendSetColor(0, 0x003);

    blitter.setModuloA(0);
    blitter.setModuloDestination(31);
    blitter.setShiftA(0);
    blitter.setShiftB(0);
    blitter.setLogicalFunction(0xf0);
    blitter.enableChannels(Blitter.MASK_ENABLE_A | Blitter.MASK_ENABLE_D);
    blitter.setChannelAMasks(0xffff, 0xffff);
    blitter.setAsFillMode(Blitter.FILL_OPTIONS_NONE);
    blitter.setDataRegisterA((char)0);
    blitter.setDataRegisterB((char)0);
    blitter.setDataRegisterC((char)0);

    ImageAmigaLogo16.init(copper, blitter);

    showAmigaLogo(blitter);

    // Main part of method to animate the stars
    for (frame = 0; frame < 360; frame++)
    {

      while (!Amiga.inVerticalBlank());
    }
  }
}

