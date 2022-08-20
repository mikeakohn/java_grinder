
import net.mikekohn.java_grinder.NES;

public class NesJavaDemo
{
  static byte[] diagnol =
  {
    -128, 0x40, 0x20, 0x10,  0x08, 0x04, 0x02, 0x01,
    0x00, 0x00, 0x00, 0x00,  0x08, 0x04, 0x02, 0x01
  };

  static byte[] square =
  {
    -1,    0,    0,    0,     0,    0,    0, -1,
    -1, -127, -127, -127,  -127, -127, -127, -1
  };

  static byte[] block =
  {
    -1, -1, -1, -1, -1, -1, -1, -1,
     0,  0,  0,  0,  0,  0,  0,  0
  };

  static public void main(String[] args)
  {
    NES.setScroll(0, 0);

    NES.setBackgroundPalette(0, 0x23);
    NES.setBackgroundPalette(1, 0x09);
    NES.setBackgroundPalette(2, 0x19);

    // This is a 2 bitplane 8x8 pixel pattern (on other platform
    // sometimes called tiles).
    NES.setPattern(0, diagnol);
    NES.setPattern(1, square);
    NES.setPattern(2, block);

    // Place some patterns on the screen by name table index.
    NES.setNameTable(0, 1);
    NES.setNameTable(1, 2);
    NES.setNameTable(40, 1);

    // Place some patterns on the screen letting Java Grinder calculate
    // (y * 32) + x = 15 * 32 + 10 = 490.
    NES.setNameTable(10, 15, 2);

    while(true)
    {
      for (int x = 0; x < 64; x++)
      {
        NES.setScroll(x, 0);
        NES.waitVerticalBlank();
      }
    }
  }
}

