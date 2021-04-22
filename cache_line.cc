#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdint.h>

struct cache {
  uint64_t a;
  uint64_t b;
};

struct cache_padding {
  uint64_t pad1[7];
  uint64_t a;
  uint64_t pad2[7];
  uint64_t b;
  uint64_t pad3[7];
};

cache c = {
  .a = 0,
  .b = 0,
};

cache_padding cl = {
  .pad1 = {0},
  .a = 0,
  .pad2 = {0},
  .b = 0,
  .pad3 = {0},
};

bool padding = false;

void* test_padding(void* p)
{
  int* flag = (int*)p;
  if (*flag % 2 == 0)
  {
    for (int i=0; i < 1000000000; ++i) 
    {
      cl.a = i;
    }
  }
  else 
  {
    for (int i=0; i < 1000000000; ++i) 
    {
      cl.b = i;
    }
  }
  return NULL;
}

void* test_nopadding(void* p)
{
  int* flag = (int*)p;
  if (*flag % 2 == 0)
  {
    for (int i=0; i < 1000000000; ++i) 
    {
      c.a = i;
    }
  }
  else 
  {
    for (int i=0; i < 1000000000; ++i) 
    {
      c.b = i;
    }
  }
  return NULL;
}

int main(int argc, char* argv[])
{
  pthread_t threads[4];
  struct timeval tv;

  gettimeofday(&tv, NULL);
  uint64_t start = tv.tv_sec * 1000000 + tv.tv_usec;
  for (int i=0; i < 4; ++i) 
  {
    pthread_create(&threads[i], NULL, &test_nopadding, (void*)&i);
  }
  for (int i=0; i < 4; ++i) 
  {
    pthread_join(threads[i], NULL);
  }
  gettimeofday(&tv, NULL);
  uint64_t end = tv.tv_sec * 1000000 + tv.tv_usec;
  printf("No Paddding %llu us\n", end - start);

  //padding
  gettimeofday(&tv, NULL);
  start = tv.tv_sec * 1000000 + tv.tv_usec;
  for (int i=0; i < 4; ++i) 
  {
    pthread_create(&threads[i], NULL, &test_padding, (void*)&i);
  }
  for (int i=0; i < 4; ++i) 
  {
    pthread_join(threads[i], NULL);
  }
  gettimeofday(&tv, NULL);
  end = tv.tv_sec * 1000000 + tv.tv_usec;
  printf("Paddding %llu us\n", end - start);
  return 0;
}
