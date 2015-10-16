// generates a sample for Apple IIgs sound

#include <stdio.h>
#include <math.h>
#include <malloc.h>

static double signum(double x)
{
  return (0 < x) - (x < 0);
}

// sound effect generator
// note: size must be 256, 512, 1024, 2048, 4096, 8192, 16384, 32768
static void sample(int waveform, double freq,
  double attack, double decay, double sustain, double release,
  double sweep, int length)
{
  if(freq < 1)
    freq = 1;
  if(freq > 16384)
    freq = 16384;

  int i, j, k;
  int rnd, test;
  int reset;
  double t;
  int size = length;

  int *buf = malloc(sizeof(int) * length);
  if(!buf)
    return;

  for(i = 0; i < length; i++)
    buf[i] = 0;

  sweep = 1.0 + (sweep / size);

  // waveform
  switch(waveform)
  {
    case 0:  // sine
      for(i = 0; i < size; i++)
      {
        t = freq * (double)i / size;
        buf[i] = (int)(127.0 * sin(2.0 * M_PI * t));
        freq *= sweep;
      }
      break;
    case 1:  // triangle
      for(i = 0; i < size; i++)
      {
        t = freq * (double)i / size;
        buf[i] = (int)(127.0 *
          (1.0 - 4.0 * abs(round(t - 0.25) - (t - 0.25))));
        freq *= sweep;
      }
      break;
    case 2:  // sawtooth
      for(i = 0; i < size; i++)
      {
        t = freq * (double)i / size;
        buf[i] = (int)(127.0 * (2.0 * (t - floor(t + 0.5))));
        freq *= sweep;
      }
      break;
    case 3:  // square
      for(i = 0; i < size; i++)
      {
        t = freq * (double)i / size;
        buf[i] = (int)(127.0 * signum(sin(2.0 * M_PI * t)));
        freq *= sweep;
      }
      break;
    case 4:  // noise
      rnd = (rand() & 255) - 128;
      reset = 1;
      for(i = 0; i < size; i++)
      {
        t = freq * (double)i / size;
        test = (int)(127.0 *
          (2.0 * (2.0 * t - floor(2.0 * t + 0.5))));
        buf[i] = rnd;
        if(!reset && test > 0)
        {
          rnd = (rand() & 255) - 128;
          reset = 1;
        }
        if(test < 0)
          reset = 0;
        freq *= sweep;
      }
      break;
  }

  // ADSR
  int mode = 0;
  int a = (int)(attack * size);
  int d = a + (int)(decay * size);
  int s = d + (int)(sustain * size);
  int r = s + (int)(release * size);

  if(a < 1)
    a = 1;
  if(d < a + 1)
    d = a + 1;
  if(s < d + 1)
    s = d + 1;
  if(r < s + 1)
    r = s + 1;

  if(a > size)
    a = size;
  if(d > size)
    d = size;
  if(s > size)
    s = size;
  if(r > size)
    r = size;

  double a_inc = 1.0 / a;
  double d_inc = 0.5 / (d - a);
  double r_inc = 0.5 / (r - s);

  double y = 0;

  for(i = 0; i < a; i++)
  {
    buf[i] *= y;
    y += a_inc;
  }

  for(i = a; i < d; i++)
  {
    buf[i] *= y;
    y -= d_inc;
  }

  for(i = d; i < s; i++)
  {
    buf[i] *= y;
  }

  for(i = s; i < r; i++)
  {
    buf[i] *= y;
    y -= r_inc;
  }

  for(i = r; i < size; i++)
  {
    buf[i] = 0;
  }

  size = ((double)r / 1024) * 1024;
printf("size = %d\n", size);

  // clip
  for(i = 0; i < length; i++)
  {
    if(buf[i] > 127)
      buf[i] = 127;
    if(buf[i] < -127)
      buf[i] = -127;
    buf[i] += 128;
  }

  printf("\n");

  for(i = 0; i < length; i++)
  {
    printf("%d, ", (char)buf[i]);
    if((i & 7) == 7)
      printf("\n");
  }

  printf("\n");

  if(buf)
    free(buf);
}

int main()
{
  sample(3, 2.0, 0.1, 0.5, 0.3, 0.2, 0.0, 256);

  return 0;
}

