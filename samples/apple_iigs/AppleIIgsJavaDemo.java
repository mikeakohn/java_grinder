import net.mikekohn.java_grinder.AppleIIgs;
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.CPU;

public class AppleIIgsJavaDemo
{
  public static void main()
  {
    int x, y, i;
    int recen = -8;
    int imcen = 0;
    int re, im, re2, im2, rec, imc;
    int yy = 0;
    int address = 0x2000;

    AppleIIgs.setBank((byte)0xe1);
    CPU.asm("sep #0x30\n");
    CPU.asm("lda.b #10000001b\n");
    CPU.asm("sta 0xc029\n");
    CPU.asm("lda.b #0x000\n");
//    CPU.asm("sta 0xc022\n");
    CPU.asm("sta 0xc034\n");
    CPU.asm("rep #0x30\n");

    CPU.asm("lda #0xfff\n");
    CPU.asm("sta 0x9e00\n");
    CPU.asm("lda #0xeee\n");
    CPU.asm("sta 0x9e02\n");
    CPU.asm("lda #0xddd\n");
    CPU.asm("sta 0x9e04\n");
    CPU.asm("lda #0xccc\n");
    CPU.asm("sta 0x9e06\n");
    CPU.asm("lda #0xbbb\n");
    CPU.asm("sta 0x9e08\n");
    CPU.asm("lda #0xaaa\n");
    CPU.asm("sta 0x9e0a\n");
    CPU.asm("lda #0x999\n");
    CPU.asm("sta 0x9e0c\n");
    CPU.asm("lda #0x888\n");
    CPU.asm("sta 0x9e0e\n");
    CPU.asm("lda #0x777\n");
    CPU.asm("sta 0x9e10\n");
    CPU.asm("lda #0x666\n");
    CPU.asm("sta 0x9e12\n");
    CPU.asm("lda #0x555\n");
    CPU.asm("sta 0x9e14\n");
    CPU.asm("lda #0x444\n");
    CPU.asm("sta 0x9e16\n");
    CPU.asm("lda #0x333\n");
    CPU.asm("sta 0x9e18\n");
    CPU.asm("lda #0x222\n");
    CPU.asm("sta 0x9e1a\n");
    CPU.asm("lda #0x111\n");
    CPU.asm("sta 0x9e1c\n");
    CPU.asm("lda #0x000\n");
    CPU.asm("sta 0x9e1e\n");
    AppleIIgs.setBank((byte)0x00);

//    AppleIIgs.printChar((char)13);

    for(y = 0; y < 200 * 16; y += 16)
    {
      imc = (y - 100 * 16) >> 5;
      imc += imcen;

      int xx = 0;
      int last = 0;

      for(x = 0; x < 320 * 16; x += 16)
      {
        rec = (x - 160 * 16) >> 6;
        rec += recen;

        re = rec;
        im = imc;

        re2 = (re * re) >> 4;
        im2 = (im * im) >> 4;

        for(i = 0; i < 15; i++)
        {
          if((re2 + im2) > 4 * 16)
            break;

          im = (re * im) >> 3;
          im += imc;

          re = (re2 - im2) + rec;

          re2 = (re * re) >> 4;
          im2 = (im * im) >> 4;
        }

        if((xx & 1) == 1)
        {
          Memory.write8(0x16, (byte)(i | (last << 4)));
          CPU.asm("sep #0x30\n");
          CPU.asm("lda.b 0x16\n");
          CPU.asm("pos:\n");
          CPU.asm("sta.l 0xe12000\n");
          CPU.asm("rep #0x30\n");
          CPU.asm("inc pos + 1\n");
          address++;
        }
        else
        {
          last = i;
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

