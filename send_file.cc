#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <arpa/inet.h>       

void test_sendfile(const char* outfile, int fd) {
  int fd_out = open(outfile, O_RDONLY);
  //int fd_in  = open(infile, O_CREAT | O_RDWR);
  if (-1 == fd_out) {
    perror("open failed");
    return;
  }
  if (-1 == sendfile(fd, fd_out, NULL, 10240)) {
    perror("send file");
  }
  close(fd_out);
}

int main()
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in addr = {
    sin_family : AF_INET,
    sin_port : htons(8181),
    //sin_addr : {s_addr : INADDR_ANY}
    sin_addr : {s_addr : inet_addr("10.139.48.96")}
  };
  connect(fd, (const struct sockaddr*)&addr, sizeof(addr)); 
  test_sendfile("c_call_lua.cc", fd);
  close(fd);
  return 0;
}
