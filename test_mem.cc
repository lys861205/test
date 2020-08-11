#include <stdio.h>
template<size_t size>
struct _XmemType 
{
  int data[size];
};

template<> struct _XmemType<0>{};


int main()
{
  typedef _XmemType<12> T;
  char* dest[128] = {0};
  char src[] = {"hello"};
  *((T*)dest) = *((T*)src);
  printf("dst ip: %p, src ip: %p dst: %s\n", dest, src, dest);
  return 0;
}
