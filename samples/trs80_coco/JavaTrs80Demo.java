
import net.mikekohn.java_grinder.TRS80Coco;

public class JavaTrs80Demo
{
  static public void main(String args[])
  {
    //TRS80Coco.setBackgroundColor(5);
    TRS80Coco.setText(1024, 0x8f);
    TRS80Coco.setText(1025, 0x9f);
    TRS80Coco.setText(1026, 0xaf);
    TRS80Coco.setText(1027, 0xbf);
    TRS80Coco.setText(1028, 0xcf);
    TRS80Coco.setText(1029, 0xdf);
    TRS80Coco.setText(1030, 0xef);
    TRS80Coco.setText(1031, 0xff);
    TRS80Coco.setText(1032, 0x80);

    TRS80Coco.plot(10, 10, TRS80Coco.COLOR_BLACK);

    while(true);
  }
}

