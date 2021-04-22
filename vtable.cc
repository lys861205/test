#include <stdio.h>

class A {
public:
  A(){}

  virtual void test() {}
};

class B {
public:
  B() {}
  void test() {}
};

int main()
{
  
  A* pa = new A();
  B* pb = new B();
  pa->test();
  pb->test();
  delete pa;
  return 0;
}
