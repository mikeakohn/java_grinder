
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.amiga.Amiga;

public class SongBach
{
  static public void play()
  {
    byte[] song = Memory.preloadByteArray("commodore.bin");

/*
    byte[] sound = new byte[8];

    sound[0] = 0;
    sound[1] = 90;
    sound[2] = 127;
    sound[3] = 90;
    sound[4] = 0;
    sound[5] = -90;
    sound[6] = -128;
    sound[7] = -90;
*/

    Amiga.setAudioData(0, song);
    Amiga.setAudioLength(0, song.length >> 1);
    Amiga.setAudioPeriod(0, 447);
    //Amiga.setAudioData(0, sound);
    //Amiga.setAudioLength(0, sound.length >> 1);
    //Amiga.setAudioPeriod(0, 892);
    Amiga.setAudioVolume(0, 63);
    Amiga.setDMA(Amiga.DMA_AUDIO_0);
  }
}

