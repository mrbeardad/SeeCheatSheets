# 计科八股

- [计科八股](#计科八股)
  - [数据结构](#数据结构)
  - [操作系统](#操作系统)
    - [编码](#编码)
    - [虚拟内存](#虚拟内存)
    - [异常控制流](#异常控制流)
    - [线程与协程](#线程与协程)
  - [网络](#网络)
    - [TCP 全双工连接](#tcp-全双工连接)
    - [TCP 可靠数据传输](#tcp-可靠数据传输)
    - [TCP 拥塞控制](#tcp-拥塞控制)
    - [快速 UDP 协议](#快速-udp-协议)
    - [网络安全](#网络安全)
    - [HTTP 协议](#http-协议)
    - [网络通讯完整流程](#网络通讯完整流程)
  - [数据库](#数据库)
    - [磁盘结构](#磁盘结构)
    - [内存结构](#内存结构)
    - [事务](#事务)
    - [索引](#索引)
    - [缓存](#缓存)

## 数据结构

- 二叉搜索树
  - 每个节点有两个子节点，父节点的键值大于左子节点，小于右子节点
  - 特点：时间复杂度与树的深度有关，平均为 O(log(N))。
  - 缺点：但最坏的输入情况（所有数据已排序）会使二叉树退化为链表，导致增删查操作时间复杂度均为 O(N)
- AVL 树
  - 每个节点保持左子树与右子树高度差绝对值（平衡因子）不超过 1，每个节点额外记录树的高度
  - 若某次插入造成一个最深的节点的平衡因子大于 1 则进行旋转，从而恢复该子树的高度
    - 如果插入左子节点的左子树或右子节点的右子树造成不平衡，则进行单旋转（画图）
    - 如果插入左子节点的右子树或右子节点的左子树造成不平衡，则进行双旋转（画图）
  - 缺点：平衡要求太严格，插入与删除导致频繁的旋转操作，开销太大
- 红黑树
  - 每个节点要么红色要么黑色
  - 根节点为黑色
  - 红色节点的子节点必须是黑色
  - 从一个节点到一个 null 指针的每一条路径必须包含相同数目的黑色节点
  - 特点：红黑树的高度最多是 2\*log(N+1)，相对 AVL 树是在树的高度平衡与增删操作性能之间的折衷
- B+树

  - 除了根外，所有非叶节点的儿子数在 M/2 与 M 之间，防止 M 叉树退化
  - 数据只存储在叶节点上，所有叶节点都在相同深度上，且数据项个数在 L/2 到 L 之间（页分裂）
  - 特点：相对 CPU 密集型，IO 密集型的应用的性能瓶颈在于磁盘 IO 的速度，
    利用 B+树大大降低了树的深度（平均$log_{M/2}N$），也就大大减少了节点访问次数，从而减少了 IO 次数。
    而且利用 B+树可以进行范围查询。

- 散列表
  - 哈希冲突：
    - 链表法：
      - 特点：将哈希值相同的元素放到一个链表中，相等元素放到一起且新插入的放前面，
        当填充因子过大（一般阀值为 0.75）时进行 rehash，防止链表过长
    - 线性探测：
      - 特点：哈希冲突时会以线性函数为下标增量(如 f(i)=i)寻找下一个空闲位置存放元素
      - 缺点：会逐渐形成一次聚集，寻找合适位置所需时间变大
    - 平方探测：
      - 特点：哈希冲突时会以二次函数为下标增量(如 f(i)=i^2)寻找下一个空闲位置存放元素
      - 缺点：当表的大小为素数且表至少有一半是空的，则必定能插入一个新元素，否则很可能插入失败
    - 双散列：性能开销较大

## 操作系统

### 编码

- 加法器

  - s1 = A ^ B
  - c1 = A & B
  - s2 = s1 ^ c
  - c2 = s1 & c
  - s = s2
  - c = c1 ^ c2

- 减法器

  - x - y
    = x + 1111 - y + 1 - 10000
    = x + ~y + 1
  - -y = ~y + 1

- 整形编码

  - 无符码与补码
  - 扩展、截断、转换
  - 求逆、加法、乘法

- 浮点数编码

  - 符号-阶码-尾数
  - 非规格化（正负 0）、规格化、无穷、NaN
  - 精度：无结合律、近似比较

### 虚拟内存

- 进程内存布局（从低地址到高地址）

  - 代码段：ELF 中的.text 与.rodata 区
    > .text 保存机器指令，.rodata 保存诸如字符串字面量与浮点数字面量；
    > x86 指令浮点数没有立即数，所以需要存储到.rodata；而整数有立即数，可以直接存储到机器指令中
  - 数据段：ELF 中的.data 与.bss 区
    > .data 保存全局和静态变量，区别在于.bss 只记录未初始化或初始化为 0 的全局和静态变量而不分配空间，
    > 为节省空间，在加载时才分配内存并初始化为 0
  - 堆：
    > 动态内存，高级语言的内存分配器帮助我们管理（内存分配、碎片整理、垃圾回收等等）
  - 共享段：
    > 调用动态共享库时会将动态库的代码段和数据段映射到这里
  - 栈：
    > 保存运行时数据
  - 内核保留空间：起始于$2^{48}$
    - 内核代码与数据
    - 物理内存映射
    - 当前进程专属信息（task、thread_info、内核栈等等）

- 运行时栈

  - 函数参数
  - 返回地址
  - 被调用者保留寄存器值
  - 局部变量
    - 结构与数组
    - 取地址的变量
    - 寄存器不足

- 对抗缓冲区溢出攻击

  - 地址空间布局随机化(ASLR)
    > 运行时随机设置关键区域的地址空间，同时保证堆、共享段、栈的相对地址不变；
    > 随机化范围大约$2^32$，加大“空操作雪橇”暴力破解难度
  - 栈破坏检测
    > 在栈帧中的返回地址前面设置运行时随机的“金丝雀值”，并将该值放在只读寄存器中，
    > 函数返回前对比“金丝雀值”与只读寄存器值是否相等，若不等则说明发生了缓冲区溢出
  - 限制可执行代码区域

- 静态链接

  - 符号解析：将多个目标文件中的符号定义与符号引用绑定起来
    > 实现上维护三个集合：未绑定引用集合、定义集合、文件集合。链接器依次解析命令行参数文件
  - 重定位：将多个目标文件聚合成一个文件后，一些引用地址需要修改
    > 外部函数、内部全局变量、外部全局变量；
    > 内部函数因为相对地址不变而无需重定位

- 动态链接

  - 编译时：提取动态库符号信息
  - 启动时：调用链接器加载动态库
    > 动态库搜索路径：
    1. 可执行文件中指定
    2. 环境变量 LD_LIBRARY_PATH 指定
    3. 配置文件/etc/ld.so.cache 指定
    4. 系统默认路径/lib
  - 内存映射：代码段母本、数据段副本（相对代码段位置不变）
  - 变量引用：立即绑定到 GOT 表
  - 函数引用：延迟绑定到 PLT 表

- 虚拟内存

  - 优点：
    - 使系统更高效地利用内存
      - 进程看起来连续的地址空间页面，实际上可能被分散地映射到了物理内存各处，系统可以很好的利用一些空间碎片
      - 进程的虚拟地址空间不仅可以映射到物理内存，还可以映射到磁盘空间，
        系统利用 LRU 算法会在内存紧张的时候将一些不常用的页面冲刷到磁盘 swap 空间，下次访问时通过缺页处理再加载回来。
        整个过程对进程透明。
    - 简化内存管理：
      - 为每个进程提供一致的地址空间，可以简化链接、简化加载、简化共享、简化内存分配
    - 内存保护机制：
      - 每个进程的地址空间都相对独立，从而避免地址空间被其它进程破坏
        > 不同进程中相同的虚拟地址一般映射到不同的物理地址，除非是共享段或者 fork 的子进程因为 COW 机制而与父进程暂时共享地址空间
  - 地址翻译：
    - 当 CPU 指令进行访存操作时，先将虚拟地址送给 MMU 进行翻译
    - 将虚拟地址分为两部分：虚拟页号与偏移量
    - 然后从页表基址寄存器读取页表起始地址访问页表（页表有单独的高速缓存 TLB）
    - 根据多级页表一层层进行索引，最终得到对应的物理页号；同时根据偏移量去高速缓存中找出可能的数据
    - 当翻译完成后检查提前找出的数据地址是否匹配，如果匹配就加速的数据访问，若不匹配则通过物理地址重新访问高速缓存
  - 页与段：
    - 页是内存分配/翻译的基本单位，段将连续的功能相同的页组织关联起来
    - 页表存储信息包括：是否已分配、超级权限、可读、可写、可执行

### 异常控制流

- 异常类型：
  - 中断（异步）
  - 陷入（同步）
  - 故障（同步）
  - 终止（同步）
- 异常处理：
  - CPU 发生异常控制流
  - 保存寄存器状态
  - 通过“异常表基址寄存器”访问“异常表”
  - 执行当前“异常号”对应的“异常处理程序”
  - 恢复寄存器状态，继续执行当前指令或下一条指令
- 内核模式与用户模式
  - 由某个寄存器的模式位表示
  - 处于内核模式的进程可以执行指令集中的任何指令，访问系统中任何内存位置
  - 用户模式使用用户栈，内核模式使用内核栈
  - 从用户模式变为内核模式的唯一方法就是通过异常控制流
- 进程/线程上下文切换

  - 线程是调度的基本单位，进程是资源分配的基本单位
  - 线程切换寄存器中的值，特别是 PC 状态
  - 进程切换页表基址寄存器中的值，即切换整个虚拟地址空间，导致大量高速缓存失效

- 进程状态

  - (R)运行 ：该进程在调度队列中
  - (S)可中断睡眠 ：该进程在响应事件等待队列中
  - (D)不可中断睡眠 ：该进程不响应异步信号以保护控制流不被打断
  - (T)停止或跟踪 ：该进程被暂停或不能响应 SIGCONT 的跟踪状态
  - (X)退出 ：该进程即将退出
  - (Z)僵尸 ：该进程已经终止但还未被父进程回收，保留了内核栈、task、thread_info

- 进程间通讯

  - 管道(pipe)
    - 用于单向通讯，一个 fd 用于读，一个用于写
    - 只能在有亲缘关系的进程间使用，因为 pipe 读写端的文件描述符只能通过 fork 子进程继承来传递
    - 缓冲区大小有限
    - 应用程序 stdin 一般默认全缓冲，进程会循环阻塞直到缓冲区读满或者触发 EOF，
      如果管道用于双向通讯，很可能造成缓冲区未满而另一边又试图读取输出而非触发 EOF，造成双方循环等待的死锁局面。
      一般这种情况用伪终端来通讯，因为 stdin 对于终端都是行缓冲。
  - 伪终端(pty)
    - 模拟终端
  - 套接字(socket)
    - 用于双向通讯
    - 可以在任一两进程间使用，甚至两进程不必在同一系统上
    - 需要对传输的数据进行解析
  - D-Bus
    - 类似总线，可以让多个进程同时进行通讯
  - 消息队列
  - 共享内存
  - 信号量
  - 信号

### 线程与协程

- 死锁必要条件

  > 简而言之，两线程互相请求对方已持有的互斥锁，导致两线程均阻塞等待对方解锁。
  > 可以通过以相同顺序加锁解决（破坏循环等待条件）

  - 互斥条件
  - 不可抢占条件
  - 请求和保持条件
  - 循环等待条件

- 多线程与多进程

  - 进程的创建、销毁、切换的开销较大
  - 同进程的线程间共享虚拟地址空间，通讯更方便，也更容易出错
  - 一个线程挂掉可能导致整个进程挂掉
    - 主线程退出
    - 某线程调用 exit 函数
    - 某线程接受信号而终止进程

- （无栈）协程

  - 两个栈帧
    - 一个是分配在栈上的普通栈帧，存储返回地址、声明周期仅限于该线程片段的变量等，虽协程的恢复与暂停而产生和销毁
    - 一个是分配在堆上的共享栈帧，存储协程句柄、伴随协程整个声明周期的变量等，在协程创建时产生，协程返回时销毁
  - 协程的切换完全在用户态进行
  - 利用协程编写异步事件驱动状态机，取代回调函数，可极大简化编程
  - 一个线程可创建多个协程，即多个协程占用一个线程的控制流，无法并发来充分利用 CPU，适用于 IO 密集型应用

- ASIO 的 Proactor 模型

  > 利用 gdb 调试追踪调用栈，观察源码，可简化为两个组件

  - 引发器(Initiator)
    > 如 socket
    1. 启动异步操作，如低速 IO、计时器等等，利用系统调用或多线程实现异步，保证快速完成返回
    2. 注册该异步事件（对应的文件描述符）及其对应的回调函数
  - 前摄器(Proactor)
    > 如 io_context
    - 如果完成事件队列中存在任务则取出（线程安全）
      1. 执行其回调函数
      2. 回调函数作为 Initiator 再次启动异步操作、注册异步事件（进入下一个状态）
    - 如果完成事件队列中无任务则阻塞
      1. 利用操作系统接口（如 epoll）实现多路复用阻塞监听
      2. 当异步操作完成时，会触发监听事件，唤醒线程进入下一次循环（此时队列非空）
  - 服务器一般存在 3 类状态机
    - 连接状态机，负责与客户端通讯
    - 监听状态机，负责与客户端建立连接状态机
    - 多路复用状态机，负责运转其它状态机

- 无锁队列
  > 并发问题的核心：一个线程读/写一个可能同时会被其他线程修改的共享数据时，会产生竞争条件。
  - 本质：我还专门去看过无锁队列的那篇论文，这个无锁队列算法利用 CAS 原子操作来实现“乐观锁”，从而让线程无阻塞的运行
  - 链表结构：维护 head 指向上次被删除的节点（哑节点），维护 tail 指向队列最后一个节点，前开后闭区间，如此可表示空集
  - enqueue 操作的核心原子操作：`CAS(tail->next, NULL, new_tail)`和`CAS(tail, read_tail, new_tail)`
  - dequeue 操作的核心原子操作：`CAS(head, read_head, read_head->next)`
  - dequeue 操作的 ABA 问题解决：
    > 问题描述：head 可能在 CAS 之前被修改多次，但因内存重用而致最后一次修改时将 head 重新改回了 read_head，
    > 导致当前线程误以为 head 与 read_head 有效匹配
    - Double-CAS(指针，版本号)
    - 引用计数，当没有线程持有 read_head 时才能销毁对应节点（避免内存重用）
    - 使用循环数组

```cpp
void push(T val) {
    Node* newTail = new Node{};
    newTail->val_ = val;
    newTail->next_ = nullptr;
#ifdef V1
    while ( tail_->next_->compare_exchange_week(nullptr, newTail) );
    tail_->exchange(newTail); // 缺点：如果此处更新失败会导致其他所有线程永久阻塞
#elif V2
    for ( ; ; ) {
        // 每次循环读取新的tail_
        Node* readTail = tail_;
        // 尝试更新tail_->next抢占enqueue权
        if ( !readTail->next_->compare_exchange_week(nullptr, newTail) ) {
            // 若抢占失败，则占用者可能处于中间态，尝试帮助其更新tail_
            tail_->compare_exchange_week(readTail, readTail->next_);
        } else {
            // 若抢占成功，则此时处于中间态，接下来更新tail_
            tail_->compare_exchange_week(readTail, newTail); // 缺点：后两个CAS操作可能导致无效竞争
            break;
        }
    }
#elif V3
    Node* readTail = tail_;
    Node* oldTail = readTail;
    do {
        // 把readTail看作是接近真正tail的一个节点，尝试移动至真正tail
        while ( readTail->next_ ) // 缺点：遍历开销可能较大，p个并发量则最大需前进2p-1步
            readTail = readTail->next_;
    } while ( readTail->next_->compare_exchange_weak(nullptr, newTail) )
    tail_->compare_exchange_strong(oldTail, newTail);
#endif
}
```

## 网络

### TCP 全双工连接

- 三次握手的原理

  - 防止旧的重复连接的初始化（旧 SYN 先到，响应 RST）
  - 同步双方的初始序列号

- SYN 泛洪攻击：发送大量合法的 SYN 包试图建立连接而无后续操作，占用服务器资源

  - 减少半连接状态持续时间
  - 设置最大半连接数量
  - 攻击者黑名单
  - SYN cookie（四元组+秘密数 hash）

- 四次挥手的原理

  - 全双工连接，两次握手关闭两个方向
  - 一个方向关闭后，另一个方向可能还有数据需要传输

- TIME_WAIT 状态原理

  - 主动发起关闭连接的一方最后所进入会进入 TIME_WAIT 状态
  - 防止具有相同四元组的旧的数据包被新连接错误地接收，等待 2MSL 让原来连接的数据包全部过期
  - 等待足够的时间以确保最后的 ACK 能让对方接收从而正常关闭连接

- TIME_WAIT 危害

  - 占用内存资源
  - 占用端口资源

- 出现大量 TIME_WAIT 原因

  - 服务器大量负载导致主动关闭大量连接
  - 客户端发起大量短连接

- 解决大量 TIME_WAIT 方案

  - 开启`tcp_timestamps`功能，从而在 TCP 报头中加入时间戳，根据时间戳来判断数据包是否过期从而可以不用等待 2MSL
    同时开启下述参数：
  - 客户端开启`tcp_tw_reuse`，调用 connect()时可重用一个 TIME_WAIT 持续超过 1 秒的端口
  - 服务器开启`tcp_tw_recyle`，快速重用 socket，但是注意无法保证 NAT 后端的主机时钟同步，
    导致如果新连接中 NAT 服务器后端的主机时钟过慢而错判其时间戳为旧连接的数据包

- 流式数据导致粘包

  - 接受方：不及时处理
    - 后台线程预处理
    - 格式化头部信息
  - 发送放：等待缓冲区满
    - TCP 紧急传送指令
    - 格式化头部信息

### TCP 可靠数据传输

可靠数据传输：使数据按序、无缺失、不重复、信息完整地到达等对方

```python
##############################
# 发送方
##############################
维护核心变量：
下个数据段的序列号 seq
最早的已发送且未被确认的数据段的序列号 base

if 从应用层接收到数据:   # 事件一
    if 未启动定时器:
        则启动定时器
    将数据分片封装MSS
    计算下个数据段的序列号

if 接受到ACK报文:       # 事件二
    if ACK报文的序号 > base:
        更新base
        if 所有已发送数据段均已被确认:
            重启定时器
    elif 如果接受到3次冗余ACK:
        立即回传冗余ACK期望的数据段

if 定时器超时:          # 事件三
    超时间隔 *= 2
    重启定时器

##############################
# 接受方
##############################
if 接受到期望序号报文且之前数据均已确认:
    延迟回传ACK，最多延迟500 ms

elif 接受到期望序号报文且另一个报文等待确认:
    立即回传ACK累积确认两个报文

elif 接受到可以填充数据缺失间隙的报文:
    立即回传ACK累积确认

elif 比期望序号大的报文到达（数据缺失）:
    立即回传ACK指定期望序号

elif 比期望序号小的报文到达（数据冗余）:
    丢弃数据

elif 校验报文数据损坏且无法修复:
    丢弃数据
    立即回传ACK指定期望序号
```

### TCP 拥塞控制

- 拥塞控制：根据当前网络状况调整数据传输速率，从而改善整个网络环境

  - 控制已发送但未被接收的数据量不仅要小于对方接收窗口大小，同时还要小于拥塞窗口大小，通过控制拥塞窗口大小从而达到限流的目的
  - 慢启动状态：
    - 特点：拥塞窗口大小呈指数增长，初始为 1×MSS
    - 转移：当计时器超时时进入，并设置拥塞窗口大小为当前的一半
  - 拥塞避免状态：
    - 特点：拥塞窗口大小呈线性增长，初始为慢启动阀值
    - 转移：当慢启动速度达到慢启动阀值，或接受到新 ACK 而退出快速重传时进入
  - 快速重传状态：
    - 特点：每接收一个冗余 ACK 就增大一个 MSS，初始为慢启动阀值
    - 转移：当连续接收 3 个冗余 ACK 时进入，并设置拥塞窗口大小为当前的一半

### 快速 UDP 协议

- RUDP 协议（KCP 为例）

  - ROT 翻倍 vs 不翻倍
    > x1.5 比 x2 更好
  - ARQ 模式响应
    > UNA（此编号前所有包已收到）+ ACK（该编号的包已收到）
  - 快速重传
    > 通过 UNA 编号可直接判断某个包被跳过了 2 次即可快速选择性重传
  - 延迟 ACK vs 非延迟 ACK
    > 可选地延迟响应
  - 非退让控流
    > 可选地取消拥塞控制机制

### 网络安全

网络安全通讯三要素：

- 机密性：对称加密算法
- 完整性：MAC 哈希算法
- 端点鉴别：非对称算法

> 信任链的形成需要根本上有一个完全可信的点

应用：

- SSH：服务器验证客户端身份，是否为本机用户
- SSL：客户端验证服务器身份，是否为权威认证机构所认证
- GPG：验证网络文件的可信度，是否为某人亲自确认并签发

SSL 协议

> 客制部分：加密套件、密钥交换算法、校验证书(CA 链与 CN)

1. `C->`传送：提供加密算法列表、随机数 1 等等
2. `<-S`传送：选择一套加密算法、随机数 2 等等
3. `<-S`传送：（可选）服务器证书
4. `<-S`传送：（可选）DH 算法参数
5. `<-S`传送：（可选）客户端证书请求
6. `C->`传送：利用随机数计算的 PMS 的密文（服务器公钥加密或 DH 算法加密）
7. `C->`传送：（可选）DH 算法参数
8. `C->`传送：（可选）客户端证书
9. `C-S`操作：双方由 PMS 计算出 MS，再生成 2 个对称密码与 2 个 MAC 密码，每个方向使用一对密码进行信息对称加密与信息完整性校验
10. `C->`传送：所有握手消息的 MAC
11. `<-S`传送：所有握手消息的 MAC

### HTTP 协议

- HTTP 协议

  - 报文
    - 请求：`方法 URI 协议版本`
    - 响应：`协议版本 状态码 描述`
  - 资源
    - URL：`<scheme>://<user>:<password>@<host>:<port>/<path>;<params>?<query>#<frag>`
  - 方法
    - GET ：安全且幂等地获取指定资源
    - POST ：向指定资源提交数据（可包含 body，可缓存）
    - PUT ：同 POST 但具有幂等性（可包含 body）
    - DELETE：删除指定资源
  - 连接与优化
    - 短连接
      - 管理方便
      - 连接的建立与断开，以及多次慢启动导致延迟高
    - 长连接
      - 同一连接中序列化进行请求-响应
      - 需要指明数据长度
      - 连接保活机制
    - 管道化连接
      - 发起请求不必等待前一个响应结束，但必须保持响应顺序，从而可能造成队头阻塞
    - HTTP/2
      - 二进制帧
      - 多路复用
      - 头部压缩
    - HTTP/3
      - 利用 UPD+QUIC 代替 TCP，解决 TCP 队头阻塞问题

### 网络通讯完整流程

- 客户端发送数据流程

  - **DHCP**：
    - 发起 DHCP 发现报文
    - DHCP 主机回传 DHCP 响应
    - 此时可能有多个主机响应，选择其中一个发送 DHCP 请求
    - DHCP 主机回传 DHCP 接收
    - 最后得到本机 IP 和子网掩码、DNS 服务器 IP、默认网关 IP
  - **DNS**：
    - 请求 DNS 服务器解析目标域名，获取对应 IP
    - 本地 DNS 服务器递归查询，其他网络中的 DNS 服务器一般都迭代查询
    - 获取的可能是目标域名的对应 IP，也可能是目标域名所属的 DNS 服务器，后者就需要我们进行迭代查询
    - 得益于 DNS 缓存机制使得可以跳过大量的 DNS 服务器（诸如根 DNS、顶级 DNS 等等），而且 DNS 使用 UPD 协议使查询速度更快
  - **应用层**：
    - 有了目的 IP，就可以利用 TCP 套接字建立连接，然后进行网络间的进程间通讯(IPC)
  - **传输层**：
    - TCP 协议会提供一些额外服务：三次握手简历全双工连接、可靠数据传输、拥塞控制等等
    - 将数据分片，每片最大 MSS（MTU 减去 TCP/IP 报头长度）
    - 添加 TCP 报头（包括源端口、目的端口、序列号等等）
  - **网络层**：
    - 查询路由表，把目的 IP 地址与路由表条目的掩码进行与运算，判断是否与该条目网段匹配，
      从而确定目的 IP 的下一跳地址（直接发送 or 通过网关），
      以及到达下一条地址需要从哪个网卡发送，从而确定源 IP 地址
    - 添加 IP 报头（包括源 IP、目的 IP 等等）
  - **链路层**：
    - 此时处于网卡驱动程序，检查目的 IP 是否为本网卡 IP，若是则直接回传到网络层（Docker 虚拟网桥就是在这层捕获发送到容器中的数据包）
    - 查询 arp 表确定下一跳 IP 地址的 MAC 地址（如果没有则发送 arp 请求报文）
    - 添加 MAC 报头（包括源 MAC、目的 MAC 地址等等）
  - **物理层**：
    - 此时处于网卡，进行信号转换

- 服务端接受数据流程
  - 全局负载均衡(GSLB)：实现就近（地域以及运营商）接入，全局流量调度和负载均衡
    - 通过 IP 定位实现地理位置就近接入
    - 通过布置运营商 DNS 服务器将目标域名 CNAME 到同运营商的服务器域名实现同运营商接入
  - 网关转发
    > 对外表现为一个服务器(Name:VIP:Port)，转发后通过某一 Node(IP:Port)进入集群
    - 四层转发：IP 收敛，负载均衡，RS 自动剔除，多级容灾，攻击拦截
    - 七层转发：协议卸载，负载均衡
    - 最终访问请求被路由到容器集群入口
  - 容器集群
    - namespace ：将 K8S 集群划分为若干个资源不可共享的虚拟集群
    - Ingress ：管理集群外部访问，提供负载均衡、SSL 终结和基于名称的虚拟托管（七层转发，主外）
    - Service ：即使 Pod 的 IP 动态变化也能通过 Service(Name or IP)访问其后的 Pod（四层转发，主内）
    - Deployment：管控 Pod 运行在用户期望状态中（调度、恢复等等）
    - Pod ：Pod 中运行若干容器共享 IP 等资源（除非服务存在依赖关系，一般一个 Pod 一个容器）

**K8S 动态地将物理 Node 组建为集群，并在其中提供稳定的 Service，而实际上底层在整个集群中调度编排运行若干 Pod 来支承一个服务**
![K8S架构图](images/components-of-kubernetes.svg)

- 内容分发网络(CDN)

  - 单一数据中心缺点：
    - 数据中心到用户之间的距离太远从而延迟太高
    - 相同的数据多次通过相同的链路从而造成浪费
    - 单点故障后服务就会停止
  - 利用 DNS 服务将客户端请求重定向到一个适当的分布式集群（缓存）
    - 被动拉取资源
    - 主动推送热点资源

- 分组交换

  - 相对电路交换优点：时分复用供多个端点共享线路进行通讯
  - 延迟包括：
    - 处理时延
    - 排队时延
    - 传输时延
    - 传播时延

- 举例：
  - Docker 网络模型
  - WSL 网络模型

## 数据库

### 磁盘结构

- 行记录

  - 变长字段长度列表:NULL 值列表:头部信息:主键列:事务 ID:回滚指针:其他字段列
    - 头部信息：包括下条行记录指针、删除位等等
    - 主键列：若表中没有 NOT NULL UNIQUE 列，则自动生成隐藏的主键列
    - 事务 ID：为最后一次修改该行的事务 ID
    - 回滚指针：指向 Undo Log
  - 行溢出处理：数据页一般为 16K 且规定一页至少放两行记录，
    一般将过长的列存储于外部的溢出页中，行记录中只存储指针

- 页结构

  - 头部信息:用户记录空间:空闲空间:页面目录:尾部校验
    - 头部信息：包括上/下个页面指针（页面即是 B+树节点）、空闲空间地址、垃圾回收链表等等
    - 用户记录：按索引列大小排列成有序链表
    - 页面目录：将用户记录分组的有序数组，可以用二分搜索来加速页面内部的搜索
    - 尾部校验：校验数据页的完整性
  - 垃圾回收：
    1. 插入新记录时，会先取垃圾链表第一个行记录判断其空间是否足够，若足够则覆盖之，并更新垃圾链表
    2. 若垃圾链表第一个记录空间不够，则直接向页面申请空间
    3. 若空闲空间不足，但页面整体的剩余空间足够，则将页面复制到新页面并清空原来页面，再将新页面中的记录重新插回原页面进行碎片重整
    4. 若剩余空间也不足，则进行页分裂
  - 页分裂处理：当页面无足够空间存放新记录时，将一般的行记录移动到一个新页面，
    并将两个页面链接起来，新纪录放入其中并保持两个页面中的行记录有序排列

- 每连续的 64 个页划分为 1 个区(1M)，当数据量大的时候，就会以区为单位给索引分配空间来较少随机 I/O；
- 每连续的 256 个区划分为 1 个组(256M)，每组的第一个区中的头几页会存储区相关元数据，第一组第一区第一页还会存储表相关元数据；
- 功能相同的“区”组成一个逻辑段，如：非叶节点段、叶节点段、回滚段、等等；
  段中各区不一定连续，将功能不同的数据区别存放于不同的区中可减少随机 I/O，并利用段来组织它们；
  如：叶节点段、非叶节点段、回滚段等等。

- B+树索引构造
  1. 一个页面就是 B+树中的一个节点
  2. 最开始根节点页面存储用户记录
  3. 当根节点页面被装满，则将页面拷贝至新页面并进行页分裂，此时根节点页面存储子节点页面的页号（树的深度+1）
  4. 插入数据到下一层节点页面，每当进行页分裂就会递归地向上级页面中插入新页面页号和键值范围
  5. 重复，直到根节点页面被插满，到第 3 步

### 内存结构

- 缓冲池

  - 借助缓存池缓存磁盘页面从而减少磁盘 IO 次数
  - 利用哈希表(key 页号, value 数据指针)与链表队列(页面数据)实现热点版本 LRU 算法
  - 存在 flush 链表记录脏页，查刷脏页有两种途径：后台线程定时冲刷，LRU 算法驱逐时冲刷

- 普通 LRU 算法缺点：以下情况读取大量使用频度少的数据而驱逐掉热点页面

  - 线性预取：如果顺序访问某个区一定数量的页面，则异步读取下个区所有页面
  - 随机预取：如果缓存了某个区 13 个连续页面且均位与 LRU Young 子队列的前 1/4 则异步读取该区其他所有页面
  - 全表扫描

- 热点版 LRU 算法：

  - LRU 队列分为两部分：前 5/8 为 Young 子队列存储热点数据，后 3/8 为 Old 子队列存储冷数据
  - 新插入的数据插入到 Old 子队列头部，所以只会挤掉冷数据
  - 当第一次访问 Old 子队列中的数据时记录下时间戳，若在一定时间后再次访问该页面则判断为热点数据，加入到 Young 子队列头部
  - 为减少调整频率，仅当访问 Young 子队列后 1/4 时才将其调整到队首

- 更改缓冲区
  - 当二级索引页面不在缓冲区时，它会缓存由增删改引起的页面更改，
    稍后当页面被其他读取操作到缓存池中时，将更改合并进页面中
  - 二级索引中的插入是相对随机的，引入更改缓冲区可以减少大量磁盘随机 IO

### 事务

- 原子性：整个事务中的操作要么全部提交成功，要么全部失败回滚 **(Undo Log)**
- 一致性：未成功提交的事务中所做的修改不会保存至数据库中 **(Undo Log)**

  - 每条行记录中至少有两个隐藏列，其中一个就是回滚指针，指向 Undo Log 链
  - 失败回滚时，利用 Undo Log 链可以还原旧版本的行记录

- 隔离性：其他事务对未提交事务在最终提交前其对数据库的修改的可见性 **(MVCC)**
  - 每条行记录中至少有两个隐藏列：最后一次修改该行的事务 ID、回滚指针
  - 回滚指针指向 Undo Log 链，形成多版本链
  - 事务之间虽然写写互斥，但可同时读写
  - 可重复读模式在第一次执行查询语句时生成 ReadView，不可重复读在每次查询时都生成一个 ReadView
  - 根据 ReadView 可以判断某一事务是否在生成 ReadView 之前就提交了
  - 每次查询时根据 ReadView 去遍历版本链查找合适的版本数据，就是那些在生成 ReadView 时就已经提交的数据
  - 二级索引虽然没有隐藏列，但是页面头部信息记录有最后一次修改该页面的事务 ID，
    如果不能确定该事务 ID 对应的事务已经提交则必须回表查询

| 隔离级别 | 并发问题   | 解决方案 |
| -------- | ---------- | -------- |
|          | 脏写       | 互斥锁   |
| 未提交读 | 脏读       | MVCC     |
| 已提交读 | 不可重复读 | MVCC     |
| 可重复读 | 幻读       | 间隙锁   |
| 串行化   |            |          |

- 持久性：一旦事务提交则其所做的修改就会永久保存到数据库中 **(Redo Log 与双写缓冲区)**

  - 在事务提交前，会先将 Redo Log 写入磁盘，Redo Log 其实相当于增量备份，数据量很小，
    当系统意外宕机时，重启系统后可以根据它来恢复页面数据
  - 脏页会在适当的时候冲刷到磁盘，而非事务一提交就冲刷
  - 先将脏页冲刷到磁盘双写缓冲区备份，再冲刷到对应的页面位置；
    因为如果直接冲刷到对应页面中而中途宕机了，这时候页面数据不完整，是无法通过 Redo Log 恢复的；
    冲刷备份到双写缓冲区可应对这种情况；
    虽然冲刷了两倍的数据，但时间开销却不是两倍，因为 IO 密集型应用的性能瓶颈与 IO 次数有关

- 锁

  - 当一个事务读取某一页面时，对这个页面加共享锁
  - 读取完毕后释放页面的共享锁，并对检索的行加间隙锁

  - 当一个事务修改某一页面时，对这个页面加互斥锁
  - 修改完毕后释放页面的互斥锁，并对修改的行加互斥锁
  - 因为 MVCC，其他事务可以并发读取被该事务锁住的行

### 索引

- Hash 索引：支持全键匹配
- B+Tree 索引：支持前缀匹配、范围查询、索引排序与分组

- 聚簇索引
  - 要点：将索引与行记录放在一起
  - 优点：将相关数据放在一起可避免随机 IO
  - 缺点：插入速度严重依赖主键顺序
- 伪哈希索引
  - 要点：利用哈希函数将字符串映射为数字
  - 优点：减小索引大小且大幅加速字符串比较过程
  - 缺点：仅支持全键匹配
- 前缀索引
  - 要点：根据索引选择性(基数/总数)选取适当长度前缀
  - 优点：一定程度减小索引大小并加速字符串比较过程
  - 缺点：仅适用于`LIKE 'prefix%'`
- 复合索引

  - 要点：将索引选择性/单独查询频率/等值查询频率较高的放在前面
  - 优点：可优化`WHERE ... AND ...`
  - 缺点：无法跳过前缀列（可用`IN`来尝试解决）

- 索引合并：通过按主键合并多个二级索引(顺序 IO)结果集来代替回表查询(随机 IO)

  - 交集合并
    - 二级索引全键匹配`AND`二级索引全键匹配
    - 二级索引全键匹配`AND`主键索引范围匹配
  - 并集合并
    - 二级索引全键匹配`OR`二级索引全键匹配
    - 二级索引全键匹配`OR`主键索引范围匹配
    - 二级索引全键匹配`OR`交集合并结果
  - 并集排序
    - 二级索引范围匹配`OR`二级索引范围匹配

- 单表查询

  - 尝试进行索引合并
  - 选取一条最优索引
    - 全键匹配或范围查询或索引扫描
    - 回表查询
  - 服务层利用`WHERE`过滤

- 联结查询
  - 对驱动表进行条件过滤（单表查询）
  - 循环取出驱动表中行到 join buffer 来过滤被驱动表（单表查询）

### 缓存

- 缓存过期

  - 每 100ms 随机扫描来删除过期数据(定期删除)
  - 每次查询前确认数据是否过期(惰性删除)

- 缓存淘汰

  - noeviction ：返回错误，不会删除任何键值
  - volatile-ttl ：从设置了过期时间的键中删除剩余时间最短的键
  - allkeys-lru ：使用 LRU 算法删除最近最少使用的键值
  - volatile-lru ：使用 LRU 算法从设置了过期时间的键集合中删除最近最少使用的键值
  - allkeys-random ：从所有 key 随机删除
  - volatile-random：从设置了过期时间的键的集合中随机删除
  - allkeys-lfu ：从所有键中删除使用频率最少的键
  - volatile-lfu ：从配置了过期时间的键中删除使用频率最少的键

- 缓存穿透：对不存在数据的大量访问直接落在了数据库上
  > 利用布隆过滤器来确认数据是否可能存在于数据库中，若可能则查询数据库，更新布隆过滤器或刷新 KV 缓存
- 缓存击穿：热点 key 失效
- 缓存雪崩：大面积缓存失效
