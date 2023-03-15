#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>


/*
实现  ps aux | grep xxx

子进程 ps aux
父进程 过滤

pipe()
execlp()
子进程将标准输出 stdout_fileno 重定向到管道的写端   dup2



*/

int main(){
  int fd[2];
  int ret = pipe(fd);
  if (ret == -1){
    perror("pipe");
    return -1;
  }

  pid_t pid = fork();

  if(pid > 0){
    // parent
    close(fd[1]);
    // read from pipe
    char buf[1024] = {0};
    int len  = -1;
    while((len = read(fd[0], buf, sizeof(buf) - 1)) > 0){
      // grep
      printf("%s", buf);
      memset(buf, 0, 1024);
      
    }
    wait(NULL); 

  }else{
    // child
    close(fd[0]);

    // 执行之前要进行文件描述符重定向
    dup2(fd[1], STDOUT_FILENO);
    // 执行 ps
    execlp("ps", "ps", NULL);
    
  }


}