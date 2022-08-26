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
#include <string.h>



// 有限状态机
// only support get
enum MEHTOD {GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT};
// 解析客户端请求,主机状态
// 请求行 头部字段  请求体
enum CHECK_STATE{CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER, CHECK_STATE_CONTENT}; 
// ok, 不ok, 行数据尚且不完整
enum LINE_STATUS{LINE_OK = 0, LINE_BAD, LINE_OPEN};
// 服务器处理HTTP请求的可能结果, 报文解析的结果
// 见 node.md
enum HTTP_CODE{NO_REQUEST,GET_REQUEST, BAD_REQUEST, NO_RESOURSE, FORBIDDEN_REQUEST, FILE_REQUEST, INTERNAL_ERROR, CLOSE_CONNECTION}



class http_conn{
public:
  static int m_epollfd; // 所有的socket上的事件都被注册到同一个epoll中
  static int m_user_count; // 统计用户数量

  static const int READ_BUFFER_SIZE = 2048; // 读缓冲区大小
  static const int WRITE_BUFFER_SIZE = 1024; // 写缓冲区大小

  http_conn(){}
  ~http_conn(){}
  void process(); // 处理客户端请求
  void init(int socketfd, const sockaddr_in & addr);
  void close_conn(); // 关闭连接
  bool read();   // 非阻塞读
  bool write();  // 非阻塞写
  HTTP_CODE process_read(); // 解析http请求
  bool process_write(HTTP_CODE ret);
  HTTP_CODE parse_request_line(char *text); // 解析http请求 首行
  HTTP_CODE parse_headers(char *text); // 解析http请求 头
  HTTP_CODE parse_content(char *text); // 解析http请求体

  LINE_STATUS parse_line();
  char *getline(){return m_read_buf+m_start_line;}
  HTTP_CODE do_request();
  
  bool add_response(const char* format, ...);
  bool add_status_line(int status, const char* title);
  bool add_header_line(int len);
  bool add_content(char *form);
  bool add_headers(int content_len);
  bool add_content_length(int content_len);
  bool add_linger();
  bool add_blank_line();
  bool add_content(const char* content);


private:
  int m_sockfd; // 此http连接的socket
  sockaddr_in m_address; 
  char m_read_buf[READ_BUFFER_SIZE];
  int m_read_idx; // 标识读缓冲区中以及读入的客户端缓冲区中最后一个字节的下一个字节的位置
  int m_checked_idx; //当前正在分析的字符在缓冲区的位置
  int m_start_line; //  正在解析行的起始位置

  void init();   // 初始化其他的信息

  CHECK_STATE m_check_state;
  char m_write_buf[WRITE_BUFFER_SIZE];

  char* m_url;   // 请求目标文件文件名
  char *m_version; // 协议版本  only support HTTP1.1
  char *m_host; // 主机
  bool m_linger; // HTTP是否要求保持连接
  MEHTOD m_method;  // 请求方法
  long m_content_length;   // 正文长度  

  // read
  char *m_real_file;
  char *m_file_address;
  struct stat m_file_stat;

  // write
  char *m_write_buf;
  int m_write_idx;
};



#endif