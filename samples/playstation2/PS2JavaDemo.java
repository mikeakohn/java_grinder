
import net.mikekohn.java_grinder.Playstation2;
import net.mikekohn.java_grinder.Draw3D.Draw3DObject;
import net.mikekohn.java_grinder.Draw3D.Draw3DPoints;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangle;
import net.mikekohn.java_grinder.Draw3D.Draw3DTexture;

public class PS2JavaDemo
{
  static int[] colors = { 0x000000ff, 0x0000ff00, 0x00ff0000 };
  static float[] points =
  {
    -100.0f, -100.0f, 0.f,
    -100.0f,  110.0f, 0.f,
       0.0f,  110.0f, 0.f,
  };

  static public void animate(int count)
  {
    int n;

    //Draw3DPoints points = new Draw3DPoints(5);
    Draw3DTriangle triangle = new Draw3DTriangle(3);
    Draw3DTexture texture = new Draw3DTexture(64, 64);

    // Define all the triangle vertexes (position and color)
    // with a reference point of (0, 0).
    //triangle.setPoint(0, -100.0f, -100.0f, 0.0f);
    //triangle.setPoint(1, -100.0f, 110.0f, 0.0f);
    //triangle.setPoint(2, 0.0f, 110.0f, 0.0f);
    //triangle.setPointColor(0, 0x000000ff);
    //triangle.setPointColor(1, 0x0000ff00);
    //triangle.setPointColor(2, 0x00ff0000);

    // Define triangle vertexes with an array instead.
    triangle.setPoints(points);
    triangle.setPointColors(colors);

    for(n = 0; n < 64 * 64; n++)
    {
      texture.setPixel(n, n);
    }

    triangle.disableGouraudShading();
    triangle.enableTexture();

    Playstation2.randomInit(Playstation2.performanceCountGet());

    n = Playstation2.randomGet();
    n = Playstation2.randomNext();

    // Set the position on the screen.
    triangle.setPosition(1900.0f, 2100.0f, 2048.0f);

    for(n = 0; n < count; n++)
    {
      Playstation2.waitVsync();
      Playstation2.clearScreen();
      texture.upload();
      triangle.draw();
      triangle.rotateZ512(n);
    }
  }

  static public void main(String args[])
  {
    Playstation2.performanceCountEnable(Playstation2.EVENT_PROCESSOR_CYCLE);

    animate(10000);

    while(true)
    {
      Playstation2.waitVsync();
      Playstation2.clearScreen();
    }
  }
}

