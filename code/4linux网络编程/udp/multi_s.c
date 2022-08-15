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
  // 设置广播属性
  struct in_addr op;
  inet_pton(AF_INET, "239.0.0.10", &op.s_addr);
  setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &op, sizeof(op));

  // 不用绑定了  只发送不接收

  // 初始化客户端信息
  struct sockaddr_in boardaddr;
  boardaddr.sin_family = AF_INET;
  boardaddr.sin_port = htons(9999);
  inet_pton(AF_INET, "239.0.0.10", &boardaddr.sin_addr.s_addr);


  char buf[1024] = {0};

  int num = 0;


  while(1){
    sprintf(buf, "hello client %d\n", num++);
    sendto(fd, buf, strlen(buf)+1,0,(struct sockaddr*)&boardaddr, sizeof(boardaddr));
    printf("multicast data : %s\n", buf);
    sleep(1);
  }
  close(fd);
}