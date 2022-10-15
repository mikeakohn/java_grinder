import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.Nintendo64;
import net.mikekohn.java_grinder.n64.Rectangle;
import net.mikekohn.java_grinder.n64.Triangle;

public class Nintendo64Demo
{
  public static short[] vertexes =
  {
      0, -30, 0,
    -30,  30, 0,
     30,  30, 0,

      0,  -40, 0,
     40,   40, 0,
      0,   40, 0
  };

  public static void main(String[] args)
  {
    int y = 50, dy = 1, rx = 0, ry = 0, rz = 30;
    int width = 50, height = 50, rect_x = 50, rect_y = 20;
    int width_delta = 1, height_delta = 1, rect_dx = 1, rect_dy = 1;
    int screen = 0;
    int x;

    short[] texture = Memory.preloadShortArray("picture.raw");

    Nintendo64.loadTexture(texture, 32, 9);

    Triangle triangle_1 = new Triangle();
    triangle_1.setVertex0(  0, -30, 0);
    triangle_1.setVertex1(-30,  30, 0);
    triangle_1.setVertex2( 30,  30, 0);
    triangle_1.setPosition(150, 100, 256 + 128);
    triangle_1.setColor(0xff00ffff);

    Triangle triangle_2 = new Triangle();
    //triangle_2.setVertex0(  0, -30, 0);
    //triangle_2.setVertex1(-30,  30, 0);
    //triangle_2.setVertex2( 30,  30, 0);
    triangle_2.setVertexes(vertexes);
    triangle_2.setPosition(180, 100, 256 + 128);
    triangle_2.setColor(0x00ff00ff);

    Triangle triangle_3 = new Triangle();
    triangle_3.setVertexes(vertexes, 9);
    triangle_3.setPosition(120, 100, 256 + 128);
    triangle_3.setColor(0xffff00ff);

    Rectangle rectangle_1 = new Rectangle();
    rectangle_1.setPosition(100, 100);
    rectangle_1.setSize(50, 50);
    rectangle_1.setColor(0x0000ffff);

    Rectangle rectangle_2 = new Rectangle();
    rectangle_2.setPosition(50, 20);
    rectangle_2.setSize(50, 50);
    rectangle_2.setTextureEnabled(32, 9);

    while (true)
    {
      Nintendo64.setScreen(screen);
      Nintendo64.clearScreen();
      rectangle_1.draw();

      rectangle_2.setPosition(rect_x, rect_y);
      rectangle_2.setSize(width, height);
      Nintendo64.waitForPolygon();
      rectangle_2.draw();

      Nintendo64.waitForPolygon();
      triangle_1.setRotation(0, 0, rz);
      triangle_1.draw();

      Nintendo64.waitForPolygon();
      triangle_2.setRotation(rx, 0, rz);
      triangle_2.draw();

      Nintendo64.waitForPolygon();
      triangle_3.setRotation(0, ry, rz);
      triangle_3.draw();

      for (x = 0; x < 319; x++) { Nintendo64.plot(x, y, 0xf800); }

      Nintendo64.waitForPolygon();
      Nintendo64.waitVsync();

      y += dy;
      if (y == 130) { dy = -1; }
      if (y == 50)  { dy =  1; }
      screen = (screen + 1) & 1;
      rx = (rx + 3) & 511;
      ry = (rx + 4) & 511;
      rz = (rz + 3) & 511;

      width += width_delta;
      height += height_delta;
      rect_x += rect_dx;
      rect_y += rect_dy;

      if (width >= 80)  { width_delta = -1; }
      if (height >= 70) { height_delta = -1; }
      if (width <= 30)  { width_delta = 1; }
      if (height <= 35) { height_delta = 1; }

      if (rect_x >= 80)  { rect_dx = -1; }
      if (rect_y >= 100) { rect_dy = -1; }
      if (rect_x <= 40)  { rect_dx = 1; }
      if (rect_y <= 20)  { rect_dy = 1; }
    }
  }
}

