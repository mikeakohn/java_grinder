import net.mikekohn.java_grinder.C64.*;

public class Mandel
{
  public static void main(String args[])
  {
    int x, y, i;
    int recen = -28;
    int imcen = 0;
    int re;
    int im;
    int re2;
    int im2;
    int rec;
    int imc;
    int s = 6;
    int loc = 55296;

    VIC.background(0);
    VIC.border(11);

    for(i = 1024; i < 2024; i++)
      VIC.poke(i, 160);
    for(i = 55296; i < 56296; i++)
      VIC.poke(i, 0);

    for(y = 0; y < 25 * 64; y += 64)
    {
      imc = ((s * (y - 12 * 64)) >> 6) + imcen;

      for(x = 0; x < 40 * 64; x += 64)
      {
        rec = ((s * (x - 20 * 64)) >> 6) + recen;

        re = rec;
        im = imc;

        re2 = (re * re) >> 6;
        im2 = (im * im) >> 6;

        for(i = 0; i < 32; i++)
        {
          if((re2 + im2) > 4 * 64)
            break;

          im = (re * im) >> 6;
          im <<= 1;
          im += imc;

          re = (re2 - im2) + rec;

          re2 = (re * re) >> 6;
          im2 = (im * im) >> 6;
        }

        VIC.poke(loc++, i & 15);
      }
    }

    while(true);
  }
}

