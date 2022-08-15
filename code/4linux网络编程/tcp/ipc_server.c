#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <sys/un.h>

int main(){
  int lfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (lfd == -1){
    perror("socket");
    exit(-1);
  }
  struct sockaddr_un addr;
  addr.sun_family = AF_LOCAL;
  strcpy(addr.sun_path, "server.sock");

  int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));

  if (ret == -1){
    perror("bind");
    exit(-1);
  }

  listen(lfd, 8);
  

  struct sockaddr_un cliaddr;
  int len = 0;
  int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

  if (cfd == -1){
    perror("accept");
    exit(-1);
  }

  printf("client socket name : %s\n", cliaddr.sun_path);


  // communicate
  while(1){
    char buf[1024] = {0};

    // read(cfd, buf, sizeof(buf));
    int recvLen = recv(cfd, buf, sizeof(buf), 0);
    if (recvLen == -1){
      perror("recv");
      exit(-1);
    }else if (recvLen == 0){
      printf("client closed \n");
      break;
    }else{
      printf("server read %s\n", buf);
      write(cfd, buf, strlen(buf));
    }
  }
  close(cfd);
  close(lfd);
  
  execlp("rm", "rm", "/home/server.sock", NULL);
}