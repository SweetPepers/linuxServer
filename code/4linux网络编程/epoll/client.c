#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
   // 1. 创建
  // listen fd
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1){
    perror("socket");
    return -1;
  }
  // 2 连接服务端
  struct sockaddr_in serveraddr;
  inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr.s_addr);
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(9999);
  int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
  if(ret == -1){
    perror("connect");
    return -1;
  }

  int i = 0;
  // 通信
  // 发送数据
  while(1){
    char str[1024];
    // sprintf(str, "data : %d\n", i); 
    scanf("%s", str);
    
    write(fd, str, strlen(str)+1);

    sleep(1);

    // 获取server数据
    char recvbuf[1024];
    int len = read(fd, recvbuf, sizeof(recvbuf));
    if(len == -1){
      perror("read");
      return -1;
    }
    if(len > 0){
      printf("recv server data : %s\n", recvbuf);
    }else if(len == 0){
      // 客户端断开连接
      printf("server closed\n");
      break;
    }
    i++;
  }
  
  // 关闭连接
  close(fd);

}