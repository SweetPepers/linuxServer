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


  // 服务器地址
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
  saddr.sin_port = htons(9999);


  char buf[1024] = {0};
  int num = 0;
  while(1){

    // send
    sprintf(buf, "hello i am client %d\n", num++);
    sendto(fd, buf, strlen(buf)+1, 0, (struct sockaddr *)&saddr, sizeof(saddr));


    // recv
    // 这里后面要填两个NULL啊, 要不server返回过来的信息就给这改了, 之后还怎么发 
    int num = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);  
    if(num == -1){
      perror("recvfrom");
      return -1;
    }
    char ipbuf[16];
    printf("server IP is %s, Port %d\n",  
      inet_ntop(AF_INET, &saddr.sin_addr.s_addr,ipbuf, sizeof(ipbuf)), 
      ntohs(saddr.sin_port)
      );
    printf("server say : %s\n", buf);

    sleep(1);
  }
  close(fd);
}