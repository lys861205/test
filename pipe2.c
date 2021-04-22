#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/syscall.h>

void* thread_fun(void* arg)
{
  size_t w;
  int* fd;
  char buffer[128];
  int size;
  int tid;
  tid = syscall(SYS_gettid);
  size = snprintf(buffer, sizeof buffer, "I'am thread of %lu.\n", tid);
  fd = (int*)arg;
  printf("write text: %s", buffer);
  while(1)
  {
    w = write(*fd, buffer, size);
    sleep(10);
  }
  return (void*)0;
}

int main()
{
  pthread_t tid; 
  int fd[2];
  char buf[256];
  size_t s;
  if(0 !=pipe(fd))
  {
    perror("create pipe failed."); 
    exit(-1);
  }

  if ( 0 != pthread_create(&tid, NULL, thread_fun, (void*)&fd[1]) )
  {
    perror("create thread failed.");
    exit(-1);
  }
  printf("thread id: %llu\n", tid);
  pid_t pid = fork();
  if ( pid < 0 )
  {
    perror("fork failed.");
    exit(-1);
  }
  else if( pid == 0 ) //child
  {
    close(fd[0]); 
    char buffer[64];
    size_t size = snprintf(buffer, sizeof buffer, "I'am process of %d.\n", getpid());
    printf("write %d text: %s", size, buffer);
    while(1)
    {
      write(fd[1], buffer, size);
      sleep(10);
    }
    close(fd[1]);
    exit(0);
  }

  printf("parent process start.\n");
  signal(SIGCHLD, SIG_IGN);
  
  for(;;)
  {
    memset(buf, 0, sizeof buf);
    s = read(fd[0], buf, sizeof buf);
    if( s<0 )
    {
      perror("read failed.");
      break;
    }
    printf("read text: %s", buf);
  }
  pthread_join(tid, NULL);
  close(fd[0]);
  close(fd[1]);
  return 0;
}
