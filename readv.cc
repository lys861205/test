#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>


int main()
{
  int fd = open("Any.cc", O_RDONLY);
  if (fd == -1) {
    printf("open Any.cc failed, error: %d\n", errno);
    return -1;
  }
  char buf1[1024];
  char buf2[1024];
  struct iovec vec[2];
  vec[0].iov_base = buf1;
  vec[0].iov_len  = sizeof buf1;
  vec[1].iov_base = buf2;
  vec[1].iov_len  = sizeof buf2;
  ssize_t n = readv(fd, vec, 2);
  printf("read bytes: %ld \n%s%s\n", n, buf1, buf2);
  close(fd);
}
