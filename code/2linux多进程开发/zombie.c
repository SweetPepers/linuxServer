#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main(){
  printf("\nbegin\n");

  if(fork() == 0){
    printf("CHILD  pid = %d, ppid = %d\n", getpid(), getppid());
  }
  else{  //parent
    while(1){
      printf("Im parent process, pid = %d, ppid = %d\n", getpid(), getppid());
      sleep(1);
    }
  }

  return 0;
}