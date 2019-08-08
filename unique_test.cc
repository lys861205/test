#include <memory>
#include <stdio.h>


std::unique_ptr<int> getPtr(int a)
{
  return std::unique_ptr<int>(new int(a));
}

int main()
{
  std::unique_ptr<int> p(new int(2));
  std::unique_ptr<int> p2 = getPtr(4);
  printf("value: %d\n", *p2);
  p2 = std::move(p);
  printf("value: %d\n", *p2);
  return 0;
}
