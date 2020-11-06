# 目录
<!-- vim-markdown-toc GFM -->

- [UNIX基础知识](#unix基础知识)
  - [UNIX体系结构](#unix体系结构)
  - [登录](#登录)
  - [文件和目录](#文件和目录)
  - [输入和输出](#输入和输出)
  - [程序与进程](#程序与进程)
  - [出错处理](#出错处理)
  - [用户标识](#用户标识)
  - [信号](#信号)
  - [时间值](#时间值)
- [UNIX标准及实现](#unix标准及实现)
  - [UNIX标准化](#unix标准化)
  - [限制](#限制)
- [文件I/O](#文件io)
  - [文件描述符](#文件描述符)
  - [标识的位运算技巧](#标识的位运算技巧)
  - [不足值](#不足值)
- [文件与目录](#文件与目录)
  - [文件信息](#文件信息)
    - [文件类型](#文件类型)
    - [权限](#权限)
  - [文件系统](#文件系统)
- [进程控制](#进程控制)
- [进程关系](#进程关系)
  - [作业控制](#作业控制)
  - [终端 TTY](#终端-tty)
  - [会话 SID](#会话-sid)
  - [进程组 PGID](#进程组-pgid)
- [信号](#信号-1)
- [线程](#线程)
- [标准与限制](#标准与限制)
- [错误处理](#错误处理)
- [用户信息](#用户信息)
  - [passwd](#passwd)
  - [shadow](#shadow)
  - [group](#group)
- [系统信息](#系统信息)
  - [操作系统信息](#操作系统信息)
  - [日期时间](#日期时间)
  - [语系与字符集](#语系与字符集)
- [日志系统](#日志系统)
- [进程管理](#进程管理)
  - [进程环境](#进程环境)
  - [进程控制](#进程控制-1)
  - [线程](#线程-1)
  - [信号处理](#信号处理)
  - [资源限制](#资源限制)
- [虚拟文件系统](#虚拟文件系统)
  - [文件系统结构](#文件系统结构)
    - [硬链接](#硬链接)
    - [文件信息](#文件信息-1)
  - [文件日期](#文件日期)
  - [权限](#权限-1)
  - [文件类型](#文件类型-1)
    - [目录](#目录)
    - [符号链接](#符号链接)
    - [设备文件](#设备文件)
    - [普通文件](#普通文件)
    - [管道](#管道)
    - [套接字](#套接字)
    - [终端](#终端)
  - [高级I/O](#高级io)
    - [非阻塞I/O](#非阻塞io)
    - [记录锁](#记录锁)
    - [分段I/O](#分段io)
    - [I/O多路复用](#io多路复用)
    - [内存映射](#内存映射)

<!-- vim-markdown-toc -->
# UNIX基础知识
## UNIX体系结构
![unix](images/unixarch.png)

## 登录
&emsp;由login程序接收用户输入的“用户名”与“密码”，进行身份验证与授权。
然后根据`/etc/{passwd,shadow,group}`文件中的内容启动并设置shell。

## 文件和目录
&emsp; **文件系统** ：目录与文件的层次结构，起点称为“根(root)”。
通过挂载技术，文件系统可映射到不同的物理设备上。

&emsp; **文件名** ：每个文件的名字存在与其父目录的目录项中（目录项中还包含i-node）。
`.`与`..`为特殊文件名，且文件名还不能包含`/`。

&emsp; **路径名** ：文件名在不同目录下可具有相同的名字，需要用路径名唯一确定。
以`/`开头的路径名为绝对路径，否则为相对路径（相对路径与文件名形似）

&emsp; **工作目录** ：上述相对路径，相当于将当前的工作目录加在相对路径名前面形成绝对路径。

## 输入和输出
&emsp; **文件描述符** ：进程的信息之一，用来帮助进程标记一个文件。

&emsp; **标准文件描述符** ：`stdin, stdout, stderr`一般继承自shell并连接到终端设备。

&emsp; **终端输入** ：由内核驱动模块将键盘中的扫描码转换成字符再传送给程序。
其中有特殊字符，
* `NL`表示添加字符`\n`并冲刷输入内容；
* `CTRL-D`表示冲刷输入内容，若内容为空则表示结束输入即触发`EOF`。

## 程序与进程
&emsp; **程序与进程** ：进程是正在运行中的程序实例。

&emsp; **进程控制块** ：存储有进程的相关信息。
与环境变量不同在于，后者可由进程随意更改，故内核只查看前者。

&emsp; **进程控制** ：主要的函数调用有`fork, exec, wait、sleep、pause、exit`

&emsp; **线程** ：一个进程内的线程共享进程的控制块信息与虚拟内存

## 出错处理

&emsp; **返回值** ：系统调用出错时大多数系统函数返回`-1`

## 用户标识
&emsp; **UID** ：一个UID可对应多个登录名（用户名），超级用户（root）的UID为0。

&emsp; **GID** ：以组的形式来管理多个用户的权限

&emsp; **附属GID** ：一个用户可加入多个组

## 信号
&emsp; **终端发送信号** ：
* `CTRL-C`：`SIGINT 2`
* `CTRL-\`：`SIGQUIT 3`
* `CTRL-Z`：`SIGTSTP 20`

&emsp; **信号处理** ：
* 忽略
* 默认
* 捕获并处理

## 时间值
&emsp; **日历时间** ：记录从Epoch（1970-1-1 00:00:00）起至今的秒数。
硬件中记录的可能是UTC时间，也可能是本地时间，通过系统调用可隐藏此细节。

&emsp; **进程时间** ：
* real  ：从进程启动到退出期间流逝的时间
* sys   ：进程进入内核态执行指令所花费的CPU时间
* user  ：进程在用户态中执行指令所花费的CPU时间

# UNIX标准及实现
## UNIX标准化
`ISO C`、`IEEE POSIX`、`USU`标准规范了系统接口，通过C头文件的形式供程序调用

## 限制
UNIX系统实现了许多常量对系统的行为进行可移植性的标准化，包括：
* 编译时限制
* 与文件系统无关的运行时限制
* 与文件系统有关的运行时限制

# 文件I/O
## 文件描述符
![fd](images/fd.jpg)
* 文件描述符（进程唯一）
    * 描述符标识
        > 即`FD_CLOEXEC`：调用exec关闭该描述符
    * 文件表项指针
* 文件表项（系统共享）
    * 文件打开标识
    * 文件偏移量
        > 注意可能造成空洞，空洞可能不会占用实际磁盘块
    * v节点表项指针
* v节点表项
    > 与文件缓存缓冲区块关联，以执行原子操作
    * 文件系统相关i节点
    * 文件系统无关i节点

## 标识的位运算技巧
* 取值：`flag & mask`
* 置位：`flag | bit`
* 清除：`flag & ~bit`

## 不足值
* EOF
* 终端每行读取
* socket
* pipe,FIFO
* 面向记录设备

# 文件与目录
## 文件信息
### 文件类型
将文件类型加入考虑后，读写目录与符号链接需要专门处理的函数

### 权限
将权限加入考虑后，许多函数需要满足某些权限才能正常工作

&emsp; **权限的意义** ：
* 对于普通文件：
    * 读：可以读取其block中的内容
    * 写：可以更改其block中的内容
    * 执：可以使用exec函数加载并执行该（程序）文件
* 对于目录：
    * 读：可以读取其目录项（文件名与i-node）
    * 写：可以添加、删除或修改其目录项
    * 执：可以通过该目录作为相对路径访问其目录项所指向的文件
* 对于符号链接：
    * 一般权限为`777`且无法更改

&emsp; **权限的判断** ：
1. 若`EUID==0`则直接授权读写（不包括执行）
2. 若`EUID==OWNER`则进行适当权限判断，然后结束
3. 若`EGID==GROUP`则进行适当权限判断，然后结束
4. 若存在`EUID的附属GID==GROUP`则进行适当权限判断，然后结束
5. 否则使用OTHER权限进行适当权限判断，然后结束

&emsp; **Sticky Bit删除权限的判断**：
* 文件的属主
* 目录的属主
* 超级用户

&emsp; **文件的权限修改** ：
* 每个进程有个umask标识表示权限掩码，除了chmod函数外，其他函数设置的权限皆受影响
* 当非root进程写一个普通文件时，自动清除其SUID与SGID

&emsp; **所有者的意义** ：
* 绝大多数修改文件属性的操作只有文件的所有者才能进行

&emsp; **新建文件的所有者** ：
* ONWER为EUID
* GROUP为EGID。但若父目录设置SGID，则GROUP为父目录的GROUP

&emsp; **文件所有者的修改** ：
* 普通用户只能修改文件UID为进程EUID，超级用户随意修改
* 普通用户只能修改文件GID为进程EGID或附属GID之一，超级用户随意修改

## 文件系统
&emsp; **硬盘文件系统结构** ：
* 超级块：存储整个文件系统的信息
* i-node：存储文件属性（包括block索引，不包括文件名）
* block ：包含文件内容

符号链接：文件内容为“路径名”的特殊文件。指向的文件可能不存在。

硬链接：指向某文件的目录项。  
当一文件的硬链接数为0且无进程正打开它，则文件判定为可删除。  
若一文件的硬链接数为0但有进程正打开它，则文件对其他进程不可见，且待最后一个打开它的进程结束后，文件被判定为可删除。

设备号：文件系统所在存储设备都由主、次设备号表示。

文件长度：只针对普通文件、目录文件、符号链接有效。
文件长度与文件占用的块大小可能并不一样（因为文件空洞与block指针），两者间的差别可判断文件是否空洞（整个块都为0则为空洞）

# 进程控制
进程的信息存储于进程控制块，环境变量可由用户更改而可能并非真实

| 进程信息      | fork子进程继承信息 | exec保留信息                                   |
|---------------|:------------------:|------------------------------------------------|
| UID, GID      |          1         | 1                                              |
| EUID, EGID    |          1         | 0 (依赖执行文件SUID与SGID)                     |
| SUID, SGID    |          1         | 0 (依赖EUID与EGID)                             |
| 附属GID       |          1         | 1                                              |
| PID           |          0         | 1                                              |
| PPID          |          0         | 1                                              |
| PGID          |          1         | 1                                              |
| SID           |          1         | 1                                              |
| PWD           |          1         | 1                                              |
| ROOTDIR       |          1         | 1                                              |
| UMASK         |          1         | 1                                              |
| FD            |          1         | 0 (依赖文件描述符FD_CLOEXEC)                   |
| ENV           |          1         | 0 (依赖exec参数)                               |
| VMEM          |          1         | 0                                              |
| LIMITS        |          1         | 1                                              |
| NICE          |          1         | 1                                              |
| SIGNAL_HANDLE |          1         | 0 (设置了处理函数的信号恢复默认，其余设置不变) |
| SIGNAL_MASK   |          1         | 1                                              |
| SIGNAL_SET    |          0         | 1                                              |
| ALARM         |          0         | 1                                              |
| TIME          |          0         | 1                                              |
| LOCK          |          0         | 1                                              |

# 进程关系
## 作业控制
&emsp;在交互shell中，执行命令的进程会拥有自己的、独立的、不同与shell的PGID，
而用管道连接的多个进程拥有相同的PGID。而每个进程组便是一个“作业”。  
&emsp;作业分为前台与后台，前台进程组号(TPGID)由TTY保存，其余均为后台进程组。
作业控制大都由终端驱动程序发送的信号构成。

## 终端 TTY
一个终端通常会与一个进程会话相关联，并能辨别其中的前台进程组。

终端的作用：
* 将键盘、屏幕、网络抽象成I/O缓冲队列供进程读写
* 特殊字符处理与作业控制

终端的模式：
* 规范模式：将字符组装成行才进行I/O
* 非规范模式：不将字符装配成行


物理终端原理：  
&emsp;键盘驱动程序将来自键盘的扫描码转换为字符，送给恰当的终端驱动程序。
&emsp;终端驱动程序根据终端属性配置，将接收的字符处理后加入输入缓冲队列供用户程序读，
并将输出缓冲队列与回显字符（若设置了回显的话）处理后送给屏幕打印驱动程序。（终端行规程）
&emsp;屏幕打印驱动程序将接收的字符流根据ANSI规范渲染成图形。

伪终端原理：  
&emsp;服务程序打开伪终端主设备(`/dev/ptmx`)，产生从设备(`/dev/pts/0`)给客户进程做控制终端。
&emsp;主从设备就像socket通讯一般工作，
也就是说服务进程为伪终端提供原本由键盘驱动程序提供的输入字符流，
而伪终端给服务进程返回原本传送给屏幕打印程序的字符流。


## 会话 SID
会话是进程组的集合，每个会话有一个会话首进程（调用setsid(3)），一个会话可以与一个终端进行关联。

新建会话的作用：
* 新建或切断与终端的联系，从而与原来的作业控制机制隔离（并不会自动关闭之前的终端文件描述符）

会话首进程的作用：
* 会话首进程调用`open()`打开第一个尚未关联会话的终端时，只要未指定`O_NOCTTY`则将本会话与该终端关联

&emsp;那些不与终端联系，不属于用户会话的进程，即被称为Daemon（守护进程）。
通过fork后退出父进程然后子进程调用setsid即可形成。
若不像与用户会话脱离则不用退出父进程。
但是注意，退出了父进程但又不调用setsid，则可能形成孤儿进程组。

&emsp;Daemon：
* fork后父进程exit，然后调用setsid。若防止Daemon取得终端，则再次fork并使父进程exit
* 更改cwd到根目录，防止不能卸载文件系统
* 关闭从父进程继承而来的文件描述符
* 调用syslog报告日志

&emsp;用户登录时，由init程序启动getty程序来连接终端。
然后getty调用exec执行login提供用户登录界面，在执行login前会调用setsid建立会话。
login调用PAM来验证用户身份并授权，再为用户会话准备基础的、安全的环境。
然后再调用exec来执行shell（其argv[0]为"-sh"表示其为登录shell），shell加载配置以初始化环境。

## 进程组 PGID
进程组是进程的集合，每个进程组有一个组长进程（调用stdpgid(3)）

进程组的作用：
* 方便使用信号来管理整个进程组中的进程

进程组组长：
* 不能调用`setsid()`新建会话

父进程：
* 可为子进程调用`setpgid()`
* 退出可形成孤儿进程：某进程的父进程终止后，该进程成为孤儿进程，由init进程收养，防止形成僵尸进程。  
* 退出也可形成孤儿进程组：某进程组中的所有进程的父进程，没有一个是同会话中其他进程组的进程，表示该进程组与同会话中的其他进程组缺乏联系了。
当孤儿进程组中有停止的进程时，内核向孤儿进程组发送信号`SIGHUP`与`SIGCONT`

同一进程组：
* fork出的子进程与父进程同组
* shell中使用管道连接的多个进程也同属一个进程组（因为不会发生终端竞争）
* 非交互模式执行shell脚本时，shell与命令进程同属一个进程组


# 信号
信号相关进程信息：
* SIGNAL_SET
    > 权限：只有发射UID或EUID匹配的进程（SIGCONT可发射给同一会话的所有进程）
* SIGNAL_MASK
    > 线程独立
* SIGNAL_HANDLE
    > 多线程中信号处理会占用一个线程的逻辑流


# 线程
同步原语：
* 互斥量：读取-测试-上锁/阻塞
* 条件量：解锁-阻塞

线程独立数据主要有：
* 栈
* errno变量
* 调度优先级
* 信号屏蔽字

除此之外，绝大多数阻塞函数只针对调用线程阻塞


# 标准与限制
* 若未特殊说明，则出错时返回-1，标记为(NOE)表示无出错返回值
* 若未特殊说明，则文件处理函数一般都会跟随符号链接
* 若未特殊说明，则at后缀函数支持参数`flag=AT_SYMLINK_NOFOLLOW`；否则仅支持特殊说明的flag
* 若未特殊说明，则at后缀函数支持参数`fd=AT_FDCWD`
* 若未特殊说明，则返回数据指针的函数都可能指向local-static对象
<!-- entry begin: sysconf pathconf fpathconf -->
```c
#include <limits.h>
/* 以_SC_开头的宏使用sysconf()，以_PC_开头的宏使用pathconf()或fpathconf()，否则可直接使用 */

#include <unistd.h>
long sysconf(int name);                         // 返回对应限制值
long pathconf(const char* pathname, int name);  // 返回对应限制值
long fpathconf(int fd, int name);               // 返回对应限制值
```
<!-- entry end -->

# 错误处理
<!-- entry begin: strerror perror -->
`errno`两条重要规则：
* 若未出错则其值不会被清除
* 任何标准函数都不会将其置零
```c
#include <errno.h>
thread_local int errno;                         // 标准只规定errno为线程独立的左值

#include <string.h>
char*   strerror(int errno);                    // 返回errno映射的消息字符串(NOE)

#include <stdio.h>
void    perror(const char* msg);                // 打印 msg + ": " + strerror(errno)
```
<!-- entry end -->

# 用户信息
## passwd
<!-- entry begin: passwd getpwuid getpwnam getpwent setpwent endpwent -->
```c
#include <pwd.h>
struct passwd
{
    char*   pw_name;
    char*   pw_passwd;
    char*   pw_uid;
    char*   pw_gid;
    char*   pw_gecos;
    char*   pw_dir;
    char*   pw_shell;
};
passwd* getpwuid(uid_t uid);        // 返回对应`passwd*`，若出错返回NULL
passwd* getpwnam(const char* name); // 返回对应`passwd*`，若出错返回NULL
passwd* getpwent(void);             // 返回当前条目对应`passwd*`。第一次调用自动打开数据文件；自动后移条目指针
void    setpwent(void);             // 打开/etc/passwd，并将条目指针移动到初始位置
void    endpwent(void);             // 关闭/etc/passwd
```
<!-- entry end -->

## shadow
<!-- entry begin: shadow spwd getspnam getspent setspent endspent -->
```c
#include <shadow.h>
struct spwd
{
    char*           sp_namp;
    char*           sp_pwdp;
    int             sp_lstchg;
    int             sp_min;
    int             sp_max;
    int             sp_warn;
    int             sp_inact;
    int             sp_expire;
    unsigned int    sp_flag;
};
spwd*   getspnam(const char* name); // 返回对应`spwd*`，若出错返回NULL
spwd*   getspent(void);             // 返回当前条目对应`spwd*`。第一次调用自动打开数据文件；自动后移条目指针 
void    setspent(void);             // 打开/etc/shadow，并将条目指针移动到初始位置
void    endspent(void);             // 关闭/etc/shadow
```
<!-- entry end -->

## group
<!-- entry begin: group getgrgid getgrnam getgrent setgrent endgrent getgroups -->
| 限制宏      | 说明           |
|-------------|----------------|
| NGROUPS_MAX | 附数组最大数量 |

```c
#include <grp.h>
struct group
{
    char*   gr_name;
    char*   gr_passwd;
    int     gr_gid;
    char**  gr_mem;
};
group*      getgrgid(gid_t gid);                        // 返回对应`group*`，若出错返回NULL
group*      getgrnam(const char* name);                 // 返回对应`group*`，若出错返回NULL
void        setgrent(void);                             // 返回当前条目对应`group*`。第一次调用自动打开数据文件；自动后移条目指针
group*      getgrent(void);                             // 打开/etc/group，并将条目指针移动到初始位置
void        endgrent(void);                             // 关闭/etc/group

#include <unistd.h>
int         getgroups(int bufsize, gid_t gidlist[]);    // 返回存入的组的数量，若bufsize==0则只返回总的gid数
```
<!-- entry end -->

# 系统信息
## 操作系统信息
<!-- entry begin: uname gethostname -->
| 限制宏        | 说明           |
|---------------|----------------|
| HOST_NAME_MAX | 主机名最大长度 |
```c
#include <sys/utsname.h>
struct utsname
{
    char sysname[];     // 系统内核
    char nodename[];    // 主机名称
    char release[];     // 内核版本
    char version[];     // 发布时间
    char machine[];     // 机器架构
}
int uname(utsname* name);                   // 返回非负

#include <unistd.h>
int gethostname(char* name, int namelen);   // 返回0
```
<!-- entry end -->

## 日期时间
<!-- entry begin: time gmtime localtime strftime strftime_l mktime tms times clock_gettime clock_settime clock_getres -->
| 限制宏      | 说明           |
|-------------|----------------|
| _SC_CLK_TCK | 每秒时钟滴答数 |
```c
/* 日期时间 */
#include <time.h>
time_t  time(time_t* calptr);                                                                       // 返回时间值。calptr可为NULL
tm*     gmtime(const time_t* calptr);                                                               // 返回UTC的`tm*`
tm*     localtime(const time_t* calptr);                                                            // 返回本地时区的`tm*`
size_t  strftime(char* buf, size_t maxsize, const char* format, const tm* tmptr);                   // 返回buf字符数
size_t  strftime_l(char* buf, size_t maxsize, const char* format, const tm* tmptr, locale_t locale);// 返回buf字符数
char*   strptime(const char* buf, const char* format, tm* tmptr);                                   // 返回下次解析位置的指针
time_t  mktime(tm* tmptr);                                                                          // 返回本地时区的`tm*`对应time_t

/* 单调时间、进程时间 */
#include <sys/time.h>
struct tms
{
    clock_t tms_utime;      // 用户CPU时间
    clock_t tms_stime;      // 系统CPU时间
    clock_t tms_cutime;     // 用户CPU时间（已终止子进程）
    clock_t tms_cstime;     // 系统CPU时间（已终止子进程）
};
clock_t times(tms* buf);    // 返回单调时间（除以每秒时钟滴答数即得秒数）。

/* 日期时间、单调时间、进程时间、线程时间 */
#include <sys/time.h>
int     clock_gettime(clockid_t clock_id, timespec* tsp);    // 返回0
int     clock_getres(clockid_t clock_id, timespec* tsp);     // 返回0
int     clock_settime(clockid_t clock_id, timespec* tsp);    // 返回0
```
| clock_id                 | 说明                           |
|--------------------------|--------------------------------|
| CLOCK_REALTIME           | 系统日期时间计时器（系统时间） |
| CLOCK_MONOTONIC          | 系统开机时间计时器（单调时间） |
| CLOCK_PROCESS_CPUTIME_ID | 进程CPU时间                    |
| CLOCK_THREAD_CPUTIME_ID  | 线程CPU时间                    |
<!-- entry end -->

<!-- entry begin: strftime format date -->
| strftime format | 说明            | Thu Jan 19 21:24:52 EST 2012 |
|-----------------|-----------------|------------------------------|
| %C              | 年前两位        | 20                           |
| %y              | 年后两位        | 12                           |
| %Y              | 年              | 2012                         |
| %b、%h          | 月名缩写        | Jan                          |
| %B              | 月名            | January                      |
| %m              | 月(01-12)       | 01                           |
| %j              | 日(年)(001-366) | 019                          |
| %d              | 日(月)(01-31)   | 19                           |
| %e              | 日(月)( 1-31)   | 19                           |
| %a              | 周名缩写        | Thu                          |
| %A              | 周名            | Thursday                     |
| %w              | 周几(0-6)       | 4                            |
| %p              | AM/PM           | PM                           |
| %I              | 时(00-12)       | 01                           |
| %H              | 时(00-23)       | 21                           |
| %M              | 分(00-59)       | 24                           |
| %S              | 秒(00-60)       | 52                           |
| %F              | 日期            | 2012-01-19                   |
| %X              | 时间            | 21:24:52                     |
| %r              | 时间            | 09:24:52 PM                  |
| %Z              | 时区            | EST                          |
| %%              | 转义`%`         |                              |
| %t              | 转义`\t`        |                              |
| %n              | 转义`\n`        |                              |
<!-- entry end -->

## 语系与字符集
<!-- entry begin: setlocale -->
```c
#include <locale.h>
char*   setlocale(int category, const char* locale);            // 返回locale字符串。category一般为LC_ALL；locale为NULL返回"C"；locale为""返回系统locale
```
<!-- entry end -->

# 日志系统
<!-- entry begin: openlog syslog closelog setlogmask -->
```c
#include <syslog.h>
void    openlog(const char* ident, int option, int facility);   // 打开日志文件/dev/log，并指定日志条目名为ident（一般为程序名称）
void    syslog(int priority, const char* format, ...);          // priority为facility与level或运算组合，若openlog指定了facility则priority等同level
void    closelog(void);                                         // 关闭日志文件
int     setlogmask(int maskpri);                                // 返回之前maskpri。只允许记录maskpri中设置的级别的日志，特殊地设置为0则函数无效
int     LOGMASK(int priority);                                  // 返回将pri转换的maskpri
```
| option     | 说明                                    |
|------------|-----------------------------------------|
| LOG_PID    | 记录日志时包含PID信息                   |
| LOG_CONS   | 若无法连接日志管理服务则打印到控制台    |
| LOG_PERROR | 额外打印到标准错误                      |
| LOG_NDELAY | 立即打开UNIX socket连接日志管理服务     |
| LOG_ODELAY | 延迟打开UNIX socket直到第一次记录日志时 |

| facility     | 说明                          |
|--------------|-------------------------------|
| LOG_AUDIT    | 审计检查设施                  |
| LOG_AUTH     | 授权程序：login、su、getty等  |
| LOG_AUTHPRIV | 同上，但写入日志时有限制      |
| LOG_CONSOLE  | 消息写入/dev/console          |
| LOG_CRON     | 计划任务：cron与at            |
| LOG_DAEMON   | 系统守护进程：inetd、routed等 |
| LOG_FTP      | ftpd                          |
| LOG_KERN     | 内核消息                      |
| LOG_LPR      | 打印机系统：lpd、lpc等        |
| LOG_MAIL     | 邮件系统                      |
| LOG_NEWS     | 新闻组                        |
| LOG_NTP      | 网络时间协议系统              |
| LOG_SECURITY | 安全子系统                    |
| LOG_SYSLOG   | 日志服务本身                  |
| LOG_UUCP     | UUCP系统                      |
| LOG_USER     | 其他用户进程                  |
| LOG_LOCAL0   | 保留本地使用                  |
| LOG_LOCAL1   | 保留本地使用                  |
| LOG_LOCAL2   | 保留本地使用                  |
| LOG_LOCAL3   | 保留本地使用                  |
| LOG_LOCAL4   | 保留本地使用                  |
| LOG_LOCAL5   | 保留本地使用                  |
| LOG_LOCAL6   | 保留本地使用                  |
| LOG_LOCAL7   | 保留本地使用                  |

| level       | 说明               |
|-------------|--------------------|
| LOG_DEBUG   | 调试消息           |
| LOG_INFO    | 信息性消息         |
| LOG_NOTICE  | 正常但重要消息     |
| LOG_WARNING | 警告情况           |
| LOG_ERR     | 出错情况           |
| LOG_CRIT    | 严重情况           |
| LOG_ALERT   | 必须立即修复的情况 |
| LOG_EMERG   | 系统不可使用的情况 |
<!-- entry end -->

# 进程管理
## 进程环境
<!-- entry begin: environ getenv setenv unsetenv clearenv getpid getppid getpgid getsid tcgetpgrp setpgid setsid tcsetpgrp tcgetsid getlogin getuid geteuid getgid getegid setuid seteuid setgid setegid -->
```c
#include <unistd.h>
char**  environ;                                                    // 指向进程环境表的第一个字符串。环境表以NULL结尾；内核并不查看环境表的信息，而查看进程控制块
char*   getenv(const char* name);                                   // 返回value字符串，若出错返回NULL
int     setenv(const char* name, const char* value, int rewrite);   // 返回0
int     unsetenv(const char* name);                                 // 返回0
int     clearenv(void);                                             // 返回0

#include <unistd.h>
pid_t   getpid(void);                   // 返回PID
pid_t   getppid(void);                  // 返回PPID
pid_t   getpgid(pid_t pid);             // 返回PGID。pid==0表示获取调用进程的PGID
pid_t   getsid(pid_t pid);              // 返回SID。pid==0表示获取调用进程的SID
pid_t   tcgetpgrp(int fd);              // 返回TPGID
int     setpgid(pid_t pid, pid_t pgid); // 返回0。pid==0表示设置调用进程；pgid==0表示设置PGID为PID；只能设置调用进程及其子进程。
int     setsid(void);                   // 返回新SID。调用进程不能是进程组组长
int     tcsetpgrp(int fd, pid_t pgid);  // 返回0。pgid必须属于同会话。若由后台进程组调用且其未忽略或阻塞SIGTTOU，则会发送SIGTTOU给该后台进程组

#include <termios.h>
pid_t   tcgetsid(int fd);               // 返回SID

#include <unistd.h>
char*   getlogin(void);                 // 返回登录名。原理是通过ttyname(STDIN_FILENO)后再与utmp日志对比
uid_t   getuid(void);                   // 返回UID
uid_t   geteuid(void);                  // 返回EUID
gid_t   getgid(void);                   // 返回GID
gid_t   getegid(void);                  // 返回EGID

ncsearch-nohlsearch)/*
 * 超级用户调用setuid()会更改UID、EUID、SUID，普通用户调用则只更改UID，且只能更改为UID、EUID、SUID之一
 * 超级用户调用seteuid()只更改EUID；普通用户调用则也只更改EUID，且只能更改为UID、EUID、SUID之一
 * setgid()与setegid()同理
*/
int     setuid(uid_t uid);              // 返回0
int     setgid(gid_t gid);              // 返回0
int     seteuid(uid_t uid);             // 返回0
int     setegid(gid_t gid);             // 返回0
```
<!-- entry end -->

## 进程控制
<!-- entry begin: fork exec wait -->
```c
#include <unistd.h>
pid_t   fork(void);                                                         // 若为父进程则返回子进程PID，若为子进程则返回0
int     execl(const char* pathname, const char* arg0, ..., NULL);           // 若成功则不返回
int     execlp(const char* pathname, const char* arg0, ..., NULL);          // 若成功则不返回
int     execle(const char* pathname, const char* arg0, ..., NULL, envp[]);  // 若成功则不返回
int     execv(const char* filename, char* const argv[]);                    // 若成功则不返回
int     execvp(const char* filename, char* const argv[]);                   // 若成功则不返回
int     execve(const char* filename, char* const argv[], char*const envp[]);// 若成功则不返回
int     fexecve(int fd, char* const argv[], char*const envp[]);             // 若成功则不返回
void    _exit(int status);                                                  // 以status作为main返回值退出进程。立即进入内核

#include <stdlib.h>
void    exit(int status);                                                   // 以status作为main返回值退出进程
int     atexit(void (*func)(void));                                         // 返回0，若出错返回非0。只在main返回与调用exit时有效，而由signal终止无效

#include <sys/wait.h>   // 注意wait函数与SIGCHLD并无关联
pid_t   wait(int* status);                                                  // 返回等待进程PID，若出错返回0或-1
pid_t   waitpid(pid_t pid, int* status, int options);                       // 返回等待进程PID，若出错返回0或-1，其他情况返回对应options
int     waitid(idtype_t idtype, id_t id, siginfo_t* infop, int options);    // 返回0

bool    WIFEXITED(int status);          // 返回bool表示是否正常退出
bool    WIFSIGNALED(int status);        // 返回bool表示是否被信号终止
bool    WIFSTOPPED(int status);         // 返回bool表示是否为停止状态
bool    WIFCONTINUED(int status);       // 返回bool表示是否停止后又继续状态
int     WEXITSTATUS(int status);        // 返回退出码
int     WTERMSIG(int status);           // 返回终止信号
int     WSTOPSIG(int status);           // 返回停止信号
int     WCOREDUMP(int status);          // 若产生core则返回非0
```
<!-- entry end -->

<!-- entry begin: wait waitpid waitid -->
| waitpid pid | 说明                                  |
|-------------|---------------------------------------|
| `pid > 0`   | 等待指定子进程                        |
| `pid < -1`  | 等待PGID为pid绝对值的进程组中的子进程 |
| `pid == 0`  | 等待同进程组中的子进程                |
| `pid == -1` | 等待任一子进程                        |

| waitpid options | 说明                                   |
|-----------------|----------------------------------------|
| WNOHANG         | 子进程状态若非立即可用，则直接返回     |
| WUNTRACED       | 任一子进程处于停止状态，且尚未报告状态 |
| WCONTINUED      | 任一子进程停止后已继续，且尚未报告状态 |

| waitid idtype | 说明                   |
|---------------|------------------------|
| P_PID         | 指定id表示PID          |
| P_PGID        | 指定id表示PGID         |
| P_ALL         | 忽略id并等待任一子进程 |

| waitid options | 说明                               |
|----------------|------------------------------------|
| WEXITED        | 等待进程正常退出或信号终止         |
| WSTOPPED       | 等待进程停止                       |
| WCONTINUED     | 等待进程停止后继续                 |
| WNOHANG        | 子进程状态若非立即可用，则直接返回 |
| WNOWAIT        | 不回收子进程退出状态               |
<!-- entry end -->

## 线程
<!-- entry begin: pthread_exit pthread_cancel exit ateixt pthread_cleanup_push pthread_cleanup_pop -->
进程终止：（导致所有线程终止）
* main函数返回
    > 调用析构函数，且调用atexit注册的函数
* 调用exit()
    > 不调用析构函数，但调用atexit注册的函数
* 调用_exit()或quick_exit()
    > 不调用析构函数，且不调用atexit注册的函数
* 终止信号的默认处理
    > 不调用析构函数，且不调用atexit注册的函数

线程终止：（导致单个线程终止）
* 线程函数返回
    > 不调用pthread_cleanup_push()注册的函数
* 调用pthread_exit()
    > 调用pthread_cleanup_push()注册的函数
* 调用pthread_cancle()
    > 调用pthread_cleanup_push()注册的函数
> 以非0实参调用pthread_cleanup_pop()时，也会调用pthread_cleanup_push()注册的函数
<!-- entry end -->

<!-- entry begin: pthread_equal pthread_self pthread_create pthread_eixt pthread_cleanup_push pthread_cleanup_pop pthread_join pthread_detach pthread_atfork -->
```c
#include <pthread.h>
int         pthread_equal(pthread_t tid1, pthread_t tid2);                          // 返回0
pthread_t   pthread_self(void);                                                     // 返回调用线程TID
int         pthread_create(                                                         // 返回0，若出错返回errno。注意tidp的初始化是异步的
    pthread_t* tidp,
    const pthread_attr_t* attr,
    void* (*start_rtn)(void*),
    void* arg
);
void        pthread_exit(void* retv_ptr);                                           // 若由主线程调用，则会等待其他所有线程终止再退出
int         pthread_cancle(pthread_t tid);                                          // 返回0，若出错返回errno
void        pthread_cleanup_push(void (*rtn)(void*), void* arg);
void        pthread_cleanup_pop(int execute);                                       // 若用非0实参调用则会调用弹出的函数
void        pthread_join(pthread_t tid, void** retv_ptr);                           // 若线程被取消则返回retv_ptr指向PTHREAD_CANCELED
int         pthread_detach(pthread_t tid);                                          // 卸离线程无需调用pthread_detach即自动释放资源

int         pthread_attr_init(pthread_attr_t* attr);
int         pthread_attr_destroy(pthread_attr_t* attr);
int         pthread_attr_getdetachstate(const pthread_attr_t *, int* detachstate);
int         pthread_attr_setdetachstate(pthread_attr_t *, int* detachstate);        // PTHREAD_CREATE_DETACHED、PTHREAD_CREATE_JOINABLE
int         pthread_setcancelstate(int state, int *oldstate);                       // PTHREAD_CANCEL_ENABLE、PTHREAD_CANCEL_DISABLE
int         pthread_setcanceltype(int type, int *oldtype);                          // PTHREAD_CANCLE_ASYNCHRONOUS、PTHREAD_CANCEL_DEFERRED
void        pthread_testcancel(void);                                               // 手动产生cancel点

/* 
 * fork时继承父进程的内存，故也继承了互斥锁与条件量。
 * 但是fork后子进程只有一个调用线程的副本，而父进程中其他线程并未fork。
 * 若子进程继承而来的锁已被上锁，且fork的线程并未持有锁，则可能造成死锁。
 * 所以需要调用pthread_atfork函数再fork时将锁释放。
*/
int         pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
```
<!-- entry end -->

## 信号处理
<!-- entry begin: signal_names strsignal sigemptyset sigfillset sigaddset sigdelset sigismember kill raise sigqueue pthread_kill sigqueue pthread_kill sigpending sigprocmask pthread_sigmask signal sigaction -->
对于SIGCONT：
* 处理停止信号（SIGTSTP、SIGSTOP、SIGTTIN、SIGTTOU）时，丢弃未决决的SIGCONT。反之亦然。
* 但若发射信号为SIGCONT则可发射给同会话中任一进程而无视权限

安全处理信号：
* `volatile sig_atomic_t`设置标识即返回
* 保存和恢复errno
* 只调用异步安全函数
* 阻塞所有信号
* 多次处理不排队的信号
```c
#include <bash/signames.h>                                              // 该头文件依赖signal.h
char*   signal_names[NSIG + 4];                                         // 信号名数组

#include <string.h>
char*   strsignal(int signo);                                           // 返回解释该信号的字符串

#include <signal.h>
void    (*signal(int signo, void (*func)(int)))(int);                   // 返回之前的Handler
int     sigaction(int signo, const sigaction* act, sigaction* oldact);  // 返回0

int     kill(pid_t pid, int signo);                                     // 返回0。普通用户只能发送给UID或EUID等于其UID或EUID的进程
int     raise(int signo);                                               // 返回0
int     sigqueue(pid_t pid, int signo, const union sigval value);       // 返回0
int     pthread_kill(pthread_t tid, int signo);                         // 返回0，错误返回errno

int     sigemptyset(sigset_t* set);                                     // 返回0
int     sigfillset(sigset_t* set);                                      // 返回0
int     sigaddset(sigset_t* set, int signo);                            // 返回0
int     sigdelset(sigset_t* set, int signo);                            // 返回0
int     sigismember(const sigset_t* set, int signo);                    // 若signo再set中则返回非0
int     sigpending(sigset_t* set);                                      // 返回0
int     sigprocmask(int how, const sigset_t* set, sigset_t* oldset);    // 返回0。how可以为SIG_BLOCK、SIG_UNBLOCK、SIG_SETMASK之一
int     pthread_sigmask(int how, const sigset_t* set, sigset_t* oleset);// 返回0，错误返回errno

/* 其他与信号有关的函数 */
int             sigwait(const sigset_t* set, int* signop);              // 返回0
int             sigsuspend(const sigset_t* sigmask);                    // 返回0

#include <unistd.h>
int             pause(void)                                             // 返回-1且errno置为EINTR。只有执行信号处理返回时返回
unsigned int    sleep(unsigned int seconds);                            // 返回未休眠的秒数。超时或信号处理返回时返回
unsigned int    alarm(unsigned int seconds);                            // 返回闹钟剩余秒数。覆盖之前注册的闹钟

#include <stdlib.h>
void            abort(void)                                             // 发送SIGABRT给调用进程，处理函数返回则直接终止
int             system(const char* cmdstring);                          // 返回shell命令行返回值（若shell的子进程被信号终止，则返回128+SIGNAL）。
                                                                        // 同步调用，且期间阻塞SIGINT, SIGQUIT, SIGCHLD
```
<!-- entry end -->

<!-- entry begin: kill -->
| kill pid    | 说明                                        |
|-------------|---------------------------------------------|
| `pid > 0`   | 发送给指定进程                              |
| `pid < 0`   | 发送给PGID等于pid绝对值的进程组中的所有进程 |
| `pid == 0`  | 发送给同进程组所有有权限发送的进程          |
| `pid == -1` | 发送给所有有权限发送的进程                  |
<!-- entry end -->

<!-- entry begin: sigaction -->
```c
#include <signal.h>
struct sigaction
{
    void        (*sa_handler)(int signo);
    sigset_t    sa_mask;
    int         sa_flag;
    void        (*sa_sigaction)(int signo, siginfo_t* info, void* context);
}
struct siginfo_t
{
    int     si_signo;   // 信号值
    int     si_errno;   // 错误码
    int     si_code;    // 信号详细信息
    pid_t   si_pid;     // 发射进程PID
    uid_t   si_uid;     // 发射进程UID
    void*   si_addr;    // 发生故障的地址
    int     si_status;  // 退出码或信号值
    sigval  si_value;   // 应用程序指定信息。union sigval{int sival_int; void* sival_ptr;};
};
```
| sigaction sa_flag | 说明                                                                               |
|-------------------|------------------------------------------------------------------------------------|
| SA_SIGINFO        | 使用sa_sigaction代替sa_handler                                                     |
| SA_RESTART        | 自动重启中断的系统调用                                                             |
| SA_INTERRUPT      | 不自动重启中断的系统调用（默认重启）                                               |
| SA_RESETHAND      | 自动重置信号处理        （默认不重置）                                             |
| SA_NODEFER        | 不自动阻塞相同信号      （默认阻塞）                                               |
| SA_NOCLDSTOP      | 若signo为SIGCHLD则设置只在子进程终止而非停止时发送该信号                           |
| SA_NOCLDWAIT      | 若signo为SIGCHLD则设置不创建僵尸进程，若调用进程随后调用wait则需等待所有子进程终止 |
<!-- entry end -->

## 资源限制
<!-- entry begin: getrlimit setrlimit nice getpriority setpriority -->
```c
#include <sys/resource.h>
struct rlimit
{
    rlimit_t rlim_cur;  // 当前限制。可调整范围`[0, rlim_max]`
    rlimit_t rlim_max;  // 最大限制。可调整范围`[rlim_cur, RLIM_INFINITY]`，普通用户只能调小，超级用户才能调大
};
int     getrlimit(int resource, rlimit* rlptr);         // 返回0，出错返回非0
int     setrlimit(int resource, const rlimit* rlptr);   // 返回0，出错返回非0

/* Linux上，nice范围[-20, 19]，其中普通用户只能设置非负数且只能调大 */
#include <unistd.h>
int     nice(int incr);                                 // 返回新友好值。自动调节incr到范围内的值

#include <sys/resource.h>
int     getpriority(int which, id_t who);               // 返回友好值。若作用于多个进程，则返回优先级最高到（nice最小的）
int     setpriority(int which, id_t who, int value);    // 返回0。
```
| rlimit resource   | 说明                            |
|-------------------|---------------------------------|
| RLIMIT_AS         | 进程虚拟内存最大长度            |
| RLIMIT_MEMLOCK    | 进程调用mlock()能锁住的最大内存 |
| RLIMIT_RSS        | 进程最大驻留内存长度            |
| RLIMIT_DATA       | 进程数据段最大长度              |
| RLIMIT_STACK      | 进程栈的最大值                  |
| RLIMIT_FSIZE      | 进程写入文件的最大长度          |
| RLIMIT_CORE       | 进程core文件最大长度            |
| RLIMIT_MSGQUEUE   | 进程POSIX消息队列最大长度       |
| RLIMIT_SIGPENDING | 进程可排队信号的最大数量        |
| RLIMIT_NOFILE     | 进程同时打开文件最大数量        |
| RLIMIT_CPU        | 进程CPU时间最大秒数             |
| RLIMIT_NPROC      | 实际用户可拥有的最大进程数      |

| setpriority which | 说明                          |
|-------------------|-------------------------------|
| PRIO_PROCESS      | 指定who为PID（0代表调用进程） |
| PRIO_PGRP         | 指定who为PGID                 |
| PRIO_USER         | 指定who为SID                  |
<!-- entry end -->

# 虚拟文件系统
## 文件系统结构
### 硬链接
<!-- entry begin: link linkat unlink unlinkat remove rename frenameat -->
```c
#include <unistd.h>
int link(const char* existingpath, const char* newpath);                                    // 返回0。若epath为符号链接则创建符号链接
int linkat(int efd, const char* existingpath, int nfd, const char* newpath, int flag);      // 返回0。若epath为符号链接则创建符号链接；支持flag=AT_SYMLINK_FOLLOW表示强制创建硬链接
int unlink(const char* pathname);                                                           // 返回0。不跟随符号链接
int unlinkat(int fd, const char* pathname, int flag);                                       // 返回0。不跟随符号链接，支持flag=AT_REMOVEDIR表示删除空目录
int remove(const char* pathname);                                                           // 返回0。不跟随符号链接
int rename(const char* oldname, const char* newname);                                       // 返回0。不跟随符号链接
int frenameat(int oldfd, const char* pathname, int newfd, const char* newname);             // 返回0。不跟随符号链接
```
<!-- entry end -->

### 文件信息
<!-- entry begin: stat lstat fstat fstatat ft -->
```c
#include <sys/stat.h>
struct stat
{
    mode_t          st_mode;        // 文件类型及权限
    ino_t           st_ino;         // inode
    dev_t           st_dev;         // 文件系统的设备号
    dev_t           st_rdev;        // 特殊文件的设备号
    nlink_t         st_nlink;       // 硬链接数
    uid_t           st_uid;         // UID
    gid_t           st_gid;         // GID
    off_t           st_size;        // 字节长度（只对普通文件、目录、符号链接有效）
    timespec        st_atime;       // atime
    timespec        st_mtime;       // mtime
    timespec        st_ctime;       // ctime
    blksize_t       st_nlksize;     // 最优I/O块大小
    blkcnt_t        st_blocks;      // 占用磁盘块数量（块大小为S_BLKSIZE）
};

int     stat(const char* pathname, stat* buf);                          // 返回0
int     lstat(const char* pathname, stat* buf);                         // 返回0。不跟随符号链接
int     fstat(int fd, stat* buf);                                       // 返回0
int     fstatat(int fd, const char* pathname, stat* buf, int flag);     // 返回0

bool    S_ISREG(mode_t st_mode);
bool    S_ISDIR(mode_t st_mode);
bool    S_ISLNK(mode_t st_mode);
bool    S_ISFIFO(mode_t st_mode);
bool    S_ISSOCK(mode_t st_mode);
bool    S_ISBLK(mode_t st_mode);
bool    S_ISCHR(mode_t st_mode);
```
<!-- entry end -->

## 文件日期
<!-- entry begin: utimes futimens utimesat -->
```c
#include <sys/time.h>
int utimes(const char* pathname, const struct timeval times[2]);                         // 返回0
int futimens(int fd, const struct timespec times[2]);                                    // 返回0
int utimensat(int fd, const char* pathname, const struct timespec times[2], int flag);   // 返回0

struct timeval  { time_t tv_sec; long tv_usec; };
struct timespec { time_t tv_sec; long tv_nsec; };
```
times[2]说明：
* times[0]表示atime，times[1]表示mtime
* 若times为空指针，则表示设置为当前时间。需要权限：写权限 | onwer | root
* 若times非空指针，且任一`tv_nsec`为`UTIME_NOW`，则设置为当前时间。需要权限：写权限 | onwer | root
* 若times非空指针，且任一`tv_nsec`为`UTIME_OMIT`，则保持时间不变。需要权限：无
* 若times非空指针，且无一`tv_nsec`为`UTIME_OMIT`或`UTIME_NOW`，则设置为指定时间。需要权限：owner | root
<!-- entry end -->

## 权限
<!-- entry begin: access faccessat umask chmod fchmod fchmodat chown lchown fchown fchowat -->
```c
#include <unistd.h>
int     access(const char* pathname, int mode);                                     // 返回0。按照实际的UID与GID进行权限判断
int     faccessat(int fd, const char* pathname, int tmode, int flag);               // 返回0。支持flag=AT_EACCESS表示用EUID与EGID进行权限判断

#include <sys/stat.h>
mode_t  umask(mode_t mode);                                                         // 返回之前umask。进程独立；不限制chmod函数
int     chmod(const char* pathname, mode_t mode);                                   // 返回0
int     fchmod(int fd, mode_t mode);                                                // 返回0
int     fchmodat(int fd, const char* pathname, mode_t mode, int flag);              // 返回0

#include <unistd.h>
int     chown(const char* pathname, uid_t owner, gid_t group);                      // 返回0。owner或group为-1表示不修改
int     lchown(const char* pathname, uid_t owner, gid_t group);                     // 返回0。不跟随符号链接
int     fchown(int fd, uid_t owner, gid_t group);                                   // 返回0
int     fchownat(int fd, const char* pathname, uid_t owner, gid_t group, int flag); // 返回0
```
<!-- entry end -->

<!-- entry begin: access mode -->
| access mode | 说明           |
|-------------|----------------|
| F_OK        | 文件是否存在   |
| R_OK        | 文件是否可读   |
| W_OK        | 文件是否可写   |
| X_OK        | 文件是否可执行 |
<!-- entry end -->

<!-- entry begin: chmod mode -->
| chmod mode | 说明 |
|------------|------|
| S_ISUID    | 4000 |
| S_ISGID    | 2000 |
| S_ISVTX    | 1000 |
| S_IRWXU    | 0700 |
| S_IRUSR    | 0600 |
| S_IWUSR    | 0400 |
| S_IXUSR    | 0100 |
| S_IRWXG    | 0070 |
| S_IRGRP    | 0060 |
| S_IWGRP    | 0040 |
| S_IXGRP    | 0010 |
| S_IRWXO    | 0007 |
| S_IROTH    | 0006 |
| S_IWOTH    | 0004 |
| S_IXOTH    | 0001 |
<!-- entry end -->

## 文件类型
### 目录
<!-- entry begin: mkdir mkdirat rmdir chdir fchdir getcwd chroot -->
```c
#include <stdio.h>
char*   mkdtemp(char* template);                                // 返回临时目录名字符串，若出错则返回NULL

#include <sys/stat.h>
int     mkdir(const char* pathname, mode_t mode);               // 返回0。不自动建立不存在的目录
int     mkdirat(int fd, const char* pathname, mode_t mode);     // 返回0。不自动建立不存在的目录

#include <unistd.h>
int     rmdir(const char* pathname);                            // 返回0
int     chdir(const char* pathname);                            // 返回0
int     fchdir(int fd);                                         // 返回0
char*   getcwd(char* buf, size_t size);                         // 返回工作目录的真实绝对路径。原理即通过`..`层层向上递归到根来获取
int     chroot(const char* pathname);                           // 返回0。切换RTD（默认为系统`/`），只能由root调用，调用后该进程及其子进程则再无法恢复`/`了（因为隔离后根本无法指定原来的目录）
```
<!-- entry end -->

### 符号链接
<!-- entry begin: symlink readlink -->
```c
#include <unistd.h>
int     symlink(const char* actualpath, const char* sympath);                   // 返回0
int     symlinkat(const char* actualpath,int fd, const char* sympath);          // 返回0
ssize_t readlink(const char* pathname, char* buf, size_t bufsize);              // 返回读取字节数
ssize_t readlinkat(int fd, const char* pathname, char* buf, size_t bufsize);    // 返回读取字节数
```
<!-- entry end -->

### 设备文件
<!-- entry begin: major minor -->
```c
#include <sys/sysmacros.h>
unsigned int    major(dev_t st_dev);
unsigned int    minor(dev_t st_dev);
unsigned int    major(dev_t st_rdev);
unsigned int    minor(dev_t st_rdev);
```
<!-- entry end -->
### 普通文件
<!-- entry begin: open openat creat fcntl close dup dup2 lseek read write pread pwrite truncate ftruncate sync fsync fdatasync -->
| 限制宏       | 说明                                                    |
|--------------|---------------------------------------------------------|
| _PC_OPEN_MAX | 同时打开文件的最大数量，即文件描述符范围[0, OPEN_MAX-1] |
| _PC_NAME_MAX | 文件名最大长度                                          |
| _PC_PATH_MAX | 路径名最大长度                                          |
| _PC_NO_TRUNC | 文件名或路径名超出限制是否直接截断而非出错              |

```c
#include <stdio.h>
int     mkstemp(char* template);                                        // 返回临时文件的文件描述符
int     fileno(FILE* file);                                             // 返回流相关的文件描述符(NOE)

#include <fcntl.h>
int     open(const char* path, int oflag, mode_t mode...);              // 返回文件描述符
int     openat(int fd, const char* path, int oflag, mode_t mode);       // 返回文件描述符
int     creat(const char* path, mode_t mode);                           // 相当于open(path, O_WRONLY|O_CREAT|O_TRUNC, mode)
int     fcntl(int fd, int cmd, int arg...);                             // 返回值依赖cmd

#include <unistd.h>
int     close(int fd);                                                  // 返回0
int     dup(int fd);                                                    // 返回新文件描述符（最小未占用）。清除FD_CLOEXEC
int     dup2(int fd, int fd2);                                          // 返回新文件描述符（指定描述符）。清除FD_CLOEXEC；原子操作(close;dup)
off_t   lseek(int fd, off_t offset, int whence);                        // 返回新偏移量。对管道、套接字调用会出错；偏移量可能为负值，故不应把负的返回值当出错
ssize_t read(int fd, void* buf, size_t nbytes);                         // 返回读取的字节数，若遇EOF返回0
ssize_t write(int fd, const void* buf, size_t nbytes);                  // 返回已写的字节数
ssize_t pread(int fd, void* buf, size_t nbytes, off_t offset);          // 返回读取的字节数，若遇EOF返回0。原子操作(lseek;read)且不影响文件偏移量
ssize_t pwrite(int fd, const void* buf, size_t nbytes, off_t offset);   // 返回已写的字节数。原子操作(lseek;write)且不影响文件偏移量
int     truncate(const char* pathname, off_t length);                   // 返回0
int     ftruncate(int fd, off_t length);                                // 返回0
void    sync(void);                                                     // 同步冲刷系统中所有文件缓冲区块
int     fsync(int fd);                                                  // 返回0。同步冲刷指定文件
int     fdatasync(int fd);                                              // 返回0。同步冲刷指定文件的数据
```
<!-- entry end -->

<!-- entry begin: open oflag -->
| open flag   | 说明                                                                                  |
|-------------|---------------------------------------------------------------------------------------|
| O_RDONLY    | 只读模式                                                                              |
| O_WRONLY    | 只写模式                                                                              |
| O_RDWR      | 读写模式                                                                              |
| O_EXEC      | 执行模式                                                                              |
| O_TRUNC     | 直接清空文件内容                                                                      |
| O_APPEND    | 每次写时自动原子性调整文件偏移量到文件末尾，但不会自动恢复之前偏移量                  |
| O_CLOEXEC   | 置位文件描述符标识位FD_CLOEXEC                                                        |
| O_CREAT     | 若文件不存在则自动创建，需要额外实参mode指定权限位                                    |
| O_EXCL      | 若文件存在则出错。若同时指定O_CREAT且目标文件为符号连接则报错。原子操作(judge;create) |
| O_NOFOLLOW  | 若为符号链接则出错                                                                    |
| O_DIRECTORY | 若不为目录则出错                                                                      |
| O_SYNC      | 数据与属性同步写入                                                                    |
| O_DSYNC     | 数据同步写入                                                                          |
| O_NONBLOCK  | 非阻塞I/O，用于低速I/O与记录锁                                                        |
| O_NOCTTY    | 会话首进程打开第一个尚未与会话关联的终端时，只要未使用该标识，则将该会话与终端关联    |
| O_TTY_INIT  | 若打开一个还未打开的终端设备，设置非标准termios参数值                                 |
<!-- entry end -->

<!-- entry begin: fcntl cmd flock -->
| fcntl cmd       | 说明                                                                                          |
|-----------------|-----------------------------------------------------------------------------------------------|
| F_DUPFD         | 返回新描述符。指定arg为期望新描述符。复制文件描述符，并清除FD_CLOEXEC                         |
| F_DUPFD_CLOEXEC | 返回新描述符。指定arg为期望新描述符。复制文件描述符，并置位FD_CLOEXEC                         |
| F_GETFD         | 返回文件描述符标识                                                                            |
| F_SETFD         | 返回0。指定arg为文件描述符标识。设置文件描述符标识                                            |
| F_GETFL         | 返回文件打开标识。4个互斥的模式位需用掩码O_ACCMODE读取                                        |
| F_SETFL         | 返回0。指定arg为文件打开标识。设置文件打开标识。可以更改O_APPEND、O_NONBLOCK、O_SYNC、O_DSYNC |
| F_GETLK         | 若检查记录锁可用返回0，否则返回-1。指定arg为`flock*`锁                                        |
| F_SETLK         | 返回0。获取记录锁（非阻塞）                                                                   |
| F_SETLKW        | 返回0。获取记录锁（阻塞）                                                                     |
```c
#include <fcntl.h>
struct flock
{
    short   l_type;     // F_RDLCK, F_WRLCK, F_UNLCK
    short   l_whence;   // SEEK_SET, SEEK_CUR, SEEK_END
    off_t   l_start;    // 锁的起始位置
    off_t   l_len;      // 锁的字节长度。0表示锁住当前字节直到EOF
    pid_t   l_pid;      // 存储F_GETLK返回锁的拥有者PID
};
```
<!-- entry end -->

<!-- entry begin: lseek whence -->
| whence   | 说明       |
|----------|------------|
| SEEK_SET | 文件开始处 |
| SEEK_CUR | 当前位置   |
| SEEK_END | 文件结尾处 |
<!-- entry end -->

### 管道
<!-- entry begin: pipe popen pclose -->
| 限制         | 说明                                                 |
|--------------|------------------------------------------------------|
| _PC_PIPE_BUF | 内核管道缓冲队列的大小，即能原子性写入的最大字节长度 |
```c
/* 注意：使用两个管道与协同进程交流时，使用标准I/O可能会因为全缓冲而导致死锁（主进程没写完就开始读） */
#include <unistd.h>
int     pipe(int fd[2]);                                // 返回0。返回的fd[0]用于读，fd[1]用于写

#include <stdio.h>
FILE*   popen(const char* shellcmd, const char* type);  // 返回管道的标准流。type指定读写模式（"r"或"w"）
int     pclose(FILE* fp);                               // 返回shell返回值。pclose会调用waitpid给popen启动的子进程收尸，若用户在pclose之前调用wait则会出错
```
<!-- entry end -->

### 套接字
<!-- entry begin: addrinfo getaddrinfo getnameinfo freeaddrinfo gai_strerrot -->
```c
#include <sys/socket.h>
#include <netdb.h>
struct addrinfo
{
    int         ai_flags;           // 见下表addrinfo ai_flags
    int         ai_familt;          // 指定为domain
    int         ai_socktype;        // 指定为socktype
    int         ai_protocol;        // 指定为protocol
    socklen_t   ai_addrlen;         // 指定sockaddr结构长度
    sockaddr*   ai_addr;            // 因历史缘故，未使用void* 取代sockaddr*
    char*       ai_canonname;       // 规范主机名
    addrinfo*   ai_next;            // 链表下个元素指针
};
int getaddrinfo(                    // 若成功返回0，若出错返回errno
    const char* host,               // 指定主机名
    const char* service,            // 指定服务名
    const addrinfo* hint,           // 可指定hist中ai_flag、ai_family、ai_socktype域ai_protocol字段
    addrinfo** res                  // 返回请求的addrinfo*的指针（链表）
);
int getnameinfo(                    // 若成功返回0，若出错返回非0
    const sockaddr* addr,           // 指定需要翻译的地址信息的指针
    socklen_t alen,                 // 指定地址信息长度
    char* host,                     // 返回主机名
    socklen_t hostlen,              // 指定主机名最大长度
    char* service,                  // 返回服务名
    socklen_t servlen,              // 指定服务名最大长度
    int flag                        // 见下表getnameinfo flag
);
void freeaddrinfo(addrinfo* ai);    // 指定ai为getaddrinfo返回的res用于销毁
const char* gai_strerrot(int errno);// 返回出错信息字符串。指定errno为getaddrinfo返回的错误码
```
| addrinfo ai_flags | 说明                                           |
|-------------------|------------------------------------------------|
| AI_ADDRCONFIG     | 当存在IPv4地址时才返回IPv4，IPv6同理           |
| AI_V4MAPPED       | 若为IPv6地址则返回映射到IPv6格式的IPv4地址     |
| AI_ALL            | 查找IPv4和IPv6（仅用于AI_V4MAPPED）            |
| AI_PASSIVE        | 若host为NULL则res.ai_addr表示监听所有本地网口  |
| AI_CANONNAME      | 返回规范主机名到res.ai_canonname（默认为NULL） |
| AI_NUMERICHOST    | 强制指定主机名用点分制或冒分制                 |
| AI_NUMERICSERV    | 强制指定服务名用数字端口号                     |

| getnameinfo flag | 说明                                     |
|------------------|------------------------------------------|
| NI_DGRAM         | 服务基于数据报而非流                     |
| NI_NAMEREQD      | 若找不到主机名则出错                     |
| NI_NOFQDN        | 若为本地主机则返回全限定域名的节点名部分 |
| NI_NUMERICSCOPE  | 返回IPv6冒分制的主机名而非域名           |
| NI_NUMERICHOST   | 返回IPv4点分制的主机名而非域名           |
| NI_NUMERICSERV   | 返回端口号而非服务名                     |
<!-- entry end -->

<!-- entry begin: socket bind listen accept connect send sendto recv recvfrom shutdown sockmark -->
```
int     socket(int domain, int socktype, int protocol);                                                     // 返回创建的套接字的描述符
int     bind(int sockfd, const sockaddr* addr, socklen_t len);                                              // 返回0
int     listen(int sockfd, int backlog);                                                                    // 返回0
int     accept(int sockfd, sockaddr* addr, socklen_t* len);                                                 // 返回连接的套接字描述符
int     connect(int sockfd, const sockaddr*, socklen_t len);                                                // 返回0。为了可移植性，当connect失败时需要close(sockfd)然后重新创建
ssize_t send(int sockfd, const void* buf, size_t nbytes, int flags);                                        // 返回发送的字节数
ssize_t sendto(int sockfd, const void* buf, size_t nbytes, int flags, const sockaddr* addr, socklen_t alen);// 若成功返回发送的字节数
ssize_t recv(int sockfd, void* buf, size_t nbytes, int flags);                                              // 返回接收的字节数
ssize_t recvfrom(int sockfd, void* buf, size_t nbytes, int flags, sockaddr* addr, socklen_t* alen);         // 返回接收的字节数
int     shutdown(int sockfd, int how);                                                                      // 返回0。how可为SHUT_RD、SHUT_WR、SHUT_WR之一
int     sockmark(int sockfd);       // 若下个读取字节为外带数据返回1，若不是则返回0，若出错返回-1。 注意下一个外带数据到来时会丢弃当前的
```
套接字接口调用流程：
| 函数           | 网络服务端 | 网络客户端 | UNIX域服务端 | UNIX域客户端 |
|----------------|------------|------------|--------------|--------------|
| socket         | 1          | 1          | 1            | 1            |
| bind           | 1          | 1/0        | 1            | 0            |
| listen         | 1          | 0          | 1            | 0            |
| accept         | 1          | 0          | 1            | 0            |
| connect        | 0          | 1          | 0            | 1            |
| send           | 1          | 1          | 1            | 1            |
| recv           | 1          | 1          | 1            | 1            |
| shutdown/close | 1          | 1          | 1            | 1            |

| domain    | 说明         |
|-----------|--------------|
| AF_INET   | IPv4因特网域 |
| AF_INET6  | IPv6因特网域 |
| AF_UNIX   | UNIX域       |
| AF_UNSPEC | 未指定       |

| socktype       | 说明                                         |
|----------------|----------------------------------------------|
| SOCK_DGRAM     | 无连接的、不可靠的、固定长度的报文传递       |
| SOCK_STREAM    | 面向连接的、可靠的、连续的流传递             |
| SOCK_SEQPACKET | 面向连接的、可靠的、固定长度的报文传递       |
| SOCK_RAW       | 跳过传输层而直接域网络层交互（需要超级权限） |

| protocol     | 说明       |
|--------------|------------|
| IPPROTO_IP   | IPv4       |
| IPPROTO_IPV6 | IPv6       |
| IPPROTO_ICMP | ICMP       |
| IPPROTO_TCP  | TCP        |
| IPPROTO_UDP  | UDP        |
| IPPROTO_RAW  | 跳过传输层 |

| send flag     | 说明                             |
|---------------|----------------------------------|
| MSG_CONFIRM   | 提供链路层反馈以保持地址映射有效 |
| MSG_DONTROUTE | 勿将数据包路由出本地网络         |
| MSG_DONTWAIT  | 非阻塞操作（等价O_NONBLOCK）     |
| MSG_EOR       | 若协议支持，标记记录结束         |
| MSG_OOB       | 若协议支持，发送外带数据         |
| MSG_MORE      | 延迟发送数据包允许写更多数据     |
| MSG_NOSIGNAL  | 写无连接的套接字不产生SIGPIPE    |

| recv flag        | 说明                                                      |
|------------------|-----------------------------------------------------------|
| MSG_CMSG_CLOEXEC | 为UNIX域套接字上接收的描述符设置执行时关闭标识            |
| MSG_DONTWAIT     | 非阻塞操作（等价O_NONBLOCK）                              |
| MSG_ERRQUEUE     | 接收错误信息作为辅助数据                                  |
| MSG_OOB          | 若协议支持，发送外带数据（当数据长于1字节时的最后一字节） |
| MSG_PEEK         | 返回数据包内容但不取走                                    |
| MSG_TRUNC        | 即使数据包被截断页返回实际长度                            |
| MSG_WAITALL      | 等到所有数据可用时（仅SOCK_STREAM）                       |
<!-- entry end -->

### 终端
<!-- entry begin: tcgetattr tcsetattr cfgetispeed cfgetospeed cfsetispeed cfsetospeed termios ctermid isatty ttyname winsize -->
```c
#include <termios.h>
struct termios
{
    tcflag_t    c_iflag;    // 见下表c_iflag
    tcflag_t    c_oflag;    // 见下表c_oflag
    tcflag_t    c_cflag;    // 见下表c_cflag
    tcflag_t    c_lflag;    // 见下表c_lflag
    cc_t        c_cc[NCCS]; // 见下表c_cc，利用下标将对应元素设置为_POSIX_VDISABLE可禁用该特殊字符
};
int     tcgetattr(int fd, termios* termptr);                // 返回0
int     tcsetattr(int fd, int opt, const termios* termptr); // 返回0
speed_t cfgetispeed(const termios* termptr);                // 返回0
speed_t cfgetospeed(const termios* termptr);                // 返回0
int     cfsetispeed(termios* termptr, speed_t speed);       // 返回0
int     cfsetospeed(termios* termptr, speed_t speed);       // 返回0

#include <stdio.h>
char*   ctermid(char* buf);                                 // 若成功返回控制终端名字符串(/dev/tty)同时返回到buf[L_ctermid]中，若出错返回NULL

#include <unistd.h>
int     isatty(int fd);                                     // 若为终端设备返回1，否则返回0
char*   ttyname(int fd);                                    // 若成功返回终端路径名字符串，若出错返回NULL

struct winsize                                              // 获取winsize：ioctl(fd, TIOCGWINSZ, winsize*)
{
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;
    unsigned short ws_ypixel;
};
```
<!-- entry end -->

<!-- entry begin: tcsetattr termios stty -->
| tcsetattr opt | 说明                         |
|---------------|------------------------------|
| TCSANOW       | 更改立即发生                 |
| TCSADRAIN     | 发送所有输出后更改才发生     |
| TCASFLUSH     | 同上，且丢弃未读取的输入数据 |

| c_iflag | 说明                                                |
|---------|-----------------------------------------------------|
| BRKINT  | 清空输入输出队列，并发射SIGINT给前台进程组          |
| IGNBRK  | 忽略BREAK条件                                       |
| ICRNL   | 将输入的CR转换为NL                                  |
| IGNCR   | 忽略CR                                              |
| INLCR   | 将输入的NL转换为CR                                  |
| INPCK   | 打开输入的奇偶校验                                  |
| IGNPAR  | 忽略奇偶校验错误                                    |
| PARMRK  | 标记奇偶校验错误                                    |
| ISTRIP  | 剥除输入字符的第8位                                 |
| IUTF8   | utf-8多字节进行字符擦除                             |
| IUCLC   | 将输入的大写字符转换为小写                          |
| IMAXBEL | 输入队列满时响铃                                    |
| IXOFF   | 输入队列填满时发送STOP给发送者，处理完后再发送START |
| IXON    | 接收并解析STOP与START                               |
| IXANY   | 任何字符都重启输出                                  |

| c_oflag | 说明                                  |
|---------|---------------------------------------|
| BSDLY   | BS延迟屏蔽字(BS0, BS1)                |
| TABDLY  | TAB延迟屏蔽字(TAB0, TAB1, TAB2, TAB3) |
| VTDLY   | VT延迟屏蔽字(VT0, VT1)                |
| FFDLY   | FF延迟屏蔽字(FF0, FF1)                |
| CRDLY   | CR延迟屏蔽字(CR0, CR1, CR2, CR3)      |
| NLDLY   | NL延迟屏蔽字(NL0, NL1)                |
| OFILL   | 延迟使用填充符                        |
| OFDEL   | 填充符为DEL而非NUL                    |
| OCRNL   | 将输出的CR映射为NL                    |
| ONLCR   | 将输出的NL映射为CR-NL                 |
| ONLRET  | NL执行CR功能                          |
| ONOCR   | 在0列不输出CR                         |
| OLCUC   | 将输出的小写字符转换为大写            |
| OPOST   | 执行输出处理（解释c_oflag）           |

| c_cflag | 说明                                       |
|---------|--------------------------------------------|
| HUPCL   | 最后一个进程关闭设备时，断开调制解调器连接 |
| CLOCAL  | 忽略调制解调器状态线                       |
| CREAD   | 可以接收字符                               |
| CRTSCTS | 允许带内/外硬件硬件流控制                  |
| PARENB  | 启用奇偶校验                               |
| PAROOD  | 使用奇校验而非偶校验                       |
| CMSPAR  | 若设置PAROOD则奇偶校验位总为1，否则总为0   |
| CSIZE   | 字符大小屏蔽字(CS5, CS6, CS7, CS8)         |
| CSTOPB  | 发送两个停止位而非一个                     |

| c_lflag | 说明                               |
|---------|------------------------------------|
| ECHO    | 启用回显                           |
| ECHOCTL | 回显控制字符为`^X`，`X`为0x40+char |
| ECHOE   | 可视ERASE擦除字符(`\b \b`)         |
| ECHOPRT | 回显提示正在擦除的字符             |
| ECHOK   | 可视KILL擦除字符                   |
| ECHOKE  | 以ECHOE的方式进行擦除              |
| ECHONL  | 即使未设置也回显NL                 |
| FLUSHO  | 冲刷输出                           |
| NOFLSH  | 在中断或退出后不冲刷               |
| PENDIN  | 重新回显未决输入                   |
| XCASE   | 规范大小写表示                     |
| TOSTOP  | 对后台输出发送SIGTTOU              |
| ISIG    | 终端产生信号                       |
| ICANON  | 规范输入（将输入字符装配成行）     |
| IEXTEN  | 扩展输入字符处理                   |

| 特殊字符 | 说明                      | c_cc下标 | 启用标识           | 典型值    |
|----------|---------------------------|----------|--------------------|-----------|
| CR       | 回车（规范行边界）        | 不能更改 | c_lflag ICANON     | `\r`      |
| NL       | 换行 （规范行边界）       | 不能更改 | c_lflag ICANON     | `\n`      |
| EOF      | 文件结束 （规范行边界）   | VEOF     | c_lflag ICANON     | `^D`      |
| EOL      | 行结束 （规范行边界）     | VEOL     | c_lflag ICANON     |           |
| EOL2     | 替换行结束 （规范行边界） | VEOL2    | c_lflag ICANON     |           |
| ERASE    | 向前擦除                  | VERASE   | c_lflag ICANON     | `^H`,`^?` |
| WERASE   | 向前擦除一个字            | VWERASE  | c_lflag ICANON     | `^W`      |
| KILL     | 擦行                      | VKILL    | c_lflag ICANON     | `^U`      |
| REPRINT  | 再打印全部输入            | VREPRINT | c_lflag ICANON     | `^R`      |
| DISCARD  | 丢弃输出                  | VDISCARD | c_lflag IEXTEN     | `^O`      |
| LNEXT    | 下个字符的字面值          | VLNEXT   | c_lflag IEXTEN     | `^V`      |
| INTR     | SIGINT                    | VINTR    | c_lflag ISIG       | `^C`      |
| QUIT     | SIGQUIT                   | VQUIT    | c_lflag ISIG       | `^\`      |
| SUSP     | SIGTSTP                   | VSUSP    | c_lflag ISIG       | `^Z`      |
| STOP     | 停止输出                  | VSTOP    | c_iflag IXON/IXOFF | `^S`      |
| START    | 恢复输出                  | VSTART   | c_iflag IXON/IXOFF | `^Q`      |
| TIME     | 非规范模式定时器          | VTIME    |                    |           |
| MIN      | 非规范模式最小读取字节数  | VMIN     |                    |           |

对于非规范模式的设置：
| 属性      |                    MIN > 0                   |               MIN == 0              |
|-----------|:--------------------------------------------:|:-----------------------------------:|
| TIME > 0  | 若未超时则读取[MIN, n]；若超时则读取[1, MIN] | 若未超时则读取[1, n]；若超时则读取0 |
| TIME == 0 |                 读取[MIN, n]                 |              读取[0, n]             |
<!-- entry end -->

<!-- entry begin: ansi color -->
* 8/16/24 colors
    > `${ID}`：见下表
    * `\033[${ID}m`     (普通)
    * `\033[${ID};1m`   (粗体，高亮)
    * `\033[${ID};2m`   (低暗)

|  color | foreground | background |
|:------:|:----------:|:----------:|
|  black |     30     |     40     |
|   red  |     31     |     41     |
|  green |     32     |     32     |
| yellow |     33     |     33     |
|  blue  |     34     |     34     |
| purple |     35     |     35     |
|  cyan  |     36     |     36     |
|  white |     37     |     37     |

* 256 color
    > `${fg_bg}`：前景为`38`，背景为`48`  
    > `${ID}`：0~255见<https://jonasjacek.github.io/colors/>
    * `\033[${fg_bg};5;${ID}m`

* true color
    > `${fg_bg}`：前景为`38`，背景为`48`  
    > `${red|green|blue}`：见系统调色板
    * `\033[${fg_bg};2;${red};${green};${blue}m`
<!-- entry end -->

<!-- entry begin: ansi escape -->
* ansi_escape
<!--  -->
|        escape       |    meaning   |
|:-------------------:|:------------:|
|       `\e[0m`       |     复原     |
|       `\e[1m`       |     加粗     |
|       `\e[2m`       |     低暗     |
|       `\e[3m`       |     斜体     |
|       `\e[4m`       |    下划线    |
|      `\e[4:3m`      |  下划波浪线  |
|       `\e[5m`       |     闪烁     |
|       `\e[7m`       |     反显     |
|       `\e[8m`       |     消隐     |
|       `\e[9m`       |    删除线    |
|      `\e[?25l`      |   隐藏光标   |
<!-- entry end -->

* * * * * * * * * *

<!-- entry begin: pty posix_openpt grantpt unlockpt ptsname openpty forkpty -->
伪终端用途：
* 协程通讯，协程的输入与输出针对终端会有特殊处理
* 协程通讯，强制协程使用的标准流进程行缓冲（相对管道为全缓冲）
* 终端模拟器
* 远程终端
```c
#include <stdlib.h>
#include <fcntl.h>
int     posix_openpt(int oflag);    // 返回PTY主设备文件描述符。oflag支持O_RDWR与O_NOCTTY
int     grantpt(int fd);            // 返回0。设置对应从设备适当的权限与属主
int     unlockpt(int fd);           // 返回0。允许对应从设备被访问
char*   ptsname(int fd);            // 返回对应从设备名称字符串

#include <pty.h> // 需要链接参数`-lutil`
int     openpty(int* master, int* aslave, char* name, const termios* termp, const winsize* winp);   // 返回0
pid_t   forkpty(int* master, char* name, const termios* termp, const winsize* winp);                // 若为父进程则返回子进程PID，若为子进程则返回0，若出错返回-1
```
<!-- entry end -->

## 高级I/O
### 非阻塞I/O
[见fcntl](#普通文件)
* 对于硬盘文件无效，因为对硬盘的读写并非低速I/O
* 对于管道、套接字、终端有效

### 记录锁
[见fcntl](#普通文件)
* 记录锁即进程间读写同步锁，与PID关联
* 关闭文件描述符时即会释放对应文件的记录锁，不管该文件描述符是否还有其他副本
* 记录锁不会阻塞本进程获取锁，转而直接替换锁
* 建议性锁需要调用记录锁接口才有用，强制性锁对所有进程强制限制读写（需要`mount -o mand`）

### 分段I/O
<!-- entry begin: readv writev iovec -->
| 限制宏  | 说明                |
|---------|---------------------|
| IOV_MAX | iov参数最大元素个数 |
```c
#include <sys/uio.h>
struct iovec
{
    void* iov_base; // 缓冲区起始处
    size_t iov_len; // 缓冲区长度
};
ssize_t readv(int fd, const iovec* iov, int iovcnt);    // 返回已读字节数
ssize_t writev(int fd, const iovec* iov, int iovcnt);   // 返回已写字节数
```
<!-- entry end -->

### I/O多路复用
<!-- entry begin: select pselect -->
```c
#include <sys/select.h>
int select(                     // 返回准备好的描述符数量。会被信号中断
    int maxfdp1,            // 等待文件描述符中的最大值 + 1
    fd_set* readfds,        // 指定可读描述符集合，并在返回时存储准备好的描述符集合
    fd_set* writefds,       // 指定可写描述符集合，并在返回时存储准备好的描述符集合
    fd_set* exceptfds,      // 指定异常描述符集合，并在返回时存储准备好的描述符集合
    timeval* tvptr          // tvptr==NULL表示无限期等待，tvptr->tv_sec==0&&tvptr->tv_usec==0表示不等待，否则等待指定时间。若提前被唤醒还将返回时存储剩余时间
);
int pselect(                    // 返回准备好的描述符数量
    int maxfdp1,            // 等待文件描述符中的最大值 + 1
    fd_set* readfds,        // 指定可读描述符集合，并在返回时存储准备好的描述符集合
    fd_set* writefds,       // 指定可写描述符集合，并在返回时存储准备好的描述符集合
    fd_set* exceptfds,      // 指定异常描述符集合，并在返回时存储准备好的描述符集合
    const timespec* tsptr,  // tvptr==NULL表示无限期等待，tvptr->tv_sec==0&&tvptr->tv_nsec==0表示不等待，否则等待指定时间
    const sigset_t* sigmask // 指定调用期间的信号屏蔽字（可为NULL）
);
```
<!-- entry end -->

### 内存映射
<!-- entry begin: mmap mprotect msync munmap shmget shmctl shmat shmdt -->
```c
#include <sys/mman.h>
void* mmap(         // 若成功返回映射内存起始地址，若出错返回MAP_FAILED。mmap直接映射的内核I/O缓冲区(MAP_SHARED)，对其更改会由内核更具缓冲区的元信息冲刷回硬盘
    void* addr,     // 建议起始地址，一般为NULL
    size_t len,     // 建议映射长度，向上取页的倍数，超出文件长度的内存用0填充
    int prot,       // 见下表prot
    int flag,       // 见下表mmap flag
    int fd,         // 指定映射的文件描述符
    off_t off       // 指定文件起始位置的偏移量，向上取页的倍数
);
int mprotect(void* addr, size_t len, int prot); // 返回0
int msync(void* addr, size_t len, int flag);    // 返回0
int munmap(void* addr, size_t len);             // 返回0

#include <sys/shm.h>
struct ipc_perm
{
    uid_t   uid;            // 拥有者的EUID
    gid_t   gid;            // 拥有者的EGID
    uid_t   cuid;           // 创建者的EUID
    gid_t   cgid;           // 创建者的EGID
    mode_t  mode;           // 读写权限
    ...
};
struct shmid_ds
{
    ipc_perm    shm_perm;   // 见上结构
    size_t      shm_segsz;  // 段长
    pid_t       shm_lpid;   // 上次调用shmop()的PID
    pid_t       shm_cpid;   // 创建者PID
    shmatt_t    shm_nattch; // 该段的引用计数
    time_t      shm_atime;  // 上次连接时间
    time_t      shm_dtime;  // 上次卸离时间
    time_t      shm_ctime;  // 上次改变时间
    ...
};
int shmget(                 // 返回shmID
    key_t key,          // 指定存在于用户态的与内核态ID关联的key（IPC_PRIVATE为特殊的key表示用于创建新键）
    size_t size,        // 指定请求共享段的长度，向上取页长倍数
    int flag            // 见下表shmget flag
);
int shmctl(                 // 返回0
    int shmid,          // 指定shmID
    int cmd,            // 指定为IPC_STAT、IPC_SET、IPC_RMID之一
    shmid_ds* buf       // IPC_STAT返回shmid对应共享内存段信息指针；IPC_SET表示设置；IPC_RMID表示删除（无论引用计数是否仅为1）
);
void* shmat(int shmid, const void* addr, int flag); // 返回共享段起始地址
int shmdt(const void* addr);                        // 返回0。引用计数减1

```
| prot       | 说明     |
|------------|----------|
| PROT_NONE  | 不可访问 |
| PROT_READ  | 可读     |
| PROT_WRITE | 可写     |
| PROT_EXEC  | 可执行   |

| mmap flag   | 说明                                                                    |
|-------------|-------------------------------------------------------------------------|
| MAP_FIXED   | 强制起始位置为指定的addr                                                |
| MAP_SHARED  | 共享内存映射，写会影响内核缓冲区块                                      |
| MAP_PRIVATE | 私有内存区块，写只影响进程缓冲区块（内核缓冲区刷新时也会刷新该缓冲区 ） |
| MAP_ANON    | 映射匿名对象，内存全置零                                                |

| shmget flag | 说明              |
|-------------|-------------------|
| IPC_CREAT   | 若key不存在则创建 |
| IPC_EXCL    | 若key已存在则出错 |
| 0666        | 读写权限          |

| shmat flag | 说明                                     |
|------------|------------------------------------------|
| SHM_RDONLY | 指定连接共享段为只读（默认读写）         |
| SHM_RND    | 使指定的共享段的映射地址向下取SHMLBA倍数 |
<!-- entry end -->

