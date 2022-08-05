#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <poll.h>

#define FDS_LEN (1024)

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

  // init fds
  struct pollfd fds[FDS_LEN];
  for(int i = 0;i<FDS_LEN;i++){
    fds[i].fd = -1;
    fds[i].events = POLLIN;
  }
  fds[0].fd = lfd;
  int nfds = 0;


  while(1){

    // call poll
    int ret = poll(fds, nfds+1, -1);
    if (ret == -1){
      perror("poll ");
      exit(-1);
    }else if(ret == 0){
      continue;
    }else {
      // have fd changed
      if(fds[0].revents & POLLIN){
        // new client connect
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len); // 每次调用 accept都是在lfd有效的情况下, 所以accept没机会阻塞

        // add cfd into fds
        for(int i = 1;i<FDS_LEN;i++){
          if(fds[i].fd == -1){
            fds[i].fd = cfd;
            fds[i].events = POLLIN;
            nfds = i > nfds? i : nfds;
            break;
          }
        }
        printf("new client connected\n");

      }

      for(int i=1;i<nfds+1;++i){
        if(fds[i].revents & POLLIN){
          // this fd have date to read
          char buf[1024]  = {0};
          int len = read(fds[i].fd, buf, sizeof(buf));
          if (len == -1){
            perror("read");
            exit(-1);
          }else if(len == 0){
            printf("client closed\n");
            close(fds[i].fd);
            fds[i].fd = -1;
          }else if(len > 0){
            printf("read :%s\n", buf);
            write(fds[i].fd, buf, strlen(buf)+1);
          }
        }
      }
    }

  }
  close(lfd);


  return 0;
}
