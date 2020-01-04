
import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.amiga.Amiga;

public class SongAmigaGrind
{
  static public void play()
  {
    byte[] song = Memory.preloadByteArray("amiga_grind_1.bin");

    Amiga.setAudioData(0, song);
    Amiga.setAudioLength(0, song.length >> 1);
    Amiga.setAudioPeriod(0, 447);
    //Amiga.setAudioData(0, sound);
    //Amiga.setAudioLength(0, sound.length >> 1);
    //Amiga.setAudioPeriod(0, 892);
    Amiga.setAudioVolume(0, 63);
    Amiga.setDMA(Amiga.DMA_AUDIO_0);
  }

  static public void playSecond()
  {
    byte[] song = Memory.preloadByteArray("amiga_grind_2.bin");

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

