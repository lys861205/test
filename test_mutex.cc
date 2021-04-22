#include <mutex>
#include <stdio.h>

int main()
{
  std::mutex m;
  printf("%d\n", sizeof(m));
  return 0;
}
