/*
#include <sys/types.h>
#include <dirent.h>
DIR *opendir(const char *name);

*/



// #include <dirent.h>
// struct dirent *readdir(DIR *dirp);
// struct dirent {
//     ino_t          d_ino;       /* Inode number */
//     off_t          d_off;       /* Not an offset; see below */
//     unsigned short d_reclen;    /* Length of this record */
//     unsigned char  d_type;      /* Type of file; not supportedby all filesystem types */
//     char           d_name[256]; /* Null-terminated filename */
// };

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 用户获取目录下普通文件的个数
int getFileNum(const char* path){
  DIR *dir = opendir(path);
  if(dir == NULL){
    perror("opendir");
    exit(0);
  }

  struct dirent *ptr; 
  int total = 0;
  while((ptr = readdir(dir)) != NULL){
    char* dname = ptr->d_name;
    if(strcmp(dname, "..") == 0 || strcmp(dname, ".") == 0){
      continue;
    }else {
      if(ptr->d_type == DT_DIR) // 如果是目录
      {
        char newpath[256];
        sprintf(newpath, "%s/%s", path, dname);
        total += getFileNum(newpath);
      }
      if(ptr->d_type == DT_REG){
        // 普通文件
        total++;

      }
    }

  }
  closedir(dir);

  return total;
}


int main(int argc, char* argv[]){
  // 读取某个目录下普通文件的个数
  if (argc < 2){
    return -1;
  }
  printf("filenum : %d\n", getFileNum(argv[1]));
}


