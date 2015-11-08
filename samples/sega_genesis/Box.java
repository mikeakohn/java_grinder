
import net.mikekohn.java_grinder.SegaGenesis;

public class Box
{
  public static void run()
  {
    int i;
    int x0, y0, x1, y1, r0, t0;
    int dx, dy;
    int color;

    SegaGenesis.initBitmap();

    SegaGenesis.setPalettePointer(0);
    SegaGenesis.setPaletteColor(0x000);
    color = 0xeee;
    for (i = 0; i < 7; i++)
    {
      SegaGenesis.setPaletteColor(color);
      color -= 0x222;
    }

    for (i = 0; i < 7; i++)
    {
      x0 = i;
      y0 = i;
      x1 = 319 - i;
      y1 = 223 - i;
      color = i + 1;

      Common.drawLine(x0, y0, x1, y0, color);
      Common.drawLine(x0, y0, x0, y1, color);
      Common.drawLine(x0, y1, x1, y1, color);
      Common.drawLine(x1, y0, x1, y1, color);
      Common.wait(1);
    }

    // Line part
    t0 = 0;
    x0 = 160; y0 = 112;

    for (i = 0; i < 60; i++)
    {
      while(!SegaGenesis.inVerticalBlank());

      Common.drawLine(160, 112, x0, y0, 0);

      //x = ((Common.cos[r] << 5) >> 6) + 160;
      //y = ((Common.sin[r] << 5) >> 6) + 112;
      x0 = (Common.cos[t0] >> 1) + 160;
      y0 = (Common.sin[t0] >> 1) + 112;

      Common.drawLine(160, 112, x0, y0, 1);

      while(SegaGenesis.inVerticalBlank());

      t0++;
      if (t0 == 90) { t0 = 0; }
    }

    // Box part
    t0 = 5;
    r0 = 10;
    color = 1;

    for (i = 0; i < 1024; i++)
    {
      while(!SegaGenesis.inVerticalBlank());

      dx = ((Common.cos[t0] * r0) >> 6);
      dy = ((Common.sin[t0] * r0) >> 6);

      x0 = 160 + dx - 10;
      y0 = 112 + dy - 10;
      x1 = 160 + dx + 10;
      y1 = 112 + dy + 10;

      Common.drawLine(x0, y0, x1, y0, color);
      Common.drawLine(x0, y0, x0, y1, color);
      Common.drawLine(x0, y1, x1, y1, color);
      Common.drawLine(x1, y0, x1, y1, color);

      while(SegaGenesis.inVerticalBlank());

      t0++;
      if (t0 == 90) { t0 = 0; }
      color++;
      if (color == 8) { color = 1; }
      if ((i & 0x7) == 0 && r0 < 90) { r0++; }
    }

    Common.wait(60);

    for (i = 0; i < 100; i++)
    {
      x0 = 160 - 100 + i;
      y0 = 112 - 100 + i;
      x1 = 160 + 100 - i;
      y1 = 112 + 100 - i;

      Common.drawLine(x0, y0, x1, y0, 0);
      Common.drawLine(x0, y0, x0, y1, 0);
      Common.drawLine(x0, y1, x1, y1, 0);
      Common.drawLine(x1, y0, x1, y1, 0);

      while(!SegaGenesis.inVerticalBlank());
      while(SegaGenesis.inVerticalBlank());
    }
  }
}

