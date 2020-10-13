# 目录
<!-- vim-markdown-toc GFM -->

- [第一章、UNIX基础知识](#第一章unix基础知识)
  - [文件和目录](#文件和目录)
  - [输入和输出](#输入和输出)
  - [进程和程序](#进程和程序)
  - [出错处理](#出错处理)
- [第二章、UNIX标准及实现](#第二章unix标准及实现)
  - [UNIX标准化](#unix标准化)
  - [限制](#限制)
- [第三章、文件I/O](#第三章文件io)
  - [文件描述符结构](#文件描述符结构)
  - [不足值](#不足值)
  - [/dev/fd](#devfd)
- [第四章、文件与目录](#第四章文件与目录)
  - [stat结构](#stat结构)
    - [类型](#类型)
  - [权限](#权限)
  - [文件系统](#文件系统)
- [附](#附)
  - [错误处理](#错误处理)
  - [标准与限制](#标准与限制)
  - [文件I/O](#文件io)
  - [文件与目录](#文件与目录)

<!-- vim-markdown-toc -->
# 第一章、UNIX基础知识
## 文件和目录
* POSIX.1推荐将文件名限制在以下字符集之内：`[[:alnum:]-_.]`

## 输入和输出
* `stdin`,`stdout`,`stderr`是由shell帮助默认打开的
* 系统调用出错时大多数系统函数返回`-1`

## 进程和程序
* `CTRL+D`会被`read()`函数解析为结束输入，若为行首第一个字符则会触发`EOF`

## 出错处理
* `errno`用于获取UNIX系统出错信息，规定为左值
* 多线程环境中，每个线程都有属于自己的局部`errno`
* 两条重要规则：
    * 若未出错则其值不会被清除
    * 任何函数都不会将其置零

# 第二章、UNIX标准及实现
## UNIX标准化
`ISO C`、`IEEE POSIX`、`USU`标准规范了系统接口，通过C头文件的形式供程序调用

## 限制
UNIX系统实现了许多常量对系统的行为进行可移植性的标准化，包括：
* 编译时限制
* 运行时限制

# 第三章、文件I/O
## 文件描述符结构
* 进程表项
    * 描述符标识
        > `FD_CLOEXEC`：调用exec关闭该描述符
    * 文件表项指针
* 文件表项
    * 文件打开标识
    * 文件偏移量
        > 注意可能造成空洞，空洞可能不会占用实际磁盘块
    * v节点表项指针
* v节点表项
    > 与文件缓存缓冲关联，已执行原子操作
    * 文件系统相关i节点
    * 文件系统无关i节点

## 不足值
* EOF
* 终端每行读取
* socket
* pipe,FIFO
* 面向记录设备

## /dev/fd
Linux实现为指向物理设备的符号链接

# 第四章、文件与目录
## stat结构
### 类型
将文件类型加入考虑后，open()对于目录与符号链接不适用，需要其他专门处理的函数

* st_mode：
    * 普通文件
    * 目录文件
    * 符号链接
    * FIFO
    * Socket
    * 块特殊文件
    * 字符特殊文件

## 权限
将权限加入考虑后，open()对于非法操作返回错误

* 判断：
    1. 若EUID==0则直接授权
    2. 若EUID==OWNER则进行适当权限判断，然后结束
    3. 若EGID==GROUP则进行适当权限判断，然后结束
    4. 若存在附属GID==GROUP则进行适当权限判断，然后结束
    5. 否则使用OTHER权限进行适当权限判断，然后结束

将权限加入考虑后，新建普通文件与目录目录时其权限与所有者的确定

* 权限：
    * 对普通文件为0666，对目录为0777
    * 考虑umask（默认022）后，普通文件为0644，目录文件为0755
    * 当非root进程写一个普通文件时，自动清除其SUID与SGID
    * 创建新目录时，继承父目录的SGID（根据实现确定）

* 所有者：
    * ONWER为EUID
    * GROUP为EGID
    * 若父目录设置SBIT，则GROUP为父目录的GROUP

## 文件系统
* 超级块
    > 包含了整个文件系统的信息
* i-node
    > 包含了文件属性（不包括文件名）与block索引号
* block
    > 包含文件内容

硬链接：表示指向某文件的目录项

符号链接：用文件内容（路径）代指另一个文件，权限为777。指向的文件可能不存在而出错

删除文件：  
当一个文件的硬链接数为0且无进程打开该文件时，自动删除并清理该文件。
若硬链接数为0但仍有程序打开该文件，则待其关闭该文件后自动删除文件，
期间对其他程序来说此文件已不存在，但它实际存在于内核I/O块缓冲区。
* 利用此特性防止程序崩溃时保留临时文件
* `du`命令计算文件的总大小，而`df`命令查看实际文件系统使用的大小（包括硬链接数已为0但仍被程序打开而未被清除）

设备号：  
文件系统所在存储设备都由主、次设备号表示。
主设备号标识设备驱动程序，有时编码为与其通信的外设板；
次设备号标识特定子设备

文件长度：  
只针对普通文件、目录文件、符号链接有效；
文件长度与文件占用的块大小可能并不一样，两者间的差别可判断文件是否空洞（整个块都为0则为空洞）

# 附
## 错误处理
<!-- entry begin: strerror perror -->
```c
#include <string.h>
char* strerror(int errnum);                     // 返回errnum映射的字符串，未知errnum映射到提示字符
#include <stdio.h>
void perror(const char* msg);                   // 打印`msg: errno映射字符串`
```
<!-- entry end -->

## 标准与限制
<!-- entry begin: sysconf pathconf fpathconf -->
```c
#include <limits.h>
#include <unistd.h>
long sysconf(int name);                         // 返回对应限制值
long pathconf(const char* pathname, int name);  // 返回对应限制值
long fpathconf(int fd, int name);               // 返回对应限制值
```
* 若未特殊说明，则函数一般都会跟随符号链接
* 若未特殊说明，则at后缀函数支持`flag=AT_SYMLINK_NOFOLLOW`；否则仅支持特殊说明的flag
* 若未特殊说明，则所有at后缀函数都支持`fd=AT_FDCWD`
<!-- entry end -->

## 文件I/O
<!-- entry begin: open creat chroot close lseek read write truncate sync dup fcntl fd oflag whence OPEN_MAX NAME_MAX NO_TRUNC ENAMETOOLONG -->
**限制**
| 常量         | 说明                               |
|--------------|------------------------------------|
| _PC_OPEN_MAX | 打开文件描述符数量                 |
| _PC_NAME_MAX | 文件名                             |
| _PC_PATH_MAX | 路径名                             |
| _PC_NO_TRUNC | 文件名或路径名超出限制是否直接截断 |

**errno**
| 常量         | 说明                             |
|--------------|----------------------------------|
| ENAMETOOLONG | 文件名或路径名超出限制           |
| ESPIPE       | 对pipe,FIFO,socket设置文件偏移量 |

```c
#include <fcntl.h>
int     open(const char* path, int oflag, .../* mode_t mode */);            // 返回打开的文件描述符，注意创建文件时的默认权限未定义，若文件已经存在则不会创建文件，故权限设置可能并非我们期望的
int     openat(int fd, const char* path, int oflag, .../*mode_t mode */);   // 返回打开的文件描述符，注意创建文件时的默认权限未定义
int     creat(const char* path, mode_t mode);                               // 返回打开的文件描述符，相当于`O_WRONLY|O_CREAT|O_TRUNC`
#include <unistd.h>
int     chroot(const char* pathname);                                       // 返回0:-1，切换`/`所代指的路径（默认为系统`/`），只能由root调用，调用后该进程及其子进程则再无法恢复`/`了（因为隔离后根本无法指定原来的目录）
int     close(int fd);                                                      // 返回0:-1
off_t   lseek(int fd, off_t offset, int whence);                            // 返回新的偏移量
ssize_t read(int fd, void* buf, size_t nbytes);                             // 返回读取的字节数，遇EOF返回0
ssize_t write(int fd, const void* buf, size_t nbytes);                      // 返回已写的字节数
ssize_t pread(int fd, void* buf, size_t nbytes, off_t offset);              // 返回读取的字节数，遇EOF返回0，并lseek与read为原子操作，不更新文件偏移量
ssize_t pwrite(int fd, const void* buf, size_t nbytes, off_t offset);       // 返回已写的字节数，合并lseek与write为原子操作，不更新文件偏移量
int     truncate(const char* pathname, off_t length);                       // 返回0:-1
int     ftruncate(int fd, off_t length);                                    // 返回0:-1
void    sync(void);                                                         // 同步冲刷系统中所有文件块缓冲
int     fsync(int fd);                                                      // 返回0:-1，冲刷指定文件
int     fdatasync(int fd);                                                  // 返回0:-1，冲刷指定文件（仅数据）
int     dup(int fd);                                                        // 返回最小的未占用描述符用于复制，清除FD_CLOEXEC
int     dup2(int fd, int fd2);                                              // 返回最小的未占用描述符用于复制，合并close与dup为原子操作，清除FD_CLOEXEC
#include <fcntl.h>
int     fcntl(int fd, int cmd, .../* int arg */);                           // 改变打开文件属性
/*
 ****** fd ******
 * STDIN_FILENO
 * STDOUT_FILENO
 * STDERR_FILENO
 *
 ****** oflag ******
 * O_RDONLY     0
 * O_WRONLY     1
 * O_RDWR       2
 * O_EXEC       3
 * O_TRUNC      直接清空文件内容
 * O_APPEND     每次写时自动原子性调整文件偏移量到文件末尾，但不会恢复之前偏移量
 * O_CLOEXEC    置位文件描述符标识位FD_CLOEXEC，表示调用exec()时关闭该文件描述符
 * O_CREAT      若文件不存在则自动创建
 * O_EXCL       与O_CREAT联用时，将测试与创建合并为原子操作。不跟随符号链接
 * O_NOFOLLOW   若为符号链接则出错
 * O_SYNC       数据与属性同步写入
 * O_DSYNC      数据同步写入
 * O_NONBLOCK
 * O_NOCTTY
 * O_TTY_INIT
 *
 ****** whence ******
 * SEEK_SET     文件开始处
 * SEEK_CUR     当前位置
 * SEEK_END     文件结尾处
 *
 ****** cmd ******
 * F_DUPFD  F_DUPFD_CLOEXEC     复制文件描述符，后者自动设置FD_CLOEXEC标识，参数为预期返回的文件描述符
 * F_GETFD  F_SETFD             获取/设置文件描述符，即FD_CLOEXEC
 * F_GETFL  F_SETFL             获取/设置文件打开标识，前4个oflag为互斥位，需用掩码O_ACCMODE
 * F_GETOWN F_SETOWN            获取/设置异步I/O所有权
 * F_GETTLK F_SETLK F_SETLKW    获取/设置记录锁
*/
```
<!-- entry end -->

## 文件与目录
<!-- entry begin: stat filetype ft -->
```c
#include <sys/stat.h>
int     stat(const char* restrict pathname, struct stat* restrict buf);                         // 返回0:-1
int     lstat(const char* restrict pathname, struct stat* restrict buf);                        // 返回0:-1，不跟随符号链接
int     fstat(int fd, struct stat* buf);                                                        // 返回0:-1
int     fstatat(int fd, const char* restrict pathname, struct stat* restrict buf, int flag);    // 返回0:-1

struct stat
{
    mode_t          st_mode;    // 文件类型及权限
    ino_t           st_ino;     // i-node
    dev_t           st_dev;     // 设备号（文件系统）
    dev_t           st_rdev;    // 特殊文件设备号
    nlink_t         st_nlink;   // 硬链接数
    uid_t           st_uid;     // UID
    gid_t           st_git;     // GID
    off_t           st_size;    // 普通文件的字节长度
    struct timespec st_atime;   // atime
    struct timespec st_mtime;   // mtime
    struct timespec st_ctime;   // ctime
    blksize_t       st_nlksize; // 最优I/O块大小
    blkcnt_t        st_blocks;  // 文件占用磁盘块数量（块大小S_BLKSIZE）
}

/* 类型 */
S_ISREG(st_mode);
S_ISDIR(st_mode);
S_ISLNK(st_mode);
S_ISFIFO(st_mode);
S_ISSOCK(st_mode);
S_ISBLK(st_mode);
S_ISCHR(st_mode);
```
<!-- entry end -->

<!-- entry begin: mkdir rmdir opendir closedir rewinddir telldir seekdir chdir getcwd -->
```c
/* 目录 */
#include <sys/stat.h>
int     mkdir(const char* pathname, mode_t mode);                                               // 返回0:-1
int     mkdirat(int fd, const char* pathname, mode_t mode);                                     // 返回0:-1
#include <unistd.h>
int     rmdir(const char* pathname);                                                            // 返回0:-1
#include <dirent.h>
DIR*    opendir(const char* pathname);                                                          // 返回`DIR*`，打开目录（类似open()）
int     closedir(DIR* dp);                                                                      // 返回0:-1
struct dirent* readdir(DIR* dp);                                                                // 返回`dirent*`，并后移目录项位置，dirent{d_ino; d_name;};
void    rewinddir(DIR* dp);                                                                     // 将目录项位置置位到开头
long    telldir(DIR* dp);                                                                       // 返回当前目录项位置
void    seekdir(DIR* dp, long loc);                                                             // 设置目录项位置
#include <unistd.h>
int     chdir(const char* pathname);                                                            // 返回0:-1
int     fchdir(int fd);                                                                         // 返回0:-1
char*   getcwd(char* buf, size_t size);                                                         // 返回工作目录的真实绝对路径，通过`..`层层向上递归获取
```
<!-- entry end -->
<!-- entry begin: symlink readlink -->
```c
/* 符号链接 */
#include <unistd.h>
int     symlink(const char* actualpath, const char* sympath);                                   // 返回0:-1
int     symlinkat(const char* actualpath,int fd, const char* sympath);                          // 返回0:-1
ssize_t readlink(const char* restrict pathname, char* restrict buf, size_t bufsize)             // 返回读取字节数，合并open,read,close为原子操作
ssize_t readlinkat(int fd, const char* restrict pathname, char* restrict buf, size_t bufsize)   // 返回读取字节数，合并open,read,close为原子操作
```
<!-- entry end -->
<!-- entry begin: major minor -->
```c
/* 特殊设备文件 */
#include <sys/sysmacros.h>
major(st_dev);
minor(st_dev);
major(st_rdev);     // 只有块文件与字符文件才有st_rdev
minor(st_rdev);
```
<!-- entry end -->
<!-- entry begin: link unlink remove rename  -->
```c
/* 硬链接 */
#include <unistd.h>
int     link(const char* existingpath, const char* newpath);                                    // 返回0:-1，若epath为符号链接则创建符号链接
int     linkat(int efd, const char* existingpath, int nfd, const char* newpath, int flag);      // 返回0:-1，若epath为符号链接则创建符号链接，flag可为AT_SYMLINK_FOLLOW表示强制创建硬链接
int     unlink(const char* pathname);                                                           // 返回0:-1，不跟随软链接
int     unlinkat(int fd, const char* pathname, int flag);                                       // 返回0:-1，不跟随软链接，flag可为AT_REMOVEDIR
int     remove(const char* pathname);                                                           // 返回0:-1，不跟随软链接
int     rename(const char* oldname, const char* newname);                                       // 返回0:-1，不跟随软链接
int     frenameat(int oldfd, const char* pathname, int newfd, const char* newname);             // 返回0:-1，不跟随软链接
```
<!-- entry end -->
<!-- entry begin: access umask chmod -->
```c
/* 权限 */
#include <unistd.h>
int     access(const char* pathname, int tmode);                                                // 返回0:-1，按照实际的UID与GID进行权限判断
int     faccessar(int fd, const char* pathname, int tmode, int flag);                           // 返回0:-1，设置flag为AT_EACCESS则使用EUID与EGID
#include <sys/stat.h>
mode_t  umask(mode_t mode);                                                                     // 返回之前umask，进程唯一（非本用户通用），强制清除指定位的权限
int     chmod(const char* pathname, mode_t mode);                                               // 返回0:-1
int     fchmod(int fd, mode_t mode);                                                            // 返回0:-1
int     fchmodat(int fd, const char* pathname, mode_t mode, int flag);                          // 返回0:-1
/*
 ****** tmode ******
 * F_OK
 * R_OK
 * W_OK
 * X_OK
 *
 ****** mode ******
 * S_ISUID
 * S_ISGID
 * S_ISVTX
 * S_IRWXU
 *  S_IRUSR
 *  S_IWUSR
 *  S_IXUSR
 * S_IRWXG
 *  S_IRGRP
 *  S_IWGRP
 *  S_IXGRP
 * S_IRWXO
 *  S_IROTH
 *  S_IWOTH
 *  S_IXOTH
*/
```
<!-- entry end -->
<!-- entry begin: chown -->
```c
/* 所有者 */
#include <unistd.h>
int chown(const char* pathname, uid_t owner, gid_t group);                      // 返回0:-1
int lchown(const char* pathname, uid_t owner, gid_t group);                     // 返回0:-1，不跟随符号链接
int fchown(int fd, uid_t owner, gid_t group);                                   // 返回0:-1
int fchownat(int fd, const char* pathname, uid_t owner, gid_t group, int flag); // 返回0:-1
/*
 * 限制：
 * _PC_CHOWN_RESTRICTED     是否只有root才能更改文件UID
*/
```
<!-- entry end -->
<!-- entry begin: utime -->
```c
/* 时间 */
#include <sys/time.h>
int utimes(const char* pathname, const truct timeval times[2]);                         // 返回0:-1，timeval{time_t tv_sec; long tv_usec;};
int futimens(int fd, const truct timespec times[2]);                                    // 返回0:-1，timespec{time_t tv_sec; long tv_nsec;};
int utimensat(int fd, const char* pathname, const truct timespec times[2], int flag);   // 返回0:-1，timespec{time_t tv_sec; long tv_nsec;};
/*
 * times[1]与times[2]分别更新atime与mtime
 *
 * 若times空指针表示设置为当前时间
 * 若times非空指针，且任一`tv_nsec`为`UTIME_NOW`，则设置当前时间
 * 若times非空指针，且任一`tv_nsec`为`UTIME_OMIT`，则保持时间不变
 *
 * 设置为当前时间时，需要权限：拥有者、或root、或具有写权限
 * 设置为任意时间时，需要权限：拥有者、或root、或具有写权限
*/
```
<!-- entry end -->
