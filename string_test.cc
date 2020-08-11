#include <string>
#include <stdio.h>

int main()
{
  std::string e("");
  printf("e size: %d\n", sizeof(e));
  std::string a = "Hello";
  std::string b = a;
  printf("a address: %p %s\n", (char*)a.data(), a.c_str());
  printf("b address: %p %s\n", (char*)b.data(), b.c_str());
  std::string c = a;
  c[0] = 'W';
  printf("a address: %p %s\n", (char*)a.data(), a.c_str());
  printf("c address: %p %s\n", (char*)c.data(), c.c_str());

  std::string d = a;
  char* p = (char*)d.data();
  p[0] = 'V';
  printf("a address: %p %s\n", (char*)a.data(), a.c_str());
  printf("d address: %p %s\n", (char*)d.data(), d.c_str());

  return 0;
}
