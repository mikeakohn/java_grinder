#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Util.h"

static int errors = 0;

int test(const char *method_name, const char *method_sig, const char *field_name, const char *field_class, const char *expected)
{
  std::string function;
  std::string a;
  std::string b;
  std::string c;
  std::string d;

  a = method_name;
  b = method_sig;
  c = field_name;
  d = field_class;

  function = Util::get_virtual_function(a, b, c, d);

  if (std::string(function) != expected)
  {
    printf("Error: method_name=%s%s\n"
           "        field_name=%s\n"
           "       field_class=%s\n"
           "          expected=%s\n"
           "            output=%s\n",
      method_name, method_sig, field_name, field_class, expected, function.c_str());
    errors++;
    return -1;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  // Not sure if this is correct output.
  test("println", "()V", "out", "java/lang/System", "system_out_println_");

  return errors == 0 ? 0 : -1;
}

