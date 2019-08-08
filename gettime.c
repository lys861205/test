#include <sys/time.h>
#include <stdio.h>

int main()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  printf("tv_sec: %ld, tv_usec: %ld\n", tv.tv_sec, tv.tv_usec);
  return 0;
}
