
import net.mikekohn.java_grinder.ADC;
import net.mikekohn.java_grinder.IOPort0;
import net.mikekohn.java_grinder.IOPort1;
import net.mikekohn.java_grinder.IOPort2;
import net.mikekohn.java_grinder.SPI1;

public class Breakout
{
  static public boolean[] bricks;
  static public int player = 3;
  static public int ball_x = 64, ball_y = 64;
  static public int dx = 4, dy = 4;
  static public int dirty_row = -1;
  static public int hits = 0;

  static public byte[] reverse_table =
  {
     0, -128, 64, -64, 32, -96, 96, -32, 16, -112, 80, -48, 48,
   -80, 112, -16, 8, -120, 72, -56, 40, -88, 104, -24, 24, -104,
    88, -40, 56, -72, 120, -8, 4, -124, 68, -60, 36, -92, 100,
   -28, 20, -108, 84, -44, 52, -76, 116, -12, 12, -116, 76, -52,
    44, -84, 108, -20, 28, -100, 92, -36, 60, -68, 124, -4, 2,
  -126, 66, -62, 34, -94, 98, -30, 18, -110, 82, -46, 50, -78, 114,
   -14, 10, -118, 74, -54, 42, -86, 106, -22, 26, -102, 90, -38, 58,
   -70, 122, -6, 6, -122, 70, -58, 38, -90, 102, -26, 22, -106, 86,
   -42, 54, -74, 118, -10, 14, -114, 78, -50, 46, -82, 110, -18, 30,
   -98, 94, -34, 62, -66, 126, -2
  };

  static public byte[] game_over =
  {
     -8, 62, -9, -48, 31, -16, 23, -48, 16, 31, -9, -35, 115,
   -105, -1, -9, -41, -41, -9, -49, -9, -5, -75, 87, -1, -9,
    -41, -41, -9, -49, -10, 23, -42, -48, 127, -9, -41, -48,
    112, 31, -9, -48, 23, -41, -1, -9, -37, -73, -9, 63, -9,
    -41, -41, -41, -1, -9, -35, 119, -9, -97, -8, 55, -41, -48,
     31, -16, 30, -16, 23, -49
  };

  static public void sendSPI(int value)
  {
    SPI1.send(value);
    while (SPI1.isBusy());
  }

  static public void drawBricks(int row)
  {
    int index = row << 3;
    int line, n, i;

    line = 20 + (row << 3);

    for (i = 0 ; i < 7; i++)
    {
      int brick = index;
      IOPort1.setPinHigh(5);

      sendSPI(0x80);
      sendSPI(reverse_table[line++]);

      for (n = 0; n < 8; n++)
      {
        if (bricks[brick++])
        {
          sendSPI(0x80);
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

  static public void drawBricksAndBall(int row)
  {
    int index = row << 3;
    int line, n, i;
    int ball_index = ball_x >> 4;
    int ball_value_1 = 0xff;
    int ball_value_2 = 0xff;
    int ball_pos = ball_y & 3;

    dirty_row = row;

    if (ball_pos == 0)
    {
      ball_value_1 = 0x0f;
    }
      else
    if (ball_pos == 1)
    {
      ball_value_1 = 0x0f;
    }
      else
    if (ball_pos == 2)
    {
      ball_value_2 = 0x0f;
    }
      else
    if (ball_pos == 3)
    {
      ball_value_2 = 0xf0;
    }

    line = 20 + (row << 3);

    for (i = 0 ; i < 7; i++)
    {
      int brick = index;
      IOPort1.setPinHigh(5);

      sendSPI(0x80);
      sendSPI(reverse_table[line++]);

      for (n = 0; n < 8; n++)
      {
        if (n == ball_index)
        {
          sendSPI(ball_value_1);
          sendSPI(ball_value_2);
        }
          else
        if (bricks[brick])
        {
          sendSPI(0x80);
          sendSPI(0x00);
        }
          else
        {
          sendSPI(0xff);
          sendSPI(0xff);
        }

        brick++;
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

      sendSPI(0x80);
      sendSPI(reverse_table[line++]);

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

      sendSPI(0x80);
      sendSPI(reverse_table[line++]);

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

  static public void drawGameOver()
  {
    int line, n, i;
    int index = 0;

    line = 80;

    for (i = 0 ; i < 7; i++)
    {
      IOPort1.setPinHigh(5);

      sendSPI(0x80);
      sendSPI(reverse_table[line++]);

      for (n = 0; n < 16; n++)
      {
        if (n >= 3 && n <13)
        {
          sendSPI(game_over[index++]);
        }
          else
        {
          sendSPI(0xff);
        }
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
      IOPort1.setPinHigh(5);

      sendSPI(0x80);
      sendSPI(reverse_table[l]);

      for (n = 0; n < 16; n++)
      {
        sendSPI(0xff);
      }

      sendSPI(0);
      sendSPI(0);

      IOPort1.setPinLow(5);
    }
  }

/*
  static public int reverse(int value)
  {
    int r = 0;
    int b = 0x80;

    while (value != 0)
    {
      if ((value & 1) != 0)
      {
        r |= b;
      }

      value = value >> 1;
      b = b >> 1;
    }

    return r;
  }
*/

  static public void clearDisplay()
  {
    IOPort1.setPinHigh(5);
    sendSPI(0x20);
    sendSPI(0x00);
    IOPort1.setPinLow(5);
  }

  static public void delay()
  {
    for (int n = 0; n < 2000; n++);
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

    dy = -4;
    ball_y = 112;
    ball_x = (player << 4) + 4;
  }

  static public void waitForButton()
  {
    drawGameOver();

    while (true)
    {
      readJoystick();
      delay();

      if (!IOPort0.isPinInputHigh(3))
      {
        return;
      }
    }
  }

  static public void calcBrickHit(int old_y)
  {
    int index = ball_x >> 4;

    if (ball_y >= 44)
    {
      if (bricks[24 + index])
      {
        bricks[24 + index] = false;
        dy = -dy;

        if ((ball_y & 1) == 0) { dx = -dx; }
      }

      drawBricksAndBall(3);
      return;
    }
      else
    if (ball_y >= 36)
    {
      if (bricks[16 + index])
      {
        bricks[16 + index] = false;
        dy = -dy;

        if ((ball_y & 1) == 0) { dx = -dx; }
      }

      drawBricksAndBall(2);
      return;
    }
      else
    if (ball_y >= 28)
    {
      if (bricks[8 + index])
      {
        bricks[8 + index] = false;
        dy = -dy;

        if ((ball_y & 1) == 0) { dx = -dx; }
      }

      drawBricksAndBall(1);
      return;
    }
      else
    if (ball_y >= 20)
    {
      if (bricks[index])
      {
        bricks[index] = false;
        dy = -dy;

        if ((ball_y & 1) == 0) { dx = -dx; }
      }

      drawBricksAndBall(0);
      return;
    }

    if (ball_y == 4)
    {
      dy = 4;
    }

    if (ball_y == 16 && dy < 0)
    {
      drawBricks(0);
    }

    drawBall();
  }

  static public void readJoystick()
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
  }

  static public void play()
  {
    while (true)
    {
      readJoystick();

      int old_y = ball_y;

      ball_x = ball_x + dx;
      ball_y = ball_y + dy;

      if (old_y >= 52 || old_y < 20)
      {
        clearLine(old_y);
      }

      if (dirty_row != -1)
      {
        drawBricks(dirty_row);
        dirty_row = -1;
      }

      if (ball_y < 52)
      {
        calcBrickHit(old_y);
        //dy = 4;
      }
        else
      if (ball_y == 116)
      {
        dy = -4;

        // Check if the ball fell through the bottom.
        int x = player << 4;

        if (ball_x < x || ball_x > x + 16)
        {
          clearLine(ball_y);
          return;
        }

        if (player > 1 && player < 7)
        {
          int a = (ball_x - x);

          if (a < 8)
          {
            dx = -4;
          }
            else
          {
            dx = 4;
          }
        }

        if ((hits & 1) == 0) { dx = -dx; }

        hits++;
      }
        else
      {
        drawBall();
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

      delay();
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

    // Setup SPI1.
    SPI1.enable();
    SPI1.init(11, 0);

    // Setup push button on P0.2.
    IOPort0.setPinAsInput(3);

    reset();

    while (true)
    {
      waitForButton();
      reset();
      play();
      //IOPort2.setPinLow(0);
    }
  }
}

