# Linux

## Bash

- 基础语法：
  - Simple-Command `cmd [args...] [redirection]`
  - Pipeline `[time] [!] simple-command [ | or |& ...]`
  - List `pipeline [ ; or || or && or &]`
  - Composite-Command `{ list; }` `(list)` `((exper))` `[[ expr ]]`
- 命令扩展：
  1. 花括号
  2. 波浪号
  3. 变量
  4. 分词
  5. 路径
  6. 引用移除
- 控制语句
  - `if ; then`
  - `case in`
  - `while ; do`
  - `for in ; do`

## OS

- 系统启动

  - ESP
  - UEFI
  - Grub
  - Kernel
  - Systemd
  - Logind

- 文件系统

  - 资源挂载
    - 硬件设备
    - 磁盘文件
    - 网络文件
    - 内存文件
    - 进程信息
    - 内核接口
  - 目录结构
    - Hard Link
    - Inode
    - Block
  - 读写接口
    - File Descriptor
    - File Item
    - Vnode

- 进程管理
  - 动态链接
  - 虚拟内存
  - 线程调度
  - 进程状态
    - Running
    - Sleep
    - Stop
    - Zombie
  - 进程关系
    - 会话
    - 进程组
    - 父子进程
  - 进程通讯
    - socket
    - pipe
    - mmap
    - signal
  - 访问控制
    - 自主访问控制
    - 强制访问控制
  - 资源限制
    - CGroup
    - Ulimit
