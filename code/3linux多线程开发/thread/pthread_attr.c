#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*
int pthread_attr_destroy(pthread_attr_t *attr)
int pthread_attr_init(pthread_attr_t *attr)
int pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate)
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)

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

int main(){
  pthread_t tid;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


  // 获取线程栈的大小
  size_t size;
  pthread_attr_getstacksize(&attr, &size);

  int num = 10;
  int ret = pthread_create(&tid, &attr, myPrint, (void *)&num);
  if(ret != 0){
    char *errstr = strerror(ret);
    printf("error : %s\n", errstr);
  }


  printf("tid : %ld ,main pthread id : %ld\n", tid, pthread_self());
  

  printf("thread stack size  : %ld\n", size);


  // release resourse
  pthread_attr_destroy(&attr);


  // 当主线程退出时 不会影响其他正常运行的线程
  pthread_exit(NULL);

  printf("this line can't executed\n");

  return 0;
}

