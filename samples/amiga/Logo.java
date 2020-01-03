
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;
import net.mikekohn.java_grinder.Memory;

public class Logo
{
  static final int CENTER_X = 145;
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
    int y, y0, y1, color;
    int dy = 1;
    int index;

    y0 = 50;
    y1 = 220;

    Display.clear();
    Display.setDisplay(copper, 4);
    Amiga.setPlayfieldPriority(0, 0, true);

    int copper_end = copper.getIndex() - 1;

    SpriteJava.init(copper);

    // Create copper bars.
    int copper_bar = copper.getIndex();

    color = 0x300;
    y = y0;

    for (n = 0; n < 4; n++)
    {
      copper.appendWait(0, y);
      copper.appendSetColor(0, color);
      color += 0x300;
      y++;
    }

    for (n = 0; n < 4; n++)
    {
      copper.appendWait(0, y);
      copper.appendSetColor(0, color);
      color -= 0x300;
      y++;
    }

    copper.appendWait(0, y);
    copper.appendSetColor(0, 0x000);

    color = 0x003;
    y = y1;

    for (n = 0; n < 4; n++)
    {
      copper.appendWait(0, y);
      copper.appendSetColor(0, color);
      color += 0x003;
      y++;
    }

    for (n = 0; n < 4; n++)
    {
      copper.appendWait(0, y);
      copper.appendSetColor(0, color);
      color -= 0x003;
      y++;
    }

    copper.appendWait(0, y);
    copper.appendSetColor(0, 0x000);
    copper.appendEnd();

    // Set index back to where copper end used to be and insert a
    // dummy instruction.
    copper.setIndex(copper_end);
    copper.appendWait(0, 0);

    //SpriteJava.setPosition0(150, 90);
    //SpriteJava.setPosition1(180, 130);
    //SpriteJava.setPosition2(150, 170);
    //SpriteJava.setPosition3(110, 130);

    Amiga.setDMA(Amiga.DMA_SPRITE);

    // Show Amiga logo.
    blitter.setShiftA(0);
    ImageAmigaLogo16.init(copper, blitter);
    showAmigaLogo(blitter);

    int t0 = 0;

    // Main part of method to animate the stars.
    for (frame = 0; frame < 500; frame++)
    {
      while (!Amiga.inVerticalBlank());

      int rx = ((Common.cos[t0] * 40) >> 6);
      int ry = ((Common.sin[t0] * 40) >> 6);

      SpriteJava.setPosition0(CENTER_X + rx, CENTER_Y + ry);
      SpriteJava.setPosition1(CENTER_X - rx, CENTER_Y + ry);
      SpriteJava.setPosition2(CENTER_X + rx, CENTER_Y - ry);
      SpriteJava.setPosition3(CENTER_X - rx, CENTER_Y - ry);

      t0++;

      if (t0 == 90) { t0 = 0; }

      y0 += dy;
      y1 -= dy;

      if (y0 == 50)
      {
        dy = 1;
      }
        else
      if (y0 == 70)
      {
        dy = -1;
      }

      index = copper_bar;

      for (y = 0; y < 9; y++)
      {
        copper.setIndex(index);
        index += 2;

        copper.appendWait(0, y0 + y);
      }

      for (y = 0; y < 9; y++)
      {
        copper.setIndex(index);
        index += 2;

        copper.appendWait(0, y1 + y);
      }
    }

    Amiga.clearDMA(Amiga.DMA_SPRITE);
  }
}

