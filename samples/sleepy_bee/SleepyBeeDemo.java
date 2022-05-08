
import net.mikekohn.java_grinder.IOPort1;
import net.mikekohn.java_grinder.IOPort2;

public class SleepyBeeDemo
{
  static public int[] blah = { 8, 1932, 1231, 23 };
  static public byte[] bricks = { 9, 100, 99 };
  static int test = 1000;
  static byte global = 2;

  // Software SPI sending bits in reverse order (least significant bit first).
  static public void sendSPI(int value)
  {
    int n = 8;

    while (n != 0)
    {
      if ((value & 0x01) == 0)
      {
        IOPort1.setPinLow(2);
      }
        else
      {
        IOPort1.setPinHigh(2);
      }

      IOPort1.setPinHigh(0);
      value = value >> 1;
      IOPort1.setPinLow(0);

      n--;
    }

    IOPort1.setPinLow(2);
    IOPort1.setPinLow(0);
  }

  static public void draw()
  {
    int line, n;

    for (line = 0 ; line < 128; line++)
    {
      IOPort1.setPinHigh(5);

      sendSPI(1);
      sendSPI(line);

      for (n = 0; n < 16; n++)
      {
        sendSPI(0x0f);
      }

      sendSPI(0);
      sendSPI(0);

      IOPort1.setPinLow(5);
    }
  }

  static public void clearDisplay()
  {
    IOPort1.setPinHigh(5);
    sendSPI(0x04);
    sendSPI(0x00);
    IOPort1.setPinLow(5);
  }

  static public void delay()
  {
    for (int n = 0; n < 20000; n++);
  }

  static public void main(String[] args)
  {
    int n;

    // SPI for LCD is P1.0 (CLK), P1.2 (MOSI), P1.5 (CS)
    // Doing a software SPI here because the LCD expects the data
    // to be in reverse order (LSb first instead of MSb first).
    IOPort1.setPinsAsOutput(0x25);
    IOPort1.setPinLow(5);
    IOPort1.setPinLow(2);
    IOPort1.setPinLow(0);

    // RGB LED is P2.0, P2.1, P2.2.
    // EFM_DISP_ENABLE for LCD display is P2.6.
    IOPort2.setPinsAsOutput(0x47);
    IOPort2.setPinHigh(6);

    clearDisplay();
    draw();

    //blah[1] = 1;

    //if (blah[1] == 1)

    if (test > 1000)
    {
      IOPort2.setPinLow(2);
    }
      else
    {
      IOPort2.setPinHigh(2);
    }

    while (true)
    {
      IOPort2.setPinHigh(0);
      //IOPort2.setPinLow(1);
      //IOPort2.setPinHigh(2);

      delay();

      IOPort2.setPinLow(0);
      //IOPort2.setPinHigh(1);
      //IOPort2.setPinHigh(2);

      delay();
    }
  }
}

