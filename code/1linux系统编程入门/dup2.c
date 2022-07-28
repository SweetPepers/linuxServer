#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>


int main(){
  int fd = open("./dup2.txt", O_RDWR | O_APPEND, 0664);
  if(fd < 0){
    perror("open");
  }
  char *str = "hellowrold\n";
  // dup2(STDOUT_FILENO, fd);
  dup2(fd, STDOUT_FILENO);
  
  write(fd, str, strlen(str));
  printf("hellowrold\n");
  return 0;
}