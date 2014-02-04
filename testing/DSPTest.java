
import net.mikekohn.java_grinder.DSP;

public class DSPTest
{
  static public void main(String args[])
  {
    int n = 20;

    DSP.addABAndStoreInA();
    DSP.addABAndStoreInB();
    DSP.addToA(50);
    DSP.addToB(n);
    DSP.squareAndAddToA(50);
    DSP.squareAndAddToB(n);
  }
}


