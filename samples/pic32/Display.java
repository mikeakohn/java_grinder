
import net.mikekohn.java_grinder.IOPort1;
import net.mikekohn.java_grinder.IOPort3;
import net.mikekohn.java_grinder.IOPort5;
import net.mikekohn.java_grinder.IOPort6;
import net.mikekohn.java_grinder.SPI1;

// 4Display-Shield-22
// Much of this code was copied from 4D Systems sample source code
// and translated to Java.

// ChipKit UNO's SPI:
// Pin 7  - /CS       (RD9)
// Pin 8  - RS        (RD10)
// Pin 9  - Backlight (RD3)
// Pin 11 - Data Out
// Pin 12 - Data In
// Pin 13 - SCLK
// Pin 14 - Reset     (RB2)

// Touch
// D15, D16, A3, A4

public class Display
{
  static public final int LCD_BACKLIGHT = 0x008;
  static public final int LCD_CS = 0x200;
  static public final int LCD_RS = 0x400;
  static public final int LCD_RESET = 0x04;

  static public final int WIDTH = 176;
  static public final int HEIGHT = 220;

  static public void send(int data)
  {
    // /CS on
    IOPort3.setPinsValue(LCD_RS|LCD_BACKLIGHT);

    SPI1.read16(data);

    // /CS off
    IOPort3.setPinsValue(LCD_RS|LCD_CS|LCD_BACKLIGHT);
  }

  static public void writeCommand(int command)
  {
    IOPort3.setPinsValue(LCD_CS|LCD_BACKLIGHT);
    IOPort3.setPinsValue(LCD_BACKLIGHT);
    SPI1.read16(command);
    IOPort3.setPinsValue(LCD_RS|LCD_CS|LCD_BACKLIGHT);
  }

  static public void writeData(int data)
  {
    IOPort3.setPinsValue(LCD_RS|LCD_BACKLIGHT);
    SPI1.read16(data);
    IOPort3.setPinsValue(LCD_RS|LCD_CS|LCD_BACKLIGHT);
  }

  static public void writeDisplay(int command, int data)
  {
    writeCommand(command);
    writeData(data);
  }

  static public void delay()
  {
    int n;
    for (n = 0; n < 20000; n++);
  }

  static public void initDisplay()
  {
    IOPort1.setPinsValue(0);
    delay();
    IOPort1.setPinsValue(LCD_RESET);
    delay();

    writeDisplay(0x10, 0x0000); // Set SAP,DSTB,STB
    writeDisplay(0x11, 0x0000); // Set APON,PON,AON,VCI1EN,VC
    writeDisplay(0x12, 0x0000); // Set BT,DC1,DC2,DC3
    writeDisplay(0x13, 0x0000); // Set GVDD
    writeDisplay(0x14, 0x0000); // Set VCOMH/VCOML voltage
    delay(); 

    // Power-on sequence
    writeDisplay(0x11, 0x0018); // Set APON,PON,AON,VCI1EN,VC
    writeDisplay(0x12, 0x6121); // Set BT,DC1,DC2,DC3
    writeDisplay(0x13, 0x006F); // Set GVDD   /*007F 0088 */
    writeDisplay(0x14, 0x495F); // Set VCOMH/VCOML voltage
    writeDisplay(0x10, 0x0800); // Set SAP,DSTB,STB
    delay();
    writeDisplay(0x11, 0x103B); // Set APON,PON,AON,VCI1EN,VC
    delay();

    writeDisplay(0x01, 0x011C); // set the display line number and direction
    writeDisplay(0x02, 0x0100); // set 1 line inversion
    writeDisplay(0x03, 0x1030); // set GRAM write direction and BGR=1.
    writeDisplay(0x07, 0x0000); // Display off
    writeDisplay(0x08, 0x0808); // set the back porch and front porch
    writeDisplay(0x0B, 0x1100); // set the clocks number per line
    writeDisplay(0x0C, 0x0000); // CPU interface
    writeDisplay(0x0F, 0x0D01); // Set Osc  /*0e01*/
    writeDisplay(0x15, 0x0020); // Set VCI recycling
    writeDisplay(0x20, 0x0000); // RAM Address
    writeDisplay(0x21, 0x0000); // RAM Address

    // GRAM area
    writeDisplay(0x30, 0x0000); 
    writeDisplay(0x31, 0x00DB); 
    writeDisplay(0x32, 0x0000); 
    writeDisplay(0x33, 0x0000); 
    writeDisplay(0x34, 0x00DB); 
    writeDisplay(0x35, 0x0000); 
    writeDisplay(0x36, 0x00AF); 
    writeDisplay(0x37, 0x0000); 
    writeDisplay(0x38, 0x00DB); 
    writeDisplay(0x39, 0x0000); 

    // Gamma 2.2 
    writeDisplay(0x50, 0x0000); 
    writeDisplay(0x51, 0x0808); 
    writeDisplay(0x52, 0x080A); 
    writeDisplay(0x53, 0x000A); 
    writeDisplay(0x54, 0x0A08); 
    writeDisplay(0x55, 0x0808); 
    writeDisplay(0x56, 0x0000); 
    writeDisplay(0x57, 0x0A00); 
    writeDisplay(0x58, 0x0710); 
    writeDisplay(0x59, 0x0710); 

    writeDisplay(0x07, 0x0012); 
    delay(); 
    writeDisplay(0x07, 0x1017);
  }

  static public void setWindow(int x0, int y0, int x1, int y1)
  {
    writeDisplay(0x36, x1);
    writeDisplay(0x37, x0);

    writeDisplay(0x38, y1);
    writeDisplay(0x39, y0);

    writeDisplay(0x20, x0);
    writeDisplay(0x21, y0);

    writeCommand(0x22);
  }

  static public void clearDisplay()
  {
    int n;

    setWindow(0, 0, WIDTH - 1, HEIGHT - 1);

    for (n = 0; n < WIDTH * HEIGHT; n++)
    {
      writeData(31);
    }
  }

  static public void main(String args[])
  {
    int n;

    IOPort1.setPinsAsOutput(0x04);
    IOPort3.setPinsAsOutput(0x608);
    IOPort5.setPinsAsOutput(0x01);
    IOPort6.setPinsAsOutput(0x40);

    IOPort1.setPinsValue(LCD_RESET);
    IOPort3.setPinsValue(LCD_RS|LCD_CS|LCD_BACKLIGHT);
    IOPort5.setPinsValue(0x01);
    //IOPort6.setPinsValue(0x40);

    SPI1.init16(SPI1.DIV128, 0);

    initDisplay();
    IOPort5.setPinsValue(0x00);
    clearDisplay();


    //send(0x3200);

    while(true);
  }
}


