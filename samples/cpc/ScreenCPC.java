import net.mikekohn.java_grinder.CPC;

public class ScreenCPC
{
  public static void main(String args[])
  {
    int x,y,screen;
    char c;
    
    CPC.screen(1);
    x=20; 
    y=4;
   
    screen = 0xc000 + (80 * ((int)(y / 8)));
    screen += (2048 * (y % 8));
    screen += x;
    
    CPC.poke8(screen, (char) 0xEC);
    
    c = (char) CPC.readChar();
  }
}