#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int base = 4 * 0xFF;

int l = 4;
int get_index(int value)
{
  int mod = value % base; 
  for (int i = 0; i < l; ++i) 
  {
    int B = l - i - 1;
    if (mod - (B*0xFF) >=0) {
      return B;
    }
  }
}

int main()
{
  srand(time(NULL)); 
  int c_0 = 0; 
  int c_1 = 0; 
  int c_2 = 0; 
  int c_3 = 0; 
  for (int i = 0; i < 10000000; ++i)
  {
    int value = rand();
    if (get_index(value) == 0) 
    {
      ++c_0;
    }
    if (get_index(value) == 1) 
    {
      ++c_1;
    }
    if (get_index(value) == 2) 
    {
      ++c_2;
    }
    if (get_index(value) == 3) 
    {
      ++c_3;
    }
  }
  printf("%d %d %d %d\n", c_0, c_1, c_2, c_3);
  return 0;
}


