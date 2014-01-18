
import net.mikekohn.java_grinder.SPI0;
import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.IOPort0;

public class LCD
{
  static final int DISON = 0xAF;
  static final int DISOFF = 0xAE;
  static final int DISNOR = 0xA6;
  static final int DISINV = 0xA7;
  static final int COMSCN = 0xBB;
  static final int DISCTL = 0xCA;
  static final int SLPIN = 0x95;
  static final int SLPOUT = 0x94;
  static final int PASET = 0x75;
  static final int CASET = 0x15;
  static final int DATCTL = 0xBC;
  static final int RGBSET8 = 0xCE;
  static final int RAMWR = 0x5C;
  static final int RAMRD = 0x5D;
  static final int PTLIN = 0xA8;
  static final int PTLOUT = 0xA9;
  static final int RMWIN = 0xE0;
  static final int RMWOUT = 0xEE;
  static final int ASCSET = 0xAA;
  static final int SCSTART = 0xAB;
  static final int OSCON = 0xD1;
  static final int PWRCTR = 0x20;
  static final int VOLCTR = 0x81;
  static final int VOLUP = 0xD6;
  static final int VOLDOWN = 0xD7;
  static final int TMPGRD = 0x82;
  static final int EPCTIN = 0xCD;
  static final int EPCOUT = 0xCC;
  static final int EPMWR = 0xFC;
  static final int EPMRD = 0xFD;
  static final int EPSRRD1 = 0x7C;
  static final int EPSRRD2 = 0x7D;
  static final int NO_OP = 0x25;

  static public void main(String args[])
  {
    int n;

    // Set the DCO to 16MHz
    CPU.setClock16();

    // Not sure how fast this can be
    SPI0.init(SPI0.DIV8, 0);

    // Setup IO port
    IOPort0.setPinsAsOutput(1);
    IOPort0.setPinsValue(0);

    // Reset LCD
    IOPort0.setPinsLow(1);
    delay();
    IOPort0.setPinsHigh(1);
    delay();

    SPI0.send(DISCTL);
    SPI0.send(0);
    SPI0.send(0x20);
    SPI0.send(0);

    SPI0.send(COMSCN);
    SPI0.send(1);

    SPI0.send(OSCON);
    SPI0.send(SLPOUT);

    SPI0.send(PWRCTR);
    SPI0.send(0x0f);

    SPI0.send(DISINV);
    SPI0.send(1);
    SPI0.send(0);
    SPI0.send(2);

    SPI0.send(VOLCTR);
    SPI0.send(32);
    SPI0.send(3);

    delay();

    drawImage();

    while(true);
  }

  public static void drawInit()
  {
    SPI0.send(DATCTL);
    SPI0.send(0);
    SPI0.send(0);
    SPI0.send(2);

    SPI0.send(PASET);
    SPI0.send(0);
    SPI0.send(131);

    SPI0.send(CASET);
    SPI0.send(0);
    SPI0.send(131);
  }

  public static void drawImage()
  {
  int n;

    drawInit();
    SPI0.send(RAMWR);

    for (n = 0; n < 50; n++)
    {
      SPI0.send(0xff);
    }
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


