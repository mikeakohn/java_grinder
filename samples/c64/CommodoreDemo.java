import net.mikekohn.java_grinder.Memory;
import net.mikekohn.java_grinder.c64.*;

public class CommodoreDemo
{
  static final int screen_ram = 0xc000;
  static final int color_ram = 0xd800;
  static final int hires_ram = 0xe000;
  static final int sprite_ram = screen_ram + 1024;
  static final int sprite_pointer = screen_ram + 1016;

  static int pixel_table[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
  static byte mandel_colors[] = { 6, 4, 14, 3, 13, 10, 7, 1, 7, 10, 13, 3 };
  static int text[] = { 10, 1, 22, 1, 100, 7, 18, 9, 14, 4, 5, 18 };
  static int text2[] = { 1, 22, 1, 32, 3, 15, 13, 16, 9, 12, 5, 18 };
  static int balloon_height[] = { 0, 0, 0, 0, 2, 2, 5, 5, 7, 7, 7, 7, 5, 5, 2, 2 };

  static int screen_row[] =
  {
    screen_ram + 40 * 0,
    screen_ram + 40 * 1,
    screen_ram + 40 * 2,
    screen_ram + 40 * 3,
    screen_ram + 40 * 4,
    screen_ram + 40 * 5,
    screen_ram + 40 * 6,
    screen_ram + 40 * 7,
    screen_ram + 40 * 8,
    screen_ram + 40 * 9,
    screen_ram + 40 * 10,
    screen_ram + 40 * 11,
    screen_ram + 40 * 12,
    screen_ram + 40 * 13,
    screen_ram + 40 * 14,
    screen_ram + 40 * 15,
    screen_ram + 40 * 16,
    screen_ram + 40 * 17,
    screen_ram + 40 * 18,
    screen_ram + 40 * 19,
    screen_ram + 40 * 20,
    screen_ram + 40 * 21,
    screen_ram + 40 * 22,
    screen_ram + 40 * 23,
    screen_ram + 40 * 24
  };

  static int color_row[] = 
  {
    color_ram + 40 * 0,
    color_ram + 40 * 1,
    color_ram + 40 * 2,
    color_ram + 40 * 3,
    color_ram + 40 * 4,
    color_ram + 40 * 5,
    color_ram + 40 * 6,
    color_ram + 40 * 7,
    color_ram + 40 * 8,
    color_ram + 40 * 9,
    color_ram + 40 * 10,
    color_ram + 40 * 11,
    color_ram + 40 * 12,
    color_ram + 40 * 13,
    color_ram + 40 * 14,
    color_ram + 40 * 15,
    color_ram + 40 * 16,
    color_ram + 40 * 17,
    color_ram + 40 * 18,
    color_ram + 40 * 19,
    color_ram + 40 * 20,
    color_ram + 40 * 21,
    color_ram + 40 * 22,
    color_ram + 40 * 23,
    color_ram + 40 * 24
  };

  static int java_logo[] =
  {
    0, 0, 0, 3, 7, 15, 15, 15, 
    0, 0, 252, 255, 255, 255, 143, 7, 
    0, 0, 0, 0, 128, 192, 192, 192, 
    31, 31, 31, 31, 31, 31, 31, 31, 
    252, 255, 255, 255, 15, 7, 7, 7, 
    15, 15, 143, 143, 192, 192, 192, 192, 
    62, 62, 62, 62, 62, 62, 62, 62, 
    124, 124, 124, 124, 124, 124, 124, 124, 
    240, 240, 240, 240, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    7, 7, 7, 7, 7, 7, 7, 7, 
    255, 255, 255, 255, 255, 193, 192, 192, 
    0, 192, 224, 240, 248, 248, 252, 124, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    31, 31, 31, 31, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    31, 31, 31, 31, 31, 31, 31, 31, 
    254, 255, 255, 255, 31, 15, 7, 7, 
    0, 0, 128, 192, 192, 192, 192, 192, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    240, 240, 240, 240, 240, 240, 240, 240, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    7, 15, 31, 255, 254, 255, 127, 15, 
    192, 192, 128, 0, 0, 0, 128, 128, 
    31, 31, 31, 31, 31, 31, 31, 31, 
    7, 15, 255, 254, 254, 255, 15, 7, 
    143, 143, 15, 15, 15, 143, 207, 207, 
    62, 62, 62, 62, 62, 62, 62, 62, 
    124, 124, 124, 124, 124, 124, 124, 124, 
    240, 240, 240, 241, 241, 241, 243, 243, 
    63, 127, 255, 251, 241, 224, 224, 224, 
    131, 195, 227, 243, 243, 251, 251, 251, 
    239, 255, 255, 247, 227, 225, 225, 225, 
    192, 192, 224, 224, 224, 224, 224, 224, 
    7, 7, 7, 7, 7, 7, 7, 7, 
    192, 192, 192, 192, 192, 192, 192, 192, 
    124, 124, 124, 124, 124, 124, 125, 125, 
    31, 127, 127, 241, 240, 240, 255, 255, 
    135, 199, 227, 243, 243, 241, 241, 241, 
    193, 193, 193, 227, 227, 227, 227, 247, 
    223, 223, 223, 223, 223, 223, 159, 159, 
    3, 15, 31, 31, 62, 62, 62, 62, 
    240, 248, 252, 124, 62, 30, 0, 0, 
    31, 63, 127, 120, 240, 240, 255, 255, 
    192, 225, 243, 243, 251, 251, 251, 249, 
    254, 255, 255, 231, 195, 240, 254, 255, 
    0, 0, 128, 192, 192, 0, 0, 0, 
    31, 31, 31, 31, 31, 31, 31, 31, 
    7, 7, 15, 255, 254, 254, 255, 31, 
    223, 159, 159, 31, 31, 31, 159, 159, 
    15, 15, 15, 15, 15, 15, 15, 15, 
    60, 63, 63, 63, 62, 62, 62, 62, 
    252, 252, 254, 126, 62, 30, 30, 30, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    240, 240, 241, 241, 241, 240, 240, 241, 
    127, 255, 255, 227, 227, 15, 255, 255, 
    31, 159, 207, 207, 207, 199, 199, 199, 
    7, 7, 15, 143, 143, 143, 158, 222, 
    143, 159, 191, 124, 120, 7, 31, 63, 
    224, 240, 248, 120, 120, 248, 248, 248, 
    0, 31, 31, 15, 15, 7, 3, 1, 
    7, 7, 7, 143, 255, 255, 254, 252, 
    192, 192, 192, 192, 128, 0, 0, 0, 
    31, 31, 31, 31, 31, 31, 31, 31, 
    3, 3, 7, 15, 255, 255, 255, 254, 
    239, 239, 239, 239, 207, 207, 143, 15, 
    62, 62, 62, 62, 62, 62, 62, 62, 
    124, 124, 124, 124, 124, 124, 124, 124, 
    243, 243, 241, 241, 241, 240, 240, 240, 
    224, 224, 224, 241, 251, 255, 127, 31, 
    251, 251, 251, 243, 243, 227, 195, 3, 
    225, 225, 225, 225, 225, 225, 225, 225, 
    224, 224, 224, 224, 224, 224, 224, 224, 
    7, 7, 7, 7, 7, 7, 7, 7, 
    192, 192, 193, 255, 255, 255, 255, 255, 
    125, 253, 248, 248, 248, 240, 192, 0, 
    255, 240, 240, 240, 249, 255, 63, 31, 
    241, 0, 0, 240, 240, 224, 224, 128, 
    247, 247, 255, 255, 127, 126, 126, 62, 
    159, 31, 31, 31, 31, 31, 31, 31, 
    62, 62, 62, 63, 31, 15, 15, 3, 
    0, 24, 62, 62, 252, 252, 248, 224, 
    255, 240, 240, 240, 125, 127, 63, 15, 
    248, 0, 3, 251, 243, 243, 225, 128, 
    127, 31, 199, 199, 239, 255, 255, 124, 
    128, 192, 192, 192, 128, 128, 0, 0, 
    31, 31, 31, 31, 31, 31, 31, 31, 
    15, 15, 15, 15, 7, 7, 7, 7, 
    159, 223, 223, 223, 223, 207, 207, 195, 
    15, 15, 15, 31, 255, 255, 239, 207, 
    62, 62, 62, 62, 62, 62, 62, 62, 
    30, 30, 30, 30, 30, 30, 30, 30, 
    1, 1, 1, 1, 0, 0, 0, 0, 
    241, 241, 241, 255, 255, 255, 127, 31, 
    243, 243, 243, 243, 227, 227, 193, 0, 
    243, 227, 195, 195, 231, 255, 251, 243, 
    199, 195, 195, 195, 193, 193, 225, 225, 
    222, 220, 252, 252, 252, 248, 248, 248, 
    124, 120, 120, 120, 125, 127, 63, 30, 
    120, 120, 120, 120, 248, 248, 120, 120, 
  };

  static int java_redlogo[] =
  {
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    248, 248, 248, 248, 248, 248, 248, 248, 
    0, 1, 3, 7, 7, 15, 31, 31, 
    240, 248, 248, 252, 254, 254, 191, 31, 
    3, 3, 1, 0, 0, 0, 0, 128, 
    240, 248, 248, 252, 252, 126, 63, 63, 
    0, 1, 3, 3, 7, 7, 15, 31, 
    252, 248, 240, 240, 224, 224, 192, 128, 
    0, 1, 3, 7, 7, 15, 31, 31, 
    240, 248, 248, 252, 254, 254, 191, 31, 
    0, 0, 0, 0, 0, 0, 0, 128, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    1, 3, 7, 15, 15, 15, 15, 15, 
    255, 255, 255, 255, 224, 128, 128, 0, 
    255, 255, 254, 252, 0, 0, 0, 0, 
    191, 63, 63, 63, 60, 60, 61, 61, 
    255, 255, 255, 255, 0, 0, 255, 255, 
    225, 241, 249, 249, 249, 249, 249, 249, 
    207, 207, 207, 207, 207, 207, 206, 206, 
    0, 128, 192, 224, 240, 248, 252, 126, 
    59, 59, 59, 59, 59, 59, 59, 59, 
    255, 255, 255, 255, 192, 192, 192, 192, 
    255, 255, 255, 255, 15, 3, 3, 1, 
    1, 131, 199, 231, 231, 239, 239, 239, 
    255, 255, 255, 255, 224, 192, 128, 255, 
    255, 255, 255, 255, 0, 0, 0, 255, 
    191, 191, 63, 63, 60, 60, 61, 61, 
    255, 255, 255, 255, 0, 0, 255, 255, 
    240, 240, 248, 248, 248, 248, 248, 248, 
    0, 0, 0, 0, 3, 7, 31, 63, 
    0, 0, 0, 0, 255, 255, 255, 255, 
    248, 248, 248, 248, 248, 249, 243, 227, 
    63, 63, 127, 252, 252, 248, 240, 240, 
    255, 255, 255, 7, 3, 1, 1, 0, 
    128, 192, 224, 224, 240, 248, 248, 252, 
    31, 15, 15, 7, 7, 3, 1, 1, 
    159, 255, 254, 254, 252, 252, 248, 240, 
    128, 0, 0, 0, 0, 1, 3, 3, 
    63, 63, 127, 252, 252, 248, 240, 240, 
    255, 255, 255, 7, 3, 1, 1, 0, 
    128, 192, 224, 224, 240, 248, 249, 252, 
    0, 0, 0, 0, 0, 0, 255, 255, 
    15, 15, 15, 15, 15, 7, 231, 243, 
    0, 0, 128, 128, 224, 255, 255, 255, 
    7, 7, 7, 7, 7, 255, 255, 255, 
    189, 188, 188, 188, 188, 188, 188, 188, 
    255, 255, 127, 63, 31, 15, 7, 3, 
    241, 225, 129, 193, 225, 241, 249, 253, 
    206, 206, 206, 206, 206, 206, 206, 206, 
    63, 31, 15, 7, 3, 1, 0, 0, 
    59, 187, 251, 251, 251, 251, 251, 123, 
    192, 192, 192, 192, 192, 255, 255, 255, 
    1, 1, 3, 7, 15, 255, 255, 255, 
    239, 239, 239, 239, 231, 199, 195, 129, 
    255, 255, 128, 192, 224, 255, 255, 255, 
    254, 252, 0, 0, 0, 255, 255, 255, 
    61, 60, 60, 60, 60, 188, 188, 60, 
    255, 255, 127, 63, 31, 15, 7, 3, 
    240, 224, 0, 128, 192, 224, 240, 248, 
  };

  static int java_sprite[] =
  {
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 3, 
    0, 0, 15, 0, 0, 30, 0, 0, 
    56, 0, 0, 112, 0, 0, 240, 0, 
    0, 224, 0, 0, 240, 0, 0, 240, 
    0, 0, 120, 0, 0, 28, 0, 0, 
    6, 0, 0, 3, 0, 0, 0, 0,

    1, 0, 0, 3, 0, 0, 6, 0, 
    0, 30, 0, 0, 60, 0, 0, 248, 
    0, 0, 240, 0, 0, 192, 224, 0, 
    135, 128, 0, 30, 0, 0, 60, 0, 
    0, 120, 0, 0, 240, 0, 0, 240, 
    0, 0, 120, 0, 0, 120, 0, 0, 
    60, 0, 0, 62, 0, 0, 30, 0, 
    0, 30, 0, 0, 28, 0, 0, 0,

    0, 248, 0, 3, 240, 0, 1, 255, 
    255, 0, 31, 255, 0, 0, 0, 0, 
    31, 224, 0, 15, 255, 0, 1, 255, 
    0, 0, 0, 0, 0, 0, 0, 7, 
    255, 0, 3, 255, 15, 128, 31, 62, 
    0, 0, 127, 128, 0, 31, 255, 255, 
    0, 255, 255, 0, 15, 255, 0, 3, 
    255, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 240, 0, 12, 60, 255, 240, 
    30, 192, 0, 30, 0, 0, 62, 0, 
    56, 124, 255, 192, 240, 248, 1, 192, 
    0, 3, 0, 0, 0, 0, 255, 224, 
    0, 255, 128, 0, 248, 0, 0, 0, 
    0, 112, 0, 255, 192, 255, 252, 8, 
    248, 7, 248, 255, 255, 192, 254, 0, 
    0, 0, 0, 0, 0, 0, 0, 0,
  };

  static int sprite1[] =
  {
    0, 127, 0, 1, 255, 192, 3, 255, 224, 3, 231, 224,
    7, 217, 240, 7, 223, 240, 7, 217, 240, 3, 231, 224,
    3, 255, 224, 3, 255, 224, 2, 255, 160, 1, 127, 64,
    1, 62, 64, 0, 156, 128, 0, 156, 128, 0, 73, 0, 0, 73, 0,
    0, 62, 0, 0, 62, 0, 0, 62, 0, 0, 28, 0
  };

  static int sprite2[] =
  {
    0b00000000, 0b11111111, 0b00000000,
    0b00000111, 0b00000000, 0b11100000,
    0b00001000, 0b00000000, 0b00010000,
    0b00010000, 0b00000000, 0b00001000,
    0b00100000, 0b00000000, 0b00000100,
    0b01000001, 0b11111111, 0b11100010,
    0b01000000, 0b00001111, 0b00000010,
    0b10000000, 0b00001111, 0b00000001,
    0b10000000, 0b00001111, 0b00000001,
    0b10000000, 0b00001111, 0b00000001,
    0b10000000, 0b00001111, 0b00000001,
    0b10000000, 0b00001111, 0b00000001,
    0b10000000, 0b00001111, 0b00000001,
    0b10000011, 0b00001111, 0b00000001,
    0b01000011, 0b00001111, 0b00000010,
    0b01000001, 0b10011110, 0b00000010,
    0b00100000, 0b11111100, 0b00000100,
    0b00010000, 0b00000000, 0b00001000,
    0b00001000, 0b00000000, 0b00010000,
    0b00000111, 0b00000000, 0b11100000,
    0b00000000, 0b11111111, 0b00000000
  };

  public static int pulse = 1024;
  public static int note[] = { 1403, 1486, 1574, 1668, 1767, 1872, 1984, 2102, 2227, 2359, 2500, 2648, 2806, 2973, 3149, 3337, 3535, 3745, 3968, 4204, 4454, 4719, 5000, 5297 };

  public static int song1pos = 0;
  public static int song2pos = 0;
  public static int song3pos = 0;

  public static int song1[] = { 0, 0, 4, 0, -1, 0, 4, 0 };

  public static int song2[] =
  {
    0, -1, 0, 3, 0, -1, 0, 7, 0, -1, 0, 5, 0, -1, 0, 0,
    0, -1, 0, 3, 0, -1, 0, 7, 0, -1, 0, 5, 0, -1, 0, 0,
    0, -1, 0, 8, 0, -1, 0, 12, 0, -1, 0, 10, 0, -1, 0, 0,
    0, -1, 0, 3, 0, -1, 0, 7, 0, -1, 0, 5, 0, -1, 0, 0
  };

  public static int song3[] =
  {
    0, 0, 0, 0, 7, 7, 7, 7, 3, 3, 7, 7, 5, 5, 3, 3,
    0, 0, 0, 0, 7, 7, 7, 7, 3, 3, 7, 7, 5, 5, 3, 3,
    5, 5, 5, 5, 12, 12, 12, 12, 8, 8, 12, 12, 10, 10, 8, 8,
    0, 0, 0, 0, 7, 7, 7, 7, 3, 3, 7, 7, 5, 5, 3, 3
  };

  public static void wait(int time)
  {
    int i;

    for(i = 0; i < time; i++);
  }

  public static void musak_init()
  {
    //                srad 
    SID.voice1_adsr(0x0004);
    SID.voice2_adsr(0x0009);
    SID.voice3_adsr(0x0060);

    SID.voice1_pulse_width(2048);
    SID.voice2_pulse_width(2048);
    SID.voice3_pulse_width(2048);
  }

  public static void musak_play()
  {
    int i;

    i = note[song1[song1pos]];
    if(i != -1)
    {
      SID.voice1_frequency(note[song1[song1pos]]);
      SID.voice1_waveform(128);
      SID.voice1_waveform(129);
    }

    i = note[song2[song2pos]];
    if(i != -1)
    {
      SID.voice2_frequency(note[song2[song2pos]] << 1);
      SID.voice2_waveform(64);
      SID.voice2_waveform(65);
    }

    i = note[song3[song3pos]];
    if(i != -1)
    {
      SID.voice3_frequency(note[song3[song3pos]] << 2);
      SID.voice3_waveform(32);
      SID.voice3_waveform(33);
    }

    song1pos++;
    if(song1pos >= song1.length)
      song1pos = 0;
    song2pos++;
    if(song2pos >= song2.length)
      song2pos = 0;
    song3pos++;
    if(song3pos >= song3.length)
      song3pos = 0;

    SID.voice2_pulse_width(pulse);
    pulse += 16;
  }

  public static void wipe()
  {
    int left = 0;
    int right = 39;
    int top = 0;
    int bottom = 24;

    int i;

    while(top <= 13)
    {
      for(i = left; i <= right; i++)
      {
        Memory.write8(color_row[top] + i, (byte)0);
        Memory.write8(screen_row[top] + i, (byte)32);
      }
      for(i = top; i <= bottom; i++)
      {
        Memory.write8(color_row[i] + right, (byte)0);
        Memory.write8(screen_row[i] + right, (byte)32);
      }
      for(i = right; i >= left; i--)
      {
        Memory.write8(color_row[bottom] + i, (byte)0);
        Memory.write8(screen_row[bottom] + i, (byte)32);
      }
      for(i = bottom; i >= top; i--)
      {
        Memory.write8(color_row[i] + left, (byte)0);
        Memory.write8(screen_row[i] + left, (byte)32);
      }

      left++;
      if(left > 39)
        left = 39;
      right--;
      if(right < 0)
        right = 0;
      top++;
      if(top > 24)
        top = 24;
      bottom--;
      if(bottom < 0)
        bottom = 0;
    }
  }

  public static void mandel()
  {
    int x, y, i;

    int recen = -8;
    int imcen = 0;
    int re, im, re2, im2, rec, imc;

    VIC.background(0);
    VIC.border(0);

    VIC.text_clear(160);
    VIC.color_ram_clear(0);

    for(i = 0; i < text.length; i++)
    {
      Memory.write8(color_ram + 40 * 12 + 19 + i, (byte)1);
      Memory.write8(screen_ram + 40 * 12 + 19 + i, (byte)text[i]);
    }

    SID.voice1_adsr(0xF000);
    SID.voice2_adsr(0xF000);
    SID.voice3_adsr(0xF000);

    int loc1 = color_ram;
    int loc2 = color_ram + 960;
    int yy = 0;
    for(y = 0; y < 13 * 16; y += 16)
    {
      imc = (y - 13 * 16) >> 4;
      imc += imcen;

      int xx = 0;
      for(x = 0; x < 40 * 16; x += 16)
      {
        if(xx >= 20 && xx <= 33 && yy >= 5 && yy <= 19)
        {
          xx++;
          continue;
        }

        rec = (x - 20 * 16) >> 4;
        rec += recen;

        re = rec;
        im = imc;

        re2 = (re * re) >> 4;
        im2 = (im * im) >> 4;

        for(i = 0; i < 12; i++)
        {
          if((re2 + im2) > 4 * 16)
            break;

          im = (re * im) >> 3;
          im += imc;

          re = (re2 - im2) + rec;

          re2 = (re * re) >> 4;
          im2 = (im * im) >> 4;
        }

        if(i < 12)
        {
          byte c = mandel_colors[i];
          Memory.write8(loc1 + xx, c);
          Memory.write8(loc2 + xx, c);
        }

        if((xx & 7) == 7)
        {
          SID.voice1_frequency(note[re2 & 15]);
          SID.voice1_waveform(32);
          SID.voice1_waveform(33);
          SID.voice2_frequency(note[im2 & 15]);
          SID.voice2_waveform(32);
          SID.voice2_waveform(33);
        }

        xx++;
      }
      yy++;
      loc1 += 40;
      loc2 -= 40;
      SID.voice3_frequency(note[yy & 15]);
      SID.voice3_waveform(16);
      SID.voice3_waveform(17);
    }

    wait(10000);
  }

  public static void sprites()
  {
    int i, j;

    VIC.background(6);
    VIC.border(0);
    for(i = 880; i < 1000; i++)
    {
      Memory.write8(color_ram + i, (byte)5);
      Memory.write8(screen_ram + i, (byte)160);
    }

    int x0 = 0;
    int x1 = 0;
    int x2 = 0;
    int x3 = 0;
    int x4 = 0;
    int x5 = 0;
    int x6 = 0;
    int x7 = 0;

    int y0 = 55;
    int y1 = 65;
    int y2 = 75;
    int y3 = 85;
    int y4 = 95;
    int y5 = 105;
    int y6 = 115;
    int y7 = 125;

    VIC.sprite_enable(255);
    Memory.write8(sprite_pointer + 0, (byte)16);
    Memory.write8(sprite_pointer + 1, (byte)16);
    Memory.write8(sprite_pointer + 2, (byte)16);
    Memory.write8(sprite_pointer + 3, (byte)16);
    Memory.write8(sprite_pointer + 4, (byte)16);
    Memory.write8(sprite_pointer + 5, (byte)16);
    Memory.write8(sprite_pointer + 6, (byte)16);
    Memory.write8(sprite_pointer + 7, (byte)16);

    VIC.sprite0color(10);
    VIC.sprite1color(8);
    VIC.sprite2color(7);
    VIC.sprite3color(13);
    VIC.sprite4color(3);
    VIC.sprite5color(14);
    VIC.sprite6color(4);
    VIC.sprite7color(1);

    for(i = 0; i < 63; i++)
      Memory.write8(sprite_ram + i, (byte)sprite1[i]);

    VIC.sprite_expandx(255);
    VIC.sprite_expandy(255);

    for(i = 0; i < 512; i++)
    {
      if((i & 7) == 7)
        musak_play();

      VIC.wait_raster(251);
      VIC.sprite0pos(x0, y0 + balloon_height[(x0 >> 3) & 15]);
      VIC.sprite1pos(x1, y1 + balloon_height[(x1 >> 3) & 15]);
      VIC.sprite2pos(x2, y2 + balloon_height[(x2 >> 4) & 15]);
      VIC.sprite3pos(x3, y3 + balloon_height[(x3 >> 4) & 15]);
      VIC.sprite4pos(x4, y4 + balloon_height[(x4 >> 3) & 15]);
      VIC.sprite5pos(x5, y5 + balloon_height[(x5 >> 3) & 15]);
      VIC.sprite6pos(x6, y6 + balloon_height[(x6 >> 4) & 15]);
      VIC.sprite7pos(x7, y7 + balloon_height[(x7 >> 4) & 15]);

      x0 -= 1;
      x0 &= 0x1ff;
      x1 += 1;
      x1 &= 0x1ff;
      x2 -= 2;
      x2 &= 0x1ff;
      x3 += 2;
      x3 &= 0x1ff;
      x4 -= 3;
      x4 &= 0x1ff;
      x5 += 3;
      x5 &= 0x1ff;
      x6 -= 4;
      x6 &= 0x1ff;
      x7 += 4;
      x7 &= 0x1ff;

      if((x0 & 3) == 3)
      {
        y0++;
        if(y0 > 160)
          y0 = 160;
        y1++;
        if(y1 > 160)
          y1 = 160;
        y2++;
        if(y2 > 160)
          y2 = 160;
        y3++;
        if(y3 > 160)
          y3 = 160;
        y4++;
        if(y4 > 160)
          y4 = 160;
        y5++;
        if(y5 > 160)
          y5 = 160;
        y6++;
        if(y6 > 160)
          y6 = 160;
        y7++;
        if(y7 > 160)
          y7 = 160;
      }
    }

    VIC.sprite_enable(0);
    VIC.background(0);

    int vol = 15;
    for(i = 0; i < 19; i++)
    {
      VIC.wait_raster(251);
      Memory.write8(1904 + i, (byte)32);
      Memory.write8(1943 - i, (byte)32);
      Memory.write8(1944 + i, (byte)32);
      Memory.write8(1983 - i, (byte)32);
      Memory.write8(1984 + i, (byte)32);
      Memory.write8(2023 - i, (byte)32);
      vol--;
      if(vol < 0)
        vol = 0;
      SID.volume(vol);
    }

    for(i = 21; i >= 0; i--)
    {
      VIC.wait_raster(251);
      Memory.write8(color_row[i] + 19, (byte)5);
      Memory.write8(color_row[i] + 20, (byte)5);
      Memory.write8(screen_row[i] + 19, (byte)160);
      Memory.write8(screen_row[i] + 20, (byte)160);
    }

    for(i = 18; i >= 0; i--)
    {
      for(j = 0; j < 1000; j += 40)
      {
        Memory.write8(color_ram + j + i, (byte)5);
        Memory.write8(color_ram + 39 + j - i, (byte)5);
        Memory.write8(screen_ram + j + i, (byte)160);
        Memory.write8(screen_ram + 39 + j - i, (byte)160);
      }
    }
  }

  public static void green()
  {
    int i, j;
    int pulse1 = 1024;
    int pulse2 = 2048;
    int pulse3 = 3072;

    wait(5000);

    //                srad 
    SID.voice1_adsr(0xf000);
    SID.voice2_adsr(0xf000);
    SID.voice3_adsr(0xf000);
    SID.voice1_frequency(1024);
    SID.voice2_frequency(2048);
    SID.voice3_frequency(4096);
    SID.voice1_pulse_width(1024);
    SID.voice2_pulse_width(1024);
    SID.voice3_pulse_width(1024);
    SID.voice1_waveform(64);
    SID.voice1_waveform(65);
    SID.voice2_waveform(64);
    SID.voice2_waveform(65);
    SID.voice3_waveform(64);
    SID.voice3_waveform(65);
    SID.volume(15);

    for(i = 0; i < 63; i++)
      Memory.write8(sprite_ram + i, (byte)sprite2[i]);

    VIC.sprite0pos(32, 58);

    VIC.sprite0color(5);
    VIC.sprite_enable(1);
    wait(500);
    VIC.sprite0color(12);
    wait(500);
    VIC.sprite0color(13);
    wait(500);
    VIC.sprite0color(15);
    wait(500);
    VIC.sprite0color(7);
    wait(500);
    VIC.sprite0color(1);

    wait(5000);

    for(i = 38; i >= 7; i--)
    {
      int k = i;
      for(j = 24; j >= 0; j--)
      {
        Memory.write8(screen_row[j] + k, (byte)105);
        Memory.write8(screen_row[j] + k + 1, (byte)32);
        k++;
        if(k > 38)
          break;
      }

      pulse1 += 16;
      pulse2 += 16;
      pulse3 += 16;
      SID.voice1_pulse_width(pulse1);
      SID.voice2_pulse_width(pulse2);
      SID.voice3_pulse_width(pulse3);
    }

    wait(5000);

    for(i = 32; i <= 175; i++)
    {
      VIC.sprite0pos(i, i + 26);
      wait(50);
      pulse1 -= 16;
      pulse2 -= 16;
      pulse3 -= 16;
      SID.voice1_pulse_width(pulse1);
      SID.voice2_pulse_width(pulse2);
      SID.voice3_pulse_width(pulse3);
    }

    wait(4000);
    int vol = 15;

    for(i = 0; i < text2.length; i++)
    {
      Memory.write8(color_row[21] + i + 25, (byte)12);
      Memory.write8(screen_row[21] + i + 25, (byte)text2[i]);
      wait(400);
      Memory.write8(color_row[21] + i + 25, (byte)13);
      wait(400);
      Memory.write8(color_row[21] + i + 25, (byte)15);
      wait(400);
      Memory.write8(color_row[21] + i + 25, (byte)7);
      wait(400);
      Memory.write8(color_row[21] + i + 25, (byte)1);

      vol--;
      SID.volume(vol);
    }
    SID.volume(0);

    wait(10000);

    VIC.sprite_enable(0);

    for(i = 38; i >= 8; i--)
    {
      int k = i;
      for(j = 24; j >= 0; j--)
      {
        Memory.write8(screen_row[j] + k, (byte)233);
        Memory.write8(screen_row[j] + k + 1, (byte)160);
        Memory.write8(color_row[j] + k, (byte)4);
        Memory.write8(color_row[j] + k + 1, (byte)4);
        k++;
        if(k > 38)
          break;
      }
    }
  }

  public static void java_screen()
  {
    VIC.background(1);
    VIC.border(0);

    int i;

    for(i = screen_ram; i < screen_ram + 160; i++)
      Memory.write8(i, (byte)1);

    for(i = screen_ram + 160; i < screen_ram + 840; i++)
      Memory.write8(i, (byte)17);

    for(i = screen_ram + 840; i < screen_ram + 960; i++)
      Memory.write8(i, (byte)18);

    for(i = screen_ram + 960; i < screen_ram + 1000; i++)
      Memory.write8(i, (byte)34);

    VIC.hires_clear(0);

    for(i = 0; i < java_logo.length; i++)
      Memory.write8(hires_ram + 320 + i, (byte)java_logo[i]);

    for(i = 0; i < 240; i++)
    {
      Memory.write8(hires_ram + 7080 + i, (byte)java_redlogo[i]);
      Memory.write8(hires_ram + 7080 + 320 + i, (byte)java_redlogo[i + 240]);
    }

    VIC.sprite_enable(15);
    VIC.sprite_expandx(15);
    VIC.sprite_expandy(15);
    VIC.sprite0color(10);
    VIC.sprite1color(10);
    VIC.sprite2color(14);
    VIC.sprite3color(14);

    for(i = 0; i < java_sprite.length; i++)
      Memory.write8(sprite_ram + i, (byte)java_sprite[i]);

    Memory.write8(sprite_pointer + 0, (byte)16);
    Memory.write8(sprite_pointer + 1, (byte)17);
    Memory.write8(sprite_pointer + 2, (byte)18);
    Memory.write8(sprite_pointer + 3, (byte)19);

    VIC.sprite0pos(136, 112);
    VIC.sprite1pos(136 + 48, 112);
    VIC.sprite2pos(136, 112 + 42);
    VIC.sprite3pos(136 + 48, 112 + 42);

    VIC.hires_enable();

    wait(20000);

    VIC.background(0);

    VIC.text_clear(160);
    VIC.color_ram_clear(1);

    VIC.sprite_enable(0);

    VIC.text_enable();
  }

  public static void yin_yang()
  {
    VIC.background(5);
    VIC.border(0);

    VIC.text_clear(16);
    VIC.color_ram_clear(0);

    VIC.hires_enable();
    VIC.hires_clear(0);

    int j, k;
    int temp1 = 0;
    int temp2 = 0;

    for(j = 1; j <= 81; j++)
    {
      for(k = 65; k <= 129; k++)
      {
        //temp1 = (13 * (k - 65)) / 21;
        temp1 = (13 * (k - 65)) >> 4;
        temp1 *= temp1;

        temp2 = (41 - j);
        temp2 *= temp2;
        if((temp1 + temp2) > 1332 )
        {
          // this makes a checkerboard background
          if(((k & 1) ^ (j & 1)) == 1)
          {
            VIC.hires_plot(95 + (129 - k), 60 + (82 - j), 1);
            VIC.hires_plot(95 + k, 60 + j, 1);
          }
          continue;
        }

        temp2 = (23 - j);
        temp2 *= temp2;
        temp2 += temp1;
        if(temp2 < 30 )
        {
          VIC.hires_plot(95 + k, 60 + j, 1);
          continue;
        }
        if(temp2 < 342 )
        {
          VIC.hires_plot(95 + (129 - k), 60 + (82 - j), 1);
          continue;
        }

        temp2 = (59 - j);
        temp2 *= temp2;
        temp2 += temp1;
        if(temp2 < 30 )
        {
          VIC.hires_plot(95 + (129 - k), 60 + (82 - j), 1);
          continue;
        }
        if(temp2 < 342 )
        {
          VIC.hires_plot(95 + k, 60 + j, 1);
          continue;
        }

        VIC.hires_plot(95 + k, 60 + j, 1);
      }
    }
  }

  public static void main()
  {
    VIC.make_hires_tables();
    java_screen();
    wipe();
    musak_init();
    SID.volume(15);
    mandel();
    wipe();
    sprites();
    green();
    wipe();
    SID.volume(0);
    yin_yang();
    while(true);
  }
}

