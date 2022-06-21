#include <unistd.h>  
#include <fcntl.h>
#include <string.h>
#include <stdio.h>


int main(){
  int fd = open("hello.txt", O_RDWR);
  if (fd == -1){
    perror("open");
    return -1;
  }
  int dupdf = fcntl(fd,F_DUPFD);
  fd = dupdf;
  // 修改文件flag, 给flag加入 O_APPEND
  // fcntl(fd, F_SETFL, O_APPEND);  这样会直接替换
  // 需要先获取原来的fl
  int oldFL = fcntl(fd, F_GETFL);
    if (oldFL == -1){
    perror("fcntl get");
    return -1;
  }
  fcntl(fd, F_SETFL, oldFL | O_APPEND);
  

  char *str = "nihao";
  write(fd, str, strlen(str));

  close(fd);
  return 0;
}