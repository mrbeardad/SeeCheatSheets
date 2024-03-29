# 目录

- [目录](#目录)
- [gcc](#gcc)
- [gperftools](#gperftools)
- [ssh](#ssh)
- [ssl](#ssl)
- [gpg](#gpg)
- [curl](#curl)
- [git](#git)
- [awk](#awk)
- [sed](#sed)

# gcc

<!-- entry begin: gcc -->

| 选项                                         | 功能                                                                                      |
| -------------------------------------------- | ----------------------------------------------------------------------------------------- |
| `-o`                                         | 指定输出文件                                                                              |
| `-E`/`-S`/`-c`                               | 指定编译程度 预编译(.i)/汇编(.s)/可链接二进制(.o)                                         |
| `-Og`/`-O0`/`-O1`/`-O2`/`-O3`/`-Ofast`/`-Os` | 优化编译                                                                                  |
| `-std=`                                      | 指定语言标准版本                                                                          |
| `-D`/`-U`                                    | 宏开关                                                                                    |
| `-I`                                         | 指定头文件目录                                                                            |
| `-l`                                         | 指定程序库                                                                                |
| `-Wall`                                      | 报错与警告信息                                                                            |
| `-g`                                         | 添加调试符号信息                                                                          |
| `-m32`/`-m64`                                | 指定机器字长                                                                              |
| `-static`                                    | 全静态链接（合并动态库到最终文件）                                                        |
| `-Wl,--rpath=`                               | 指定运行时动态库搜索路径                                                                  |
| `-Wl,--wrap,symbol`                          | 链接库打桩：call: symbol -> callee: **wrap_symbol ; call: **real_symbol -> callee: symbol |
| `-mcmodel=large`                             | 默认 32 位地址用于相对跳转，即代码段与数据段不能超过 4GB                                  |

<!-- entry end -->

# gperftools

<!-- entry begin: gperftool pprof -->

1. 静态链接打桩`-ltcmalloc_and_profiler`或动态链接打桩`LD_PRELOAD="/usr/lib/libtcmalloc_and_profiler.so"`
2. 设置环境变量

| 环境变量                    | 功能                                                                                                       |
| --------------------------- | ---------------------------------------------------------------------------------------------------------- |
| `HEAPPROFILE=<pre>`         | turns on heap profiling and dumps data using this prefix                                                   |
| `HEAPCHECK=<type>`          | turns on heap checking with strictness 'type': `normal` (equivalent to "`1`"), `strict`, `draconian`       |
| `CPUPROFILE=<file>`         | turns on cpu profiling and dumps data to this file.                                                        |
| `PROFILESELECTED=1`         | if set, cpu-profiler will only profile regions of code surrounded with ProfilerEnable()/ProfilerDisable(). |
| `CPUPROFILE_FREQUENCY=100`  | how many interrupts/second the cpu-profiler samples.                                                       |
| `PERFTOOLS_VERBOSE=<level>` | the higher level, the more messages malloc emits                                                           |
| `MALLOCSTATS=<level>`       | prints memory-use stats at program-exit                                                                    |

3. 运行程序
4. 利用 pprof 展示信息

```sh
pprof <path/to/binary> /tmp/prof.out
# 选项
--maxdegree=<n>     最大递归深度
--focus=<regexp>    关注指定节点
--ignore=<regexp>   忽略指定节点
--text              文本打印
--web               浏览器展示图形，需要安装graphviz包
# **注意**：逻辑使用CPU时间太短则会报告`No nodes to print`
```

![pprof](images/pprof-test.gif)

<!-- entry end -->

# ssh

<!-- entry begin: ssh -->

- `ssh-keygen -t ed25519 -a 100 -C "comment"`
  > 速度最快最安全的算法生成密钥对
- `ssh-keygen -t rsa -b 4096 -C "comment"`
  > 应用最广泛的算法生成密钥对
- `ssh-keygen -f ~/.ssh/id_de25519 -p`
  > 更改私钥的密码短语
- `ssh -p Port User@Host`
  > 连接主机
- `scp -P port <file> <user>@<host>:<dest>`
  > 利用 ssh 协议复制本地文件到服务器
- `scp -P port <user>@<host>:<src> <dest>`
  > 利用 ssh 协议复制服务器文件到本地
  <!-- entry end -->

# ssl

<!-- entry begin: openssl -->

- `openssl genrsa -out server.key 2048`
  > 创建私钥
- `openssl req -new -key server.key -out server.csr`
  > 用私钥创建证书请求
- `openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt`
  > 用私钥与证书请求签发证书
- `openssl req -newkey rsa:2048 -keyout server.key -x509 -days 365 -out server.crt`
  > 同时创建私钥与自签证书，`-nodes`不设置私钥密码
- `openssl x509 -in server.crt -signkey server.key -x509toreq server.csr`
  > 利用已有证书创建证书请求，用于续订证书
- `openssl req -text -noout -in server.csr`
  > 查看证书请求
- `openssl x509 -text -noout -in server.crt`
  > 查看证书
- `openssl rsa -des3 -in unencrypted.key -out encrypted.key`
  > 加密私钥
- `openssl rsa -in encrypted.key -out decrypted.key`
  > 解密私钥
  <!-- entry end -->

# gpg

<!-- entry begin: gpg -->

- `gpg --gen-key`
  > 创建 GPG key
- `gpg --list-keys`
  > 列出 GPG keys
- `gpg --edit-key <KEY_ID>`  
  `addkey`  
  `save`
  > 创建子 GPG key
- `gpg --delete-keys <KEY_ID>`
  > 删除公钥
- `gpg --delete-secret-keys <KEY_ID>`
  > 删除私钥
- `gpg --output ~/public_key.txt --armor --export <KEY_ID>`
  > 导出公钥
- `gpg --output ~/private_key.txt --armor --export-secret-key <KEY_ID>`
  > 导出私钥
- `gpg --import ~/public_key.txt`
  > 导入公钥
- `gpg --allow-secret-key-import --import ~/private_key.txt`
  > 导入私钥
- `gpg --keyserver <keyserver> --search-keys 'user@emailaddress.com'`
  > 搜索公钥
- `gpg --keyserver <keyserver> --send-keys <KEY_ID>`
  > 上传公钥
- `gpg --keyserver <keyserver> --recv-keys <KEY_ID>`
  > 下载公钥
- `gpg --sign-key <KEY_ID>`
  > 签名公钥
- `gpg --output ~/revoke.asc --gen-revoke KEY_ID`  
  `gpg --import ~/revoke.asc`  
  `gpg --keyserver <keyserver> --send-keys <KEY_ID>`

  > 撤销公钥

- `gpg --encrypt --recipient 'user@emailaddress.com' example.txt`
  > 加密文件
- `gpg --output example.txt --decrypt example.txt.gpg`
  > 解密文件
- `gpg -ba example.txt`
  > 签名文件
- `gpg --verify example.txt example.txt.acs`
  > 签证文件（完整性与端点鉴别）
  <!-- entry end -->

# curl

<!-- entry begin: curl -->

```sh
# 下载文件
curl -Lo filename URL
# 断点续传，文件同由URL指定
curl -OLC - URL
# 执行远程脚本
sh -c "$(curl -fsSL URL)"
# 快速查阅远程手册
curl cht.sh/<cmd>
curl cht.sh/<lang>/<question with + instead of space>
```

<!-- entry end -->

# git

**项目分区**

- 工作区`W`相对暂存区`S`：untraced，deleted，modified
- 暂存区`S`相对仓库区`R`：new，deleted，modified
- 仓库区`R`

**分支模型**
![图片来自网络](images/git-branch.png)

- data：
  > 这就是每个文件的每次历史的数据文件实体，每次提交其实并不会备份未改动的文件，
  > 而只要一个文件改动的，就会被全量备份而创建一个新的 data 对象  
  > 注：其实 git 会在每次 push 时整理 data 对象，将一些小改动改为增量备份而非全量备份
- tree：
  > 为每个 commit 管理“快照”，指向该 commit 所有的各个 data 对象
- commit：
  > 作者、提交者、日期、校验、描述、父指针（指定父 commit）
- refs：
  > 指向某个 commit 的分支指针
  - master：本地 master 分支（clone 检出分支时自动创建）
  - origin/master：远程仓库 origin 中的 master 分支（远程分支不可修改改）
  - HEAD：指向工作区当前检出的分支或 commit

**重要概念**

- `工作区`指当前检出的目录文件，`暂存区`指暂时保存到.git/index 的目录文件，`仓库`指在历史记录分支模型中创建完整的 commit 数据节点存储目录文件
- `远程仓库`存储与服务器的 git 仓库，拉取远程仓库后在本地产生只读的`远程仓库镜像`，一般需要创建一个`本地分支`来跟踪`远程分支`来修改远程分支内容

**工作流程**

- 贡献者
  1. 在 github 上将上游项目仓库 fork 成为自己的仓库
  2. clone 自己 fork 的仓库到本地进行线下开发
  3. origin/master 指向自己的仓库分支，upstream/master 指向上游的
  4. 开发完成后 push 到自己的仓库
  5. 在 github 上从自己的仓库向上游仓库发起 pull request(PR)
  6. 若在自己开发期间，上游仓库更新了新的 commit，则会导致 RP 失败，需要 pull upstream/master -> master 再提交
- 维护者
  1. 维护者对贡献值代码进行 code review(CR)
  2. 若贡献者提交的 PR 没有问题，则直接合并即可
  3. 若有问题则贡献者继续修改，PR 会自动更新数据

**特殊文件**

<!-- entry begin: .gitconfig .gitignore .gitmodules .gitkeep -->

_.gitconfig_

- git 配置，主要包括 core.name 与 core.email

_.gitignore_

- `# comment`：注释
- `**` `*` `?` `[]`：支持的通配符
- `filename`：忽略所有名为 filename 的文件，递归搜索
- `dirname/`：同上，但限制为目录
- `/filename`：忽略指定路径的文件，路径相对项目根目录
- `dirname/filename`：隐式在头部添加`/`
- `!file`：取消忽略前面已忽略的文件

_.gitmodules_

- 用于管理项目依赖，由`git submodule`命令自动创建并修改
- git 会在仓库中记录下当前子模块的依赖版本(直接存储于 tree 对象)，想要重新指定依赖版本需要进入子模块目录并执行 git 操作修改 HEAD(如 commit, pull 等)，然后再回到父模块提交修改
- 若只是简单的更新依赖版本，可执行`git submodule update --remote`
- 若想跟踪特定分支（持续更新）而非固定的 commit，可执行`git config -f .gitmodules submodule.<branch name>.branch stable`

_.gitkeep_

- 作为一个约定俗成的文件名，用来保留空目录
<!-- entry end -->

<!-- entry begin: awk -->

# awk

```sh
awk 'pattern{statment;} 模式{语句;}' file # 若未指定file则从stdin读取数据
# 选项
-F  # 指定分隔符，默认[[:space:]]
-f  # 指定执行脚本文件
-v  # 预先从Shell设置变量
```

**匹配模式**

- BEGIN
- END
- /pattern/
- !/pattern/
- expression
- 支持关系运算与逻辑运算

**变量**

- `var=val` `map[key]=val`
- 变量无需声明，初值为空(string)或 0(int)

| 内置变量   | 描述                          |
| ---------- | ----------------------------- |
| ARGC       | awk 参数个数                  |
| ARGV       | awk 参数 map                  |
| RS         | 行分隔符(默认`newline`)       |
| FS         | 字段分隔符(默认`Space`)       |
| ORS        | 输出的行分隔符(默认`newline`) |
| OFS        | 输出的字段分隔符(默认`,`)     |
| NR         | 行数                          |
| FNR        | 在当前文件中的行数            |
| NF         | 字段数                        |
| OFMT       | 数字输出格式(默认`%.6g`)      |
| IGNORECASE | 为 true 则忽略大小写          |

**表达式**
| 操作符 | 描述 |
| --------------------------------- | ------------------ |
| `(...)` | 分组 |
| `$` | 字段引用，`$0`整行 |
| `++` `--` | 自增自减 |
| `^` | 指数运算 |
| `+` `-` `!` | 正、负、逻辑非 |
| `*` `/` `%` | 乘 除 模 |
| `+` `-` | 加 减 |
| `space` | 字符连接 |
| `|` `|&` | 管道连接 |
| `<` `>` `<=` `>=` `==` `!=` | 关系运算 |
| `~` `!~` | 正则匹配 |
| `in` | 数组成员 |
| `&&` | 逻辑与 |
| `||` | 逻辑或 |
| `?:` | 条件运算 |
| `=` `+=` `-=` `*=` `/=` `%=` `^=` | 赋值 |

**IO 语句**

```sh
print expr-list       # expr-list中表达式以逗号分隔，打印每个表达式以OFS，结尾ORS
printf fmt, expr-list # fmt为C风格

print ... >file
print ... >>file
print ... | "command"
print ... |& "command"

getline                     # 立即读取缓冲区中下一行，并覆盖当前字段引用
getline var                 # 同上，但将其赋值到变量var
getline <file               # 读取文件到当前字段引用
getline var <file           # 读取文件到变量var
"command" | getline [var]   # 读取command标准输出
"command" |& getline [var]  # 读取command标准输出与标准错误

next  # 立即处理下行，跳过后续处理
```

<!-- entry end -->

<!-- entry begin: sed -->

# sed

```sh
sed '模式{语句;语句}' file # 若未指定file则从stdin读取数据
sed -e'模式 语句' -e'模式 语句' file # a i c r w操作无法共用模式匹配
# 选项
-n  # 仅显示a i c r p操作的文本行
-i  # 修改原文件
-r  # 支持扩展正则表达式
```

**模式匹配**

- /pattern/
- !/pattern/
- /pattern/I
- /pattern/,n
- n,m
- 0,$

**操作**

| 操作符      | 功能                                       |
| ----------- | ------------------------------------------ |
| a i         | 行后或行前添加字符串                       |
| c           | 会把所有匹配行转换成一个字符串             |
| r w         | 读取 file 到行后，写入匹配行到 file        |
| p           | 打印本行                                   |
| s/pat/repl/ | 文本替换: 默认首次替换, g 全部，Ng 第 N 个 |
| d           | 删除匹配行                                 |
| n           | 立即处理下一行，跳过后续操作               |
| q           | 退出                                       |

<!-- entry end -->
