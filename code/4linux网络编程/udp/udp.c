#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>


int main(){

  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(fd == -1){
    perror("socket udp");
    exit(-1);
  }

  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
  saddr.sin_port = htons(9999);

  int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
  if(ret == -1){
    perror("bind");
    return -1;
  }

  struct sockaddr_in cliaddr;
  int len = sizeof(cliaddr);
  char buf[1024] = {0};
  while(1){
    int num = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, &len);
    if(num == -1){
      perror("recvfrom");
      return -1;
    }
    char ipbuf[16];
    printf("client IP is %s, Port %d\n",  
      inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr,ipbuf, sizeof(ipbuf)), 
      ntohs(cliaddr.sin_port)
      );
    printf("client say : %s\n", buf);

    sendto(fd, buf, strlen(buf)+1, 0, (struct sockaddr *)&cliaddr, len);
  }
  close(fd);
}