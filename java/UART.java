package cc.naken.java_grinder;

public class UART
{
  public UART()
  {
  }

  public void send(byte c)
  {
     System.out.println("Sent char " + c);
  }

  public byte read(byte c)
  {
    return 0;
  }

  public boolean isDataAvailable()
  {
    return false;
  }

  public boolean isSendReady()
  {
    return false;
  }
}

