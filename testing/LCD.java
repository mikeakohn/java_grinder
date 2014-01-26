
import net.mikekohn.java_grinder.SPI0;
import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.IOPort0;

public class LCD
{
  // For Philips base Nokia 6100 LCD
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

  static final int LCD_RESET = 0x08;
  static final int SPI_CS = 0x10;
  static final int SPI_CLK = 0x20;
  static final int SPI_SDO = 0x40;

  static public void main(String args[])
  {
    int x=60,y=30;
    int dx=1,dy=1;
    int n;
    int del;

    // Set the DCO to 16MHz
    CPU.setClock16();

    // Not sure how fast this can be
    SPI0.init(SPI0.DIV8, 2);

    // Setup IO port.
    // Pin 3 is LCD reset
    // Pin 4 is /CS
    // Pin 5 is SCLK
    // Pin 6 is SDO
    IOPort0.setPinsAsOutput(LCD_RESET|SPI_CS|SPI_CLK|SPI_SDO);
    IOPort0.setPinsValue(LCD_RESET|SPI_CS|SPI_CLK);

    // Reset LCD
    IOPort0.setPinsLow(LCD_RESET);
    delay();
    IOPort0.setPinsHigh(LCD_RESET);
    delay();
    delay();

    // Wake up
    lcdCommand(SLEEPOUT);

    // Set contrast
    lcdCommand(SETCON);
    lcdData(0x40);

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

    while(true)
    {
      // Draw a red box
      setArea(x, y, x+23, y+23);
      for (n = 0; n < 24*24/2; n++)
      {
        lcdData(0x00);
        lcdData(0xf0);
        lcdData(0x0f);
      }

/*
      // Erase box
      setArea(x, y, x+23, y+23);
      for (n = 0; n < 24*24/2; n++)
      {
        lcdData(0x0f);
        lcdData(0x00);
        lcdData(0xf0);
      }
*/

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

      x += dx;
      y += dy;

      if (x >= 131-30) { dx = -1; }
      if (y >= 133-30) { dy = -1; }
      if (x == 0) { dx = 1; }
      if (y == 0) { dy = 1; }
    }
  }

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

  public static void clearDisplay()
  {
  int n;

    setArea(0,0, 131,131);

    for (n = 0; n < 132*132/2; n++)
    {
      lcdData(0x0f);
      lcdData(0x00);
      lcdData(0xf0);
    }
  }

  public static void lcdCommand(int a)
  {
  int n;
    IOPort0.setPinsLow(SPI_SDO);

    // /CS = 0
    IOPort0.setPinsLow(SPI_CS);
    clock();

    // Hardware clock out the rest of the bits
    SPI0.send(a);

    // /CS = 1
    IOPort0.setPinsHigh(SPI_CS);
  }

  public static void lcdData(int a)
  {
  int n;
    IOPort0.setPinsHigh(SPI_SDO);

    // /CS = 0
    IOPort0.setPinsLow(SPI_CS);
    clock();
    IOPort0.setPinsLow(SPI_SDO);

    // Hardware clock out the rest of the bits
    SPI0.send(a);

    // /CS = 1
    IOPort0.setPinsHigh(SPI_CS);
  }

  public static void clock()
  {
    // Manually clock out a bit to SPI
    SPI0.disable();
    IOPort0.setPinsLow(SPI_CLK);
    CPU.nop();
    CPU.nop();
    CPU.nop();
    CPU.nop();
    IOPort0.setPinsHigh(SPI_CLK);
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


