import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.CPC;

public class ScreenCPC
{
  public static void main(String args[])
  {
    int x,y,screen,c;
    char arr;
    CPC.screen(1);
    x=20; y=4; arr=129;
    for (y=4;y<16;y++) //195
    {
      screen = 0xc000 + (80 * ((int)(y / 8))) + (2048 * (y % 8)) + x;
      CPC.poke8(screen, arr);
      Memory.write8(screen+0x50, (byte) arr);
    }
  c = (char) CPC.readChar();
  }
}