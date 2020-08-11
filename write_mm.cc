#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct Rep {
  int a;
  int b;
};

void* func1(void* p) 
{
  Rep* rep = (Rep*)p;
  for( ;; )
  {
    usleep(100); 
    rep->a = 10;
    rep->b = 20;
    printf("fun1: %d %d\n", rep->a, rep->b);
  }
  return NULL;
}

void* func2(void* p) 
{
  Rep* rep = (Rep*)p;
  for( ;; )
  {
    usleep(100); 
    rep->a = 40;
    rep->b = 50;
    printf("fun2: %d %d\n", rep->a, rep->b);
  }
  return NULL;
}

int main()
{
  Rep* _rep = (Rep*)malloc(sizeof(Rep)); 
  
  pthread_t tid[2];
  pthread_create(&tid[0], NULL,  func1, _rep);
  pthread_create(&tid[1], NULL,  func2, _rep);

  for ( int i=0; i < 2; i++ )
  {
    pthread_join(tid[i], NULL);
  }
  return  0;
}
