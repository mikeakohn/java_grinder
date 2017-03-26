
import net.mikekohn.java_grinder.Playstation2;

public class PS2JavaDemo
{
  static public void main(String args[])
  {
    Playstation2.setVideoMode(Playstation2.INTERLACED,
                              Playstation2.VIDEO_TYPE_NTSC,
                              Playstation2.FRAME_FRAME);


    Playstation2.setFrameBuffer2(0, 640, Playstation2.PSMCT32, 0, 0);    
    Playstation2.setDisplay2(656, 36, Playstation2.MAGNIFY_H_8, Playstation2.MAGNIFY_V_1, 2560, 224);    

    Playstation2.waitVsync();

    while(true);
  }
}

