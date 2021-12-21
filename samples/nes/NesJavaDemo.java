
import net.mikekohn.java_grinder.NES;

public class NesJavaDemo
{
  static byte[] diagnol =
  {
    -128, 0x40, 0x20, 0x10,  0x08, 0x04, 0x02, 0x01,
    0x00, 0x00, 0x00, 0x00,  0x08, 0x04, 0x02, 0x01
  }

  static public void main(String args[])
  {
    NES.setBackgroundPalette(0, 0x23);
    NES.setBackgroundPalette(1, 0x09);

    // This is a 2 bitplane 8x8 pixel pattern (on other platform
    // sometimes called tiles).
    NES.setPattern(0, diagnol);

    // Place tile 0 at (0, 0), (1, 0), (2, 0) on the screen.
    NES.setNameTable(0, 0);
    NES.setNameTable(1, 0);
    NES.setNameTable(2, 0);

    while(true);
  }
}

