#include <sys/timerfd.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <string.h>

void* func(void* arg)
{
  int* timerfd = (int*)arg;  
  struct itimerspec newValue;
  struct itimerspec oldValue;
  while(1)
  {
    memset(&newValue, 0, sizeof newValue);
    memset(&oldValue, 0, sizeof oldValue);
    newValue.it_value.tv_sec = 2;
    newValue.it_interval.tv_sec = 4;
    ::timerfd_settime(*timerfd, 0, &newValue, &oldValue);
    pause();
  }
}


int main()
{
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  pthread_t tid;
  pthread_create(&tid, NULL, func, &timerfd);

  int epollfd = ::epoll_create(256);

  struct epoll_event ev;
  ev.data.fd = timerfd;
  ev.events = EPOLLIN | EPOLLOUT;
  ::epoll_ctl(epollfd, EPOLL_CTL_ADD, timerfd, &ev);

  struct epoll_event oev[2];
  int nev = 2;
  for(;;)
  {
    int r = epoll_wait(epollfd, oev, nev, -1);
    if ( r == 0)
    {
      printf("epoll_wait timeout!\n");
      continue;
    }
    else if ( r < 0 )
    {
      perror("epoll_wait error!");
      break;
    }
    printf("event comming!\n");
    for ( int i=0; i < r ; i++ )
    {
      if ( oev[i].events & EPOLLIN )
      {
        uint64_t how;
        read(oev[i].data.fd, &how, sizeof how);
        printf("timerfd readable %llu.\n", how); 
      }
      if ( oev[i].events & EPOLLOUT )
      {
        printf("timerfd writeable.\n"); 
      }
    }
  }
  close(epollfd);
  close(timerfd);
  return 0;
}
