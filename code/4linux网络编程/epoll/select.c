#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>

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
  int maxfd = lfd;
  // make a fd_set

  fd_set rdset, tmp;
  FD_ZERO(&rdset);
  FD_SET(lfd, &rdset);


  while(1){
    tmp = rdset;

    // call select 
    int ret = select(maxfd +1, &tmp, NULL, NULL, NULL);
    if (ret == -1){
      perror("select ");
      exit(0);
    }else if(ret == 0){
      continue;
    }else {
      // have fd changed
      if(FD_ISSET(lfd, &tmp)){
        // new client connect
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len); // 每次调用 accept都是在lfd有效的情况下, 所以accept没机会阻塞

        // add cfd into rdset
        FD_SET(cfd, &rdset);
        maxfd = cfd > maxfd?cfd : maxfd;

      }

      for(int i=lfd+1;i<maxfd+1;++i){
        if(FD_ISSET(i,&tmp)){
          // this fd have date to read
          char buf[1024]  = {0};
          int len = read(i, buf, sizeof(buf));
          if (len == -1){
            perror("read");
            exit(-1);
          }else if(len == 0){
            printf("client closed\n");
            close(i);
            FD_CLR(i, &rdset);
          }else if(len > 0){
            printf("read :%s\n", buf);
            write(i, buf, strlen(buf)+1);
          }
        }
      }
    }

  }
  close(lfd);


  return 0;
}
