#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

const int N = 10;

class Rep
{
  char cc;
  float f;

  public:
  void print() { std::cout << "ADDR: " << this << std::endl; }
  void set_f( float _f ) { std::cout << "set f val : " << _f << std::endl; f = _f; }
  void get_f() { std::cout << "get f val : " << f << std::endl; }
};

int main()
{
  // char* buff = new char[ sizeof(Rep) * N ];
  // memset( buff, 0, sizeof(Rep)*N );

  void* buff = malloc(200);
  printf("buff ADDR: %p\n", buff);

  Rep* pfoo = new (buff)Rep;

  pfoo->print();

  pfoo->set_f(1.0f);
  pfoo->get_f();


  /*
  void* __p = malloc(100);
  int* pi = new (__p) int;
  printf("p %p\n", __p);
  printf("pi %p\n", pi);
  */
  return 0;
}
