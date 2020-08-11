#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;

void* func(void* p)
{
while (1) {
  pthread_mutex_lock(&mutex);
  int a = 2;
  int b = 2;
  //pthread_mutex_lock(&mutex);
  int sum = a + b;
  pthread_mutex_unlock(&mutex);
  sleep(2);
  break;
}
}

int main()
{
  pthread_mutex_init(&mutex, NULL);
  pthread_t t;
  pthread_create(&t, NULL, func, NULL);
  pthread_join(t, NULL);
  return 0;
}
