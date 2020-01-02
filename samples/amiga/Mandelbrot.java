
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.amiga.Amiga;
import net.mikekohn.java_grinder.amiga.Blitter;
import net.mikekohn.java_grinder.amiga.Copper;

public class Mandelbrot
{
  public static short[] colors =
  {
    0x000,  // 0
    0x007,  // 1
    0x00c,  // 2
    0x00f,  // 3
    0x30c,  // 4
    0xc37,  // 5
    0xf73,  // 6
    0xff3,  // 7
    0xfc7,  // 8
    0xf7c,  // 9
    0xfc7,  // 10
    0xff3,  // 11
    0xfc0,  // 12
    0xf70,  // 13
    0xf30,  // 14
    0xf00,  // 15
  };

  public static void run(Copper copper, Blitter blitter)
  {
    int n;
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
    int dx = (r1 - r0) / 320;
    int dy = (i1 - i0) / 200;
    byte[] bitplanes = Display.bitplanes;
    int bit, index;

    index = 0;
    bit = 128;

    Display.clear();
    Display.setDisplay(copper, 4);

    for (n = 0; n < 16; n++)
    {
      Display.setPalette(copper, n, colors[n]);
    }

    blitter.setModuloA(0);
    //blitter.setModuloDestination(31);
    blitter.setLogicalFunction(0xf0);
    blitter.enableChannels(Blitter.MASK_ENABLE_A | Blitter.MASK_ENABLE_D);
    blitter.setChannelAMasks(0xffff, 0xffff);
    blitter.setAsFillMode(Blitter.FILL_OPTIONS_NONE);
    blitter.setDataRegisterA((char)0);
    blitter.setDataRegisterB((char)0);
    blitter.setDataRegisterC((char)0);

    ImageJavaGrinderText.init(blitter);
    ImageJavaGrinderText.show(blitter);
    ImageCodedUsing.init(blitter);
    ImageCodedUsing.show(blitter);

    is = i0;

    for (y = 0; y < 200; y++)
    {
      rs = r0;

      for (x = 0; x < 320; x++)
      {
        zr = zi = 0;
        //zi = 0;

        for (count = 0; count < 16; count++)
        {
          zr2 = (zr * zr) >> DEC_PLACE;
          zi2 = (zi * zi) >> DEC_PLACE;

          if (zr2 + zi2 > (4 << DEC_PLACE)) { break; }
          tr = zr2 - zi2;
          ti = ((zr * zi) >> DEC_PLACE) << 1;

          zr = tr + rs;
          zi = ti + is;
        }

        if (count < 15)
        {
          //Display.plot(x, y, count);

          if (count != 0)
          {
            if ((count & 1) != 0) { bitplanes[index] |= bit; }
            if ((count & 2) != 0) { bitplanes[index + 8000] |= bit; }
            if ((count & 4) != 0) { bitplanes[index + 16000] |= bit; }
            if ((count & 8) != 0) { bitplanes[index + 24000] |= bit; }
          }
        }

        if (bit == 1)
        {
          bit = 128;
          index++;
        }
          else
        {
          bit = bit >> 1;
        }

        rs += dx;
      }

      is += dy;
    }
  }
}

