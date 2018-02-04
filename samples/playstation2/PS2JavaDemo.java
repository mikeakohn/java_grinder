
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

    triangle.setPointPosition(0, 1800 << 4, 2000 << 4, 128);
    triangle.setPointColor(0, 0x0000ff00);
    triangle.setPointPosition(1, 1800 << 4, 2010 << 4, 128);
    triangle.setPointColor(1, 0x0000ff00);
    triangle.setPointPosition(2, 1900 << 4, 2010 << 4, 128);
    triangle.setPointColor(2, 0x0000ff00);

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

