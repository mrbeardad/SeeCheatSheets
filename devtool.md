# 目录
- [目录](#目录)
- [gcc](#gcc)
- [gperftools](#gperftools)
- [ssh](#ssh)
- [ssl](#ssl)
- [gpg](#gpg)
- [curl](#curl)
- [git](#git)

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
| `-Wl,--wrap,symbol`                          | 链接库打桩：call: symbol -> callee: __wrap_symbol ; call: __real_symbol -> callee: symbol |
| `-mcmodel=large`                             | 默认32位地址用于相对跳转，即代码段与数据段不能超过4GB                                     |
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
4. 利用pprof展示信息
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
* `ssh-keygen -t ed25519 -a 100 -C "comment"`
  > 速度最快最安全的算法生成密钥对
* `ssh-keygen -t rsa -b 4096 -C "comment"`
  > 应用最广泛的算法生成密钥对
* `ssh-keygen -f ~/.ssh/id_de25519 -p`
  > 更改私钥的密码短语
* `ssh -p Port User@Host`
  > 连接主机
* `scp -P port <file> <user>@<host>:<dest>`
  > 利用ssh协议复制本地文件到服务器
* `scp -P port <user>@<host>:<src> <dest>`
  > 利用ssh协议复制服务器文件到本地
<!-- entry end -->

# ssl
<!-- entry begin: openssl -->
* `openssl genrsa -out server.key 2048`
  > 创建私钥
* `openssl req -new -key server.key -out server.csr`
  > 用私钥创建证书请求
* `openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt`
  > 用私钥与证书请求签发证书
* `openssl req -newkey rsa:2048 -keyout server.key -x509 -days 365 -out server.crt`
  > 同时创建私钥与自签证书
* `openssl x509 -in server.crt -signkey server.key -x509toreq server.csr`
  > 利用已有证书创建证书请求，用于续订证书
* `openssl req  -text -noout -in server.csr`
  > 查看证书请求
* `openssl x509 -text -noout -in server.crt`
  > 查看证书
* `openssl rsa -des3 -in unencrypted.key -out encrypted.key`
  > 加密私钥
* `openssl rsa -in encrypted.key -out decrypted.key`
  > 解密私钥
<!-- entry end -->

# gpg
<!-- entry begin: gpg -->
* `gpg --gen-key`
  > 创建GPG key
* `gpg --list-keys`
  > 列出GPG keys
* `gpg --edit-key <KEY_ID>`  
  `addkey`  
  `save`
  > 创建子GPG key
* `gpg --delete-keys <KEY_ID>`
  > 删除公钥
* `gpg --delete-secret-keys <KEY_ID>`
  > 删除私钥
* `gpg --output ~/public_key.txt --armor --export <KEY_ID>`
  > 导出公钥
* `gpg --output ~/private_key.txt --armor --export-secret-key <KEY_ID>`
  > 导出私钥
* `gpg --import ~/public_key.txt`
  > 导入公钥
* `gpg --allow-secret-key-import --import ~/private_key.txt`
  > 导入私钥
* `gpg --keyserver <keyserver> --search-keys 'user@emailaddress.com'`
  > 搜索公钥
* `gpg --keyserver <keyserver> --send-keys <KEY_ID>`
  > 上传公钥
* `gpg --keyserver <keyserver> --recv-keys <KEY_ID>`
  > 下载公钥
* `gpg --sign-key <KEY_ID>`
  > 签名公钥
* `gpg --output ~/revoke.asc --gen-revoke KEY_ID`  
  `gpg --import ~/revoke.asc`  
  `gpg --keyserver <keyserver> --send-keys <KEY_ID>`
  > 撤销公钥

* `gpg --encrypt --recipient 'user@emailaddress.com' example.txt`
  > 加密文件
* `gpg --output example.txt --decrypt example.txt.gpg`
  > 解密文件
* `gpg -ba example.txt`
  > 签名文件
* `gpg --verify example.txt example.txt.acs`
  > 签证文件（完整性与端点鉴别）
<!-- entry end -->

# curl
<!-- entry begin: curl -->
```sh
curl <url> -o <file>
sh -c "$(curl -fsSL <url>)"
curl cht.sh/<cmd>
curl cht.sh/<lang>/<question with + instead of space>
```
<!-- entry end -->

# git
**项目分区**
* 工作区`W`相对暂存区`S`：untraced，deleted，modified
* 暂存区`S`相对仓库区`R`：new，deleted，modified
* 仓库区`R`

**分支模型**
![图片来自网络](images/git-branch.png)
* data：
    > 这就是每个文件的每次历史的数据文件实体，每次提交其实并不会备份未改动的文件，
    > 而只要一个文件改动的，就会被全量备份而创建一个新的data对象  
    > 注：其实git会在每次push时整理data对象，将一些小改动改为增量备份而非全量备份
* tree：
    > 为每个commit管理“快照”，指向该commit所有的各个data对象
* commit：
    > 作者、提交者、日期、校验、描述、父指针（指定父commit）
* refs：
    > 指向某个commit的分支指针
    * master：本地master分支（clone检出分支时自动创建）
    * origin/master：远程仓库origin中的master分支（远程分支不可修改改）
    * HEAD：指向工作区当前检出的分支或commit

**工作流程**
* 贡献者
    1. 在github上将上游项目仓库fork成为自己的仓库
    2. clone自己fork的仓库到本地进行线下开发
    3. origin/master指向自己的仓库分支，upstream/master指向上游的
    4. 开发完成后push到自己的仓库
    5. 在github上从自己的仓库向上游仓库发起pull request(PR)
    6. 若在自己开发期间，上游仓库更新了新的commit，则会导致RP失败，需要pull upstream/master -> master再提交
* 维护者
    1. 维护者对贡献值代码进行code review(CR)
    2. 若贡献者提交的PR没有问题，则直接合并即可
    3. 若有问题则贡献者继续修改，PR会自动更新数据

**特殊文件**
<!-- entry begin: .gitconfig .gitignore .gitmodules .gitkeep -->
*.gitconfig*
* git配置，主要包括core.name与core.email

*.gitignore*
* `# comment`：注释
* `**` `*` `?` `[]`：支持的通配符
* `filename`：忽略所有名为filename的文件，递归搜索
* `dirname/`：同上，但限制为目录
* `/filename`：忽略指定路径的文件，路径相对项目根目录
* `dirname/filename`：隐式在头部添加`/`
* `!file`：取消忽略前面已忽略的文件

*.gitmodules*
* 用于管理项目依赖，由`git submodule`命令自动创建并修改
* git会在仓库中记录下子模块的依赖版本（即pull子模块哪个commit）
* 更新依赖版本，需要手动进入子模块目录修改HEAD（`commit`,`pull`...）

*.gitkeep*
* 作为一个约定俗成的文件名，用来保留空目录
<!-- entry end -->
