#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*

 #include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
  - 功能 create
  - parameter 
    - thread: 传出参数
    - attr : 设置线程属性, 一般默认 NULL
    - start_routine : 子线程处理的代码
    - arg : 传参
  - ret : 成功0 失败 errno,获取错误号信息 char* strerror(int errum)


int pthread_equal(pthread_t t1, pthread_t t2);
  - 比较是否相等
  不同os对pthread的定义不同, 有的为结构体, 所以不能 ==

*/
int myprint_ret = 10;
void * myPrint(void * arg){
  int a = 7777;
  if (arg == NULL){
    arg = (void*)&a;
  }
  for(int i =0 ;i<5;i++){
    printf("child %d\n", i);
  }
  printf("child thraed helloworld  arg :%d\n", *(int*)arg);
  printf("child pthread id : %ld\n", pthread_self());
  

  sleep(1);

  // return NULL; // pthread_exit(NULL)  
  pthread_exit((void *)&myprint_ret);
}


int create_demo(){
  pthread_t tid;


  int num = 10;
  int ret = pthread_create(&tid, NULL, myPrint, (void *)&num);
  if(ret != 0){
    char *errstr = strerror(ret);
    printf("error : %s\n", errstr);
  }


  printf("tid : %ld ,main pthread id : %ld\n", tid, pthread_self());
  
  // pthread_join回收子线程资源  阻塞
  int *retval;
  pthread_join(tid, (void **)&retval);

  printf("recover child thread succeed, retval : %d\n", *retval);




  // 当主线程退出时 不会影响其他正常运行的线程
  pthread_exit(NULL);

  printf("this line can't executed\n");

  return 0;

}


/*

void pthread_exit(void *retval);
  - retval, 可传递NULL, 可在pthread_join()中使用

pthread_t pthread_self(void);
  - 获取当前的线程id

*/


/*
int pthread_join(pthread_t thread, void **retval);
  - join with a terminated thread
    回收子线程的资源(僵尸线程)
    阻塞, 调用一次只能回收一个子线程
    一般在主线程中使用
  - parameter
    - thread : 回收的id
    - retval : 接受子进程退出时的返回值



*/


/*
int pthread_detach(pthread_t thread);
  - 当一个分离的线程终止时会自动将资源返还给os, 而不需要其他线程的join
  不能多次 detach 线程
  不能连接一个已分离的线程
*/

int detach_demo(){
  pthread_t tid;
  int ret = pthread_create(&tid, NULL,myPrint, NULL);
  if(ret != 0){
    char *errstr = strerror(ret);
    printf("error : %s\n", errstr);
  }

  // print tid
  printf("tid : %ld, main thread id : %ld\n", tid, pthread_self());

  // detach
  pthread_detach(tid);

  

  // ret = pthread_join(tid, NULL);
  // if(ret != 0){
  //   char *errstr = strerror(ret);
  //   //error : Invalid argument
  //   printf("error : %s\n", errstr);
  // }

  pthread_exit(NULL);
}

int cancel_demo(){
  pthread_t tid;
  int ret = pthread_create(&tid, NULL,myPrint, NULL);
  if(ret != 0){
    char *errstr = strerror(ret);
    printf("error : %s\n", errstr);
  }

  // cancel thread
  pthread_cancel(tid);


  for(int i =0 ;i<5;i++){
    printf("%d\n", i);
  }

  // print tid
  printf("tid : %ld, main thread id : %ld\n", tid, pthread_self());


  
  pthread_exit(NULL);
}

int main(){

  // create_demo();
  // detach_demo();
  cancel_demo();
  return 0;
}