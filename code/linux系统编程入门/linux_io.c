/*

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int open(const char* pathname, int flags);

int open(const char* pathname, int flags, mode_t mode);
    - mode 八进制的数, 表示用户对创建出的新文件的操作权限 比如 0777, 后面三位8进制表示 root  group user的权限
    mode & ~umask  (root权限umask 为 0022 再取反)  主要抹去 write权限
*/


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int opendemo(){
  int fd = open("a.txt", O_RDONLY);
  if (fd == -1){
    perror("open");
  }

  close(fd);

  return 0;
}


int create(){
  int fd = open("create.txt", O_RDWR|O_CREAT, 0777);
  if (fd == -1){
    perror("open");
  }

  close(fd);

  return 0;
}

/*
  ssize_t read(int fd, void *buf, size_t count);
    - return:  On success, the number of bytes read is returned 
              (zero indicates end of file), 
              and the file position is advanced by this number.
              -1 if failed, meanwhile set errno
  ssize_t write(int fd, const void *buf, size_t count);

*/
int rdwr_demo(){
  int srcfd = open("english.txt", O_RDONLY);

  int dstfd = open("chinese.txt", O_RDWR|O_CREAT, 0777);
  if (srcfd == -1 || dstfd == -1){
    perror("rdwr_demo");
  }
  char buf[1024];

  int read_size = read(srcfd, buf, sizeof(buf));
  while (read_size > 0){
    int write_size = write(dstfd, buf, sizeof(buf));
    if (write_size == -1){
      perror("write");
    }
    read_size = read(srcfd, buf, sizeof(buf));
  }


  close(srcfd);
  close(dstfd);
}


/*
标准C库
# include <stdio.h>
int fseek(FILE *stream , long offset, int whence);


linux系统函数
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
  - parameter : whence 
    SEEK_SET
          The file offset is set to offset bytes.
          绝对offset 即 0 + offset
    SEEK_CUR
          The file offset is set to its current location plus offset bytes.
          相对offset : curr + offset
    SEEK_END
          The file offset is set to the size of the file plus offset bytes.
          size + offset
  - return : 设置之后的文件指针位置
作用: 
  1. 移动文件指针到头文件 lseek(fd, 0, SEEK_SET)
  2. 获取当前文件指针的位置  a = lseek(fd, 0, SEEK_CUR)
  3. 获取文件长度 len = lseek(fd, 0, SEEK_END)
  4. 拓展文件长度, 比如增加100字节, (当前是10) lseek(fd,100, SEEK_END)


*/
int lseekdemo(){
  // 演示 4  拓展文件长度
  int fd = open("hello.txt", O_RDWR);
  // 略

  int ret = lseek(fd,100, SEEK_END);
  write(fd, " ", 1);
  close(fd);
  return ret;

}

/*

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stat(const char *pathname, struct stat *statbuf);
int lstat(const char *pathname, struct stat *statbuf);
获取文件信息
失败返回-1,并设置errno
成功返回0


*/

#include <sys/stat.h>

int statdemo(){
  struct stat statbuf;
  int ret = stat("hello.txt", &statbuf);
  if (ret == -1){
    perror("stat");
    return -1;
  }
  printf("size : %ld\n", statbuf.st_size);
  return 0 ;
}

int main(){
  // opendemo();
  // create();
  // rdwr_demo();
  // lseekdemo();
  statdemo();
}