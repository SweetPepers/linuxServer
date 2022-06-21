# 了解linux

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