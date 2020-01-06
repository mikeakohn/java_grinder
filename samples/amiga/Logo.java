
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;
import net.mikekohn.java_grinder.Memory;

public class Logo
{
  static final int CENTER_X = 145;
  static final int CENTER_Y = 130;
  static int t0, a0;
  static int dy, y0, y1;
  static int copper_bar;
  static short[] rx_lookup;
  static short[] ry_lookup;
  static short[] box_x;
  static short[] box_y;
  static int box_state;

  static public void drawLine(int x0, int y0, int x1, int y1)
  {
    int count;
    int dx, dy;
    int i, j;
    int address = Memory.addressOf(Display.bitplanes);

    if (x0 == x1)
    { 
      if (y1 < y0) { j = -1; } else { j = 1; }
      
      while (y0 <= y1)
      { 
        Amiga.plot(address, x0, y0, 1, 1);
        y0 += j;
      }

      return;
    }

    if (y0 == y1)
    {
      if (x1 < x0) { i = -1; } else { i = 1; }

      while (x0 <= x1)
      {
        Amiga.plot(address, x0, y0, 1, 1);
        x0 += i;
      }

      return;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    i = dx;
    j = dy;

    if (i < 0) { i = -i; }
    if (j < 0) { j = -j; }

    count = 0;

    if (i > j)
    {
      // If the change in X is bigger, move the line horizontal

      i = 1; j = 1;

      if (dx < 0) { dx = -dx; i = -1; }
      if (dy < 0) { dy = -dy; j = -1; }

      count = dx >> 1;

      while (x0 != x1)
      {
        Amiga.plot(address, x0, y0, 1, 1);

        x0 += i;
        count += dy;
        if (count > dx) { count -= dx; y0 += j; }
      }
    }
      else
    {
      // If the change in Y is bigger, move the line vertical

      i = 1; j = 1;

      if (dy < 0) { dy = -dy; j = -1; }
      if (dx < 0) { dx = -dx; i = -1; }

      count = dy >> 1;

      while (y0 != y1)
      {
        Amiga.plot(address, x0, y0, 1, 1);

        y0 += j;
        count += dx;
        if (count > dy) { count -= dy; x0 += i; }
      }
    }
  }

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

  public static void clearLogo(Blitter blitter)
  {
    int offset = Memory.addressOf(Display.bitplanes);

    offset += ImageAmigaLogo16.getCenter();

    blitter.enableChannels(Blitter.MASK_ENABLE_D);
    blitter.setDataRegisterA((char)0);

    blitter.waitBusy();
    blitter.setDestination(offset);
    blitter.runFill(7, 37);

    offset += 8000;

    blitter.waitBusy();
    blitter.setDestination(offset);
    blitter.runFill(7, 37);

    offset += 8000;

    blitter.waitBusy();
    blitter.setDestination(offset);
    blitter.runFill(7, 37);

    offset += 8000;

    blitter.waitBusy();
    blitter.setDestination(offset);
    blitter.runFill(7, 37);
  }

  public static void clearBox(Blitter blitter)
  {
    int offset = Memory.addressOf(Display.bitplanes);

    offset += (4000 - (20 * 40)) + (20 - 4);

    blitter.setModuloDestination(30);
    blitter.enableChannels(Blitter.MASK_ENABLE_D);
    blitter.setDataRegisterA((char)0);

    blitter.waitBusy();
    blitter.setDestination(offset);
    blitter.runFill(5, 40);
  }

  public static void moveSprites()
  {
    int rx = ((Common.cos[t0] * 40) >> 6);
    int ry = ((Common.sin[t0] * 40) >> 6);

    SpriteJava.setPosition0(CENTER_X + rx, CENTER_Y + ry);
    SpriteJava.setPosition1(CENTER_X - rx, CENTER_Y + ry);
    SpriteJava.setPosition2(CENTER_X + rx, CENTER_Y - ry);
    SpriteJava.setPosition3(CENTER_X - rx, CENTER_Y - ry);

    t0++;

    if (t0 == 90) { t0 = 0; }
  }

  public static void moveCopperBars(Copper copper)
  {
    int y;
    int index;

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

  public static void computeBox()
  {
    if (box_state == 0)
    {
      a0 -= 1;
      if (a0 < 0) { a0 = 89; }
    }

    //rx = ((Common.cos[t0] * 30) >> 6);
    //ry = ((Common.sin[t0] * 20) >> 6);

    box_x[box_state] = (short)(rx_lookup[a0] + 168);
    box_y[box_state] = (short)(ry_lookup[a0] + 100);

    a0 += 22;
    if (a0 >= 90) { a0 -= 90; }

    box_state++;

    if (box_state == 4) { box_state = 0; }
  }

  public static void drawBox()
  {
    drawLine(box_x[0], box_y[0], box_x[1], box_y[1]);
    drawLine(box_x[1], box_y[1], box_x[2], box_y[2]);
    drawLine(box_x[2], box_y[2], box_x[3], box_y[3]);
    drawLine(box_x[3], box_y[3], box_x[0], box_y[0]);
  }

  public static void run(Copper copper, Blitter blitter)
  {
    int n, frame;
    int y, color;
    //int index;

    dy = 1;
    y0 = 50;
    y1 = 220;

    Display.clear();
    Display.setDisplay(copper, 4);
    Amiga.setPlayfieldPriority(0, 0, true);

    int copper_end = copper.getIndex() - 1;

    SpriteJava.init(copper);

    SongAmigaGrind.playSecond();

    // Create lookup tables for box.
    rx_lookup = new short[90];
    ry_lookup = new short[90];
    box_x = new short[4];
    box_y = new short[4];

    for (y = 0; y < 90; y++)
    {
      rx_lookup[y] = (short)((Common.cos[y] * 30) >> 6);
      ry_lookup[y] = (short)((Common.sin[y] * 20) >> 6);
    }

    box_state = 0;
    a0 = 0;

    // Create copper bars.
    copper_bar = copper.getIndex();

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

    t0 = 0;

    // Animate the sprites and copper while showing Amiga logo.
    for (frame = 0; frame < 445; frame++)
    {
      while (!Amiga.inVerticalBlank());

      moveSprites();
      moveCopperBars(copper);
    }

    clearLogo(blitter);
    Display.setPalette(copper, 1, 0xfff);

    // Animate the sprites, copper, and cube.
    for (frame = 0; frame < 445; frame++)
    {
      computeBox();
      computeBox();

      while (!Amiga.inVerticalBlank());

      if (box_state == 0)
      {
        clearBox(blitter);
        drawBox();
      }

      moveSprites();
      moveCopperBars(copper);
    }

    Amiga.clearDMA(Amiga.DMA_SPRITE);
  }
}

