/*

// 设置 阻塞集
int sigemptyset(sigset_t *set);
  - 所有标志位置0
  - retval: 成功0, 失败-1
int sigfillset(sigset_t *set);
  - 所有标志位置1
  - retval: 成功0, 失败-1
int sigaddset(sigset_t *set, int signum);
  - 设置指定为1
int sigdelset(sigset_t *set, int signum);
  - 设置指定为1
int sigismember(const sigset_t *set, int signum);
  - 判断signum在set中是否阻塞
  - 1 阻塞
  - 0 不阻塞
*/


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int easydemo(){
  // 
  sigset_t set;
  sigemptyset(&set);
  
  int ret = sigismember(&set, SIGINT);

  if(ret == 0){
    printf("sigint 不阻塞\n");
  }else{
    printf("sigint 阻塞\n");
  }

  // add signal
  sigaddset(&set, SIGINT);
  ret = sigismember(&set, SIGINT);
  if(ret == 0){
    printf("sigint 不阻塞\n");
  }else{
    printf("sigint 阻塞\n");
  }
  

  // del

  sigdelset(&set, SIGINT);
  ret = sigismember(&set, SIGINT);
  if(ret == 0){
    printf("sigint 不阻塞\n");
  }else{
    printf("sigint 阻塞\n");
  }
  
  return 0;
}



/*
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
  - 将自定义信号集的数据设置到内核中
  - parameters
    - how : 
      - SIG_BLOCK : 类似添加, 做 mask |= set 操作 原来的 110 and 001 = 111
      - SIG_UNBLOCK : 解除阻塞 mask & ~set
      - SIG_SETMASK : 覆盖
    - set
    - oldset : 保存设置之前的信号集(类似于返回值的作用), 可以是NULL

int sigpending(sigset_t *set);
  - 获取os中的未决信号集
    - set 传出参数
*/


int procmaskdemo(){
  // 设置 SIGINT(2), SIGQUIT(3)信号阻塞
  sigset_t set, pendingset;    (long int)
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGQUIT);

  sigprocmask(SIG_BLOCK, &set, NULL);
  int k = 0;
  while(1){
    k++;
    // 获取当前未决信号集
    sigemptyset(&pendingset);
    sigpending(&pendingset);
    for(int i = 1; i<=32;i++){
      if(sigismember(&pendingset, i) == 1){
        printf("1");
      }else{
        printf("0");
      }
    }
    printf("\n");
    sleep(1);
    // printf("%lu\n", pendingset);
    
    if(k == 10){
      // 解除阻塞
      printf("马上解除阻塞, 之前被阻塞的信号将执行\n");
      sigprocmask(SIG_UNBLOCK, &set, NULL);
    }
  }
}

int main(){
  // easydemo();
  procmaskdemo();
}