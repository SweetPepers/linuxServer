# 了解linux
运行进程的时候加个 `&` 表示在后台运行
## gcc

- `gcc test_gcc.c -E -o test.i` 预处理
- ` gcc test.i -S -o test.s` 编译为汇编代码
- `gcc test.s -s -o test.o` 

![](../picture/2gcc工作流程.jpg)

![](../picture/2gcc常见选项.jpg)

![](../picture/2gcc常见选项2.jpg)
- -D 就类似这种
  ```c
  #ifdef MYDEFINE
    printf("kjahsdka");
  #endif
  ```

## 静态库

库的好处: 1. 代码保密 2. 方便部署和开发
静态库在链接阶段被复制到程序中

### 静态库的制作

- 命名规则: 
  - linux: libxxx.a
  - windows: libxxx.lib
- 制作 
  - gcc 获得 `.o` 文件
  - 将 `.o`文件打包, 使用ar工具(archive)
    - `ar rcs libxxx.a xxx.o xxxxx.o xxxx.o`
    - r -将文件插入备存文件中
    - c -建立备存文件
    - s -索引


### 静态库的使用

`gcc main.c -o app -I ./include -l cal -L ./lib` 
- -I 查找 `#include`里面的文件
- -l 添加库  libcal(省略lib) -L 去哪找这个库

## 动态库

动态库在链接阶段没被复制到程序中,而是程序在运行时由系统动态加载到内存中供程序调用

### 动态库的制作和使用

- 命名规则: 
  - linux: libxxx.so
    - 在linux下是一个可执行文件
  - windows: libxxx.dll
- 制作
  - gcc 得到 .o文件, 得到和位置无关的代码
    - gcc -c -fpic/-fPIC a.c b.c
  - gcc 得到动态库
    - gcc -shared a.o b.o -o libcalc.so

- 使用 `gcc main.c -o app -I ./include -l cal -L ./lib`  
  运行出现了错误`./main: error while loading shared libraries: libcal.so: cannot open shared object file: No such file or directory`


### 动态库加载失败的原因

- `ldd ./main` 列出动态库依赖关系 `list dynamic dependencies`
  ![](../picture/ldd.jpg)

定位共享库的位置
![](../picture/定位动态库.jpg)

### 解决动态库加载失败问题

根据上图的搜索顺序
- 可以添加到环境变量里
  - `exprot LD_LIBRARY_PATH=$LD_LIBRAY_PATH:`
  - `pwd` 显示当前目录
  - 只有一次有效
- 永久配置环境变量
  - 用户级别: 修改用户目录下的 `.bashrc`文件, 末尾加上 `export 啥啥`, 然后cmd输入 `source .bashrc`
  - 系统级别: `sudo vim /etc/profile`, 加上`exprot啥`, 然后source一下 

### 静态库和动态库对比

- 静态库的优缺点
  - 优点
    - 静态库被打包到应用程序中加载速度快
    - 发布程序无需提供静态库, 移植方便
  - 缺点
    - 消耗系统资源,浪费内存
    - 更新部署发布麻烦
- 动态库优缺点
  - 优点
    - 可以实现进程间资源共享
    - 更新部署发布简单
    - 可以控制何时加载动态库
  - 缺点
    - 加载慢
    - 发布时需要提供依赖的动态库

## makefile

自动化编译
make命令默认只执行第一条规则
1. 命名 makefile/Makefile
2. 规则![](../picture/makefile规则.jpg) 一般其他规则都是为第一条规则服务的
3. 工作原理
   1. 执行命令前,检查规则中的依赖是否存在
   2. 检查更新,在执行规则中的命令时, 会比较目标和依赖文件的时间(依赖是否需要重新生成)
4. 变量![](../picture/makefile变量.jpg)
5. 模式匹配
    `%.o:%.c`
6. 函数
   1. wildcard![](../picture/makefile_wildcard.jpg)
   2. patsubst替换![](../picture/makefile_patsubst.jpg)获取`.o`文件
7. 伪目标 `.PHONY:clean` 之后就不会和外面对比了(工作原理2, 检查更新)

 
## GDB
功能
  - 启动程序
  - 让程序在指定断点处停下(断点可以为表达式)
  - 当程序被停住时,可以检查此时程序的状态
  - 可以改变程序, 将一个BUG产生的影响修正从而测试其他BUG
2. 使用
    编译时加 `-g`, 然后 `gdb main`
3. 命令![](../picture/gdb命令.jpg)设置参数就是cmd里面给的那个
4. 断点![](../picture/gdb断点.jpg)
5. 调试![](../picture/gdb调试.jpg)


## linux文件

### 标准C库IO函数和linux系统IO函数对比

跨平台实现: java虚拟机, C调用系统API
标准C库带有缓冲区, linux自带IO无缓冲区
![](../picture/1标准C库IO.jpg)
### 虚拟地址空间

![](../picture/1虚拟地址空间.jpg)

### 文件描述符
![](../picture/1文件描述符.jpg)
PCB里面存在一个文件描述符表


### linux系统IO函数

- open
  int open(const char* pathname, int flags);
  int open(const char* pathname, int flags, mode_t mode);
  打开文件  open.c

  创建文件 open.c/ceate()
  ![](../picture/1文件create.jpg)  **这里和umask不符**

- read/write
  ssize_t read(int fd, void *buf, size_t count);
  ssize_t write(int fd, const   void *buf, size_t count);
- lseek
  c语言库里为fseek
  off_t lseek(int fd, off_t offset, int whence);
  作用: 
  1. 移动文件指针到头文件 lseek(fd, 0, SEEK_SET)
  2. 获取当前文件指针的位置  a = lseek(fd, 0, SEEK_CUR)
  3. 获取文件长度 len = lseek(fd, 0, SEEK_END)
  4. 拓展文件长度, 比如增加100字节, (当前是10) lseek(fd,100, SEEK_END), 比如下载的时候, 先拓展占用位置, 然后慢慢下载
  写一个`" "` 就从10 变成 110了, (见 `hello.txt`)
- stat/lstat 
  linux还有 `stat`这个命名: 查看文件信息
  int stat(const char *pathname, struct stat *statbuf)
  int lstat(const char *pathname, struct stat *statbuf)
  ![](../picture/1stat结构题.jpg)
  ![](../picture/1st_mode.jpg)

### 文件属性操作函数
`file_property.c`  
int access(const char* paathname, int mode);
  - 判断某个文件是个否有mode指定的权限
  - mode取值: R_OK W_OK X_OK F_OK(是否存在)

int chmod(const char* filename, int mode);
  - 修改文件权限
  - mode取值: 具体查看`man 2 chmod`

int chown(const char* path, uid_t owner, gid_t group);
int truncate(const char * path, off_t length);
  - 缩减或扩展文件的大小


### 目录操作函数
`filedir.c`  

int mkdir(const char* pathname, mode_t mode);

int rmdir(const char* pathname);

int rename(const char* oldpath, const char* newpath)

int chdir(const char* path);
  - 修改进程的工作目录  

char* getcwd(char *buf, size_t size);
  - 获取当前工作路径


### 目录遍历函数 

`man 3 opendir`  
DIR *opendir(const char* name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);

### dup dup2函数
- int dup(int oldfd) : 复制一个新的文件描述符
  从空闲的文件描述符表中找一个最小的, 作为新的拷贝的文件描述符, 两个文件描述符指向同一个文件
- int dup2(int oldfd, int newfd)
  用指定的newfd(必须有效)指向oldfd指向的文件, 重定向文件描述符


### fcntl
int fcntl(int fd, int cmd, .../*arg*/)
  - 复制文件描述符
  - 设置/获取文件的状态标志  
     #include <unistd.h>  
     #include <fcntl.h>

       int fcntl(int fd, int cmd, ... /* arg */ );
    - 参数 cmd
      - F_DUPFD : 复制文件描述符 int ret = fcntl(fd, D_DUPFD)
      - F_GETFL : 获取文件状态flag
      - F_SETFL : 设置



# linux多进程开发

## 进程

在传统的操作系统中, 进程既是基本的分配单元, 也是基本的执行单元

内核数据结构
  - 进程相关的标识号(IDs)
  - 虚拟内存表
  - 打开的文件描述符
  - 信号传递及处理的有关信息
  - 进程资源使用及限制
  - 当前工作目录等 

![](../picture/2单道多道程序.jpg)

![](../picture/2PCB.jpg)
![](../picture/2PCb2.jpg)
- 进程可以使用的资源上限(可用`ulimit -a`查看)


### 进程状态切换
新建态,就绪态,运行态,阻塞态(wait/sleep),终止态
![](../picture/2进程状态转换.jpg)

- 显示进程信息 指令`ps`
  `ps aux`![](../picture/2指令ps.jpg)
  - 表头
    - tty : 当前进程所属的终端
    - STAT状态![](../picture/2STAT参数意义.jpg)
    - command : 什么命令产生的进程
  `ps ajx`![](../picture/2psajx.jpg)
    - PPID: parent process ID
    - PGID: process group ID
    - SID : 会话ID(例: 多个group组成一个会话)

- 动态显示进程 `top`![](../picture/2动态显示进程.jpg)
- `kill` 杀死进程
  - `kill [-signal] pid`
  - `kill -l`列出所有信号
  - `kill -SIGKILL pid`强制干掉
  - `kill -9 pid`强制干掉

### fork
改的 : 
- pid,ppid 
- 返回值rax
- 信号集

fork()通过 **写时拷贝(copy-on-write)** 实现.写时拷贝可以推迟甚至避免拷贝数据的技术.
内核此时并不复制整个进程的地址空间,而是让父子进程共享同一个地址空间.即资源的复制只有在需要写入的时候才会进行,在此之前都是以只读方式共享

fork产生的子进程与父进程相同文件的文件描述符指向相同的文件表, 引用计数增加

### GDB多进程调试
`hello.c`

gdb默认追踪父进程
设置 调试父进程或者子进程`set follow-fork-mode [parent|child]`
`set detach-on-fork [on|off]`调试的时候 另一个进程是否悬停在fork()

其他几个命令 
 - `info inferiors` 查看调试的进程
 - `inferiors id` 切换当前调试的进程
 - `detach inferiors id`使进程脱离GDB调试

### exec函数族  `elecl.c`
exec根据文件名找到可执行文件,并用它来取代调用进程的内容,即在调用程序内部执行一个可执行文件
 
**保留内核区, 但用户区直接被替换为调用函数**

返回: **执行成功不会返回**, 失败返回-1 
![](../picture/2_3exec函数族.jpg)


执行exec 
![](../picture/2_6execl2.jpg)
并不会出现下面那种两次终端的情况

孤儿进程会多一次终端
![](../picture/2_6孤儿.jpg)
最后无了,
原因: 父进程已经结束运行, 子进程变为了孤儿进程
另外可以看到, 调用进程的pid = 4382 等于exec中子进程的pid, 
//但是ppid两者不一样???
 1924 是init的进程号, 见下面孤儿进程, TODO **教学里面父进程是1**


### 进程退出, 孤儿进程,僵尸进程 
- 进程退出
  ![](../picture/2_7进程退出.jpg)
  `_exit()`不会刷新I/O缓冲区
  ![](../picture/2_7exitIO.jpg)

- 孤儿进程
  - 父进程结束子进程还运行
  - 出现孤儿进程时, 内核会把孤儿进程的父进程设置为init, init会循环的wait()这些子进程, 
  - 孤儿进程也不会有什么危害
- 僵尸进程![](../picture/2_7僵尸进程.jpg)
  - zombie.c
  - 内核区没有释放
  - 子进程结束而父进程没有结束导致自己成内核区不能释放

### wait  waitpid
`waitdemo.c`
![](../picture/2_8进程回抽.jpg)

```c
pid_t wait(int *wstatus);
  - 成功返回子进程id
  - 失败返回-1(所有子进程结束 or  调用失败)
  调用wait会阻塞
  - wstatus  存储退出信息 

  有些退出信息宏函数, 
    - WIFEXITED(wstatus) 
    - WEXITSTATUS(wstatus)

    等等 
```

```c
pid_t waitpid(pid_t pid, int *wstatus, int options);
  回收指定进程号的子进程,可以设置是否阻塞

  - pid :
      >0 : 子进程
      =0 : 回收当前进程组的所有子进程
      -1 : 回收所有子进程(相当于wait())
      <-1: 回收某个进程组的组id, 进程组id为 -pid, 
  - ret :
      >0 子进程id
      =0 :在 option=WHNOHANG下, 表示还有子进程
      =-1: 错误或者没子进程
```


## 进程间通信 (IPC)

![](../picture/2_10进程间通信.jpg)

- 匿名管道
  - 半双工
  - 只能在具有公共祖先的进程(父与子, 或者兄弟进程)之间使用
  
  为啥可以用管道进行进程间通信?
    - 亲缘进程之间的文件描述符共享
  
  数据结构
    - 环形队列


### 匿名管道通信  pipe.c
![](../picture/2_10匿名管道的使用.jpg)

管道 默认**阻塞**

一个例子 : 实现 ps aux | grep xxx  `parent-child-ipc.c`


### 管道的读写特点

1. 指向管道写端的文件描述符都被关闭了(管道写端引用计数为0), 当管道中的数据被读完后, 再read就会返回0,(类似文件EOF)
2. 没有关闭(管道写端引用计数大于0), 数据被读完后再read就会阻塞
3. 管道读端引用计数等于0(**读端都关闭**), 继续写, 该进程就会收到SIGPIPE信号, 通常导致进程异常终止
4. 读端引用计数大于0,管道满了会阻塞 