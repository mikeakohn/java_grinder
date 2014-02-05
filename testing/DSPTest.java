
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
    DSP.shiftA(16);
    DSP.shiftB(n);
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
    //DSP.euclideanDistanceToA(50,m);
    //DSP.euclideanDistanceToB(n,m);
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
    //DSP.euclideanDistanceAndAddToA(50,m);
    //DSP.euclideanDistanceAndAddToB(n,m);
  }

  static public void store_accum()
  {
    int n;
    n = DSP.getA();
    n = DSP.getB();
    n = DSP.getUpperA();
    n = DSP.getUpperB();
    n = DSP.getLowerA();
    n = DSP.getLowerB();
    DSP.getA();
  }
}


