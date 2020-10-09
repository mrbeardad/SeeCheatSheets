# 第一章、UNIX基础知识
## 文件和目录
* POSIX.1推荐将文件名限制在以下字符集之内：`[[:alnum:]-_.]`

## 输入和输出
* `stdin`,`stdout`,`stderr`是由shell帮助默认打开的
* 系统调用出错时大多数系统函数返回`-1`

## 进程和程序
* `CTRL+D`会被`read()`函数解析为结束输入，若为行首第一个字符则会触发`EOF`

## 出错处理
* `errno`用于获取UNIX系统出错信息，可以定义为出错编号的整数或返回出错编号指针的函数
* 多线程环境中，每个线程都有属于自己的局部`errno`
* 两条重要规则：
    * 若未出错则其值不会被清除
    * 任何函数都不会将其置零
<!-- entry begin: apue unix strerror perror -->
```c
#include <string.h>
char* strerror(int errnum); // 将errnum映射为字符串并返回

#include <stdio.h>
void perror(const char* msg); // 打印`msg: errno解释字符串`
```
<!-- entry end -->

# 第二章、UNIX标准及实现
## UNIX标准化
`ISO C`、`IEEE POSIX`、`USU`标准规范了系统接口，通过C头文件的形式供程序调用

## 限制
UNIX系统实现了许多常量对系统的行为进行可移植性的标准化，包括：
* 编译时限制
* 运行时限制

**注意** ：本手册将各种限制安插在各个主题中

# 第三章、文件I/O
## 文件描述符
**限制** ：`0~OPEN_MAX-1`
```c
#include <fcntl.h>
int open(const char* path, int oflag, .../* mode_t mode */);
int openat(int fd, const char* path, int oflag, .../*mode_t mode */);
/* openat()
 * 一：fd为AT_FDCWD，同open()
 * 二：path为相对路径，将path接在fd所指路径后面
 * 三：path为绝对路径，则忽略fd
 */

/* mode
 * O_RDONLY     0
 * O_WRONLY     1
 * O_RDWR       2
 * O_EXEC       3
 * O_TRUNC
 * O_APPEND     每次写时自动原子性调整文件偏移量到文件末尾，但不会恢复之前偏移量
 * O_CLOEXEC    置位文件描述符标识位FD_CLOEXEC，表示调用exec()时关闭该文件描述符
 * O_DIRECTORY
 * O_CREAT
 * O_EXCL       与O_CREAT联用时，若文件不存在则原子性创建它
 * O_NONBLOCK
 * O_NOCTTY
 * O_TTY_INIT
 * O_NOFOLLOW
 * O_SYNC
 * O_DSYNC
 */
```
