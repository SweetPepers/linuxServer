#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>


struct sockInfo{
  int fd;
  struct sockaddr_in addr;
  pthread_t tid;
};
#define SOCK_INFO_NUM (128)
#define SOCK_INFO_SIZE (sizeof(sockInfos[0]))

struct sockInfo sockInfos[SOCK_INFO_NUM];


void recycleChild(int arg);
void *working(void *arg);

int main(){
  // 1. 创建
  // listen fd
  int lfd = socket(AF_INET, SOCK_STREAM, 0);
  if (lfd == -1){
    perror("socket");
    return -1;
  }

  // 2. 绑定
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
  saddr.sin_port = htons(9999);

  int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
  if(ret == -1){
    perror("bind");
    return -1;
  }
    // 3. 监听
  ret = listen(lfd, 8);
  if(ret == -1){
    perror("listen");
    return -1;
  }


  // init sockinfos
  for(int i=0;i<SOCK_INFO_NUM;i++){
    bzero(&sockInfos[i], SOCK_INFO_SIZE);
    sockInfos[i].fd = -1;
    sockInfos[i].tid = -1;
  }
  

  while(1){
    // 4 接收连接
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &addrlen);
    // accept 的阻塞被信号终止了

    if(cfd == -1){
      perror("accept");
      return -1;
    }


    // 从数组中找一个没用的
    struct sockInfo *pinfo = NULL;
    for (int i = 0;i<SOCK_INFO_NUM;i++){
      if(sockInfos[i].fd == -1){
        pinfo = &sockInfos[i];
        break;
      }
      if(i == SOCK_INFO_NUM - 1 ){
        // the last time
        sleep(5);
        i=-1;
      }
    }
    assert(pinfo != NULL);
    

    pinfo->fd = cfd;
    memcpy(&pinfo->addr, &clientaddr, addrlen);
    pthread_create(&pinfo->tid, NULL, working, pinfo);
    
    pthread_detach(pinfo->tid);
  }
  
  close(lfd);
  return 0;
}



void *working(void *arg){
  // 通信
  // 输出客户端信息
  struct sockInfo *pinfo = (struct sockInfo*)arg;
  char clientIP[16];
  inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, clientIP, sizeof(clientIP));

  unsigned short clientPort = ntohs(pinfo->addr.sin_port);
  printf("client ip is %s, port is %d\n", clientIP, clientPort);

  // 5 通信
  while(1){
    // 获取客户端数据
    char recvbuf[1024];
    int len = read(pinfo->fd, recvbuf, sizeof(recvbuf));
    if(len == -1){
      perror("read");
      return NULL;
    }
    if(len > 0){
      printf("recv client data : %s\n", recvbuf);
    }else if(len == 0){
      // 客户端断开连接
      printf("client closed\n");
      break;
    }

    // 发送数据
    char *str = "hello i am a server\n";
    write(pinfo->fd, str, strlen(str));
  }

  // close
  close(pinfo->fd);
  exit(0);
}
