#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>






// 使用内存映射完成文件拷贝

int main(){
  // 1. 对原始文件进行内存映射
  int fd = open("test.txt", O_RDWR);
  if(fd == -1){
    perror("open");
    exit(0);
  }

  // 2. 创建一个映射文件
  int fd1 = open("cpy.txt", O_CREAT|O_RDWR);
  if(fd1 == -1){
    perror("open");
    exit(0);
  }
  // 对创建的文件进行扩展
  // get size of text.txt
  int size = lseek(fd, 0, SEEK_END);

  truncate("cpy.txt",size);
  write(fd1, " ", 1);


  // 3. 分别做内存映射
  void * ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  void * ptr1 = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd1, 0);


  if(ptr == MAP_FAILED || ptr1 == MAP_FAILED){
    perror("mmap");
    exit(0);
  }

  // memcpy
  memcpy(ptr1, ptr, size);

  // release file
  munmap(ptr, size);
  munmap(ptr1, size);


  close(fd);
  close(fd1);

  return 0;
}