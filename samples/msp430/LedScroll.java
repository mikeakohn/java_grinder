
import net.mikekohn.java_grinder.IOPort0;
import net.mikekohn.java_grinder.IOPort1;

public class LedScroll
{
  static int letter = 0;
  static String text = "   HELLO TO YOU   ";

  static public void main(String[] args)
  {
    int n;

    IOPort0.setPinsAsOutput(0x7f);
    IOPort1.setPinsAsOutput(0x0f);
    IOPort0.setPinsValue(0x01);
    IOPort1.setPinsValue(0x01);

    while(true)
    {
      display();
    }
  }

  static public void display()
  {
    int a;

    for (a = 0; a < 512; a++)
    {
      IOPort0.setPinsValue(getLetter(letter));
      IOPort1.setPinsValue(0x01);
      delay();

      IOPort0.setPinsValue(getLetter(letter + 1));
      IOPort1.setPinsValue(0x02);
      delay();

      IOPort0.setPinsValue(getLetter(letter + 2));
      IOPort1.setPinsValue(0x04);
      delay();

      IOPort0.setPinsValue(getLetter(letter + 3));
      IOPort1.setPinsValue(0x08);
      delay();
    }

    letter++;
    if (letter == text.length() - 3) { letter = 0; }
  }

  static byte getLetter(int letter)
  {
    char ch = text.charAt(letter);
    if (ch == ' ') { return 0; }
    return letters[ch - 'A'];
  }

  static public void delay()
  {
    int a;
    for (a = 0; a < 10; a++);
  }

  static byte[] letters =
  {
    // A     B     C     D     E     F     G     H     I     J    *K
    0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x6f, 0x76, 0x06, 0x1e, 0x00,
    // L,    M     N     O     P     Q     R     S     T     U     V
    0x38, 0x00, 0x54, 0x3f, 0x73, 0x00, 0x50, 0x6d, 0x07, 0x3e, 0x00,
    // W     X     Y     Z
    0x00, 0x00, 0x6e, 0x5b, 0x00, 0x00, 0x00, 0x00
  };
}


