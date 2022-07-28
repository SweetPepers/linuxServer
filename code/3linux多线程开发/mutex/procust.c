#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

// a easy version : producer  customer

struct node{
  int num;
  struct node* next;
};

pthread_mutex_t m;


/*
pthread_cond_t 

int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
int pthread_cond_destory(pthread_cond_t * cond);
int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
  - 阻塞
int pthread_cond_timewait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
  - 阻塞指定时间
int pthread_cond_signal(pthread_cond_t *cond);
  - 唤醒一个或者多个线程
int pthread_cond_broaacast(pthread_cond_t *cond);
  - 唤醒所有线程
*/
pthread_cond_t cond;


struct node *curr = NULL;


void *customer(void *arg);
void * producer(void *arg);
void* printlink(struct node *head);

int main(){
  // create 5 pro and 5 cust
  pthread_t ptids[5], ctids[5];

  pthread_mutex_init(&m, NULL);
  pthread_cond_init(&cond, NULL);

  for(int i = 0; i< 5 ; ++i){
    pthread_create(&ptids[i], NULL, producer, NULL);
    pthread_create(&ctids[i], NULL, customer, NULL);
    pthread_detach(ptids[i]);
    pthread_detach(ctids[i]);
  } 

  while(1){
    sleep(10);
  }
  pthread_mutex_destroy(&m);
  pthread_cond_destroy(&cond);
  pthread_exit(NULL);
  return 0;

}


void* printlink(struct node *head){

  printf("head");
  for(;head != NULL;head = head->next){
    printf("->%d", head->num);
  }
  printf("->end\n");
}

void * producer(void *arg){
  while(1){
    struct node * newnode = malloc(sizeof(struct node));
    // malloc 保证 申请的内存有指针指着, 加在上面, 导致 好像不会满啊
    pthread_mutex_lock(&m);

    newnode->next = curr;
    curr = newnode;

    curr->num = rand()%1000;
    printf("add node, num : %d, tid : %ld\n", curr->num, pthread_self());
    // printlink(curr);
    pthread_mutex_unlock(&m);
    // notice customer
    pthread_cond_signal(&cond);
    sleep(5);
  }
  return NULL;
}

void *customer(void *arg){
  // printf("customer in \n");
  // sleep(1000);
  while(1){
    pthread_mutex_lock(&m);
    if (curr == NULL){
      printf("no enough product  waiting...\n"); 
      // wait
      pthread_cond_wait(&cond, &m);
      pthread_mutex_unlock(&m);
    }else{

      struct node * merchandise = curr;
      assert(curr != NULL);
      curr = curr->next;

      pthread_mutex_unlock(&m);
      printf("del node num : %d, tid : %ld\n", merchandise->num, pthread_self());
      free(merchandise);
    }
    usleep(100);
  }
  return NULL;
}
