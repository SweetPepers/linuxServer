#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h> 
#include <signal.h>
#include "header/locker.h"
#include "header/threadpool.h"
#include "header/http_conn.h"


#define MAX_FD (65535)
#define MAX_EVENT_NUM (10000)

// 添加信号捕捉
void addsig(int sig, void(handler)(int)){
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handler;
  sigfillset(&sa.sa_mask);
  sigaction(sig, &sa, NULL);
}

// 添加文件描述符到epoll
extern void addfd(int epollfd, int fd, bool oneshot);
// 删除epoll中的fd
extern void rmfd(int epollfd, int fd);
// 修改fd
void modfd(int epollfd, int fd, int ev);


// argv[1] 为监听端口
int main(int argc, char * argv[]){

  if(argc <= 1){
    printf("Run as follows : %s port_number \n", basename(argv[0]));
    exit(-1);
  }

  // get port
  int port = atoi(argv[1]);

  // 对SIGPIPE信号处理
  addsig(SIGPIPE, SIG_IGN);

  // 创建线程池并初始化
  threadpool<http_conn> *pool = NULL;
  try{
    pool = new threadpool<http_conn>;
  }catch(...){
    printf("err in threadpool\n");
    exit(-1);
  }

  // 保存所有客户端信息
  http_conn *users = new http_conn[MAX_FD];
  // 监听的套接字
  int listenfd = socket(PF_INET, SOCK_STREAM, 0); // tcp
  if(listenfd == -1){
    perror("listen");
    exit(-1);
  }

  // 设置端口复用
  int reuse = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  // bind
  struct sockaddr_in addrin;
  addrin.sin_family = AF_INET;
  addrin.sin_addr.s_addr = INADDR_ANY;
  addrin.sin_port = htons(port);
  bind(listenfd, (sockaddr*)&addrin, sizeof(addrin));

  // listen

  listen(listenfd, 8);

  // epoll对象, 事件数组
  struct epoll_event events[MAX_EVENT_NUM];
  int epollfd = epoll_create(5);

  // 将监听的文件描述符添加到epoll对象中
  addfd(epollfd, listenfd, false);
  
  http_conn::m_epollfd = epollfd;

  while(true){
    printf("waiting epoll_wait\n");
    int num = epoll_wait(epollfd, events, MAX_EVENT_NUM, -1); // 获取num个事件
    if(num < 0 && errno != EINTR){
      printf("epoll failed\n");
      exit(-1);
    }

    // 循环遍历事件数组
    for(int i = 0;i<num;i++){
      int sockfd = events[i].data.fd;
      if(sockfd == listenfd){
        // client connected 
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int connfd = accept(sockfd,(sockaddr *)&client_addr, &client_len);

        if(http_conn::m_user_count >= MAX_FD){
          printf("支持连接已满\n");
          // TODO  给客户端信息   服务器正忙
          close(connfd);
          continue;
        }

        //  将客户放到 users数组中
        users[connfd].init(connfd, client_addr);
      }else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
        // 对方异常 或错误
        users[sockfd].close_conn();
      } else if(events[i].events &(EPOLLIN)){
        if(users[sockfd].read()){
          // 一次性把所有数据读完
          pool->append(users + sockfd);
        }else{
          users[sockfd].close_conn();
        }
      }else if(events[i].events &(EPOLLOUT)){
        if(!users[sockfd].write()){  // 一次性写完
          users[sockfd].close_conn();
        } 
      }
    }

  }


  close(epollfd);
  close(listenfd);
  delete []users;
  delete pool;

  return 0;
}