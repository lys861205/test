#include <thread>
#include <memory>
#include <unistd.h>
#include <stdio.h>
#ifdef __LINUX__
#include <sys/epoll.h>
#else 
     #include <sys/event.h>
     #include <sys/time.h>
#endif 
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <arpa/inet.h>       
#include <errno.h>
#include <assert.h>
#include <sys/wait.h>
#include <string>
#include <sched.h>

#ifdef __LINUX__ 
  typedef struct epoll_event poller_event_t;
#else 
  typedef struct kevent poller_event_t;
#endif 

#define kReadEvent 1
#define kWriteEvent (1 << 1)
#define kErrorEvent (1 << 2)

class Thread {
public:
  Thread(int no):_running(false), _no(no) {
  }
  virtual ~Thread() {
  }

  void Start() {
    _running = true;
    _thread.reset(new std::thread(std::bind(&Thread::Serve, this)));
  }

  void Wait() {
    _thread->join();
  }

  void Stop() {
    _running = false;
    _thread->join();
  }

  int SetAffinity() {
  #ifdef __LINUX__
     cpu_set_t mask;
     CPU_ZERO(&mask);  
     CPU_SET(_no,&mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
      printf("sched_setaffinitya failed.\n");
    }
 #endif 
    return 0;
  }

  virtual void Serve() {};

protected:
  int _no;
  int _running;
  std::unique_ptr<std::thread> _thread;
};

class Server2 : public Thread {
public:

  Server2(short port, int no):
#ifdef __LINUX__
    _epollfd(::epoll_create1(EPOLL_CLOEXEC)),
#else  //macOS
    _epollfd(::kqueue()),
#endif 
    _port(port), 
    Thread(no) {
      assert(_epollfd != -1);
  }

  ~Server2() {
    ::close(_epollfd);
    ::close(_fd);
  }

  int create_fd_and_listen() {
    #ifdef __LINUX__
    _fd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC, 0);
    #else 
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    #endif 
    if (_fd < 0) {
      return -1;
    }
    int reuse = 1;
    if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
      perror("SO_REUSEADDR error");
      return -1;
    }
    if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &reuse , sizeof(int)) < 0) {
      perror("SO_REUSEPORT error");
      return -1;
    }
    sockaddr_in addr = {
             sin_family : AF_INET,
             sin_port : htons(_port),
             sin_addr : {s_addr : INADDR_ANY}
             //sin_addr : {s_addr : inet_addr(_ip.c_str())}
    };
    if (::bind(_fd, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
      return -1;
    }
    if (::listen(_fd, 10)) {
      return -1;
    }

{
    struct sockaddr_in addr;
    socklen_t addr_len;
    if (::getsockname(_fd, (sockaddr*)&addr, &addr_len) < 0) {
      return -1;
    }
    printf("local: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
}
    return 0;
  }

  inline int add_fd_epoll(int fd) {
  #ifdef __LINUX__
    struct epoll_event event; 
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    return ::epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event);
  #else //macOS
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_READ, EV_ADD, 0, 0, (void*)&fd);
    return kevent(_epollfd, &ev, 1, NULL, 0, NULL);
  #endif 
  }


  inline int del_fd_epoll(int fd) {
  #ifdef __LINUX__
    struct epoll_event event; 
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    return ::epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &event);
  #else //macOS 
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_READ, EV_DELETE, 0, 0, (void*)&fd);
    return kevent(_epollfd, &ev, 1, NULL, 0, NULL);
  #endif 
  }

  int poller_wait(poller_event_t* events, int maxevents)
  {
    #ifdef __LINUX__ 
      return ::epoll_wait(_epollfd, events, maxevents, -1);
    #else  //macOS
      return ::kevent(_epollfd, NULL, 0, events, maxevents, NULL);
    #endif 
  }
  int poller_fd(poller_event_t* event)
  {
    #ifdef __LINUX__ 
    return event->data.fd; 
    #else //macOS
    //return *(int*)event->udata;
    return event->ident;
    #endif
  }
  int poller_event(poller_event_t* event)
  {
    #ifdef __LINUX__ 
      if (event->events & EPOLLIN) {
        return kReadEvent;
      }
      else if (event->events & EPOLLOUT) {
        return kWriteEvent;
      }
      else {
        return kErrorEvent;
      }
    #else 
      if (event->filter & EVFILT_READ) {
        return kReadEvent;
      }
      else if (event->filter & EVFILT_WRITE) {
        return kWriteEvent;
      }
      else {
        return kErrorEvent;
      }
    #endif 
  }

  virtual void Serve() {

    //SetAffinity();

    if (create_fd_and_listen() < 0) {

      perror("create fd and listen failed");
      return;
    }
    add_fd_epoll(_fd);

    const int maxevents = 128;
    poller_event_t events[maxevents];

    while (_running) {
      int nevent = poller_wait(events, maxevents);
      if (nevent == 0) {
        printf("epoll_wait timeout\n");
        continue;                                                                                                                                                                                         
      }
      else if (nevent < 0 && errno == EINTR) {
        continue;
      } else if (nevent < 0) {
        printf("epoll_wait error: %d\n", errno);
        break;
      }
      for (int i = 0; i < nevent; ++i) {
        int fd = poller_fd(&events[i]);
        if (fd == _fd) {
          struct sockaddr_in addr;
          socklen_t addr_len;
          #ifdef __LINUX__ 
          int connfd = ::accept4(_fd, (struct sockaddr*)&addr, &addr_len, SOCK_NONBLOCK|SOCK_CLOEXEC);
          #else 
          int connfd = ::accept(_fd, (struct sockaddr*)&addr, &addr_len);
          #endif 
          if (connfd < 0) {
            printf("accept error: %d\n", errno); 
            continue;
          }
          printf("\033[35m%ldnew connecting ...\033[0m\n", _thread->get_id());
          if (add_fd_epoll(connfd) < 0) {
            ::close(connfd);
          }
        } else {
          int event = poller_event(&events[i]);
          if (event & kReadEvent) {
            char buf[1024] = {0};
            int n = read(fd, buf, 1024);
            if (n) {
              printf("\033[35m%ld\033[0m:\n%s\n", _thread->get_id(), buf);
            }
          }
          del_fd_epoll(fd);
          ::close(fd);
        }
      }
    }
  }

private:
  int _fd;
  int _epollfd;
  short _port;
};

int main()
{
  std::vector<std::unique_ptr<Server2>> services;
  for (int i = 0; i < 10; ++i) {
    services.emplace_back(new Server2(8282, i));
    services.back()->Start();
  }
  for (int i = 0; i < 1; ++i) {
    services[i]->Wait();
  }
  return 0;
}

