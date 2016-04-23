
import net.mikekohn.java_grinder.IOPort1;
import net.mikekohn.java_grinder.IOPort3;
import net.mikekohn.java_grinder.IOPort5;
import net.mikekohn.java_grinder.IOPort6;
import net.mikekohn.java_grinder.SPI1;

// 4Display-Shield-22
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

  static public void initDisplay()
  {
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
    IOPort6.setPinsValue(0x40);

    SPI1.init16(SPI1.DIV128, 0);

    // /CS on
    IOPort3.setPinsValue(LCD_RS|LCD_BACKLIGHT);

    SPI1.send16(0x3200);

    // /CS off
    IOPort3.setPinsValue(LCD_RS|LCD_CS|LCD_BACKLIGHT);

    while(true);
  }
}


