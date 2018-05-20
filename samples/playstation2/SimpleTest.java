// Rotate two triangles in opposite directions using arrays of objects.

import net.mikekohn.java_grinder.Playstation2;
import net.mikekohn.java_grinder.Draw3D.Draw3DObject;
import net.mikekohn.java_grinder.Draw3D.Draw3DPoints;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangle;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangleFan;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangleFanWithTexture;
import net.mikekohn.java_grinder.Draw3D.Draw3DTexture32;

public class SimpleTest
{
  static int[] triangle_colors = { 0x000000ff, 0x0000ff00, 0x00ff0000 };

  static float[] triangle_points =
  {
    -100.0f, -100.0f, 0.f,
    -100.0f,  110.0f, 0.f,
       0.0f,  110.0f, 0.f,
  };

  static public void main(String args[])
  {
    int n;

    Draw3DTriangle[] triangles = new Draw3DTriangle[2];

    for (n = 0; n < 2; n++)
    {
      triangles[n] = new Draw3DTriangle(3);
      triangles[n].setPoints(triangle_points);
      triangles[n].setContext(0);
      triangles[n].setPointColors(triangle_colors);
    }

    triangles[0].setPosition(1250.0f, 1250.0f, 2048.0f);
    triangles[1].setPosition(1450.0f, 1250.0f, 2048.0f);

    n = 0;

    // Changing contexts (double buffering).
    // Since it's interlaced, one context is odd scan lines and
    // the other is even.
    while(true)
    {
      // Tell video system which context needs to be drawn next.
      Playstation2.showContext(n + 1);

      // Set which context these objects need to be drawn on.
      // The API automatically does a logical AND with the value
      // passed in.
      triangles[0].setContext(n);
      //triangles[1].setContext(n);

      // Clear the entire context of where this is going to draw.
      Playstation2.clearContext(n);

      triangles[0].draw();
      //triangles[1].draw();
      triangles[0].rotateZ512(n);
      //triangles[1].rotateZ512(-n);

      // Wait until the video beam is done drawing the last frame.
      Playstation2.waitVsync();

      n++;
    }
  }
}

