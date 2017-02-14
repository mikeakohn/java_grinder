
import net.mikekohn.java_grinder.ADC;
import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.IOPort0;
import net.mikekohn.java_grinder.SPI0;

public class Nokia5110
{
  // The masks for the IO pins used to communicate with the LCD.
  static final int LCD_RESET = 0;
  static final int LCD_DC = 1;
  static final int SPI_CS = 2;

/*
  static byte[] data = {
   0, 0, 0, 0, 0, 0, 0, 0, -128, -32, -16, -8, -8, -8, -4, -4,
   -4, -100, -100, -98, -98, -98, -2, -2, -2, -2, -2, -98, -98, -98, -100, -100,
   -4, -4, -4, -8, -8, -8, -16, -32, -128, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -5, -77, -89,
   -113, -97, -65, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   1, 7, 15, 31, 31, 31, 63, 63, 63, 63, 59, 115, 119, 119, 119, 119,
   119, 119, 119, 119, 119, 119, 51, 59, 63, 63, 63, 31, 31, 31, 15, 7
  };
*/

  static public void main(String args[])
  {
    int n;

    // Set the DCO to 16MHz
    CPU.setClock16();

    // Setup ADC
    ADC.enable();
    ADC.setChannel(4);

    // Not sure how fast this can be
    SPI0.init(SPI0.DIV8, 2);

    // Setup IO port.
    // Pin 1 is SPI_DC
    // Pin 2 is SPI_CS
    IOPort0.setPinsAsOutput((1<<SPI_CS)|(1<<LCD_DC)|(1<<LCD_RESET));
    IOPort0.setPinsHigh((1<<SPI_CS)|(1<<LCD_RESET));

    // Reset LCD
    IOPort0.setPinLow(LCD_RESET);
    delay();
    IOPort0.setPinHigh(LCD_RESET);
    delay();
    delay();

    // Power up mode, no extended instruction set, horizontal addressing
    // 0 0 1 0  0 P V H
    //lcdCommand(0x20);

    // Display control: DE = 10 normal mode
    // 0 0 0 0  1 D 0 E
    //lcdCommand(0x0c);

    lcdCommand(0x21); // Basic instruction set
    lcdCommand(0xb0); // Constrast
    lcdCommand(0x04); // Temp coefficient
    lcdCommand(0x13); // LCD bias mode
    lcdCommand(0x20);
    lcdCommand(0x0c);

    delay();

    clearDisplay();

    moveCursor(0, 0);
    for (n = 0; n < LittleBird.logo.length; n++)
    {
      lcdData(LittleBird.logo[n]);
    }

    while(true);
  }

  /** Set the address RAM pointer.  y must be a multiple of 8. */
  public static void moveCursor(int x, int y)
  {
    // Set X address of RAM
    lcdCommand(0x80 | x);

    // Set Y address of RAM  (multiple of 8 so div by 8)
    lcdCommand(0x40 | (y>>3));
  }

  /** Clear all pixels. */
  public static void clearDisplay()
  {
  int n;

    moveCursor(0,0);

    // Write out 12 bit color information.. 2 pixels at a time.
    for (n = 0; n < 84*6; n++)
    {
      lcdData(0x00);
    }
  }

  /** Send a 8 bit command to the LCD display.  LCD_DC is always a 0 for
      a command */
  public static void lcdCommand(int a)
  {
  int n;
    IOPort0.setPinLow(LCD_DC);

    // /CS = 0
    IOPort0.setPinLow(SPI_CS);

    // Hardware clock out the rest of the bits
    SPI0.read(a);

    // /CS = 1
    IOPort0.setPinHigh(SPI_CS);
  }

  /** Send a 8 bit data byte to the LCD display.  LCD_DC is always a 1 for
      data */
  public static void lcdData(int a)
  {
  int n;
    IOPort0.setPinHigh(LCD_DC);

    // /CS = 0
    IOPort0.setPinLow(SPI_CS);

    // Hardware clock out the rest of the bits
    SPI0.read(a);

    // /CS = 1
    IOPort0.setPinHigh(SPI_CS);
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


