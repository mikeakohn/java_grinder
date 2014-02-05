
import net.mikekohn.java_grinder.DSP;

public class DSPTest
{
  static public void main(String args[])
  {
  }

  static public void single()
  {
    int n = 100;

    DSP.clearA();
    DSP.clearB();
    DSP.loadA(16);
    DSP.loadB(n);
    DSP.negA();
    DSP.negB();
    DSP.addABAndStoreInA();
    DSP.addABAndStoreInB();
  }

  static public void to_accum()
  {
    int n = 20;
    int m = 10;

    DSP.addToA(50);
    DSP.addToB(n);
    DSP.squareToA(50);
    DSP.squareToB(n);
    DSP.mulToA(50,m);
    DSP.mulToB(n,m);
  }

  static public void add_accum()
  {
    int n = 20;
    int m = 10;

    DSP.squareAndAddToA(50);
    DSP.squareAndAddToB(n);
    DSP.mulAndAddToA(n,m);
    DSP.mulAndAddToB(10,16);
    DSP.mulAndSubFromA(10,m);
    DSP.mulAndSubFromB(n,16);
  }
}


