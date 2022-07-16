#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void exitdemo(){
  printf("hello\n");
  printf("world");  
  exit(0);
}

void _exitdemo(){
  printf("hello\n");
  printf("world");  
  _exit(0);
}



int main(){
  if (fork() == 0){
    printf("exitdemo\n");
    exitdemo();
  }
  sleep(1);
  if (fork() == 0){
    printf("\n\n__exitdemo\n");
    _exitdemo();
  }
  sleep(1);
  return 0;


}