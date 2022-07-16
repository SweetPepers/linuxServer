/*

#include <sys/type.h>
#include <signal.h>
int kill(pid_t pid, int sig);
    给pid进程发送 sig信号
    - pid :
      - >0 normal situation
      - =0 发送给当前的进程组
      - =-1, 发送给每一个有权限接收信号的进程
      - <-1, 发送给 -(pid)进程组
int raise(int sig);
    给当前进程发送信号sig
void abort(void);
    发送 SIGABRT信号给当前进程, 杀死进程
*/


#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int killdemo(){
  pid_t pid = fork();

  if(pid == 0){
    //child
    for(int i = 0;i<5;i++){
      printf("child print %d\n", i);
      sleep(1);
    }
  }
  else{
    printf("parent process\n");
    sleep(2);
    printf("parent kill child\n");
    kill(pid, SIGINT);
  }
}

/*
  unsigned int alarm(unsigned int seconds);
    当时间到时,会给当前进程发送一个 SIGALARM信号
    不阻塞
  - SIGALARM: 默认终止当前进程,每个进程有唯一的计时器
  return value : 上个alarm剩余的时间
*/
int alarmdemo(){
  int seconds = alarm(5);
  printf("seconds = %d\n", seconds);  //0
  sleep(1);
  seconds = alarm(10);
  printf("seconds = %d\n", seconds);  //3
  // why 3?
  alarm(1);
  int i = 0;
  while(1){
    i++;
    printf("%d\n", i);
  };
}

/*
#include <sys/time.h>

int getitimer(int which, struct itimerval *curr_value);

int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value);
  设置定时器(提待alarm), 精度 us   可以周期产生信号

  - which: 以什么时间计时
     ITIMER_REAL:真实时间, 发送SIGALRM
     ITIMER_VIRTUAL:用户(模式下的)时间, 发送 SIGVTALRM
     ITIMER_PROF: 用户时间+内核时间, 发送 SIGPROF
  - new_value :
          struct itimerval {
               struct timeval it_interval;   //间隔时间
               struct timeval it_value;      // 延迟多长时间执行定时器
           };

           过 it_value 后每隔 it_interval 发送一次信号

           struct timeval {
               time_t      tv_sec;   // 秒
               suseconds_t tv_usec;  // 微秒     
           };
    
  - old_value 一般不使用

*/

#include <sys/time.h>

void alrm_handler(int a);


int setitimerdemo(){
  struct itimerval new_val;
  new_val.it_interval.tv_sec = 2;
  new_val.it_interval.tv_usec = 0;

  new_val.it_value.tv_sec = 2;
  new_val.it_value.tv_usec = 0;
  



  int ret = setitimer(ITIMER_REAL, &new_val, NULL); 
  if(ret == -1){
    perror("setitimer");
    return -1;
  }
  printf("setitimer start\n");
  signal(SIGALRM, &alrm_handler);

  getchar();
}


/*
信号捕捉函数

#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
  SIGKILL SIGSTOP不能被捕捉, 不能被忽略
  - handler
    - SIG_IGN 忽略
    - SIG_DFT 默认

  - ret val : 成功返回上一次设置的handler指针(第一次返回NULL)
  失败返回 SIG_ERR
int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
  - change the action taken by a process on receipt of a specific signal
  - parameters
    - signum : 
    - act : 捕捉到信号的处理act
    - oldact : 上次的, 一般 NULL
    0 成功, -1 失败
*/

/*
c {
          void     (*sa_handler)(int); // 函数指针, 处理函数
          void     (*sa_sigaction)(int, siginfo_t *, void *); //不常用
          sigset_t   sa_mask; //临时阻塞信号集(只在此信号捕捉函数执行期间阻塞)
          int        sa_flags; // 一般为0或SA_SIGINFO, 0使用 sa_handler, SA_SIGINFO表示使用 sa_sigaction
          void     (*sa_restorer)(void); // 不用了
      };
*/
void alrm_handler(int a){
  // para a : 捕捉到信号的值 
  // 14 : SIGALRM
  printf("alrm_handler signal %d\n", a);
  return;
}

int sigactiondemo(){
  struct sigaction act;
  act.sa_flags = 0;
  act.sa_handler = &alrm_handler;
  sigemptyset(&act.sa_mask);

  sigaction(SIGALRM, &act, NULL);


  struct itimerval new_val;
  // 间隔时间
  new_val.it_interval.tv_sec = 2;
  new_val.it_interval.tv_usec = 0;
  // 延迟时间
  new_val.it_value.tv_sec = 3;
  new_val.it_value.tv_usec = 0;
  

  int ret = setitimer(ITIMER_REAL, &new_val, NULL); 
  if(ret == -1){
    perror("setitimer");
    return -1;
  }
  printf("setitimer start\n");
  // signal(SIGALRM, &alrm_handler);

  // getchar();
  while(1);
}



int main(){
  // killdemo();
  // alarmdemo();
  // setitimerdemo();
  sigactiondemo();
}