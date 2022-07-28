#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


pthread_mutex_t mutex;
static int tickets = 100;

void *sellticket(void *arg){
  while(tickets > 0){
    pthread_mutex_lock(&mutex);
    if(tickets <= 0){
      pthread_mutex_unlock(&mutex);
      break;
    }
    printf("%ld is selling the %dth ticket\n", pthread_self(), tickets);
    tickets--;
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}


int main(){
  // create 3 threads
  pthread_t tid1, tid2, tid3;
  

  pthread_mutex_init(&mutex, NULL);
  printf("init lock\n");
  pthread_create(&tid1, NULL, sellticket, NULL);
  pthread_create(&tid2, NULL, sellticket, NULL);
  pthread_create(&tid3, NULL, sellticket, NULL);


  // 回收子线程资源

  // 阻塞
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);

  // pthread_detach(tid1)
  
  pthread_mutex_destroy(&mutex);
  pthread_exit(NULL);





  return 0;
}