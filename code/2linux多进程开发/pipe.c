#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>


int pipedemo(){
  int pipefd[2];

  int ret = pipe(pipefd);
  if(ret == -1){
    perror("pipe");
    exit(0);
  }

  pid_t pid = fork();
  if (pid > 0){
    // parent 
    while(1){
      
      // close(pipefd[0]);
      char *str = "I am parent";
      write(pipefd[1], str, strlen(str));

      sleep(1);
      
      char buf[1024] = {0};
      int len = read(pipefd[0], buf, sizeof(buf));
      printf("parent receive :  %s, pid %d\n", buf, getpid());
    }
    
  }else{
    // child
    while(1){
      
      char buf[1024] = {0};
      int len = read(pipefd[0], buf, sizeof(buf));
      printf("child receive :  %s, pid %d\n", buf, getpid());

      
      char *str = "I am child";
      write(pipefd[1], str, strlen(str));
      sleep(1);
    }
  }
}


int main(){

  pipedemo();

  return 0;
}