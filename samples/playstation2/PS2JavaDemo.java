
import net.mikekohn.java_grinder.Playstation2;
import net.mikekohn.java_grinder.Draw3D.Draw3DObject;
import net.mikekohn.java_grinder.Draw3D.Draw3DPoints;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangle;

public class PS2JavaDemo
{
  static public void animate()
  {
    int n;

    //Draw3DPoints points = new Draw3DPoints(5);
    Draw3DTriangle triangle = new Draw3DTriangle(3);

    // Define all the triangle vertexes (position and color)
    // with a reference point of (0, 0).
    triangle.setPointPosition(0, -100.0f, -100.0f, 0.0f);
    triangle.setPointColor(0, 0x000000ff);
    triangle.setPointPosition(1, -100.0f, 110.0f, 0.0f);
    triangle.setPointColor(1, 0x0000ff00);
    triangle.setPointPosition(2, 0.0f, 110.0f, 0.0f);
    triangle.setPointColor(2, 0x00ff0000);

    // Set the position on the screen.
    triangle.setPosition(1900.0f, 2100.0f, 128.0f);

    for(n = 0; n < 1000; n++)
    {
      Playstation2.waitVsync();
      Playstation2.clearScreen();
      triangle.draw();
    }
  }

  static public void main(String args[])
  {
    animate();

    while(true)
    {
      Playstation2.waitVsync();
      Playstation2.clearScreen();
    }
  }
}

