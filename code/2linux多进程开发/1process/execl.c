/*
#include <unistd.h>
int execl(const char *path, const char *arg, ...);
    - path : 推荐使用绝对路径, (换个目录执行可能找不到了)
    - arg
      第一个一般是文件名
      参数以null(哨兵)结尾
*/

#include <unistd.h>
#include <stdio.h>


int execldemo(){
  // 创建一个子进程, 在子进程中 执行exec族中的函数
  pid_t pid = fork();
  if(pid > 0){
    sleep(1); // 看一下孤儿进程相关
    printf("\n\nEXEC im parent process, pid : %d\n", getpid());
  }else{
    printf("\n\nEXEC im child process, pid : %d, ppid: %d\n", getpid(), getppid());

    // 第一个参数并不影响结果啊
    // execl("hello", "hello.exe", NULL);
    // 也可以运行系统的可执行文件
    execl("/bin/ps", "ps", "aux", NULL); 


    // 这句话 在上面hello执行成功后就不会被执行了
    printf("EXEC  im child process, pid : %d\n", getpid());
  }

  printf("\n\nEXEC  my pid : %d\n",getpid());
}

/*
#include <unistd.h>
int execl(const char *file, const char *arg, ...);
    - file : 直接文件名(不用加路径)
    - arg
      第一个一般是文件名
      参数以null(哨兵)结尾
会到环境变量中找可执行文件, 找不到就返回-1

*/

int execlpdemo(){
  // execldemo();
  // 创建一个子进程, 在子进程中 执行exec族中的函数
  pid_t pid = fork();
  if(pid > 0){   // parent
    printf("\n\nEXEC im parent process, pid : %d\n", getpid());
    sleep(1);
  }else{ // child
    printf("\n\nEXEC im child process, pid : %d, ppid: %d\n", getpid(), getppid());

    // 第一个参数并不影响结果啊
    execl("ps", "ps", "aux", NULL); 
    perror("execlp");


    // 这句话 在上面hello执行成功后就不会被执行了
    printf("EXEC  im child process, pid : %d\n", getpid());
  }

  printf("\n\nEXEC  my pid : %d\n",getpid());
}


// 还一个 系统函数   基本一样
// int execv(const char* path, char* argv[]);
// 也得弄个没用的第一个函数


int main(){
  execldemo();
  // execlpdemo();
}