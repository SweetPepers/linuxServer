

/*
       #include <sys/ipc.h>
       #include <sys/shm.h>


int shmget(key_t key, size_t size, int shmflg);
  - 创建一个新的共享内存段,或者获取一个既有的共享内存段的标识
    新建的内存段会被初始化为0
  - parameters:
    - key  : 一般用16进制表示
    - size :
    - shmflg : 属性  IPC_CREAT|0664
      - 访问权限 such as 0664   
      - 附加属性：创建(IPC_CREAT)/判断是否存在(IPC_EXCL)(This flag is used with IPC_CREAT to ensure that this call creates the segment.)
    - return value : -1 for faied ; 成功: shmid

void *shmat(int shmid, const void* shmaddr, int shmflg);
  - 关联当前进程
  - parameters:
    - shmid
    - shmaddr: 申请的共享内存的启示地址, 一般为NULL(内核指定)
    - shflg: 
      - read : SHM_RDONLY(must have)
      - read&write : 0
  - ret : 成功返回共享内存首地址, 失败返回 (void*)(-1)
int shmdt(const void* shmaddr);
  - detach : 解除当前进程和共享内存的绑定
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
  - 共享内存控制,(主要是删除共享内存)
    - shmid
    - cmd:
      - IPC_STAT : get state
      - IPC_SET : set 状态
      - IPC_RMID : 标记共享内存为 待销毁(没有attach时删除)
    - buf: 具体信息 (IPC_RMID 没用, NULL)
key_t ftok(const char *pathname, int proj_id);
  - 根据路径名和int值生成一个key(用于shmget)
    - pathname需存在
    - proj_id 系统调用只会使用该值的一个字节, 0-255

*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>


int main(){
  // 1. create a shm
  int shmid = shmget(100, 4096, IPC_CREAT|0664);
  printf("shmid %d\n", shmid);
  // 2. attach with current process
  void * addr = shmat(shmid, NULL, 0);

  // 3. write data
  char* str = "hello world";
  memcpy(addr, str, strlen(str)+1);

  printf("按任意键结束\n");
  getchar();

  // 4. shmdt
  shmctl(shmid, IPC_RMID, NULL);
  shmdt(addr);

  return 0;

}