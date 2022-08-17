#ifndef HTTP_CONN_H
#define HTTP_CONN_H

#include <stdlib.h>
#include <sys/epoll.h> 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <stdarg.h>
#include <errno.h>
#include "locker.h"
#include <sys/uio.h>
#include <string>

class http_conn{
public:
  static int m_epollfd; // 所有的socket上的事件都被注册到同一个epoll中
  static int m_user_count; // 统计用户数量


  http_conn(){}
  ~http_conn(){}
  void process(); // 处理客户端请求
  void init(int socketfd, const sockaddr_in & addr);
  void close_conn(); // 关闭连接
  bool read();   // 非阻塞读
  bool write();  // 非阻塞写
private:
  int m_sockfd; // 此http连接的socket
  sockaddr_in m_address; 

};



#endif