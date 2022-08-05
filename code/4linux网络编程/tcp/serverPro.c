#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <signal.h>


void recycleChild(int arg){
  while(1){
    int ret = waitpid(-1, NULL, WNOHANG);
    if(ret == -1){
      break;// 所有子进程都g了
    }else if(ret == 0){
      // 还有活着
      break;
    }else{
      printf("child pro %d is recycled \n", arg);
    }
  }
}

int main(){
  // 信号捕捉
  struct sigaction act;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  act.sa_handler = recycleChild;
  sigaction(SIGCHLD, &act, NULL);


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


  while(1){
    // 4 接收连接
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &addrlen);
    // accept 的阻塞被信号终止了

    if(cfd == -1){
      if(errno == EINTR0){
        continue;
      }
      perror("accept");
      return -1;
    }

    pid_t pid = fork();
    if(pid == 0){
      // 输出客户端信息
      char clientIP[16];
      inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));

      unsigned short clientPort = ntohs(clientaddr.sin_port);
      printf("client ip is %s, port is %d\n", clientIP, clientPort);

      // 5 通信
      while(1){
        // 获取客户端数据
        char recvbuf[1024];
        int len = read(cfd, recvbuf, sizeof(recvbuf));
        if(len == -1){
          perror("read");
          return -1;
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
        write(cfd, str, strlen(str));
      }


      // close
      close(cfd);
      exit(0);
    }else{
      // waitpid(-1, NULL, WNOHANG); 这样不好
    }

    
  }
  
  close(lfd);
  return 0;
}