
import net.mikekohn.java_grinder.IOPort0;
import net.mikekohn.java_grinder.IOPort1;
import net.mikekohn.java_grinder.IOPort2;
import net.mikekohn.java_grinder.ADC;

public class Breakout
{
  static public boolean[] bricks;
  static public int player = 3;
  static public int ball_x = 64, ball_y = 64;
  static public int dx = 4, dy = 4;

  static public byte[] ball_graphic_1 =
  {
    //15, -121, -61, -31, -16, -8, -4, -2
    //-2, -4, -8, -16, -31, -61, -121, 15

    //0xf0, 0xe1, 0xc3, 0x87, 0x0f, 0x07, 0x03, 0x01
    -16, -31, -61, -121, 15, 7, 3, 1 
  };

  static public byte[] ball_graphic_2 =
  {
    //-1, 127, 63, 31

    //0x01, 0x03, 0x07, 0x0f
    1, 3, 7, 15
  };

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

  static public void drawBricks(int row)
  {
    int index = row << 4;
    int line, n, i;

    line = 20 + (row << 3);

    for (i = 0 ; i < 7; i++)
    {
      IOPort1.setPinHigh(5);

      sendSPI(1);
      sendSPI(line++);

      for (n = 0; n < 8; n++)
      {
        if (bricks[index++])
        {
          sendSPI(0x00);
          sendSPI(0x80);
        }
          else
        {
          sendSPI(0xff);
          sendSPI(0xff);
        }
      }

      sendSPI(0);
      sendSPI(0);

      IOPort1.setPinLow(5);
    }
  }

  static public void drawPlayer()
  {
    int line, n, i;

    line = 120;

    for (i = 0 ; i < 7; i++)
    {
      IOPort1.setPinHigh(5);

      sendSPI(1);
      sendSPI(line++);

      for (n = 0; n < 8; n++)
      {
        if (n == player)
        {
          sendSPI(0x00);
          sendSPI(0x00);
        }
          else
        {
          sendSPI(0xff);
          sendSPI(0xff);
        }
      }

      sendSPI(0);
      sendSPI(0);

      IOPort1.setPinLow(5);
    }
  }

  static public void drawBall()
  {
    int line, n, i;

    line = ball_y;

    for (i = 0 ; i < 4; i++)
    {
      IOPort1.setPinHigh(5);

      sendSPI(1);
      sendSPI(line++);

      int pos_x = 0;

      for (n = 0; n < 16; n++)
      {
        if (ball_x >= pos_x && ball_x < pos_x + 8)
        {
          if ((ball_x & 1) == 0)
          {
            sendSPI(0x0f);
          }
            else
          {
            sendSPI(0xf0);
          }
        }
          else
        {
          sendSPI(0xff);
        }

        pos_x += 8;
      }

      sendSPI(0);
      sendSPI(0);

      IOPort1.setPinLow(5);
    }
  }

  static public void clearLine(int line)
  {
    int n, i, l;

    for (l = line; l < line + 4; l++)
    {
      for (i = 0 ; i < 7; i++)
      {
        IOPort1.setPinHigh(5);

        sendSPI(1);
        sendSPI(l);

        for (n = 0; n < 16; n++)
        {
          sendSPI(0xff);
        }

        sendSPI(0);
        sendSPI(0);

        IOPort1.setPinLow(5);
      }
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

  static public void reset()
  {
    int n;

    for (n = 0; n < 32; n++)
    {
      bricks[n] = true;
    }

    clearDisplay();

    drawBricks(0);
    drawBricks(1);
    drawBricks(2);
    drawBricks(3);

    drawPlayer();
  }

  static public void waitForButton()
  {
    while (true)
    {
      if (!IOPort0.isPinInputHigh(2))
      {
        return;
      }
    }
  }

  static public void play()
  {
    while (true)
    {
      int joystick = ADC.read();

      if (joystick > 300 && joystick < 320)
      {
        if (player > 0)
        {
          player--;
          drawPlayer();
        }
      }
        else
      if (joystick > 390 && joystick < 410)
      {
        if (player < 7)
        {
          player++;
          drawPlayer();
        }
      }

      int old_y = ball_y;

      ball_x = ball_x + dx;
      ball_y = ball_y + dy;

      clearLine(old_y);
      drawBall();

      if (ball_y == 52)
      {
        dy = 4;
      }
        else
      if (ball_y == 112)
      {
        dy = -4;
      }

      if (ball_x == 0)
      {
        dx = 4;
      }
        else
      if (ball_x >= 124)
      {
        dx = -4;
      }

      //IOPort2.setPinHigh(0);
      //IOPort2.setPinLow(1);
      //IOPort2.setPinHigh(2);

      //delay();

      //IOPort2.setPinLow(0);
      //IOPort2.setPinHigh(1);
      //IOPort2.setPinHigh(2);

      //delay();
    }
  }

  static public void main(String[] args)
  {
    int n;

    bricks = new boolean[8 * 4];

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

    // Setup ADC.
    ADC.enable();
    ADC.setChannel(15);

    // Setup push button on P0.2.
    IOPort0.setPinAsInput(2);

    reset();

    while (true)
    {
      waitForButton();
      play();
    }
  }
}

