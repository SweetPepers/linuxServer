#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <sys/un.h>

int main(){
  int cfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (cfd == -1){
    perror("socket");
    exit(-1);
  }
  struct sockaddr_un addr;
  addr.sun_family = AF_LOCAL;
  strcpy(addr.sun_path, "client.sock");

  int ret = bind(cfd, (struct sockaddr *)&addr, sizeof(addr));

  if (ret == -1){
    perror("bind");
    exit(-1);
  }


  struct sockaddr_un seraddr;
  seraddr.sun_family = AF_LOCAL;
  strcpy(seraddr.sun_path, "server.sock");
  ret = connect(cfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
  if (ret == -1){
    perror("connect");
    exit(-1);
  }

  int num = 0;
  // communicate
  while(num < 5){
    char buf[1024] = {0};
    sprintf(buf, "hello server %d\n", num++);
    send(cfd, buf, strlen(buf)+1, 0);
    printf("client say %s\n", buf);


    // read(cfd, buf, sizeof(buf));
    int recvLen = recv(cfd, buf, sizeof(buf), 0);
    if (recvLen == -1){
      perror("recv");
      exit(-1);
    }else if (recvLen == 0){
      printf("server closed \n");
      break;
    }else{
      printf("client read %s\n", buf);
    }
    sleep(1);

  }
  close(cfd);
  execlp("rm", "rm", "/home/client.sock", NULL);

}