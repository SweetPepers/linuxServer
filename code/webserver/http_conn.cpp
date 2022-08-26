#include "header/http_conn.h"


int http_conn::m_epollfd = -1;
int http_conn::m_user_count = 0;

const char* doc_root = "./resourse";

const char* ok_200_title = "OK";
const char* error_400_title = "bad request";
const char* error_404_form = "The request file was not found on this machine\n";
const char *error_500_title = "Internet Error";
const char *error_500_form = "there was an unusual problem serving the "

#define FILENAME_LEN (strlen(doc_root));


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
  event.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
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

  init();
}

void http_conn::init(){
  m_check_state = CHECK_STATE_REQUESTLINE; 
  m_checked_idx = 0; 
  m_start_line = 0;
  m_read_idx = 0;
  m_url = 0;
  m_version = 0;
  m_method = GET;
  memset(m_check_state, 0, sizeof(m_check_state));
  m_linger = false;
}

// 关闭连接
void http_conn::close_conn(){
  if(m_sockfd != -1){
    rmfd(m_epollfd, m_sockfd);
    m_sockfd = -1;
    m_user_count--;
  }
}




bool http_conn::read(){
  if(m_read_idx > READ_BUFFER_SIZE){
    return false;
  }
  // 读取到的字节
  int bytes_read = 0;
  while(true){
    bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
    if(bytes_read == -1){
      if(errno == EAGAIN || errno == EWOULDBLOCK){
        // 没有数据
        break;
      }
      return false;
    }else if(bytes_read == 0){
      return false;
    }
    m_read_idx += bytes_read;
  }
  printf("read : %s\n", m_read_buf);
  return true;
}

bool http_conn::write(){
  printf("write data in one go\n");
  return true;
}

// 处理http请求的入口函数
void http_conn::process(){
  // 解析http请求
  HTTP_CODE read_ret =  process_read();
  if (read_ret == NO_REQUEST){
    // 重新检测一下
    modfd(m_epollfd, m_sockfd, EPOLLIN);
    return;
  }
  bool write_ret = process_write(read_ret);
  if(!write_ret){
    close_conn();
  }
  modfd(m_epollfd, m_sockfd, EPOLLOUT);
}




HTTP_CODE http_conn::process_read(){
  LINE_STATUS line_status = LINE_OK;
  HTTP_CODE ret = NO_REQUEST;

  char *text = 0;
  while((line_status = parse_line()) == LINE_OK 
    || m_check_state == CHECK_STATE_CONTENT && line_status == LINE_OK){
    // 解析到一行完整的数据, 或者解析到请求体也是OK
    text = getline();
    m_start_line = m_checked_idx;
    printf("got 1 http line : %s\n", text);

    switch (m_check_state)
    {
    case CHECK_STATE_REQUESTLINE:
      ret = parse_request_line(text);
      if (ret == BAD_REQUEST){
        return BAD_REQUEST; 
      }
      break;
    case CHECK_STATE_HEADER:
      ret = parse_headers(text);
      if (ret == BAD_REQUEST){
        return BAD_REQUEST; 
      }else if( ret == GET_REQUEST){
        return do_request();
      }
      break;
    case CHECK_STATE_CONTENT:
      ret = parse_content(text);
      if( ret == GET_REQUEST){
        return do_request();
      }
      line_status = LINE_OPEN;
      break;
    default:
      return INTERNAL_ERROR;
      break;
    }
    return NO_REQUEST;
  }

}


bool http_conn::process_write(HTTP_CODE ret){
  switch (ret)
  {
  case INTERNAL_ERROR:
    add_status_line(500, error_500_title);
    add_header_line(strlen(err_500_form));
    if(!add_content(error_500_form)){
      return false;
    }
    break;
  case BAD_REQUEST:
    add_status_line(400, error_400_title);
    add_header_line(strlen(err_400_form));
    if(!add_content(error_400_form)){
      return false;
    }
    break;
  case NO_RESOURSE:
    add_status_line(404, error_404_title);
    add_header_line(strlen(err_404_form));
    if(!add_content(error_404_form)){
      return false;
    }
    break;
  case FORBIDDEN_REQUEST:
    add_status_line(403, error_403_title);
    add_header_line(strlen(err_403_form));
    if(!add_content(error_403_form)){
      return false;
    }
    break;
  case FILE_REQUEST:
    add_status_line(200, ok_200_title);
    add_header_line(m_file_stat.st_size);
    m_iv[0].iov_base = m_write_buf;
    m_iv[1].iov_len = m_write_idx;
    m_iv[1].iov_base = m_file_address;
    m_iv[1].iov_len = m_file_stat.st_size;
    m_iv_count = 2;
    return true;
    
    break;
  default:
    return false;
    break;
  }

}
// 解析http请求 首行
HTTP_CODE http_conn::parse_request_line(char *text){  

  // GET / HTTP/1.1
   m_url = strpbrk(text, " \t");
  *m_url++ = '\0'; 

  // GET\0 / HTTP/1.1
  char *method = text;
  if (strcasecmp(method), "GET"){
    m_method = GET;  
  }else return BAD_REQUEST;   // only support GET
  
  // / HTTP/1.1
  m_version = strpbrk(m_url, " \t");
  if (!m_version){
    return BAD_REQUEST;
  }
  // /\0HTTP/1.1
  *m_version++ = '\0';
  if (strcasecmp(m_version, "HTTP/1.1")){
    return BAD_REQUEST;
  }

  // http://192.186.19.1:1000/index.html
  if(strncasecmp(m_url, "http://", 7) == 0){
    m_url+= 7;
    m_url = strchr(m_url,'/')  // 找 '/'第一次出现的地方
  }

  if(!m_url || m_url[0] != '/'{
    return BAD_REQUEST;
  }
  m_check_state = CHECK_STATE_HEADER;  // 更新主状态机
  return NO_REQUEST;
}

// 解析http请求 头
HTTP_CODE http_conn::parse_headers(char *text){
  if(text[0] == '\0'){
    if(m_content_length != 0){
      m_check_state = CHECK_STATE_CONTENT;
      return NO_REQUEST;
    }
    return GET_REQUEST;
  }else if(strncasecmp(text, "Connection:", 11) == 0){
    text += 11;
    text += strspn(text, '\t');
    if(strcasecmp(text, "keep-alive") == 0){
      m_linger = true;
    }
  }else if(strcasecmp(text, "Content-Length:", 15) == 0){
    text+=15;
    text+= strspn(text, " \t");
    m_content_length = atol(text);
  }else if(strcasecmp(text, "Host:", 5) == 0){
    text+=5;
    text+= strspn(text, " \t");
    m_host = text;
  }else{
    printf("oop! unkown headers %s\n", text);
  }
  return NO_REQUEST;
}

// 解析http请求体 
// 没有真正解析, 只是判断是否完整读入
HTTP_CODE http_conn::parse_content(char *text){
  if(m_read_idx >= (m_content_length + m_checked_idx)){
    text[m_content_length] = '\0';
    return GET_REQUEST;
  }
  return NO_REQUEST;
}


// \r  \n
LINE_STATUS http_conn::parse_line(){
  for (; m_checked_idx < m_read_idx; m_checked_idx++){
    tmp = m_read_buf[m_checked_idx];
    if(tmp == '\r'){
      if(m_checked_idx+1 == m_read_idx){ // 不完整
        return LINE_OPEN;
      }else if(m_read_buf[m_checked_idx+1] == '\n'){
        m_read_buf[m_check_idx++] = '\0';
        m_read_buf[m_check_idx++] = '\0';
        return LINE_OK;
      }
      return LINE_BAD;
    }else if(tmp == '\n'){
      if(m_checked_idx > 1 && m_read_buf[m_checked_idx-1] == '\r'){
        m_read_buf[m_checked_idx-1] = '\0';
        m_read_buf[m_checked_idx++] = '\0';
        return LINE_OK;
      }
      return LINE_BAD;
    }
    return LINE_OPEN;
  }

  return LINE_OK;
}



// 分析目标文件属性
// 存在且可读且不是目录, 则使用mmap将其映射到内存地址m_file_address处, 并告诉调用者调用成功
HTTP_CODE http_conn::do_request(){
  // 资源在 "./resourse"
  strcpy(m_real_file, doc_root);
  int len = strlen(doc_root);
  strncpy(m_real_file+len, m_url, FILENAME_LEN-len-1);
  if(stat(m_real_file, &m_file_stat) < 0){
    return NO_RESOURSE;
  }
  // 判断访问权限
  if (!(m_file_stat.st_mode & S_IROTH)){
    return FORBIDDEN_REQUEST;
  }

  // 是否目录
  if(S_ISDIR(m_file_stat.st_mode)){
    return BAD_REQUEST;
  }
  // open the file
  int fd = open(m_real_file,O_RDONLY);
  // 创建内存映射
  m_file_address = (char*)mmap(0, m_file_stat.st_size, PROT_READ,MAP_SHARED, fd, 0);
  close(fd);
  
  return FILE_REQUEST;
}



// TODO
bool http_conn::add_response(const char* format, ...){
  return ture;
}
bool http_conn::add_status_line(int status, const char* title){
  return add_status_line("%s %d %s\r\n")
}
bool http_conn::add_header_line(int len){
return ture;
}
bool http_conn::add_content(char *form){
return ture;
}
bool http_conn::add_headers(int content_len){
return ture;
}
bool http_conn::add_content_length(int content_len){
return ture;
}
bool http_conn::add_linger(){
return ture;
}
bool http_conn::add_blank_line(){
return ture;
}
bool http_conn::add_content(const char* content){
return ture;
}


