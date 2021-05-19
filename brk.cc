#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>


int main()
{
  printf("cat /proc/%d/maps\n", getpid());
  getchar();
  printf("sbrk 1G\n");
  void* p = sbrk(1024*1024*1024);
  printf("%p\nsbrk 4", p);
  getchar();
  p = sbrk(4);
  printf("%p\nbrk", p);
  getchar();
  brk(p);
  getchar();
  return 0;
}
