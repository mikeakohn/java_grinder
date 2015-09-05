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

    AppleIIgs.hiresEnable();

    // make palette (api method coming soon when arrays are working)
    CPU.asm("lda #0xfff\n");
    CPU.asm("sta.l 0xe19e00\n");
    CPU.asm("lda #0xeee\n");
    CPU.asm("sta.l 0xe19e02\n");
    CPU.asm("lda #0xddd\n");
    CPU.asm("sta.l 0xe19e04\n");
    CPU.asm("lda #0xccc\n");
    CPU.asm("sta.l 0xe19e06\n");
    CPU.asm("lda #0xbbb\n");
    CPU.asm("sta.l 0xe19e08\n");
    CPU.asm("lda #0xaaa\n");
    CPU.asm("sta.l 0xe19e0a\n");
    CPU.asm("lda #0x999\n");
    CPU.asm("sta.l 0xe19e0c\n");
    CPU.asm("lda #0x888\n");
    CPU.asm("sta.l 0xe19e0e\n");
    CPU.asm("lda #0x777\n");
    CPU.asm("sta.l 0xe19e10\n");
    CPU.asm("lda #0x666\n");
    CPU.asm("sta.l 0xe19e12\n");
    CPU.asm("lda #0x555\n");
    CPU.asm("sta.l 0xe19e14\n");
    CPU.asm("lda #0x444\n");
    CPU.asm("sta.l 0xe19e16\n");
    CPU.asm("lda #0x333\n");
    CPU.asm("sta.l 0xe19e18\n");
    CPU.asm("lda #0x222\n");
    CPU.asm("sta.l 0xe19e1a\n");
    CPU.asm("lda #0x111\n");
    CPU.asm("sta.l 0xe19e1c\n");
    CPU.asm("lda #0x000\n");
    CPU.asm("sta.l 0xe19e1e\n");

    for(y = 0; y < 200 * 16; y += 16)
    {
      imc = (y - 100 * 16) >> 5;
      imc += imcen;

      int xx = 0;

      for(x = 0; x < 160 * 16; x += 16)
      {
        rec = (x - 80 * 16) >> 5;
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

        AppleIIgs.hiresPlot(address, (byte)(i | (i << 4)));

        xx++;
        address++;
      }

      yy++;
    }

    while(true)
    {
    }
  }
}

