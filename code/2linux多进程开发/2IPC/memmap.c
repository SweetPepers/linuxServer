// mmap-parent-child-ipc

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>


int anonymous_map(){
  int len = 4096;
  void *ptr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
  if(ptr == MAP_FAILED){
    perror("anon_map");
    exit(0);
  }

  pid_t pid = fork();

  if (pid > 0){
    // parent 
    strcpy((char*)ptr, "helloworld");
    wait(NULL);
  }else{
    // child 
    sleep(1);
    printf("%s\n", (char*)ptr);
  }

  int ret = munmap(ptr, len);
  // 略
  return 0;
}

int main(){
  // 1. open a file
  int fd = open("test.txt", O_RDWR);
  int size = lseek(fd, 0, SEEK_END);
  // 2. create a memory-mapped area
  void * ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);


  if (ptr == MAP_FAILED){
    perror("mmap");
    exit(0);
  }

  // 3. create child process
  pid_t pid = fork();
  if(pid > 0){
    // parent process
    wait(NULL);
    char buf[1024];
    strcpy(buf, (char*)ptr);
    printf("parent read data : %s \n", buf);
  }else {
    // child process
    strcpy((char*)ptr, "nihao shabi ");
  }

  // 4. close memory-mapped area
  munmap(ptr, size);
}