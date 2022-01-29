
import net.mikekohn.java_grinder.CPU;

public class InlineTest
{
  static public void main(String[] args)
  {
    CPU.asm("  mov.w #1, r13\n" +
            "  add.w #4, r13\n");

    while(true);
  }
}

