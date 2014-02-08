
import net.mikekohn.java_grinder.SPI0;
import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.IOPort1;
import net.mikekohn.java_grinder.DSP;

// Unfortunately this has to be different than the MSP430 since
// different pins are being used to do this.

public class LCDDSPIC
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
  static final int LCD_RESET = 0x01;
  static final int SPI_CS = 0x10;
  static final int SPI_CLK = 0x02;
  static final int SPI_SDO = 0x08;

  static public void main(String args[])
  {
    int x=60,y=30;
    int dx=1,dy=1;
    int n;
    int del;

    // Set the DCO to 16MHz
    //CPU.setClock16();

    // Not sure how fast this can be
    SPI0.init(SPI0.DIV8, 2);

    // Setup IO port.
    // Pin 0 is LCD reset
    // Pin 1 is SCLK
    // Pin 3 is SDO
    // Pin 4 is /CS
    IOPort1.setPinsAsOutput(LCD_RESET|SPI_CS|SPI_CLK|SPI_SDO);
    IOPort1.setPinsValue(LCD_RESET|SPI_CS|SPI_CLK);

    // Reset LCD
    IOPort1.setPinsLow(LCD_RESET);
    delay();
    IOPort1.setPinsHigh(LCD_RESET);
    delay();
    delay();

    // Wake up
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

    delay();

    clearDisplay();
    mandel(-2<<8, 1<<8, -1<<8, 1<<8);

    while(true)
    {
      // Draw a blue box
      setArea(x, y, x+23, y+23);
      for (n = 0; n < 24*24/2; n++)
      {
        lcdData(0xf0);
        lcdData(0x0f);
        lcdData(0x00);
      }

      // FIXME - this fails for some reason.
      //del = (dx == 1) ? x : x+23;
      if (dx == 1) { del = x; }
      else { del = x+23; }

      setArea(del, y, del, y+23);
      for (n = 0; n < 24/2; n++)
      {
        lcdData(0x0f);
        lcdData(0x00);
        lcdData(0xf0);
      }

      // FIXME - this fails for some reason.
      //del = (dy == 1) ? y : y+23;
      if (dy == 1) { del = y; }
      else { del = y+23; }

      setArea(x, del, x+23, del);
      for (n = 0; n < 24/2; n++)
      {
        lcdData(0x0f);
        lcdData(0x00);
        lcdData(0xf0);
      }

      // Move the box by 1 pixel
      x += dx;
      y += dy;

      // If bounds are hit, change the direction of the box.
      if (x >= 131-30) { dx = -1; }
      if (y >= 133-30) { dy = -1; }
      if (x == 0) { dx = 1; }
      if (y == 0) { dy = 1; }
    }
  }

  static void mandel(int rs, int re, int is, int ie)
  {
    int zi,zr;
    //int tr,ti;
    int dx = (re - rs) / 60;
    int dy = (ie - is) / 22;
    int rs_save = rs;
    int count;
    int x,y;
    int t;

    for (y = 0; y < 22; y++)
    {
      rs = rs_save;
      for (x = 0; x < 60; x++)
      {
        zr = 0;
        zi = 0;
        //DSP.clearA();
        //DSP.clearB();

        count = 255;
        while(count >= 0)
        {
          //tr = ((zr * zr) >> 8) - ((zi * zi) >> 8);
          DSP.squareToA(zr);
          DSP.squareToB(zi);
          DSP.subABAndStoreInA();

          //ti = 2 * ((zr * zi) >> 8);
          DSP.mulToB(zr,zi);
          DSP.shiftB(-1);

          //zr = tr + rs;
          //zi = ti + is;
          zr = DSP.getLowerA() + rs;
          zi = DSP.getLowerB() + is;

          //if (((tr * tr) >> 8) + ((ti * ti) >> 8) > 4 << 8) { break; }
          DSP.squareToA(zr);
          DSP.squareAndAddToA(zi);
          t = DSP.getLowerA();
          if (t > 4 << 8) { break; }

          count--;
        }

        System.out.print(count / 32);
        rs += dx;
      }

      System.out.println();

      is += dy;
    }
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

  /** Send a 9 bit command to the LCD display.  Bit 8 is always a 0 for
      a command */
  public static void lcdCommand(int a)
  {
  int n;
    IOPort1.setPinsLow(SPI_SDO);

    // /CS = 0
    IOPort1.setPinsLow(SPI_CS);
    clock();

    // Hardware clock out the rest of the bits
    SPI0.send(a);
    while(!SPI0.isDataAvailable());

    // /CS = 1
    IOPort1.setPinsHigh(SPI_CS);
  }

  /** Send a 9 bit data byte to the LCD display.  Bit 8 is always a 1 for
      data */
  public static void lcdData(int a)
  {
  int n;
    IOPort1.setPinsHigh(SPI_SDO);

    // /CS = 0
    IOPort1.setPinsLow(SPI_CS);
    clock();
    IOPort1.setPinsLow(SPI_SDO);

    // Hardware clock out the rest of the bits
    SPI0.send(a);
    while(!SPI0.isDataAvailable());

    // /CS = 1
    IOPort1.setPinsHigh(SPI_CS);
  }

  public static void clock()
  {
    // Manually clock out a bit to SPI since MSP430 hardware only supports
    // 8 bit or 16 bit data.
    SPI0.disable();
    IOPort1.setPinsLow(SPI_CLK);
    CPU.nop();
    CPU.nop();
    CPU.nop();
    CPU.nop();
    IOPort1.setPinsHigh(SPI_CLK);
    CPU.nop();
    CPU.nop();
    CPU.nop();
    CPU.nop();
    SPI0.enable();
  }

  public static void delay()
  {
    int n,a;
    for (a = 0; a < 3; a++)
    {
      for (n = 0; n < 65535; n++) { }
    }
  }
}


