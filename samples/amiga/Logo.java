
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;
import net.mikekohn.java_grinder.Memory;

public class Logo
{
  static final int CENTER_X = 140;
  static final int CENTER_Y = 130;

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

    copper.appendWait(0, 180);
    copper.appendSetColor(0, 0x003);
    copper.appendEnd();

    // Set index back to where copper end used to be and insert a
    // dummy instruction.
    copper.setIndex(copper_end);
    //copper.appendEnd();

    copper.appendWait(0, 0);
    //copper.appendSetColor(0, 0x003);

    SpriteJava.setPosition0(150, 90);
    SpriteJava.setPosition1(180, 130);
    SpriteJava.setPosition2(150, 170);
    SpriteJava.setPosition3(110, 130);

    Amiga.setDMA(Amiga.DMA_SPRITE);

/*
    blitter.setModuloA(0);
    blitter.setModuloDestination(31);
*/
    blitter.setShiftA(0);
/*
    blitter.setShiftB(0);
    blitter.setLogicalFunction(0xf0);
    blitter.enableChannels(Blitter.MASK_ENABLE_A | Blitter.MASK_ENABLE_D);
    blitter.setChannelAMasks(0xffff, 0xffff);
    blitter.setAsFillMode(Blitter.FILL_OPTIONS_NONE);
    blitter.setDataRegisterA((char)0);
    blitter.setDataRegisterB((char)0);
    blitter.setDataRegisterC((char)0);
*/

    ImageAmigaLogo16.init(copper, blitter);

    showAmigaLogo(blitter);

    int t0 = 0;

    // Main part of method to animate the stars
    for (frame = 0; frame < 700; frame++)
    {

      while (!Amiga.inVerticalBlank());

      int dx = ((Common.cos[t0] * 40) >> 6);
      int dy = ((Common.sin[t0] * 40) >> 6);

      SpriteJava.setPosition0(CENTER_X + dx, CENTER_Y + dy);
      SpriteJava.setPosition1(CENTER_X - dx, CENTER_Y + dy);
      SpriteJava.setPosition2(CENTER_X + dx, CENTER_Y - dy);
      SpriteJava.setPosition3(CENTER_X - dx, CENTER_Y - dy);

      t0++;

      if (t0 == 90) { t0 = 0; }
    }

    Amiga.clearDMA(Amiga.DMA_SPRITE);
  }
}

