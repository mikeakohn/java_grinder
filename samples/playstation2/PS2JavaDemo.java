
import net.mikekohn.java_grinder.Playstation2;
import net.mikekohn.java_grinder.Draw3D.Draw3DObject;
import net.mikekohn.java_grinder.Draw3D.Draw3DPoints;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangle;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangleFan;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangleFanWithTexture;
import net.mikekohn.java_grinder.Draw3D.Draw3DTexture24;

public class PS2JavaDemo
{
  static int[] triangle_colors = { 0x000000ff, 0x0000ff00, 0x00ff0000 };
  static int[] square_colors = { 0x00ffff00, 0x00ffffff, 0x00ff0000, 0x00ff00ff };

  static float[] triangle_points =
  {
    -100.0f, -100.0f, 0.f,
    -100.0f,  110.0f, 0.f,
       0.0f,  110.0f, 0.f,
  };

  static float[] square_points =
  {
    -50.0f,  50.0f, 0.f,
     50.0f,  50.0f, 0.f,
     50.0f, -50.0f, 0.f,
    -50.0f, -50.0f, 0.f,
  };

  //static int[] pixels = { 0x00ff00, 0x00ff00, 0x00ff00, 0x008000 };

  static byte[] vu0_prog =
  {
        0,    0,    1,    9,   -1,    2,    0,    0,
        0,    0, -126,    0,   -1,    2,    0,    0,
       -3,    3,  -29, -127,   -1,    2,    0,    0,
      -16,    0,    0, -128,   65,   24,  -30,    1,
        1,    8,    1,   18,   -1,    2,    0,    0,
      125,   11,  -29, -127,   -1,    2,    0,    0,
       -3,    7,    1,   82,   -1,    2,    0,    0,
       60,    3,    0, -128,   -1,    2,    0,    0,
       60,    3,    0, -128,   -1,    2,    0,   64,
       60,    3,    0, -128,   -1,    2,    0,    0,
  };

  static public void animate(int count)
  {
    int n;
    int[] vu0_params = new int[4];
    int[] vu0_data = new int[640];
    float z = 0.0f;
    float dz = 10.0f;

    Draw3DTriangle triangle = new Draw3DTriangle(3);
    Draw3DTexture24 texture = new Draw3DTexture24(64, 64);
    Draw3DTriangleFanWithTexture picture = new Draw3DTriangleFanWithTexture(4);
    Draw3DTriangleFan square = new Draw3DTriangleFan(4);

    // Define all the triangle vertexes (position and color)
    // with a reference point of (0, 0).
    picture.setPoint(0, -100.0f, 100.0f, 0.0f);
    picture.setPoint(1, 100.0f, 100.0f, 0.0f);
    picture.setPoint(2, 100.0f, -100.0f, 0.0f);
    picture.setPoint(3, -100.0f, -100.0f, 0.0f);
    picture.setPointColor(0, 0x00ffffff);
    picture.setPointColor(1, 0x00ffffff);
    picture.setPointColor(2, 0x00ffffff);
    picture.setPointColor(3, 0x00ffffff);
    picture.setTextureCoord(0, 0.0f, 0.0f);
    picture.setTextureCoord(1, 0.0f, 1.0f);
    picture.setTextureCoord(2, 1.0f, 1.0f);
    picture.setTextureCoord(3, 1.0f, 0.0f);

    // Define triangle vertexes with an array instead.
    triangle.setPoints(triangle_points);
    triangle.setPointColors(triangle_colors);

    // Define square vertexes with an array.
    square.setPoints(square_points);
    square.setPointColors(square_colors);

    for (n = 0; n < 64 * 64; n++)
    {
      texture.setPixel(n, 0xff0000);
    }

    for (n = 64 * 30; n < 64 * 64; n++)
    {
      texture.setPixel(n, 0x0000ff);
    }

/*
    // It's possible to set the pixels in the texture using an array
    // that can be copied into mutiple parts of the image using the
    // index parameter.
    int index = 64 * 40;

    for (n = 0; n < 64 * 5; n = n + 4)
    {
      texture.setPixels(index + n, pixels);
    }
*/

    // Set up VU0 to set a buffer to a specific color.
    vu0_params[0] = vu0_data.length / 4;
    vu0_params[1] = 0x00ff00ff;

    // Clear the data area (this gets overwritten when downloading from
    // VU0 anyway.  This is pointless
    //for (n = 0; n < vu0_data.length; n++) { vu0_data[n] = 0; }

    // Upload the code, then upload the 2 paramters that the VU0 will
    // use to draw (just the color purple and the length of the data
    // array (in quad words, aka 16 byte elements), start VU0 executing
    // the code, wait while it's running, and finally download the
    // generated data.
    Playstation2.vu0UploadCode(vu0_prog);
    Playstation2.vu0UploadData(0, vu0_params);
    Playstation2.vu0Start();
    while(Playstation2.vu0IsRunning()) { }
    Playstation2.vu0DownloadData(0, vu0_data);

    // Copy into the texture the data generated by VU0.
    texture.setPixels(0, vu0_data);

    //triangle.disableGouraudShading();
    //picture.disableTexture();
    //picture.enableGouraudShading();

/*
    Playstation2.randomInit(Playstation2.performanceCountGet());

    n = Playstation2.randomGet();
    n = Playstation2.randomNext();
*/

    // Set the position on the screen.
    triangle.setPosition(1250.0f, 1250.0f, 2048.0f);
    picture.setPosition(1400.0f, 1200.0f, 2048.0f);

    // Changing contexts (double buffering).
    // Since it's interlaced, one context is odd scan lines and
    // the other is even.
    for (n = 0; n < count; n++)
    {
      // Set which context these objects need to be drawn on.
      // The API automatically does a logical AND with the value
      // passed in.
      triangle.setContext(n);
      square.setContext(n);
      picture.setContext(n);

      // Clear the entire context of where this is going to draw.
      Playstation2.clearContext(n);

      // Draw all the objects into the memory of the context
      triangle.draw();
      texture.upload();
      picture.draw();

      // Rotate the triangle so the next time it draws it's rotated
      // around the Z axis.
      triangle.rotateZ512(n);

      // Rotate the square around the X axis.
      square.setPosition(1500.0f, 1300.0f, 2048.0f);
      square.rotateX512(n);
      square.draw();

      // Draw the same square again further up with no rotation, but
      // move it forward and back.
      square.setPosition(1500.0f, 1100.0f, 2048.0f + z);
      square.rotateX512(0);
      square.draw();

      z = z + dz;

      if (z < -500.0f) { dz = 10.0f; }
      if (z > 500.0f) { dz = -10.0f; }

      // Wait until the video beam is done drawing the last frame.
      Playstation2.waitVsync();

      // Tell video system which context needs to be drawn next.
      Playstation2.showContext(n);
    }

    Playstation2.showContext(0);
    triangle.setContext(0);
    picture.setContext(0);

    // Drawing without changing contexts.  Since there isn't much to draw
    // there is plenty of time to do it with 1 context.
    for (n = 0; n < count; n++)
    {
      Playstation2.waitVsync();
      Playstation2.clearContext(0);
      triangle.draw();
      texture.upload();
      picture.draw();
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
      Playstation2.clearContext(0);
    }
  }
}

