
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void myfunc(int a){
  printf("signal captured: %d\n", a);
  // 回收子进程资源
  // wait(NULL);   阻塞
  // 这里回收后仍然无法回收全部子进程 why??
  // because : 很多个SIGCHLD产生, 但信号集只会保留一个, 多余的丢弃

  while(1){
    int ret = waitpid(-1, NULL, WNOHANG);
    if(ret > 0){
      printf("%d child die\n", ret);
    }else if(ret == 0){
      // some child processes are still alive
      break;
    }else{
      // ret == -1
      break;
    }
  }

  // segmentfault??
  // 并发的问题, 有可能信号还未注册成功, SIGCHLD信号就已经产生了
}

int main(){
  // 提前设置好阻塞信号集, 解决segmentfault问题
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGCHLD);
  sigprocmask(SIG_BLOCK, &set, NULL);

  // create some(20) child process
  pid_t pid;
  for(int i = 0; i<20 ;i++){
    pid = fork();
    if(pid == 0){
      break;
    }
  }

  
  if(pid > 0){
    //parent
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myfunc;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, NULL);

    sigprocmask(SIG_UNBLOCK, &set, NULL);

    while(1){
      // capture SIGCHILD while child dead
      
      printf("parent process pid = %d\n", getpid());
      sleep(2);
    };
  }else{
    //child 
    printf("child pid = %d\n",getpid());
  }


  return 0;
}