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

int main()
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in addr = {
    sin_family : AF_INET,
    sin_port : htons(8181),
    //sin_addr : {s_addr : INADDR_ANY}
    sin_addr : {s_addr : inet_addr("10.129.148.209")}
  };
  // int on = 1;
  // if (setsockopt(fd,SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on)) < 0) {
  //   perror("setsockopt failed");
  // }
  char buffer[32];
  connect(fd, (const struct sockaddr*)&addr, sizeof(addr)); 
  int n = recv(fd, (void*)buffer, 32, 0);
  printf("recv: %d\n", n);
  close(fd);
  return 0;
}
