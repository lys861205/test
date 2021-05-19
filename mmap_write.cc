#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)



int main(int argc, char *argv[]) 
{
  int fd = open(argv[1], O_RDWR);
  size_t length = 1024 * 1024;
  ftruncate(fd, length);
  char* addr = (char*)mmap(NULL, length, PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    handle_error("mmap");
  }
  close(fd);
  strcpy(addr, "Hello mmap");
  munmap(addr, length);
  return 0;
}
