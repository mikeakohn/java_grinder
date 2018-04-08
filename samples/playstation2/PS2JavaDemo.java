
import net.mikekohn.java_grinder.Playstation2;
import net.mikekohn.java_grinder.Draw3D.Draw3DObject;
import net.mikekohn.java_grinder.Draw3D.Draw3DPoints;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangle;
import net.mikekohn.java_grinder.Draw3D.Draw3DTriangleFanWithTexture;
import net.mikekohn.java_grinder.Draw3D.Draw3DTexture24;

public class PS2JavaDemo
{
  static int[] colors = { 0x000000ff, 0x0000ff00, 0x00ff0000 };

  static float[] points =
  {
    -100.0f, -100.0f, 0.f,
    -100.0f,  110.0f, 0.f,
       0.0f,  110.0f, 0.f,
  };

  static int[] pixels = { 0x00ff00, 0x00ff00, 0x00ff00, 0x008000 };

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

    //Draw3DPoints points = new Draw3DPoints(5);
    Draw3DTriangle triangle = new Draw3DTriangle(3);
    Draw3DTexture24 texture = new Draw3DTexture24(64, 64);
    Draw3DTriangleFanWithTexture picture = new Draw3DTriangleFanWithTexture(4);

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
    triangle.setPoints(points);
    triangle.setPointColors(colors);

    for (n = 0; n < 64 * 64; n++)
    {
      texture.setPixel(n, 0xff0000);
    }

    for (n = 64 * 30; n < 64 * 64; n++)
    {
      texture.setPixel(n, 0x0000ff);
    }

/*
    int index = 64 * 40;
    for (n = 0; n < 64 * 5; n = n + 4)
    {
      texture.setPixels(index + n, pixels);
    }
*/

    // Set up VU0 to set a buffer to a specific color.
    vu0_params[0] = vu0_data.length / 4;
    vu0_params[1] = 0x00ff00ff;

    for (n = 0; n < vu0_data.length; n++) { vu0_data[n] = 0; }

    Playstation2.vu0UploadCode(vu0_prog);
    Playstation2.vu0UploadData(0, vu0_params);
    Playstation2.vu0Start();
    while(Playstation2.vu0IsRunning()) { }
    Playstation2.vu0DownloadData(0, vu0_data);

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
    triangle.setPosition(1900.0f, 2100.0f, 2048.0f);
    picture.setPosition(2000.0f, 2100.0f, 2048.0f);

    for(n = 0; n < count; n++)
    {
      Playstation2.waitVsync();
      Playstation2.clearScreen();
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
      Playstation2.clearScreen();
    }
  }
}

