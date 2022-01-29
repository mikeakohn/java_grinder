import net.mikekohn.java_grinder.W65C265SXB;
import net.mikekohn.java_grinder.IOPort5;

public class LCD
{
  // For Philips base Nokia 6100 LCD, we use these commands
  static final int NOP = 0x00;      // nop
  static final int SWRESET = 0x01;  // software reset
  static final int BSTROFF = 0x02;  // booster voltage OFF
  static final int BSTRON = 0x03;   // booster voltage ON
  static final int RDDIDIF = 0x04;  // read display identification
  static final int RDDST = 0x09;    // read display status
  static final int SLEEPIN = 0x10;  // sleep in
  static final int SLEEPOUT = 0x11; // sleep out
  static final int PTLON = 0x12;    // partial display mode
  static final int NORON = 0x13;    // display normal mode
  static final int INVOFF = 0x20;   // inversion OFF
  static final int INVON = 0x21;    // inversion ON
  static final int DALO = 0x22;     // all pixel OFF
  static final int DAL = 0x23;      // all pixel ON
  static final int SETCON = 0x25;   // write contrast
  static final int DISPOFF = 0x28;  // display OFF
  static final int DISPON = 0x29;   // display ON
  static final int CASET = 0x2A;    // column address set
  static final int PASET = 0x2B;    // page address set
  static final int RAMWR = 0x2C;    // memory write
  static final int RGBSET = 0x2D;   // colour set
  static final int PTLAR = 0x30;    // partial area
  static final int VSCRDEF = 0x33;  // vertical scrolling definition
  static final int TEOFF = 0x34;    // test mode
  static final int TEON = 0x35;     // test mode
  static final int MADCTL = 0x36;   // memory access control
  static final int SEP = 0x37;      // vertical scrolling start address
  static final int IDMOFF = 0x38;   // idle mode OFF
  static final int IDMON = 0x39;    // idle mode ON
  static final int COLMOD = 0x3A;   // interface pixel format
  static final int SETVOP = 0xB0;   // set Vop
  static final int BRS = 0xB4;      // bottom row swap
  static final int TRS = 0xB6;      // top row swap
  static final int DISCTR = 0xB9;   // display control
  static final int DOR = 0xBA;      // data order
  static final int TCDFE = 0xBD;    // enable/disable DF temp compensation
  static final int TCVOPE = 0xBF;   // enable/disable Vop temp comp
  static final int EC = 0xC0;       // internal or external oscillator
  static final int SETMUL = 0xC2;   // set multiplication factor
  static final int TCVOPAB = 0xC3;  // set TCVOP slopes A and B
  static final int TCVOPCD = 0xC4;  // set TCVOP slopes c and d
  static final int TCDF = 0xC5;     // set divider frequency
  static final int DF8COLOR = 0xC6; // set divider frequency 8-color mode
  static final int SETBS = 0xC7;    // set bias system
  static final int RDTEMP = 0xC8;   // temperature read back
  static final int NLI = 0xC9;      // n-line inversion
  static final int RDID1 = 0xDA;    // read ID1
  static final int RDID2 = 0xDB;    // read ID2
  static final int RDID3 = 0xDC;    // read ID3

  // The masks for the IO pins used to communicate with the LCD.
  static final int LCD_RESET = 0x08;
  static final int SPI_CS = 0x04;
  static final int SPI_CLK = 0x01;
  static final int SPI_SDO = 0x02;

  // color palette
  static int palette[] =
  {
    0xf0f, 0xf00, 0xf80, 0xff0, 0x0f0, 0x0ff, 0x08f, 0x00f,
    0x80f, 0xf0f, 0xf08, 0x000, 0x444, 0x888, 0xccc, 0xfff
  };

  public static void delay()
  {
    int i;

    for(i = 0; i < 10000; i++)
    {
    }
  } 

  /** Send a 9 bit command to the LCD display.  Bit 8 is always a 0 for
      a command */
  public static void lcdCommand(int a)
  {
    int i;

    IOPort5.setPinsHigh(SPI_CS);
    IOPort5.setPinsLow(SPI_CLK);
    IOPort5.setPinsLow(SPI_CS);

    for(i = 0; i < 9; i++)
    {
      if((a & 0x100) > 0)
        IOPort5.setPinsHigh(SPI_SDO);
      else
        IOPort5.setPinsLow(SPI_SDO);

      IOPort5.setPinsHigh(SPI_CLK);
      IOPort5.setPinsLow(SPI_CLK);

      a <<= 1;
    }

    IOPort5.setPinsHigh(SPI_CS);
  }

  /** Send a 9 bit data byte to the LCD display.  Bit 8 is always a 1 for
      data */
  public static void lcdData(int a)
  {
    int i;

    a |= 0x100;

    IOPort5.setPinsHigh(SPI_CS);
    IOPort5.setPinsLow(SPI_CLK);
    IOPort5.setPinsLow(SPI_CS);

    for(i = 0; i < 9; i++)
    {
      if((a & 0x100) > 0)
        IOPort5.setPinsHigh(SPI_SDO);
      else
        IOPort5.setPinsLow(SPI_SDO);

      IOPort5.setPinsHigh(SPI_CLK);
      IOPort5.setPinsLow(SPI_CLK);

      a <<= 1;
    }

    IOPort5.setPinsHigh(SPI_CS);
  }

  /** This function tells the LCD (x0,y0) to (x1,y1) area to draw
      into.  The next data bytes written to the LCD will be 12 bit
      color information */
  public static void setArea(int x0, int y0, int x1, int y1)
  {
    // Page Address Set
    lcdCommand(PASET);
    lcdData(x0);
    lcdData(x1);

    // Column Address Set
    lcdCommand(CASET);
    lcdData(y0);
    lcdData(y1);

    // Ready to write
    lcdCommand(RAMWR);
  }

  /** Draw green pixels over the whole display. */
  public static void clearDisplay()
  {
  int n;

    setArea(0,0, 131,131);

    // Write out 12 bit color information.. 2 pixels at a time.
    for (n = 0; n < 132*132/2; n++)
    {
      lcdData(0x0f);
      lcdData(0x00);
      lcdData(0xf0);
    }
  }

  public static void mandel()
  {
    int recen = -8;
    int imcen = 0;
    int re, im, re2, im2, rec, imc;
    int x, y, xx, yy, i;
    int last, result;

    setArea(0,0, 131,131);

    yy = 0;

    for(y = 0; y < 132 * 16; y += 16, yy++)
    {
      imc = (y - 66 * 16) >> 5;
      imc += imcen;

      xx = 0;
      last = 0;

      for(x = 0; x < 132 * 16; x += 16, xx++)
      {
        rec = (x - 66 * 16) >> 5;
        rec += recen;

        re = rec;
        im = imc;

        re2 = (re * re) >> 4;
        im2 = (im * im) >> 4;

        for(i = 1; i < 11; i++)
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
          int temp1 = palette[last];
          int temp2 = palette[i];
//          result = (palette[last] << 12) | (palette[i]);
//          lcdData((result >> 16) & 0xff);
//          lcdData((result >> 8) & 0xff);
//          lcdData((result & 0xff));
          lcdData((temp1 >> 4) & 0xff);
          lcdData(((temp1 & 0xf) << 4) | ((temp2 >> 8) & 0xf));
          lcdData(temp2 & 0xff);
        }

        last = i;
      }
    }
  }

//WriteSpiData((color >> 4) & 0xFF);
//WriteSpiData(((color & 0xF) << 4) | ((color >> 8) & 0xF));
//WriteSpiData(color & 0xFF);
  public static void main(String[] args)
  {
    IOPort5.setPinsAsOutput(LCD_RESET|SPI_CS|SPI_CLK|SPI_SDO);
    IOPort5.setPinsValue(LCD_RESET|SPI_CS|SPI_CLK);

    IOPort5.setPinsLow(LCD_RESET);
//    delay();
    IOPort5.setPinsHigh(LCD_RESET);
//    delay();

    lcdCommand(SLEEPOUT);
    // Set contrast
    lcdCommand(SETCON);
    lcdData(0x30);

    // Set color mode (12 bit)
    lcdCommand(COLMOD);
    lcdData(0x03);

    // Reverse some stuff
    lcdCommand(MADCTL);
    lcdData(0xc8);

    // Display On (should already be on by reset, but owell)
    lcdCommand(DISPON);
    mandel();

    W65C265SXB.putString("\ngot here\n");
  }
}

