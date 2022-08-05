#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <arpa/inet.h>

int main(){
  // create a socket
  int lfd = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in saddr;
  saddr.sin_port = htons(9999);
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = INADDR_ANY;


  // bind
  bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

  // listen
  listen(lfd, 8);
  int maxfd = lfd
  // make a fd_set

  fd_set rdset;
  FD_ZERO(&rdset);
  FD_SET(lfd, &rdset);


  while(1){
    // call select
    int ret = select(maxfd +1, &rdset, NULL, NULL, NULL);
    if (ret == -1){
      perror("select ");
      exit(0);
    }else if(ret == 0){
      
    }

  }


  return 0;
}
