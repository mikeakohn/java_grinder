
import net.mikekohn.java_grinder.Playstation2;
import net.mikekohn.java_grinder.Draw3D.Draw3DObject;
import net.mikekohn.java_grinder.Draw3D.Draw3DPoints;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangle;

public class PS2JavaDemo
{
  static public void animate()
  {
    int n;

    Draw3DPoints points = new Draw3DPoints(5);
    Draw3DTriangle triangle = new Draw3DTriangle(3);

    for(n = 0; n < 1000; n++)
    {
      Playstation2.waitVsync();
    }
  }

  static public void main(String args[])
  {
    animate();
    while(true);
  }
}

