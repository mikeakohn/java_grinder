
import net.mikekohn.java_grinder.Nintendo64;
import net.mikekohn.java_grinder.n64.Rectangle;
import net.mikekohn.java_grinder.n64.Triangle;

public class Nintendo64Demo
{
  public static void main(String[] args)
  {
    int y = 50, dy = 1, rz = 30;
    int screen = 0;
    int x;

    Triangle triangle = new Triangle();
    triangle.setVertex0(  0, -30, 0);
    triangle.setVertex1(-30,  30, 0);
    triangle.setVertex2( 30,  30, 0);
    triangle.setPosition(150, 100, 256 + 128);
    triangle.setColor(0xff00ffff);

    Rectangle rectangle = new Rectangle();
    rectangle.setPosition(100, 100);
    rectangle.setSize(50, 50);
    rectangle.setColor(0x0000ffff);

    while (true)
    {
      Nintendo64.setScreen(screen);
      Nintendo64.clearScreen();
      rectangle.draw();
      Nintendo64.waitForPolygon();
      triangle.setRotation(0, 0, rz);
      triangle.draw();
      Nintendo64.waitForPolygon();
      for (x = 0; x < 319; x++) { Nintendo64.plot(x, y, 0xf800); }
      Nintendo64.waitVsync();

      y += dy;
      if (y == 130) { dy = -1; }
      if (y == 50)  { dy =  1; }
      screen = (screen + 1) & 1;
      rz = (rz + 3) & 511;
    }
  }
}

