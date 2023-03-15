#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>



int main(){
  // 1. get a shm
  int shmid = shmget(100, 0, IPC_CREAT);
  printf("shmid %d\n", shmid);
  // 2. attach with current process
  void * addr = shmat(shmid, NULL, 0);

  // 3. read data
  printf("%s\n", (char*)addr);

  printf("按任意键结束\n");
  getchar();

  // 4. shmdt
  shmctl(shmid, IPC_RMID, NULL);
  shmdt(addr);

  return 0;

}