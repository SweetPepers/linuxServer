/*
  守护进程, 每隔两秒获取系统时间并写入文件中


*/
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




void work(int a){
  // 获取系统时间, 写入文件
  time_t tm = time(NULL);
  struct tm *local = localtime(&tm);
  // char buf[20];
  // sprintf(buf, "%d-%d-%d %d:%d:%d\n", local->tm_year, local->tm_mon, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
  char *str = asctime(local);

  int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);

  write(fd, str, strlen(str));
  close(fd);
  // printf("%s", buf);
}

int main(){
  pid_t pid = fork();
  if(pid > 0){
    exit(0);
  }
  // 2. 子进程创建会话
  setsid();

  // 3. 设置掩码
  umask(022);
  // 4. 更改工作目录
  chdir(".");
  // 5. 关闭,重定向fd
  int fd = open("dev/null", O_RDWR);

  // int dup2(int oldfd, int newfd);
  dup2(STDIN_FILENO, fd);
  dup2(STDOUT_FILENO, fd);
  dup2(STDERR_FILENO, fd);


  // 6. gongneng 

  // 捕捉定时信号
  struct sigaction act;
  act.sa_flags = 0;
  act.sa_handler = work;
  sigemptyset(&act.sa_mask); 

  sigaction(SIGALRM, &act, NULL);


  // 设置定时器
  struct itimerval val;
  val.it_value.tv_sec = 2;
  val.it_value.tv_usec = 0;

  val.it_interval.tv_sec = 2;
  val.it_interval.tv_usec = 0;

  setitimer(ITIMER_REAL, &val, NULL);
  while(1);
  return 0;
}