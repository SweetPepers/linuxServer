#include <unistd.h>
#include <stdio.h>




int accessdemo(){
  int file_exist = access("hello.txt", F_OK);

}

int truncate(const char * path, off_t length);

int main(){

}