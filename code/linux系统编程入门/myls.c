
// 实现ls -l指令
// -rwxrwxrwx 1 root root  4096 Jun 20 19:51 chinese.txt


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>



int main(int argc, char* argv[]){
  if (argc < 2){
    printf("%s filename \n", argv[0]);
    // exit(0);
    return -1;
  }

  struct stat st;
  int ret = stat(argv[1], &st);
  if (ret == -1){
    perror(" ");
    return -1;
  }

  char perms[11] = {0};
  // 获取文件类型和文件权限
  switch (st.st_mode & S_IFMT)
  {
  case S_IFDIR:
    perms[0] = 'd';
    break;
  case S_IFBLK:
    perms[0] = 'b';
    break;
  case S_IFCHR:
    perms[0] = 'c';
    break;
  case S_IFIFO:
    perms[0] = 'i';
    break;
  case S_IFREG:
    perms[0] = 'r';
    break;
  default:
    perms[0] = '?';
    break;
  }
  // 文件访问权限
  perms[1] =  (st.st_mode & S_IRUSR)?'r':'-';
  perms[2] =  (st.st_mode & S_IWUSR)?'w':'-';
  perms[3] =  (st.st_mode & S_IXUSR)?'x':'-';
  perms[4] =  (st.st_mode & S_IXGRP)?'r':'-';
  perms[5] =  (st.st_mode & S_IWGRP)?'w':'-';
  perms[6] =  (st.st_mode & S_IXGRP)?'x':'-';
  perms[7] =  (st.st_mode & S_IXOTH)?'r':'-';
  perms[8] =  (st.st_mode & S_IWOTH)?'w':'-';
  perms[9] =  (st.st_mode & S_IXOTH)?'x':'-';

  // 硬连接数
  int linkNum = st.st_nlink;

  // 文件所有者
  char * fileUser = getpwuid(st.st_uid)->pw_name;
  // 文件所在组
  char* fileGrp = getgrgid(st.st_gid)->gr_name;
  // 文件大小
  long int fileSize = st.st_size;
  // 修改时间 
  char * time = ctime(&st.st_ctime);
  char mtime[512] = {0};
  strncpy(mtime, time, strlen(time)-1);
  char buf[1024];

  sprintf(buf, "%s %d %s %s %ld %s %s", perms, linkNum, fileUser, fileGrp, fileSize, mtime, argv[1]);

  printf("%s\n", buf);

  return 0;
}