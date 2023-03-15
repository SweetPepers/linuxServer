/*
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *wstatus);
  - 成功返回子进程id
  - 失败返回-1(所有子进程结束 or  调用失败)
  调用wait会阻塞


pid_t waitpid(pid_t pid, int *wstatus, int options);
  回收指定进程号的子进程,可以设置是否阻塞

  - pid :
      >0 : 子进程
      =0 : 回收当前进程组的所有子进程
      -1 : 回收所有子进程(相当于wait())
      <-1: 回收某个进程组的组id, 进程组id为 -pid, 
  - ret :
      >0 子进程id
      =0 :在 option=WHNOHANG下, 表示还有子进程
      =-1: 错误或者没子进程


*/



#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>


int waitdemo(){
  pid_t pid;
  for(int i = 0; i<5; i++){
    pid = fork();
    if(pid == 0){
      break;
      // 子进程不产生新的进程
    }
  }

  if(pid > 0){
    // parent process
    while(1){
      printf(" PARENT pid = %d \n", getpid());
      // int ret = wait(NULL);
      int st;
      int ret = wait(&st);
      if(ret == -1) break;
      printf("child die, child id = %d", ret);
      sleep(1); 
    }
  }else{
    // child process
    while(1){
      printf(" CHILD pid = %d \n", getpid());
      sleep(1);
    }

  }
}

int waitpiddemo(){
  pid_t pid;
  for(int i = 0; i<5; i++){
    pid = fork();
    if(pid == 0){
      break;
      // 子进程不产生新的进程
    }
  }

  if(pid > 0){
    // parent process
    while(1){
      sleep(1); 
      printf(" PARENT pid = %d \n", getpid());
      // int ret = wait(NULL);
      int st;
      int ret = waitpid(-1, &st, WNOHANG);
      // int ret = waitpid(-1, &st, 0);
      if(ret == -1) break;
      if(ret == 0){
        continue;
      }
      if(WIFEXITED(st)){
        // 是否正常退出
        printf("exit code : %d\n", WEXITSTATUS(st));
      }
      if(WIFSIGNALED(st)){
        // 是否被信号终止
        printf("killed by signal %d\n", WTERMSIG(st)); 
      }


      printf("child die, child id = %d", ret);
      
    }
  }else{
    // child process
    while(1){
      printf(" CHILD pid = %d \n", getpid());
      sleep(1);
    }

  }
}


int main(){
  waitpiddemo();
  return 0;
}