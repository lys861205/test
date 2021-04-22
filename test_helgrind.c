#include <pthread.h>
#include <stdio.h>

int var = 0;

void* child_fn(void* arg)
{
  int* p = (int*)arg;
  printf("%d\n", *p);
  return NULL;
}

int main()
{
  pthread_t child;
  int* p = (int*)malloc(4);
  pthread_create(&child, NULL, child_fn,  (void*)p);
  *p = 10;
  pthread_join(child, NULL);
  return 0;
}
