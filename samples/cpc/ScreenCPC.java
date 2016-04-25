import net.mikekohn.java_grinder.CPC;

public class ScreenCPC
{
  public static void main(String args[])
  {
    int i,scr,z;
    
    // x = 0 to 79
    // y = 0 to 199
    
    char x,y,c;
    
    CPC.screen(1);
           
    x = 0xAA;
    
    for (i=0xC000; i<0xFFFF; i++)
    {
      CPC.poke8( i, x);      
    };

    CPC.setCursor(18,10);
    CPC.setTxtPen(1);
    CPC.putChar('H');
    CPC.setTxtPen(2);
    CPC.putChar('e');
    CPC.setTxtPen(3);
    CPC.putChar('l');  
    CPC.putChar('l');
    CPC.setTxtPen(1);    
    CPC.putChar('o');
   
    c = CPC.readChar();
    
    for (i=1;i<500;i++)
    {
      i--;
      i++;
    }
    
    CPC.cls();
  
    y=100; scr = 0xC000;
        
    for (x=20; x<60; x++)
    {
      z = CPC.getVMEM(scr,x,y);
      CPC.poke8( z, (char) 0x1a);
      CPC.poke8( (z+0x800), (char) 215);
    }
    
    
    for (i=1;i<5000;i++)
    {
      c=1;
      x=c;
    }
   
    c = CPC.readChar();

    return;      
  }
  
 
}
