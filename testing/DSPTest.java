
import net.mikekohn.java_grinder.DSP;

public class DSPTest
{
  static public void main(String args[])
  {
    int n = 20;
    int m = 10;

    DSP.addABAndStoreInA();
    DSP.addABAndStoreInB();
    DSP.addToA(50);
    DSP.addToB(n);
    DSP.squareAndAddToA(50);
    DSP.squareAndAddToB(n);
    DSP.mulAndAddToA(n,m);
    DSP.mulAndAddToB(10,16);
    DSP.mulAndSubFromA(10,m);
    DSP.mulAndSubFromB(n,16);
  }
}


