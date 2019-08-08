#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

void func(void *arg)
{
  puts("1");
  puts("11");
  puts("111");
  puts("1111");
}

void context_test()
{
  char stack[128*1024];
  ucontext_t main, child;

  getcontext(&child);
  child.uc_stack.ss_sp = stack;
  child.uc_stack.ss_size = sizeof(stack);
  child.uc_stack.ss_flags = 0;
  child.uc_link = &main;

  makecontext(&child, (void(*)(void))func, 0);

  swapcontext(&main, &child);

  puts("main");
}

int main()
{

  context_test();
  return 0;
  
}
