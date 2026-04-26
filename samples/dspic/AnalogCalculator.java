
import net.mikekohn.java_grinder.ADC;
import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.IOPort0;
import net.mikekohn.java_grinder.IOPort1;
import net.mikekohn.java_grinder.IOPort2;
import net.mikekohn.java_grinder.IOPort3;
import net.mikekohn.java_grinder.Timer;
import net.mikekohn.java_grinder.TimerListener;
import net.mikekohn.java_grinder.UART;
import net.mikekohn.java_grinder.UART1;

public class AnalogCalculator implements TimerListener
{
  static public void main(String[] args)
  {
    int value0 = 0;
    int value1 = 0;
    int operator = ' ';

    // RD13: /CS 0
    // RB0:  /CS 1
    // RB1:  /CS 2
    // RA1:  SCK
    // RD1:  SDI
    // RD10: LED
    // RD8:  Analog memory cell High-Z MOSFET control.
    IOPort3.setPinAsOutput(13);
    IOPort1.setPinAsOutput(0);
    IOPort1.setPinAsOutput(1);
    IOPort0.setPinAsOutput(1);
    IOPort3.setPinAsOutput(1);
    IOPort3.setPinAsOutput(10);
    IOPort3.setPinAsOutput(8);

    // Disconnect memory cell.
    IOPort3.setPinLow(8);

    // Init software SPI.
    IOPort3.setPinHigh(13);
    IOPort1.setPinHigh(0);
    IOPort1.setPinHigh(1);
    IOPort0.setPinLow(1);
    IOPort3.setPinLow(1);

    // Init UART.
    UART1.init(UART.BAUD_9600);

    // AN0:  RA0  (sum)
    // AN1:  RB2
    // AN8:  RB3
    // AN10: RB8
    // AN11: RB9
    // AN12: RC0  (sub)
    // AN13: RC1  (memory)
    // AN14: RC2
    ADC.setChannel(0);
    ADC.enable();

    // Setup timer.
    Timer.setInterval(200, 3);
    Timer.setListener(true);

    sendPrompt();

    //int i = 0;

    while (true)
    {
/*
      if ((i & 1) == 0)
      {
        IOPort3.setPinHigh(10);
      }
        else
      {
        IOPort3.setPinLow(10);
      }
*/

      while (!UART1.isDataAvailable());

      int data = UART1.read();

      if (data >= '0' && data <= '9')
      {
        if (operator == ' ')
        {
          value0 = value0 * 10;
          value0 += data - '0';
        }
          else
        {
          value1 = value1 * 10;
          value1 += data - '0';
        }

        sendChar(data);
      }
        else
      if (data == '+')
      {
        operator = '+';
        sendChar(data);
      }
        else
      if (data == '-')
      {
        operator = '-';
        sendChar(data);
      }
        else
      if (data == '.')
      {
        sendChar(data);
        CPU.interruptDisable();
        //clearMemory();
        writeMemory(value0);
        CPU.interruptEnable();
        sendPrompt();
        value0 = 0;
        value1 = 0;
      }
        else
      if (data == '=')
      {
        sendChar(data);
        CPU.interruptDisable();
        int value = readMemory();
        CPU.interruptEnable();
        sendInt(value);
        sendPrompt();
        value0 = 0;
        value1 = 0;
      }
        else
      if (data == '\r')
      {
        value0 = value0 & 0xfff;
        value0 |= 0x3000;
        value1 = value1 & 0xfff;
        value1 |= 0x3000;

        int value = 0;

        if (operator == '+')
        {
          ADC.setChannel(0);
        }
          else
        if (operator == '-')
        {
          ADC.setChannel(12);
        }

        spiSendData(value0, 0);
        spiSendData(value1, 1);

        if (operator == '+')
        {
          value = ADC.read(0);
        }
          else
        if (operator == '-')
        {
          value = ADC.read(12);
        }

        sendInt(value);
        sendPrompt();

        value0 = 0;
        value1 = 0;
        operator = ' ';
      }
        else
      if (data == '\n')
      {
      }

      //i++;
    }
  }

  static public void sendChar(int value)
  {
    while (!UART1.isSendReady());
    UART1.send(value);
  }

  static public void sendString(String value)
  {
    for (int i = 0; i < value.length(); i++)
    {
      sendChar(value.charAt(i));
    }
  }

  static public void sendPrompt()
  {
    sendChar('\r');
    sendChar('\n');
    sendChar('>');
    sendChar(' ');
  }

  static public void sendDigits(int value)
  {
    if (value == 0) { return; }

    int digit = (value % 10) + '0';

    sendDigits(value / 10);
    sendChar(digit);
    //value = value / 10;
  }

  static public void sendInt(int value)
  {
    int length = 0;

    sendChar('\r');
    sendChar('\n');

    if (value == 0)
    {
      sendChar('0');
      return;
    }

    sendDigits(value);
  }

  static public void spiSendData(int value, int channel)
  {
    if (channel == 0)
    {
      IOPort3.setPinLow(13);
    }
      else
    if (channel == 1)
    {
      IOPort1.setPinLow(0);
    }
      else
    {
      IOPort1.setPinLow(1);
    }

    for (int bit = 0; bit < 16; bit++)
    {
      if ((value & 0x8000) == 0)
      {
        IOPort3.setPinLow(1);
      }
        else
      {
        IOPort3.setPinHigh(1);
      }

      // Clock high.
      IOPort0.setPinHigh(1);

      value = value << 1;

      // Clock low.
      IOPort0.setPinLow(1);
    }

    IOPort3.setPinLow(1);

    IOPort3.setPinHigh(13);
    IOPort1.setPinHigh(0);
    IOPort1.setPinHigh(1);
  }

  static public void clearMemory()
  {
    ADC.setChannel(13);
    IOPort3.setPinHigh(8);

    while (true)
    {
      int value = ADC.read(13);
      if (value == 0) { break; }
    }

    IOPort3.setPinLow(8);
  }

  static public void writeMemory(int value)
  {
    spiSendData(0x3000 | value, 2);
    //IOPort2.setPinAsOutput(3);
    //IOPort2.setPinLow(3);
    IOPort3.setPinHigh(8);

    for (int i = 0; i < 10000; i++)
    {
    }

    //IOPort2.setPinAsHighZ(3);
    IOPort3.setPinLow(8);
    spiSendData(0x0000, 2);
  }

  static public int readMemory()
  {
    ADC.setChannel(13);

    IOPort3.setPinHigh(8);
    int value = ADC.read(13);
    IOPort3.setPinLow(8);

    writeMemory(value);

value = 100;

    return value;
  }

  static int interrupt_count = 0;

  public void timerInterrupt()
  {
    // Refresh memory by doing a read operation.
    readMemory();

    // Blink LED in timer interrupt.
    if ((interrupt_count & 1) == 0)
    {
      IOPort3.setPinHigh(10);
    }
      else
    {
      IOPort3.setPinLow(10);
    }

    interrupt_count += 1;
  }
}

