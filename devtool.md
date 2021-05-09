**注意：整个工具集的快捷键或命令都依赖[这套配置](https://github.com/mrbeardad/DotFiles)或[这套配置](https://github.com/mrbeardad/Windows10)，即某些操作可能非原生的**
# 目录
<!-- vim-markdown-toc GFM -->

- [编译工具链](#编译工具链)
- [调试](#调试)
  - [gdb](#gdb)
  - [cgdb](#cgdb)
  - [lldb](#lldb)
- [性能剖析](#性能剖析)
  - [Gprof](#gprof)
  - [Valgrind](#valgrind)
  - [Gperftools](#gperftools)
- [ssh](#ssh)
- [ssl](#ssl)
- [tmux](#tmux)
- [zsh](#zsh)
  - [Some Alias](#some-alias)
  - [Some Tools](#some-tools)
- [网络工具](#网络工具)
- [git](#git)
  - [git概念](#git概念)
  - [git命令](#git命令)

<!-- vim-markdown-toc -->

# 编译工具链
<!-- entry begin: gcc -->
* gcc
    * 指定输出文件          ：-o
    * 优化级别              ：-O0 -O1 -O2 -O3 -Ofast -Os -Og
    * 使用AVX2指令          ：-mavx2
    * 编译程度              ：-E  -S  -c
        > cpp.i、ccl.s、as.o
    * 指定标准              ：-std=c11
    * 宏开关                ：-Dmacro=defin -Umacro
    * 指定头文件            ：-Idir
    * 指定程序库            ：-Ldir  -lxxx（库名libxxx.a、libxxx.so）
    * 指定字长              ：-m32 -m64
    * 指定程序规模          ：-mcmodel=large
        > 默认32位地址用于相对跳转，即代码段与数据段不能超过4GB
    * 全静态链接            ：-static
    * 制作动态库            ：-shared -fpic
    * 调用链接器            ：-ldl -rdynamic
    * 动态库搜索            ：-Wl,--rpath=PATH
    * 链接库打桩            ：-Wl,--wrap,symbol
        > 对symbol的调用链接到__wrap_symbol，对__real_symbol的调用链接到symbol
    * GPROF剖析             ：-pg -g3   -fno-inline
    * 调试                  ：-g3 -ggdb -fno-inline
    * 报错                  ：-Werror -Wall -Wextra
    * 开启C++标准库debug模式：-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
<!-- entry end -->

<!-- entry begin: ar 静态库 -->
* ar：
    * r：替换，d：删除，t：显示
    * c：创建，s：符号，u：更新，v：详述
    * 创建：rscv
    * 显示：tv
    * 追加：rsv
    * 更新：rsuv
    * 删除：dsv
    * 提取：xv
    > 注：BSD风格：`cmd  opts  lib.a  a.o`
<!-- entry end -->

<!-- entry begin: objdump readelf ldd strace pmap -->
* objdump
    * -dx   ：仅执行区域
    * -Dx   ：所有区域
* readelf
* ldd -V
* strace
* pmap
* size
* nm
<!-- entry end -->

# 调试
<!-- entry begin: coredump -->
* coredump设置
```bash
ulimit -c unlimited
echo "$HOME/Coredumps/%e.%p.coredump" | sudo tee /proc/sys/kernel/core_pattern
```
<!-- entry end -->

## gdb
<!-- entry begin: gdb -->
* gdb [EXEC] [CORE] [PID]

**断点**
* break     b   ：`func | linenum | *address`，`if CONDITION` 
* tbreak    tb  ：`func | linenum`
* condition     ：`break-id CONDITION`
* command   cmd ：`break-id COMMANDs`
* watch     w   ：`expr`
* rwatch    rw  ：`expr`
* awatch    aw  ：`expr`
> breakpoint 与 watchpoint的区别在于，前者监听位置，后者监听变量（的读/写）
* catch         ：`syscall | signal | exception`
* enable    e   ：`break-id`
* disable   d   ：`break-id`
* ignore    ig  ：`break-id N`
* delete    del ：`break-id`
* i break   ib
* save b        ：`FILE`
* source        ：`FILE`

**运行**
* set/show      ：`args | path | env`
* run       r   ：`shell-like-cmd`
* continue  c
* kill      k
* quit      q

**跟踪**
* step      s   ：`N`
* stepi     si  ：`N`
* next      n   ：`N`
* nexti     ni  ：`N`
* until     u   ：`line-num`
* return    ret ：`ret-val`
* finish    f
* backtrace bt

**打印与篡改**
* print     p   ：`/x | /t`，`val | expr | "%s",addr | $reg`
    > 注意：使用`$rip=address`调整执行位置时，注意栈帧是否合法
* x             ：`/12b | /12c | /12g`，`adress`
* display       ：`expr`
* whatis        ：`value`
* i locals  v

**代码**
> 当前行表示将要执行（还未执行）
* layout    la  ：src|asm|reg|sp

**调试多线程**
* i thread  it
* thread        ：`thread-id`

**调试子进程**
* set follow-fork-mode child
<!-- entry end -->

## cgdb
<!-- entry begin: cgdb -->
* cgdb
    * `i`：进入GDB模式
    * `Esc`：进入CGDB模式
    * `s`：进入滚动模式，可以滚动GDB窗口，`i`退出
    * `<space>`：切换设置断点
    * `o`：打开文件
<!-- entry end -->

## lldb
<!-- entry begin: lldb -->
```
* lldb `<exe>` [-c `<core>`] [-p `<pid>`]

* 断点：
    b       ：<查看>  <linenum>  <function>  0x<address>  <filename>:<linenum>  <module>`<function>  /regex/
    br      ：disable|enable|delete|read|write
    w       ：set var <var>
    w       ：modify -c '<condition>'

* 设置：
    env     ：<查看>  <env-var>=<value>
    settings： set|show  target.run-args|target.run-input-path|target.run-output-path

* 运行：
    r       ：<以/bin/sh的语法格式启动程序>
    c       ：
    s       ：[-e <linenum>]
    n       ：
    j       ：0x<address>
    finish  ：
    kill    ：
    q       ：

* 调试：
    p       ：打印
    e       ：赋值
    l       ：源码
    bt      ：调用栈
    f       ：[framenum]
    dis     ：反汇编
    reg     ：寄存器
    thread info
    t [threadnum]
```
<!-- entry end -->

# 性能剖析
<!-- entry begin: gprof valgrind gperftools -->
## Gprof
1. 编译参数指定`-gp`

2. 运行测试程序，会在当前目录生成gmon.out文件

3. 执行`gprof exefile`得到输出

## Valgrind
1. memcheck：检查程序中的内存问题，如泄漏、越界、非法指针等。

2. callgrind：检测程序代码的运行时间和调用过程，以及分析程序性能。

3. cachegrind：分析CPU的cache命中率、丢失率，用于进行代码优化。

4. helgrind：用于检查多线程程序的竞态条件。

5. massif：堆栈分析器，指示程序中使用了多少堆内存等信息。

这几个工具的使用是通过命令：`valgrand --tool=tool_name exefile` 分别调用的，
当不指定tool参数时默认是 --tool=memcheck

## Gperftools
1. `-g3 -ggdb3 -rdynamic -lprofiler`：链接指定程序库
    > 内存检测则添加参数`-ltcmalloc`
    > 若不指定链接库，则运行时打桩（LD_PRELOAD=/usr/lib/libprofiler.so）
2. `CPUPROFILE=prof.out exe`：设置环境变量并运行程序
    > `CPUPROFILE_FREQUENCY=1000`指定取样频率(interrupts/second)，默认100  
     内存检测则添加参数`HEAPPROFILE=heap.out`
3. `pprof --text exe_file prof.out`：剖析CPU性能
4. `pprof --text exe_file heap.out`：剖析内存使用

```sh
pprof --help

--maxdegree=<n>     最大递归深度
--focus=<regexp>    关注指定节点
--ignore=<regexp>   忽略指定节点
--text              文本打印
--web               浏览器图形，需要安装graphviz包
```

**注意**：逻辑使用CPU时间太短则会报告`No nodes to print`

![pprof](images/pprof-test.gif)

**信息解释：**
* 节点信息：
    ```
    类名
    方法名
    本地时间：即函数本体以及内联到该函数的过程的总执行时间
    累计时间：即包括本地时间以及该函数调用的其他函数的总执行时间（若等于本地时间则省略）
    ```
    * 时间单位与取样频率有关，默认每个时间单位大概10ms。累计时间的单位时间数目也即该函数的取样数目
    * 单位时间数量后的括号中的百分比即该时间与程序运行总时间之比
    * 节点大小与本地时间有关
* 边信息：
    * 箭头表示调用依赖关系
    * 数字表示caller的取样总数中，有N个调用了callee
<!-- entry end -->

# ssh
<!-- entry begin: ssh -->
* `ssh-keygen -t ecdsa -b 521  -C "注释"`
* `ssh-keygen -t rsa   -b 4096 -C "注释"`
* `ssh-keygen -f ~/.ssh/id_ecdsa -p`        ：更改密码短语
* `ssh-copy-id -i KEY.pub -p port user@host`：将公钥上传到主机
* `ssh -p Port User@Host`                   ：SSH连接主机
<!-- entry end -->

# ssl
<!-- entry begin: openssl -->
**openssl命令**：参考至[OSCHINA](https://my.oschina.net/u/3794778/blog/3021643)
* openssl genrsa -des3 -out server.key 2048
    > 创建私钥。`-des3`表示对私钥进行加密，默认不加密`-nodes`
* openssl req -key server.key -new -out server.csr
    > 利用已有私钥创建证书请求。
* openssl req -newkey rsa:2048 -keyout server.key -out server.csr
    > 同时创建私钥与证书请求
* openssl x509 -in server.crt -signkey server.key -x509toreq server.csr
    > 利用已有证书创建证书请求。用于续订证书
* openssl req -newkey rsa:2048 -keyout server.key -x509 -days 360 -out server.crt
    > 同时创建私钥与自签证书。用于SSL加密
* openssl req -key server.key -new -x509 -days 365 -out server.crt
    > 利用已有私钥创建自签证书
* openssl req  -text -noout -in server.csr
    > 查看证书请求
* openssl x509 -text -noout -in server.crt
    > 查看证书
* openssl ca -in server.csr -out server.crt -cert ca.crt -keyfile ca.key
    > 利用CA自签证书与证书请求创建证书

* openssl rsa -des3 -in unencrypted.key -out encrypted.key
    > 加密私钥
* openssl rsa -in encrypted.key -out decrypted.key
    > 解密私钥
* openssl rsa -check -in server.key
    > 验证私钥是否有效
* openssl verify -verbose -CAFile ca.crt server.crt
    > 验证证书是否由CA签发

* 验证私钥相关性
    * openssl rsa  -noout -modulus -in domain.key | openssl md5
    * openssl x509 -noout -modulus -in domain.crt | openssl md5
    * openssl req  -noout -modulus -in domain.csr | openssl md5
* 证书转换
* openssl x509 -in domain.crt -outform der -out domain.der
    > PEM转DER
* openssl x509 -inform der -in domain.der -out domain.crt
    > DER转PEM
* openssl crl2pkcs7 -nocrl -certfile domain.crt -certfile ca-chain.crt -out domain.p7b
    > PEM转PKCS7
* openssl pkcs7 -in domain.p7b -print_certs -out domain.crt
    > PKCS7转PEM
* openssl pkcs12 -inkey domain.key -in domain.crt -export -out domain.pfx
    > PEM转PKCS12
* openssl pkcs12 -in domain.pfx -nodes -out domain.combined.crt
    > PKCS12转PEM
<!-- entry end -->

# tmux
<!-- entry begin: tmux -->
* tmux
    * -S socketfile [attach]
    * tl    ：`tmux list-session`
    * ts    ：`tmux new-session -s <session-name>`
    * ta    ：`tmux attach -t <session-name>`
    * tad   ：`tmux attach -d -t <session-name>`
    * tkss  ：`tmux kill-session -t <session-name>`
    * tksv  ：`tmux kill-server`

* 快捷键： `Alt`+`w` 为快捷键前缀
    * meta操作
        * `R`       ：重载配置
        * `:`       ：命令模式
    * pane操作：
        * `s`       ：水平切分panes
        * `v`       ：竖直切分panes
        * `x`       ：关闭当前pane
        * `z`       ：最大化该pane或恢复大小
        * `方向键`  ：跳转到该方向的pane
        * `q`       ：选择一个pane
        * `Ctrl`+`o`：交换pane
        * `!`       ：新窗口打开该pane
    * window操作：
        * `c`       ：创建新窗口
        * `&`       ：删除当前窗口
        * `b`       ：上一个window
        * `n`       ：下一个window
    * session操作：
        * `d`       ：卸离会话(可通过`tmux a -t session-name`重新恢复)
        * `w`       ：预览整个会话窗口
        * `Ctrl`+`S`：保存会话到磁盘文件
        * `Ctrl`+`R`：从磁盘文件恢复会话
    * 其他操作：
        * `h`       ：打开htop
        * `g`       ：打开gtop
        * `i`       ：打开iotop
        * `r`       ：打开ranger
        * `m`       ：打开cmatrix
        * `f`       ：打开fzf
<!-- entry end -->

# zsh
## Some Alias
<!-- entry begin: alias -->
```sh
# alias l='ls -lah  --time-style="+%Y %m-%d %H:%M:%S "'
# alias l.='ls -ladh  --time-style="+%Y %m-%d %H:%M:%S " .*'
# alias ll='ls -lh  --time-style="+%Y %m-%d %H:%M:%S "'
alias l='lsd -lah'
alias l.='lsd -lah .*'
alias ll='lsd -lh'
alias cp='cp -i'
alias mv='mv -i'
alias rm='rm -i'
alias jobs='jobs -l'
alias df='df -hT'
alias psa='ps axo stat,euid,ruid,tty,tpgid,sess,pgrp,ppid,pid,pcpu,pmem,comm'
alias pstree='pstree -Uup'
alias free='free -wh'
alias vmstat='vmstat -w'
alias ip='ip -c'
alias cgdb='cgdb -- -q'
alias gdb='gdb --tui'
alias dif='diff -Naur'
alias ra='ranger'
alias apt='sudo apt'
alias expactf='expac --timefmt="%Y-%m-%d %T" "%l\t%n" | sort'
alias stl='sudo systemctl'
alias dok='docker'
```
<!-- entry end -->

## Some Tools
<!-- entry begin: tools -->
| 命令    | 用途                                        |
|---------|---------------------------------------------|
| f       | 利用fzf搜索当前目录并用ranger预览选择的文件 |
| j       | 模糊搜索跳转目录历史并跳转                  |
| x       | 解压各种压缩/归档包                         |
| h       | history                                     |
| ncdu    | 查看当前目录下文件的磁盘占用情况            |
| htop    | 系统资源监控                                |
| iotop   | 系统磁盘IO监控                              |
| cloc    | 计算指定目录下的代码量                      |
| cpv     | 使用rsync代替cp                             |
| fuck    | 纠正命令行错误                              |
| Ctrl+\[ | 进入vi模式，该模式下可使用vi快捷键修改命令  |
| v       | vi模式下，打开`$EDITOR`编辑命令               |
<!-- entry end -->

<!-- entry begin: ranger -->
* ranger
    * `<left>`      ：关闭目录，或进入上级目录
    * `<right>`     ：展开目录，或打开文件
    * `!`           ：执行shell命令
    * `S`           ：在选择目录下开启shell
    * `<octal>=`    ：修改文件权限
    * `<c-f>`       ：fzf搜索
    * `rn`          ：重命名
    * `rm`          ：删除文件
    * `yp`          ：复制文件路径
    * `yy`          ：复制
    * `dd`          ：剪切
    * `pp`          ：粘贴（冲突时自动更名）
    * `po`          ：粘贴时强制覆盖
    * `pl`          ：为复制的文件创建软链接
<!-- entry end -->

# 网络工具
<!-- entry begin: pacman -->
* pacman
    * 更新数据库
        * -Sy   ：更新软件包列表
        * -Syy  ：强制更新软件包列表
    * 查询软件包
        * -Ss   ：模糊搜索远程数据库
        * -Si   ：从远程数据库获取目的包的详细信息
        * -Sl   ：列出目的仓库所有包
        * -Qs   ：模糊搜索本地数据库
        * -Qi   ：从本地数据库获取目的包的详细信息
        * -Qc   ：查看目的包更新日志
        * -Qg   ：查看软件包组中的包
        * -Ql   ：查看目的包的文件安装路径
        * -Qo   ：查询目的文件所属包
        * -Qu   ：查询所有需要升级的包
    * 安装软件包
        * -S    ：远程下载并安装，若以安装则用本地包重装
        * -Sw   ：只下载软件包
        * -U    ：安装已下载的软件包
        * -Su   ：更新软件包
    * 删除软件包
        > 缓存位于：/var/cache/pacman/pkg/
        * -R    ：删除目的包
        * -Rc   ：还包括依赖它的包
        * -Rs   ：还包括只被它依赖的包
        * -Rn   ：还包括其配置文件
        * -Sc   ：清理未安装的软件包
        * -Scc  ：清理所有软件包与数据库
    * 从iso文件安装软件包
        ```sh
        mount -o ro,loop  /path/to/iso  /mnt/iso
        echo '
        [custom]     #添加在其他仓库之前
        Server = file:///mnt/iso/arch/pkg' | sudo tee /etc/pacman.conf
        pacman  -Sy
        ```
<!-- entry end -->

<!-- entry begin: curl -->
* curl
    * -s        ：silent mode
    * -S        ：打印错误消息，即使使用了`-s`
    * -L        ：接受链接跳转
    * -o        ：指定输出文件
* curl -Lo File URL
* curl -fsSL URL | bash
* curl -fsSL URL | bash -s -- {-opt}
* curl cheat.sh/`CMD`
* curl cheat.sh/`LANG`/`SPECIFIC`
    > [cheat.sh](https://github.com/chubin/cheat.sh)是github上一个nice的项目
    * 空白用`+`代替
    * `curl cheat.sh/~keyword`
    * `curl cheat.sh/python/:list` 列出可选项
<!-- entry end -->

# git
## git概念
* 管理目录
    > 注：不管git底层是如何实现的，我们需要知道git给我们的抽象模型是每次提交历史都是**全量备份**的，这一点很重要
    * 工作区`W`相对暂存区`S`：untraced，deleted，modified，rename
    * 暂存区`S`相对仓库区`R`：new，deleted，modified，rename
    * 仓库区`R`

* 分支模型
    * data对象：
        > 这就是每个文件的每次历史的数据文件实体，每次提交其实并不会备份未改动的文件，
        > 而只要一个文件改动的，就会被全量备份而创建一个新的data对象  
        > 注：其实git会在每次push时整理data对象，将一些小改动改为增量备份而非全量备份
    * tree对象：
        > 为每个commit管理“快照”，指向该commit所有的各个data对象
    * commit信息：
        * 作者、提交者、日期、校验、描述、父指针、"快照"抽象
            > 父指针指向上次**commit**，可能有多个
    * 分支指针：
        > 分支指针指向某个commit
        * HEAD          ：指向本地当前分支
        * BRANCH        ：指向本地已存在的分支
        * REMOTE/BRANCH ：指向远程仓库的分支
            > 远程分支只读，通过创建本地分支来跟踪并修改
![图片来自网络](images/git-branch.png)

* 工作流程
    * 贡献者
        * 将目标项目仓库fork到自己的远程仓库
        * 从自己站点clone项目到本地进行线下开发修改
        * 本地master分支跟踪origin/master
        * 设置upstream/master连接项目发布点
        * 完成后push到自己的公开站点
        * 向发布点发送pull request
        * 若冲突则拉取upstream再合并、修改、推送
    * 维护者
        * 若贡献者提交的PR没有问题，则直接合并即可
        * 若有问题且需要自己修改，则：
            * 添加贡献者的仓库到remote并fetch到本地
            * 若贡献太多，则配置remote仓库的[refs](#refs)
            * 审查、修改、合并后push到项目发布点

<!-- entry begin: git config -->
* git config
    * git config --system
        > /etc/gitconfig
    * git config --global
        > ~/.gitconfig
    * git config --local
        > ~/.git/config
    * 选项
        * user.name
        * user.email
        * core.editor
        * credential.helper cache ：托管私钥的密码短语
    * 配置<span id="refs">refs</span>
    ```gitconfig
    [remote "REMOTE"]
    url = URL
    fetch = +refs/heads/*:refs/remote/origin/*
    fetch = +refs/pull/*/head:refs/remote/origin/pr/*
    ```
<!-- entry end -->

<!-- entry begin: .gitignore -->
* .gitignore
    * `#`作为注释符号
    * 支持通配符`**` `*` `?` `[]`
        > 注意不支持`{}`
    * 以`/`结尾强制指定为目录
    * 以`/`开头表示禁止自动递归搜索
        > 递归只会针对单层路径，即`test/build`并不能表示忽略名为test的目录下的build文件，
        > 应该这样写`/**/test/build`
    * 以`!`开头则取消之前已被忽略的文件，若该文件的父目录已被忽略则无效  

* .gitkeep
    * 作为一个约定俗成的文件名，用来保留空目录
<!-- entry end -->

<!-- entry begin: .gitmodules -->
* .gitmodules
    * 引用一个项目作为该项目的子模块，以子目录的形式存在
    * git一般并不会展示子模块的细节信息，进入子模块的目录后便相当于进入一个独立的新项目
    * 提交时git会记录每个引用子模块的检出的HEAD，故需要手动更新子模块而非由git自动拉取上游更新
<!-- entry end -->

<!-- entry begin: .git/ -->
* .git/
    * description   ：gitweb程序使用
    * config        ：仓库配置
    * objects       ：所有数据对象
    * HEAD          ：当前checkout的指针
    * index         ：暂存区
    * refs          ：指向分支的提交对象的指针
        * remote/*/     ：远程仓库的分支引用
        * heads/        ：本地分支引用
        * tags/         ：本地标签引用
<!-- entry end -->

## git命令
* FILE表示文件
* DIR表示目录
* PATH表示路径
* COMMIT表示提交，一般可用SHA值、TAG值、分支指针
* BRANCH表示分支
* REMOTE表示远程仓库
* REF表示引用，即指定ref目录下的文件

<!-- entry begin: git add -->
* git add PATH
    > 将工作区中变更的文件添加到暂存区
    * ga    ：`git add`
    * gaa   ：`git add --all`
<!-- entry end -->

<!-- entry begin: git rm -->
* git rm PATH
    > 删除工作区与暂存区中的文件（只需要暂存区中存在目标文件）。
    * grm   ：`git rm`
    * grmc  ：`git rm --cached`
<!-- entry end -->

<!-- entry begin: git mv -->
* git mv PATH newPATH
    > 移动或重命名工作区与暂存区中的文件（需要工作区与暂存区都存在目标文件）
    * gmv   ：`git mv`
<!-- entry end -->

<!-- entry begin: git restore -->
* git restore PATH
    > 撤销工作区或暂存区的变更
    * grs   ：`git restore`
    * grst  ：`git restore --staged`
<!-- entry end -->

<!-- entry begin: git status -->
* git status
    > 查看当前项目变更与提交状态
    * gst   ：`git status`
    * gss   ：`git status -s`
<!-- entry end -->

<!-- entry begin: git commit -->
* git commit
    > 提交变更到本地仓库中当前分支  
    > `!`代表覆盖上次提交，`a`代表直接从工作区提交而跳过暂存区
    * gc    ：`git commit`
    * gc!   ：`git commit --amend`
    * gca   ：`git commit -a`
    * gca!  ：`git commit --amend`
<!-- entry end -->

<!-- entry begin: git tag -->
* git tag   [COMMIT]
    * gt        ：`git tag`
    * gtl与gtv  ：显示tag
    * 其他
    ```sh
    git tag -a TAG -m 'discription'     #强标签
    git tag -f TAG-lw                   #强制轻量标签
    git tag -d TAG                      #删除标签
    ```
<!-- entry end -->

<!-- entry begin: git log -->
* git log [FILE|COMMIT]
    * glgga     ：提交历史可视化
    * glola     ：提交历史精简可视化
<!-- entry end -->

<!-- entry begin: git diff patch -->
* git diff
    > 参数：
    >
    >       [PATH]
    >       COMMIT [PATH]
    >       COMMIT COMMIT [PATH]
    * gd    ：`git diff`
    * gds   ：`git diff --staged`
    * gdt   ：`git difftool --tool=vimdiff`
    * gdts  ：`git difftool --tool=vimdiff --staged`
    * gdi   ：`git diff-index`
```sh
diff -Naur OLD NEW > patch
patch -p`n`  < patch
```
> * new和old不要在同一目录下
> * n为去掉的/个数
<!-- entry end -->

<!-- entry begin: git branch -->
* git branch
    * gb        ：`git branch`
    * gbav      ：`git branch -a -vv`
    * gbda      ：删除已合并的分支
    * gbnm      ：查看未合并的分支
<!-- entry end -->

<!-- entry begin: git checkout -->
* git checkout
    * gco       ：`git checkout`
    * gcm       ：`git checkout master`
    * gcd       ：`git checkout develop`
    * gcb       ：`git checkout -b BRANCH REMOTE/BRANCH`（创建、检出并跟踪）
    * gct       ：`git checkout --track REMOTE/BRANCH`（当前分支直接跟踪）
<!-- entry end -->

<!-- entry begin: git stash -->
* git stash
    > 隐藏暂存区已安全跳转分支
    * gsta      ：`git stash push`
    * gstp      ：`git stash pop`
    * gstl      ：`git stash list`
<!-- entry end -->

<!-- entry begin: git reset -->
* git reset COMMIT
    > 撤销/回退当前分支指针到指定COMMIT
    * grh   ：`git reset`
<!-- entry end -->

<!-- entry begin: git cherry-pick -->
* git cherry-pick COMMIT
    > 复制(合并)目标commit相对其前一次的diff到当前分支
    * gcp       ：`git cherry-pick`
    * gcpc      ：`git cherry-pick --continue`
    * gcpa      ：`git cherry-pick --abort`
<!-- entry end -->

<!-- entry begin: git merge -->
* git merge
    > 原理      ：将两分支相对共同祖先的**变更**合并，若对同一个文件均有变更则冲突  
    * gm        ：`git merge`
    * gmt       ：`git mergetool --tool=vimdiff`
    * gmc       ：`git merge --continue`
    * gma       ：`git merge --abort`
<!-- entry end -->

<!-- entry begin: git rebase -->
* git rebase
    > 原理：将本分支相对共同祖先的各个变更作用于目标分支而形成各个commit，并删除该分支历史  
    * grb       ：`git rebase`
    * grbc      ：`git rebase --continue`
    * grba      ：`git rebase --abort`
    * 其他
    ```sh
    git rebase 主BRANCH                 # 本分支变基到<主branch>
    git rebase 主BRANCH 被BRANCH        # <被branch>变基到<主branch>
    git rebase -i REV                   # 压缩前n次commit：pick为主，squash为辅
    ```
<!-- entry end -->

<!-- entry begin: git remote -->
* git remote
    * gr        ：`git remote`
    * grv       ：`git remote -v`
    * gra       ：`git remote add REMOTE URL`
    * grrm      ：`git remote remove REMOTE URL`
    * grmv      ：`git remote rename REMOTE newRepo`
    * grset     ：`git remote set-utl REMOTE URL`
    * glr       ：`git ls-remote`
<!-- entry end -->

<!-- entry begin: git push -->
* git push
    > 参数：
    >
    >       git push REMOTE 本地BRANCH            # 推送本地分支到远程仓库同名分支
    >       git push REMOTE 本地BRANCH:远程BRANCH # 推送本地分支到远程仓库分支
    >       git push REMOTE TAG                   # 推送指定TAG
    >       git push REMOTE --tags                # 推送所有TAG
    >       git push REMOTE 本地REF:远程REF       # 推送本地引用到远程仓库引用
    >       git push REMOTE --delete REF          # 删除云端仓库指定引用
    * gp        ：`git push`
    * gpf       ：`git push --force-with-lease`
    * gpf!      ：`git push --force`
<!-- entry end -->

<!-- entry begin: git fetch pull -->
* git fetch REMOTE
    * gf
* git pull REMOTE
    * gl
<!-- entry end -->

<!-- entry begin: git clone -->
* git clone URL DIR
    * --depth=1
    * --recurse-submodules
<!-- entry end -->

<!-- entry begin: git submodule -->
* git submodule
    ```sh
    git submodule add URL [Dir]               # 添加子模块信息
    git submodule init                        # 根据`.gitmodules`初始化子模块
    git submodule update                      # 拉取上游来更新子模块
    git submodule update --init               # 初始化并更新
    git submodule update --init --recursive   # 递归初始化并更新所有子模块
    ```
<!-- entry end -->

<!-- entry begin: git bundle 打包 -->
* git bundle create BUNDLE_FILE HEAD BRANCH
    > 注：最好带上HEAD自动checkout  
    > 注：可用`git clone BUNDLE_FILE|FILE_URL [DIR]`重建目录
<!-- entry end -->

<!-- entry begin: git archive 打包 -->
* git archive
    > 不同于bundle，该操作打包不带有`.git`目录
    * --preffix=PATH：指定打包路径
    * -o FILE       ：指定打包文件，归档包的格式也通过文件后缀自动判断
    * COMMIT        ：指定打包版本
<!-- entry end -->

<!-- entry begin: git filter-branch 彻底删除文件历史 -->
* 彻底删除文件历史
```bash
git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch path-to-your-remove-file' --prune-empty --tag-name-filter cat -- --all
```
<!-- entry end -->

