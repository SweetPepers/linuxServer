#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

/*
int sem_init(sem_t *sem, int pshared, unsigned int value);
  - 初始化信号量
  - parameter
    - sem:
    - pshared: 0用在线程间, nonzero用在进程间

int sem_destory(sem_t *sem);
  - 释放资源

int sem_wait(sem_t *sem)
  - 信号量-1, 值为0 阻塞
int sem_post(sem_t *sem)
 - 信号量+1
int sem_getvalue(sem_t *sem, int *sval);
*/
#define LTLEN (8)

struct node{
  int num;
  struct node* next;
};

pthread_mutex_t m;
sem_t ps, cs;

struct node *curr = NULL;


void *customer(void *arg);
void * producer(void *arg);
void* printlink(struct node *head);

int main(){

  pthread_mutex_init(&m, NULL);
  sem_init(&ps, 0, LTLEN);  
  sem_init(&cs, 0, 0);  
  
  // create 5 pro and 5 cust
  pthread_t ptids[5], ctids[5];

  
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
    sem_wait(&ps);
    struct node * newnode = malloc(sizeof(struct node));
    // malloc 保证 申请的内存有指针指着, 加在上面, 导致 好像不会满啊
    pthread_mutex_lock(&m);

    newnode->next = curr;
    curr = newnode;

    curr->num = rand()%1000;
    printf("add node, num : %d, tid : %ld\n", curr->num, pthread_self());
    printlink(curr);
    pthread_mutex_unlock(&m);
    sem_post(&cs);
    usleep(10);
  }
  return NULL;
}

void *customer(void *arg){
  // printf("customer in \n");
  // sleep(1000);
  while(1){
    sem_wait(&cs);
    pthread_mutex_lock(&m);
    assert(curr != NULL);
    struct node * merchandise = curr;
    curr = curr->next;

    pthread_mutex_unlock(&m);
    printf("del node num : %d, tid : %ld\n", merchandise->num, pthread_self());
    free(merchandise);
    sem_post(&ps);
    usleep(100);
  }
  return NULL;
}
