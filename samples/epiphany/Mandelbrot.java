
import net.mikekohn.java_grinder.Parallella;
import net.mikekohn.java_grinder.UserInterruptListener;

public class Mandelbrot implements UserInterruptListener
{
  static boolean start_signal = false;

  public static void main(String args[])
  {
    float real_start;
    float imaginary_start;
    float real_inc;
    int data_count;
    int core_id;
    int data;
    int picture_start, picture;
    int n;

    n = 0;

    core_id = Parallella.getCoreId();
    data = 2048 * core_id;
    picture_start = data + 1024;

    Parallella.setUserInterruptListener(true);

    while(true)
    {
      while(start_signal == false);

      picture = picture_start;

      // Get data set coordinates.
      real_start = Parallella.readSharedRamFloat(data);
      imaginary_start = Parallella.readSharedRamFloat(data + 4);
      real_inc = Parallella.readSharedRamFloat(data + 8);
      data_count = Parallella.readSharedRamInt(data + 12);

      for (n = 0; n < data_count; n++)
      {
        int count = 127;
        float zr = 0;
        float zi = 0;
        float tr, ti;

        while(count > 0)
        {
          tr = ((zr * zr) - (zi * zi));
          ti = (2 * zr * zi);
          zr = tr + real_start;
          zi = ti + imaginary_start;

          if ((zr * zr) + (zi * zi) > 4) { break; }
        }

        Parallella.writeSharedRamInt(picture, count >> 3);
        real_start += real_inc;

        picture++;
      }

      // Signal to the Parallella host core that this is done.
      Parallella.writeSharedRamInt(data + 16, 1);
    }
  }

  public void userInterrupt()
  {
    start_signal = true;
  }
}


