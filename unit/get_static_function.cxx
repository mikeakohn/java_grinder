#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "invoke.h"

static int errors = 0;

int test(const char *method_name, const char *method_sig, const char *expected)
{
  char function[256];
  std::string a;
  std::string b;

  a = method_name;
  b = method_sig;

  //strcpy(a, method_name);
  //strcpy(b, method_sig);

  //Util::method_sanitize(a, b, param_count);
  get_static_function(function, a, b);

  //if (function != expected)
  if (strcmp(function, expected) != 0)
  {
    printf("Error: method=%s%s\n"
           "     expected=%s\n"
           "       output=%s\n",
      method_name, method_sig, expected, function);
    errors++;
    return -1;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  test("allocStackShorts", "(I)[S", "allocStackShorts_I");
  test("Box_run", "()V", "Box_run");
  test("clearBitmap", "()V", "clearBitmap");
  test("clearText", "()V", "clearText");
  test("ClosingPage_run", "()V", "ClosingPage_run");
  test("Common_drawLine", "(IIIII)V", "Common_drawLine_IIIII");
  test("Common_wait", "(I)V", "Common_wait_I");
  test("ImageInside_fadeIn", "()V", "ImageInside_fadeIn");
  test("ImageInside_run", "()V", "ImageInside_run");
  test("ImageInside_shiftFull", "()V", "ImageInside_shiftFull");
  test("ImageInside_shiftLines", "()V", "ImageInside_shiftLines");
  test("ImageJavaGrinder_run", "()V", "ImageJavaGrinder_run");
  test("ImageJava_run", "()V", "ImageJava_run");
  test("initBitmap", "()V", "initBitmap");
  test("inVerticalBlank", "()Z", "inVerticalBlank");
  test("loadFonts", "()V", "loadFonts");
  test("loadZ80", "([B)V", "loadZ80_aB");
  test("Mandelbrots_run", "()V", "Mandelbrots_run");
  test("Model_run", "()V", "Model_run");
  test("NakenLogo_run", "()V", "NakenLogo_run");
  test("pauseZ80", "()V", "pauseZ80");
  test("plot", "(III)V", "plot_III");
  test("print", "(Ljava/lang/String;)V", "print_X");
  test("setCursor", "(II)V", "setCursor_II");
  test("setHorizontalScrollB", "(II)V", "setHorizontalScrollB_II");
  test("setHorizontalScrollModeFull", "()V", "setHorizontalScrollModeFull");
  test("setHorizontalScrollModeLine", "()V", "setHorizontalScrollModeLine");
  test("setImageData", "([S)V", "setImageData_aS");
  test("setPaletteColor", "(I)V", "setPaletteColor_I");
  test("setPaletteColorsAtIndex", "(I[S)V", "setPaletteColorsAtIndex_IaS");
  test("setPaletteColors", "([S)V", "setPaletteColors_aS");
  test("setPalettePointer", "(I)V", "setPalettePointer_I");
  test("setPatternTableAtIndex", "(I[I)V", "setPatternTableAtIndex_IaI");
  test("setPatternTable", "([I)V", "setPatternTable_aI");
  test("setSpriteConfig1", "(II)V", "setSpriteConfig1_II");
  test("setSpriteConfig2", "(II)V", "setSpriteConfig2_II");
  test("setSpritePosition", "(III)V", "setSpritePosition_III");
  test("setVerticalScrollB", "(I)V", "setVerticalScrollB_I");
  test("Sprites_run", "()V", "Sprites_run");
  test("Sprites_spin", "()V", "Sprites_spin");
  test("Stars_moveStars", "([S)V", "Stars_moveStars_aS");
  test("Stars_run", "()V", "Stars_run");
  test("waitHorizontalBlank", "()V", "waitHorizontalBlank");
  test("waitVerticalBlank", "()V", "waitVerticalBlank");

  return errors == 0 ? 0 : -1;
}

