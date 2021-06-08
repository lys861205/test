#include <stdio.h> 
#include<signal.h> 
#include <unistd.h>
char **args;
void exithandle(int sig)
{
  printf("%s : sighup received ",args[1]);
} 

int main(int argc,char **argv)
{
  args = argv;
  //signal(SIGHUP,exithandle);
  signal(SIGHUP,SIG_IGN);
  pause();
  return 0;
}
