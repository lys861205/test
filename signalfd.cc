#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

  int
main(int argc, char *argv[])
{
  sigset_t mask;
  int sfd;
  struct signalfd_siginfo fdsi;
  ssize_t s;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGQUIT);

  /* Block signals so that they aren't handled
   *               according to their default dispositions */

  if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
    handle_error("sigprocmask");

  sfd = signalfd(-1, &mask, SFD_NONBLOCK|SFD_CLOEXEC);
  if (sfd == -1)
    handle_error("signalfd");

  int efd = epoll_create(256);

  struct epoll_event ev;
  ev.data.fd = sfd;
  ev.events |= EPOLLIN;
  epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &ev);
  struct epoll_event events[2];
  for (;;) {
    int nevents = epoll_wait(efd, events, 2, -1);
    for (int i=0; i < nevents; ++i) {
      int fd = events[i].data.fd;
      s = read(fd, &fdsi, sizeof(struct signalfd_siginfo));
      if (s != sizeof(struct signalfd_siginfo))
        handle_error("read");

      if (fdsi.ssi_signo == SIGINT) {
        printf("Got SIGINT\n");
      } else if (fdsi.ssi_signo == SIGQUIT) {
        printf("Got SIGQUIT\n");
        exit(EXIT_SUCCESS);
      } else {
        printf("Read unexpected signal\n");
      }
    }
  }
}

