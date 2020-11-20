#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

const int N = 10;

#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))

class Rep
{
  char cc;
  float f;

  public:
  void print() { std::cout << "ADDR: " << this << std::endl; }
  void set_f( float _f ) { std::cout << "set f val : " << _f << std::endl; f = _f; }
  void get_f() { std::cout << "get f val : " << f << std::endl; }
};

struct M_st {
  int ref;
  int value;
  char ptr[0];
};

void* Malloc(int size) 
{
  M_st* pM = (M_st*)malloc(sizeof(M_st) + size);
  pM->ref = 1;
  pM->value = 1;
  printf("malloc: %p %p\n", pM, pM->ptr);
  return pM->ptr;
}

void Free(void* ptr) 
{
  M_st* pM = (M_st*)((char*)(ptr) - offsetof(M_st, ptr));
  
  printf("free %p value: %d ref: %d ptr: %d\n", pM, pM->value, pM->ref, *(int*)pM->ptr);
}

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

  printf("-------------------------\n");
  int* p = (int*)Malloc(4);
  *p = 40;
  Free((void*)p);
  /*
  void* __p = malloc(100);
  int* pi = new (__p) int;
  printf("p %p\n", __p);
  printf("pi %p\n", pi);
  */
  return 0;
}
