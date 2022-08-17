#include "header/http_conn.h"
#include <sys/epoll.h>

// 设置fd非阻塞
int setnonblocking(int fd){
  int old_flag = fcntl(fd, F_GETFL);
  int new_flag = old_flag | O_NONBLOCK;
  return fcntl(fd, F_SETFL, new_flag);
  
}

// 向epoll中添加需要监的fd
void addfd(int epollfd, int fd, bool oneshot){
  epoll_event event;
  event.data.fd = fd;
  // TODO add EPOLLET
  event.events = EPOLLIN | EPOLLRDHUP;
  if (oneshot){
    event.events | EPOLLONESHOT;
  }
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd,&event);
  // 设置文件描述符非阻塞
  setnonblocking(fd);
} 

// modify fd, 重置socket上的EPOLLONESHOT事件, 以确保下次可读时 EPOLLIN事件被触发
void modfd(int epollfd, int fd, int ev){
  epoll_event event;
  event.data.fd = fd;
  event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
  epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

void rmfd(int epollfd, int fd){
  epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
  close(fd);
}

// 初始化连接
void http_conn::init(int socketfd, const sockaddr_in & addr){
  m_sockfd = socketfd;
  m_address = addr;


  // 设置端口复用
  int reuse = 1;
  setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  // 添加到epoll对象中
  addfd(m_epollfd, socketfd, true);
  m_user_count++;
}

// 关闭连接
void http_conn::close_conn(){
  if(m_sockfd != -1){
    rmfd(m_epollfd, m_sockfd);
    m_sockfd = -1;
    m_user_count--;
  }
}


  // setnonblocking(m_sockfd);
  // char buf[1024];
  // int len = recv(sockfd, buf, sizeof(buf), 0);
  // while()

bool http_conn::read(){
  printf("一次性读数据  read data in one go\n");
  return true;
}

bool http_conn::write(){
  printf("write data in one go\n");
  return true;
}

// 处理http请求的入口函数
void http_conn::process(){
  // 解析http请求
  printf("parse request, create response\n");
}

int http_conn::m_epollfd = -1;
int http_conn::m_user_count = 0;
