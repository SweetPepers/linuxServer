#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){
  // create a socket
  int lfd = socket(PF_INET, SOCK_STREAM, 0);


  // bind
  struct sockaddr_in saddr;
  saddr.sin_port = htons(9999);
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = INADDR_ANY;
  bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

  // listen
  listen(lfd, 8);

  // create an epoll instance
  int epfd = epoll_create(1);

  // add fd into epfd
  struct epoll_event epev = {
    .events = EPOLLIN,
    .data.fd = lfd,
  };
  
  epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);
  struct epoll_event epevs[1024];

  while(1){
    int ret = epoll_wait(epfd, epevs, 1024, -1);
    if (ret == -1){
      perror("epoll_wait ");
      exit(-1);
    }else if(ret == 0){
      continue;
    }else {
      printf("ret == %d\n", ret);
      // have fd changed
      for(int i = 0;i<ret;i++){
        int currfd = epevs[i].data.fd;
        if(currfd == lfd){
        // new client arrive  
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len); // 每次调用 accept都是在lfd有效的情况下, 所以accept没机会阻塞
        // 设置cfd 非阻塞
        int flag = fcntl(cfd, F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(cfd, F_SETFL, flag);


        printf("client in\n");
        epev.events = EPOLLIN | EPOLLET;
        epev.data.fd = cfd;
        epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
        }else{
          // 有数据到达
          char buf[5]  = {0};
          int len = 0;
          // 循环读出所有数据
          while((len = read(currfd, buf, sizeof(buf))) > 0){  // 优先级问题
            printf("recv : %s, len %d\n", buf, len);
            write(currfd, buf, len);
          }
          if (len == -1){
            perror("read");
            exit(-1);
          }
          if(len == 0){
            printf("client closed\n");
            epoll_ctl(epfd, EPOLL_CTL_DEL, currfd, NULL);
            close(currfd);
          }
          // }else if(len > 0){
          //   printf("read :%s\n", buf);
          //   write(currfd, buf, strlen(buf)+1);
          // }
        }
      }
    }
  }
  close(lfd);
  close(epfd);
  return 0;
}
