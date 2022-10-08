
import net.mikekohn.java_grinder.Nintendo64;
import net.mikekohn.java_grinder.n64.Rectangle;
import net.mikekohn.java_grinder.n64.Triangle;

public class Nintendo64Demo
{
  public static void main(String[] args)
  {
    int y = 50, dy = 1;
    int x;
    Rectangle rectangle = new Rectangle();
    rectangle.setPosition(100, 100);
    rectangle.setSize(50, 50);
    rectangle.setColor(0x00ff00ff);

    while (true)
    {
      Nintendo64.setScreen(0);
      Nintendo64.clearScreen();
      rectangle.draw();
      Nintendo64.waitForPolygon();
      for (x = 0; x < 319; x++) { Nintendo64.plot(x, y, 0xf800); }
      Nintendo64.waitVsync();

      y += dy;
      if (y == 100) { dy = -1; }
      if (y == 10)  { dy =  1; }

      Nintendo64.setScreen(1);
      Nintendo64.clearScreen();
      rectangle.draw();
      Nintendo64.waitForPolygon();
      for (x = 0; x < 319; x++) { Nintendo64.plot(x, y, 0xf800); }
      Nintendo64.waitVsync();

      y += dy;
      if (y == 100) { dy = -1; }
      if (y == 10)  { dy =  1; }
    }
  }
}

