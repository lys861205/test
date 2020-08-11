#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>

typedef struct _Rep_tag
{
  long   _M_length;
  long   _M_capacity;
  long  _M_refcount;
}Rep;

void bar(int a)
{
  void* buff = malloc(1583680956688180);
  Rep* _rep = new (buff)Rep;
  _rep->_M_capacity = 10;
  _rep->_M_refcount = 0;
}

int main()
{
  int b = 3;
  bar(b);
  return 0;
}
