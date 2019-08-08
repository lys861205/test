#include <stdio.h>
#include <unistd.h>
#include <ucontext.h>

int main()
{
  ucontext_t context;
  getcontext(&context);
  puts("Hello context");
  sleep(1);
  setcontext(&context);
  return 0;
}
