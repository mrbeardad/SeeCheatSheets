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

- 磁盘管理

  - GPT
  - LVM
  - XFS

- 文件系统

  - 挂载
    - 内核信息
    - 内存文件
    - 硬盘文件
    - 网络文件
    - 设备文件
  - 结构
    - Hard Link
    - Inode
    - Block
  - IO
    - 文件描述符
    - 文件表项
    - Vnode

- 进程管理
  - 用户权限
    - 自主访问控制
    - 强制访问控制
  - 进程关系
    - 会话
    - 进程组
    - 父子进程
  - 进程通讯
    - socket
    - pipe
    - mmap
    - signal
  - 线程调度
    - 线程状态
    - 线程信息
    - 并发同步
    - 虚拟内存
    - 动态链接
  - 资源限制
    - CGroup
    - Ulimit
