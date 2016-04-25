import net.mikekohn.java_grinder.CPC;

public class GrafCPC
{
  public static void main(String args[])
  {
   int i;
   char c;  
	  
   CPC.screen(1);
   CPC.plot(1,1,2);
   CPC.draw(640,200,1);
   CPC.draw(0,200,2);
   CPC.draw(640,0,3);
      
   
   String txt = "Very simple Grafix for CPC in JAVA"; 
    
   
   for (i = 0; i < txt.length(); i++)
   {
    c = txt.charAt(i);
    CPC.putChar(c);
   }
   
   c=CPC.readChar();
   
   return;
   
  }
}
