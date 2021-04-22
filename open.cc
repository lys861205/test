#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 512

void test_open_direct() {
  unsigned char *buf;
  int ret = posix_memalign((void **)&buf, 512, BUF_SIZE);
  if (ret) {
    perror("posix_memalign failed");
    exit(1);
  }
  //int fd = open("w2v_buf.bin", O_DIRECT | O_RDONLY);
  int fd = open("w2v_buf.bin", O_RDONLY);
  while (1) {
    ssize_t n = read(fd, (void*)buf, 10240);
    if (n <= 0) {
      printf("read failed: %d\n", errno);
      break;
    }
    printf("nread: %ld\n", n);
  }
  close(fd);
}

int main()
{
  test_open_direct();
  return 0;
}
