#include <stdio.h>

int fun(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k)
{
  printf("fun\n");
  return 100;
}

int main()
{
  int i = 2;
  int* p = &i;
  fun(1,1,1,1,1,1,1,1,1,1,1);
  return 0;
}
