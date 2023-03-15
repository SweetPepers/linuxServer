#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main(){
  printf("\nbegin\n");

  if(fork() == 0){
    printf("Im child process, pid = %d, ppid = %d\n", getpid(), getppid());

    int i;
    for(i = 0; i < 10; i++){
      printf("i = %d\n", i);
      sleep(1);
    }
  }
  else{
    printf("Im parent process, pid = %d, ppid = %d\n", getpid(), getppid());

    int j;
    for(j = 0; j < 10; j++){
      printf("j = %d\n", j);
      sleep(1);
    }
  }

  return 0;
}