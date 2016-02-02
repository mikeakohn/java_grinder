
import net.mikekohn.java_grinder.TRS80Coco;

public class JavaTrs80Demo
{
  static public int extra = 4;

  static public int sub(int a, int b)
  {
    int c;

    c = a - b;

    c = c + extra;

    return c;
  }

  static public void main(String args[])
  {
    int a,n,x,y;
    int dx,dy;

    //TRS80Coco.initSound();
    TRS80Coco.initVideoFlags();

    //TRS80Coco.setBackgroundColor(5);
    TRS80Coco.setText(1024, 0x8f);
    TRS80Coco.setText(1025, 0x9f);
    TRS80Coco.setText(1026, 0xaf);
    TRS80Coco.setText(1027, 0xbf);
    TRS80Coco.setText(1028, 0xcf);
    TRS80Coco.setText(1029, 0xdf);
    TRS80Coco.setText(1030, 0xef);
    TRS80Coco.setText(1031, 0xff);
    TRS80Coco.setText(1032, 0x80);

    for (a = 1; a < 10; a++)
    {
      TRS80Coco.plotLores(a, a, TRS80Coco.COLOR_ORANGE);
    }

    TRS80Coco.plotMidres(20, 20, TRS80Coco.COLOR_ORANGE);

    //a = 0xff00;
    //dx = a >> 3;

    dx = 1; dy = 1;
    x = 20; y = 10;

    for (a = 1; a < 25; a++)
    {
      TRS80Coco.setText(y * 32 + x + 1024, TRS80Coco.COLOR_RED);
      //for (n = 0; n < 5000; n++);
      for (n = 0; n < 60; n++)
      {
        TRS80Coco.waitForVsync();
      }

      TRS80Coco.setText(y * 32 + x + 1024, TRS80Coco.COLOR_GREEN);

      x += dx;
      y += dy;

      if (x <= 20) { dx = 1; }
      if (y <= 10) { dy = 1; }
      if (x >= 31) { dx = -1; }
      if (y >= 15) { dy = -1; }
    }

    for (a = 0x90; a < 0x9f; a++)
    {
      TRS80Coco.setText(1024, a);
      for (n = 0; n < 1000; n++);
    }

    //TRS80Coco.setSound(Waveforms.a440);

    //TRS80Coco.clearScreenLores();
    //TRS80Coco.print("JAVA GRINDER");

    TRS80Coco.clearScreenMidres();

    Mandelbrot.run();

    y = sub(5, 3);
    extra = 7;
    y = sub(5, 3);

    TRS80Coco.setGraphicsMode();

    while(true);
  }
}

