import net.mikekohn.java_grinder.CPC;

public class ScreenCPC
{
  public static void main(String args[])
  {
    int x,y;
    // x = 0 to 79
    // y = 0 to 399
    
    CPC.screen(1);
  
    y=4;
    
    for (x=20; x<60; x++)
    {
      CPC.poke8( vmem(x,y), (char) 0x1a);
      CPC.poke8( vmem(x,y+96), (char) 215);
    }
    char c = (char) CPC.readChar();
	
	return;
  }
  
  public static int vmem (int x, int y)
  {
    int screen,h,u;
    screen = (y >> 3);
    screen = (screen << 3); // *8
    u = screen;
    screen = (screen << 2); // * 4 = *32
    screen += u; // = 40*
    screen = screen << 1; // *2 = 80 *
    h = (y & (8-1)); // % 8
    h = h << 11; // *2048
    screen = screen + h + x + 0xC000;
    return screen;	  
  }
}