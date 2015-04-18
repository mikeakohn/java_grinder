#include <stdio.h>
#include <stdlib.h>

void init();
int add_nums_II(int a, int b);
int sub_nums_II(int a, int b);
int mul_nums_II(int a, int b);
int div_nums_II(int a, int b);
int mod_nums_II(int a, int b);
int shift_nums_II(int a, int b);
int loop_test_II(int a, int b);
void put_static_I(int a);
int get_static();
int method_call_test_II(int a, int b);
int method_call_long_test_I(int a);
int add_array_aB(int8_t *a);
void fill_array_aB(int8_t *a);
int add_array_aS(int16_t *a);
void fill_array_aS(int16_t *a);
int add_array_aI(int32_t *a);
void fill_array_aI(int32_t *a);

struct _byte_array
{
  int len;
  int8_t data[];
};

struct _short_array
{
  int len;
  int16_t data[];
};

struct _int_array
{
  int len;
  int32_t data[];
};

int main(int argc, char *argv[])
{
  int a,b,i;
  struct _byte_array *byte_array = alloca(4 + sizeof(int));
  struct _short_array *short_array = alloca(4 * sizeof(int16_t) + sizeof(int));
  struct _int_array *int_array = alloca(4 * sizeof(int32_t) + sizeof(int));

  if (argc != 3)
  {
    printf("Usage: %s <num> <num>\n", argv[0]);
    exit(0);
  }

  byte_array->len = 4;
  byte_array->data[0] = 1;
  byte_array->data[1] = 2;
  byte_array->data[2] = 3;
  byte_array->data[3] = 4;

  short_array->len = 4;
  short_array->data[0] = 10;
  short_array->data[1] = 20;
  short_array->data[2] = 30;
  short_array->data[3] = 40;

  int_array->len = 4;
  int_array->data[0] = 100;
  int_array->data[1] = 200;
  int_array->data[2] = 300;
  int_array->data[3] = 400;

  a = atoi(argv[1]);
  b = atoi(argv[2]);

  init();

  printf("%d + %d = %d\n", a, b, add_nums_II(a,b));
  printf("%d - %d = %d\n", a, b, sub_nums_II(a,b));
  printf("%d * %d = %d\n", a, b, mul_nums_II(a,b));
  printf("%d / %d = %d\n", a, b, div_nums_II(a,b));
  printf("%d %% %d = %d\n", a, b, mod_nums_II(a,b));
  printf("%d << %d = %d\n", a, b, shift_nums_II(a,b));
  printf("loop test (%d,%d)  %d\n", a, b, loop_test_II(a,b));
  printf("get_static()=%d\n", get_static());
  printf("put_static(%d)\n", a);
  put_static_I(a);
  printf("get_static()=%d\n", get_static());
  printf("method_call_test_II(%d,%d)=%d\n", a, b, method_call_test_II(a,b));
  printf("method_call_long_test_I(%d)=%d\n", a, method_call_long_test_I(a));

  printf("add_array_aB(%p)=%d\n", byte_array, add_array_aB(byte_array->data));
  printf("fill_array_aB(%p)={", byte_array);
  fill_array_aB(byte_array->data);
  for (i = 0; i < 4; i++) { printf(" %d", byte_array->data[i]); }
  printf(" }\n");

  printf("add_array_aS(%p)=%d\n", short_array, add_array_aS(short_array->data));
  printf("fill_array_aS(%p)={", short_array);
  fill_array_aS(short_array->data);
  for (i = 0; i < 4; i++) { printf(" %d", short_array->data[i]); }
  printf(" }\n");

  printf("add_array_aI(%p)=%d\n", int_array, add_array_aI(int_array->data));
  printf("fill_array_aI(%p)={", int_array);
  fill_array_aI(int_array->data);
  for (i = 0; i < 4; i++) { printf(" %d", int_array->data[i]); }
  printf(" }\n");

  return 0;
}

