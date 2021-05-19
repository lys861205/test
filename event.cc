#ifdef __LINUX__
#include <sys/eventfd.h>
#endif //
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE);} while(0)

int main(int argc, char** argv) 
{
  int efd, j;
  uint64_t u;
  ssize_t s;

#ifdef __LINUX__
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <num> ...\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  efd = ::eventfd(0, EFD_NONBLOCK|EFD_CLOEXEC); 
  if (efd == -1) {
    handle_error("eventfd");
  }

  switch(fork()) {
  case 0: //child process
    for (j = 1; j < argc; j++) {
      printf("Child writing %s to efd\n", argv[j]);
      u = strtoull(argv[j], NULL, 0);
      s = write(efd, &u, sizeof(u));
      if (s != sizeof(u)) {
        handle_error("write"); 
      }
    }
    printf("Child completed write loop\n");
    exit(EXIT_SUCCESS);
  break;
  default:
    sleep(2);
    printf("Parent about to read\n");
    s = read(efd, &u, sizeof(u));
    if (s != sizeof(u)) {
      handle_error("read");
    }
    printf("Parent read %llu (0x%x) from efd\n", (unsigned long long) u, (unsigned long long)u);
    exit(EXIT_SUCCESS);
  case -1:
    handle_error("fork");
  break;
  }
#endif //
  return 0;
}
