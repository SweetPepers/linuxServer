#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>





int main(){
  int ret = mkfifo("fifo1", 0664);
  if(ret == -1){
    perror("fifo");
  }
  return 0;
}