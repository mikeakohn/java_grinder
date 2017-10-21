
import net.mikekohn.java_grinder.IOPort0;

public class Mandelbrot
{
  static final int DATA = 1;
  static final int CLK = 2;
  static final int CS = 4;
  static final int DC = 8;
  static final int RES = 16;

  public static int[] colors =
  {
    0x0000,
    0x000c,
    0x0013,
    0x0015,
    0x0195,
    0x0335,
    0x04d5,
    0x34c0,
    0x64c0,
    0x9cc0,
    0x6320,
    0xa980,
    0xaaa0,
    0xcaa0,
    0xe980,
    0xf800,
  };

  static public void main(String args[])
  {
    final int DEC_PLACE = 10;
    int x,y;
    int rs,is;
    int zi,zr;
    int tr,ti;
    int zr2,zi2;
    int count;
    final int r0 = (-2 << DEC_PLACE);
    final int i0 = (-1 << DEC_PLACE);
    final int r1 = (1 << DEC_PLACE);
    final int i1 = (1 << DEC_PLACE);
    int dx = (r1 - r0) / 96;
    int dy = (i1 - i0) / 64;
    int i;

    IOPort0.setPinsAsOutput(0x1f | (1 << 26) | (1 << 27));
    IOPort0.setPinsHigh(1 << 26);

    is = i0;

    for (y = 0; y < 64; y++)
    {
      rs = r0;

      for (x = 0; x < 96; x++)
      {
        zr = 0;
        zi = 0;

        count = 127;

        while(count != 0)
        {
          zr2 = (zr * zr) >> DEC_PLACE;
          zi2 = (zi * zi) >> DEC_PLACE;

          if (zr2 + zi2 > (4 << DEC_PLACE)) { break; }

          tr = zr2 - zi2;
          ti = 2 * ((zr * zi) >> DEC_PLACE);

          zr = tr + rs;
          zi = ti + is;

          count--;
        }

        int color = colors[count >> 3];

        sendData(color >> 8);
        sendData(color & 0xff);

        rs += dx;
      }

      is += dy;
    }

    while(true)
    {
      for (i = 0; i < 100000; i++);
      IOPort0.setPinsLow(1 << 26);
      IOPort0.setPinsHigh(1 << 27);

      for (i = 0; i < 100000; i++);
      IOPort0.setPinsHigh(1 << 26);
      IOPort0.setPinsLow(1 << 27);
    }
  }

  static public void sendData(int data)
  {
    int i;

    IOPort0.setPinsHigh(DC);
    IOPort0.setPinsLow(CS);

    for (i = 0; i < 8; i++)
    {
      if ((data & 0x80) != 0)
      {
        IOPort0.setPinsHigh(DATA);
      }
        else
      {
        IOPort0.setPinsLow(DATA);
      }

      IOPort0.setPinsLow(CLK);
      data = data << 1;
      IOPort0.setPinsHigh(CLK);
    }

    IOPort0.setPinsHigh(CS);
  }
}

