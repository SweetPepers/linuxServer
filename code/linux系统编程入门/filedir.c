/*
#include <sys/stat.h>
#include <sys/types.h>
int mkdir(const char *pathname, mode_t mode);

*/

/*
#include <unistd.h>
int chdir(const char *path);
  修改进程工作目录
char* getcwd(char *buf, size_t size);
  返回值就是 buf的地址
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
  char buf[128];
  getcwd(buf, sizeof(buf));

  printf("current working directory is : %s\n",buf);


  int ret = chdir("..");
  
  if(ret == -1){
    perror("chdir");
    return -1;
  }

  // 创建一个文件
  int fd = open("linux第二章", O_CREAT|O_RDWR, 0777);
  if (fd == -1){
    perror("open");
    return -1;
  }

  close(fd);

  // 在获取一次  
  getcwd(buf, sizeof(buf));

  printf("after chdir current working directory is : %s\n",buf);

  return 0;
}

