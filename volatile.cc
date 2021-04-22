#include <stdio.h>
#include <unistd.h>

volatile bool quit = false;

class A {
  explicit A() {
  }
};

int main()
{
  while (!quit) {
    sleep(1); 
  }
  return 0;
}
