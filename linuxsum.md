# Linux

## Bash

- 基础语法：
  - Command
  - Pipeline
  - List
  - Composite-Command
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

  - 文件系统挂载
    - 磁盘分区
    - 内存
    - 设备
    - 内核
  - 文件系统结构
    - Hard Link
    - Inode
    - Block
  - 文件 IO
    - 文件描述符
    - 文件表项
    - Vnode
  - IO 缓冲区分类
    - 管道：`← ▥ ←`
    - 套接字：`⮀ ⬒⮀⬒ ⮀`
    - 普通文件：`⮀ ▣`

- 进程管理
  - 用户权限
    - 自主访问控制
    - 强制访问控制
  - 进程关系
    - 会话
    - 进程组
    - 父子进程
  - 线程调度
    - 进程退出
    - 线程阻塞
    - 独立信息
  - 信号机制
    - 触发机制
    - 安全处理
  - 资源限制
    - CGroup
    - Ulimit
