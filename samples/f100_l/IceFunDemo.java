
import net.mikekohn.java_grinder.IceFun;
import net.mikekohn.java_grinder.IOPort0;
import net.mikekohn.java_grinder.IOPort1;
import net.mikekohn.java_grinder.Joystick;
import net.mikekohn.java_grinder.SPI0;

public class IceFunDemo
{
  static int counter = 1;

  static short[] music =
  {
    89,  375, 89,  125, 84,  250, 89, 1000,  0, 250, 82,  250, 84,  500,
    77, 1000, 82,  125, 84,  125, 89,  250, 84, 250, 91,  250, 89, 1000,
     0,  250, 87,  375, 86,  125, 84,  125, 82, 500, 84, 1000, 89,  375,
    89,  125, 84,  250, 89, 1000,  0,  250, 81, 250, 82,  500, 84,  250,
    77, 1000, 82,  250, 81,  250,  0,  250, 77, 250, 82,  500, 81,  500,
    82,  500, 81,  500, 77,  250, 79,  750, 77, 500, 84,  500, 82,  500,
    87,  500, 89, 1000, 77,  250
  };

  public static final int command_display_off = 0xae;
  public static final int command_set_remap = 0xa0;
  public static final int command_start_line = 0xa1;
  public static final int command_display_offset = 0xa2;
  public static final int command_normal_display = 0xa4;
  public static final int command_set_multiplex = 0xa8;
  public static final int command_set_master = 0xad;
  public static final int command_power_mode = 0xb0;
  public static final int command_precharge = 0xb1;
  public static final int command_clockdiv = 0xb3;
  public static final int command_precharge_a = 0x8a;
  public static final int command_precharge_b = 0x8b;
  public static final int command_precharge_c = 0x8c;
  public static final int command_precharge_level = 0xbb;
  public static final int command_vcomh = 0xbe;
  public static final int command_master_current = 0x87;
  public static final int command_contrasta = 0x81;
  public static final int command_contrastb = 0x82;
  public static final int command_contrastc = 0x83;
  public static final int command_display_on = 0xaf;

  public static final int LCD_RES = 1;
  public static final int LCD_DC = 2;
  public static final int LCD_CS = 4;

  public static void delay(int value)
  {
    for (int b = 0; b < value; b++);
  }

  public static void sendCommand(int value)
  {
    IOPort1.setPinsValue(LCD_RES);
    SPI0.send(value);
    while (SPI0.isBusy());

    IOPort1.setPinsValue(LCD_CS|LCD_RES);
  }

  public static void sendData(int value)
  {
    IOPort1.setPinsValue(LCD_DC|LCD_RES);
    SPI0.send16(value);
    while (SPI0.isBusy());

    IOPort1.setPinsValue(LCD_CS|LCD_RES);
  }

  public static void initLCD()
  {
    sendCommand(command_display_off);
    sendCommand(command_set_remap);
    sendCommand(0x72);
    sendCommand(command_start_line);
    sendCommand(0x00);
    sendCommand(command_display_offset);
    sendCommand(0x00);
    sendCommand(command_normal_display);
    sendCommand(command_set_multiplex);
    sendCommand(0x3f);
    sendCommand(command_set_master);
    sendCommand(0x8e);
    sendCommand(command_power_mode);
    sendCommand(command_precharge);
    sendCommand(0x31);
    sendCommand(command_clockdiv);
    sendCommand(0xf0);
    sendCommand(command_precharge_a);
    sendCommand(0x64);
    sendCommand(command_precharge_b);
    sendCommand(0x78);
    sendCommand(command_precharge_c);
    sendCommand(0x64);
    sendCommand(command_precharge_level);
    sendCommand(0x3a);
    sendCommand(command_vcomh);
    sendCommand(0x3e);
    sendCommand(command_master_current);
    sendCommand(0x06);
    sendCommand(command_contrasta);
    sendCommand(0x91);
    sendCommand(command_contrastb);
    sendCommand(0x50);
    sendCommand(command_contrastc);
    sendCommand(0x7d);
    sendCommand(command_display_on);
  }

  public static int getValue(int a, int b)
  {
    return a + b;
  }

  public static void playSong()
  {
    int n, tone, duration;

    for (n = 0; n < music.length; n = n + 2)
    {
      tone     = music[n + 0];
      duration = music[n + 1] << 4;

      IceFun.setTone(tone);
      delay(duration);

      IceFun.setTone(0);
      delay(100);
    }
  }

  public static void main(String[] args)
  {
    int b;

    IOPort1.setPinsValue(LCD_CS);
    delay(5000);
    IOPort1.setPinsValue(LCD_CS|LCD_RES);

    initLCD();

    for (b = 0; b < 96 * 16; b++)
    {
      sendData(0xf000);
    }

    for (b = 0; b < 96 * 16; b++)
    {
      sendData(0x000f);
    }

    for (b = 0; b < 96 * 16; b++)
    {
      sendData(0xff00);
    }

    for (b = 0; b < 96 * 16; b++)
    {
      sendData(0x00ff);
    }

    while (true)
    {
      if (Joystick.isButtonDown_0(0)) { playSong(); }

      IOPort0.setPinsValue(0x01);
      for (counter = 0; counter < 30000; counter++);

      if (Joystick.isButtonDown_0(0)) { playSong(); }

      IOPort0.setPinsValue(0x00);
      for (b = 0; b < 30000; b++);

      if (Joystick.isButtonDown_0(0)) { playSong(); }

      b = getValue(1000, 5000);

      IOPort0.setPinsValue(0x01);
      delay(b);

      if (Joystick.isButtonDown_0(0)) { playSong(); }

      b = getValue(1000, 5000);

      IOPort0.setPinsValue(0x00);
      delay(b);
    }
  }
}

