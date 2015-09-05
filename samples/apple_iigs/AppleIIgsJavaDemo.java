import net.mikekohn.java_grinder.CPU;
import net.mikekohn.java_grinder.AppleIIgs;

public class AppleIIgsJavaDemo
{
  static public void main(String args[])
  {
    int i, j;

    for(j = (int)'A'; j < (int)'Z'; j++) 
    {
      for(i = (int)'A'; i < (int)'Z'; i++) 
      {
        AppleIIgs.printChar((char)i);
        AppleIIgs.printChar((char)j);
        AppleIIgs.printChar((char)i);
        AppleIIgs.printChar((char)j);
        AppleIIgs.printChar((char)i);
        AppleIIgs.printChar((char)j);
      }
    }

//    AppleIIgs.printChar('M');
//    AppleIIgs.printChar('I');
//    AppleIIgs.printChar('K');
//    AppleIIgs.printChar('E');

//    CPU.asm("brk\n");

    while(true) { }
  }
}

