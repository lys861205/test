#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  size_t r;
  int i;
  int status;
  char buf[1024];
  pid_t p;
  int fd[2];
  if(0!=pipe(fd))
  {
    exit(0);
  }
  p = fork();
  if ( p < 0 ){
    exit(0);
  }
  else if( p==0 )
  {
    close(fd[0]);
    for(i=0; i < 2; i++){
      write(fd[1], "Hello Pipe", sizeof("Hello Pipe")); 
      sleep(1);
    }
    close(fd[1]);
    exit(1);
  }
  signal(SIGCHLD, SIG_IGN);
  printf("parent start!\n");
  close(fd[1]);
  do {
    r = read(fd[0], buf, 1024);
    if ( r <= 0){
      perror("read error:");
      sleep(10);
    }
    printf("%s\n", buf);
  }while(1);
  close(fd[0]);
  waitpid(p, &status, 0);
  return 0;
}


