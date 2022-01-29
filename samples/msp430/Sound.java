// This is for testing i2c with a SparkFun Qwiic MP3 Trigger board (DEV-16892).

import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.I2C;
import net.mikekohn.java_grinder.IOPort0;

public class Sound
{
  static int mp3_id = 0;
  static final int MP3_RESET = 0x10;
  static final int MP3_INT = 0x20;

  static void getId()
  {
    I2C.start();
    I2C.write(0x6e);
    I2C.write(0x10);
    I2C.stop();
    for (int i = 0; i < 100; i++);
    I2C.start();
    I2C.write(0x6f);
    mp3_id = I2C.read();
    I2C.stop();
  }

  static void setVolume()
  {
    // Sets the volume to 22.
    I2C.start();
    I2C.write(0x6e);
    I2C.write(0x07);
    I2C.write(22);
    I2C.stop();
  }

  static void clearInterrupts()
  {
    I2C.start();
    I2C.write(0x6e);
    I2C.write(0x0d);
    I2C.stop();
  }

  static void playSong(int index)
  {
    clearInterrupts();

    I2C.start();
    I2C.write(0x6e);
    I2C.write(0x01);
    I2C.write(index);
    I2C.stop();
  }

  static boolean isPlaying()
  {
    return (IOPort0.getPortInputValue() & MP3_INT) != 0;
  }

  static void delay()
  {
    for (int n = 0; n < 40; n++)
    {
      for (int i = 0; i < 30000; i++);
    }
  }

  static public void main(String[] args)
  {
    // Set the DCO to 8MHz
    CPU.setClock8();

    // Set up control pins.
    IOPort0.setPinsAsOutput(MP3_RESET);
    IOPort0.setPinsValue(0);

    // Initialize i2c.
    I2C.init(I2C.DIV8);

    // Take MP3 Trigger out of reset mode and pause a little bit.
    IOPort0.setPinsValue(MP3_RESET);

    delay();

    getId();
    setVolume();
    playSong(1);

    while (true)
    {
    }
  }
}

