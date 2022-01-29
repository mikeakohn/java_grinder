import net.mikekohn.java_grinder.CPC;
import net.mikekohn.java_grinder.Memory;

public class ScreenCPC
{
  public static void main(String[] args)
  {
    int i,scr,z;
    
    // x = 0 to 79
    // y = 0 to 199
    
    char x,y,c;
    
    CPC.screen(1);
           
    x = 0xAA;
    
    for (i=0xC000; i<0xFFFF; i++)
    {
      Memory.write8( i, (byte) x);      
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
    
  
    
  
    y=100; 
    scr = 0xC000; 
    short help = (short) 0xa11a;
        
    for (x=20; x<60; x++)
    {
      z = CPC.getVMEM(scr,x,y);
      Memory.write8( z, (byte) 0x22);
      z += 0x800;
      Memory.write16( z, help);   
    }
    
    CPC.setCursor(18,10);    
    CPC.printI(CPC.getTime());
    
    for (i=1;i<10000;i++)
    {
      c=1;
      x=c;
    }
    
    CPC.VSync();
    
    CPC.setCursor(18,15);    
    CPC.printI(CPC.getTime());
        
    c = CPC.readChar();

    return;      
  }
  
 
}
