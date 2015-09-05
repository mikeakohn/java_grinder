import net.mikekohn.java_grinder.AppleIIgs;

public class AppleIIgsJavaDemo
{
  public static void main()
  {
    int x, y, i;
    int recen = -8;
    int imcen = 0;
    int re, im, re2, im2, rec, imc;
    int yy = 0;

    AppleIIgs.printChar((char)13);

    for(y = 0; y < 20 * 16; y += 16)
    {
      imc = (y - 10 * 16) >> 4;
      imc += imcen;

      int xx = 0;

      for(x = 0; x < 40 * 16; x += 16)
      {
        rec = (x - 20 * 16) >> 4;
        rec += recen;

        re = rec;
        im = imc;

        re2 = (re * re) >> 4;
        im2 = (im * im) >> 4;

        for(i = 0; i < 16; i++)
        {
          if((re2 + im2) > 4 * 16)
            break;

          im = (re * im) >> 3;
          im += imc;

          re = (re2 - im2) + rec;

          re2 = (re * re) >> 4;
          im2 = (im * im) >> 4;
        }

        if(i == 16)
        {
          AppleIIgs.printChar(' ');
        }
        else
        {
          AppleIIgs.printChar('*');
        }

        xx++;
      }

      yy++;
    }

    while(true)
    {
    }
  }
}

