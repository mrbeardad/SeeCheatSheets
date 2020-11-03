# 目录
<!-- vim-markdown-toc GFM -->

- [文件搜索](#文件搜索)
- [文件I/O](#文件io)
- [流处理命令](#流处理命令)
  - [常用命令](#常用命令)
  - [编辑三剑客](#编辑三剑客)
    - [grep](#grep)
    - [awk](#awk)
    - [sed](#sed)
- [文件与目录](#文件与目录)
  - [文件信息](#文件信息)
  - [目录文件](#目录文件)
  - [符号链接](#符号链接)
  - [硬链接](#硬链接)
  - [权限与属主](#权限与属主)
  - [文件日期](#文件日期)
- [系统数据文件和信息](#系统数据文件和信息)
- [日期与时间](#日期与时间)
- [UEFI BIOS](#uefi-bios)
  - [规范](#规范)
  - [安全启动](#安全启动)
- [硬盘与文件系统](#硬盘与文件系统)
  - [分区工具](#分区工具)
  - [LVM](#lvm)
  - [XFS](#xfs)
  - [挂载](#挂载)
  - [归档包](#归档包)
- [用户与登录](#用户与登录)
- [系统资源](#系统资源)
  - [CGroup](#cgroup)
  - [进程管理](#进程管理)
- [日志](#日志)
- [systemd](#systemd)
  - [units配置](#units配置)
    - [[Unit]](#unit)
    - [[Install]](#install)
    - [[Service]](#service)
    - [[Timer]](#timer)
    - [[Socket]](#socket)
  - [命令](#命令)
- [GRUB](#grub)
  - [GRUB配置](#grub配置)
  - [GRUB Shell](#grub-shell)
  - [GRUB选单](#grub选单)
  - [系统救援](#系统救援)
- [主机配置](#主机配置)
- [计算机网络](#计算机网络)
  - [网络配置](#网络配置)
  - [NetworkManager](#networkmanager)
  - [其它网络命令](#其它网络命令)
- [其他命令](#其他命令)

<!-- vim-markdown-toc -->
# 文件搜索
<!-- entry begin: whereis -->
* whereis       ：搜索可执行，头文件和帮助信息的位置，使用系统内建数据库
<!-- entry end -->

<!-- entry begin: locate updatedb -->
* locate
    * -i        ：忽略大小写
    * -r        ：正则表达式（默认通配符）
    * -c        ：显示匹配的文件数量
    * -S        ：显示数据库信息
* updatedb
    > 更新locate命令需要的数据库  
    > 配置  ：/etc/updatedb.conf  
    > 数据库：/var/lib/mlocate/mlocate.db  
<!-- entry end -->

<!-- entry begin: find -->
* find *DIR* *OPTS...*
    * 打印：
        * -print
    * 目录深度
        * -depth
    * 正则名字：
        * -regex
        * -iregex
    * 通配符名字：
        * -name
        * -iname
    * 用户：
        * -uid
        * -gid
        * -user
        * -group
        * -nouser
    * 权限：
        * -writable
        * -readable
        * -executable
        * -perm
            > `-`表示权限为指定范围的非空子集  
            > `/`表示权限为指定范围的非空交集  
            > `无前缀`表示精准权限
    * 大小：
        * -size
            > `+`表示大于  
            > `-`表示小于  
            > 单位用c/k/M/G
    * 时间：
        * -[a|m|c]time *n*  ：指定`n*24 h`之前的文件，`+/-`表示更早/更晚
        * -[a|m|c]min  *n*  ：指定`n min`之前的文件
        * -[a|m|c]newer *file*
    * 文件类型：
        * -type [f|d|l|b|c|p|s]
    * 其他信息：
        * -links            ：硬连接数
        * -inum             ：inode号
    * 复合逻辑关系          ：-a、-o、-not
    * 对搜索到的文件执行CMD ：-exec  *CMD*  {} \;
<!-- entry end -->

# 文件I/O
<!-- entry begin: sync -->
* sync
<!-- entry end -->

<!-- entry begin: echo -->
* echo
    * -n        ：不自动加入换行符（zsh会将无换行结尾的输出的尾部标记`反显的%`）
    * -e        ：启用转义语义（zsh自动开启）
<!-- entry end -->

<!-- entry begin: split -->
* split *FILE* *Preffix*
    > 注：preffix最后最好加dot，大小单位可指定，默认byte
    * -b        ：按大小分割
    * -l        ：按行数分割
<!-- entry end -->

<!-- entry begin: iconv  FILE -->
* iconv  FILE
    * -f        ：原字符集
    * -t        ：目标字符集
    * -o        ：输出文件
    * --list    ：列出可选字符集
<!-- entry end -->
<!-- entry begin: read -->
* read VARNAME
    * -p    ：提示符
    * -t    ：限时
    * -n    ：读取字符数
    * -s    ：关闭回显
<!-- entry end -->

<!-- entry begin: cat -->
* cat：打印每行
    * -A        ：打印特殊空白符
    * -n        ：显示行号
    * -b        ：显示行号但空行不算行号
* tac：翻转首尾
* rev：每行翻转
<!-- entry end -->

<!-- entry begin: head -->
* head
    * -v        ：显示文件名
    * -n        ：指定显示多少行
<!-- entry end -->

<!-- entry begin: tail -->
* tail
    * -v        ：显示文件名
    * -n        ：指定显示多少行
    * -f        ：监听
<!-- entry end -->

<!-- entry begin: xxd -->
* xxd：以十六进制读取文件数据
    * -l        ：指定读取字节数
<!-- entry end -->

# 流处理命令
## 常用命令
<!-- entry begin: col -->
* col -x ：将tab替换为等宽space，该命令只从stdin读取
<!-- entry end -->

<!-- entry begin: wc -->
* wc
    * -l：行数
    * -w：单词数
    * -m：字符数
<!-- entry end -->

<!-- entry begin: xargs -->
* xargs CMD
    > 原理：将管道中的字符串的空白符符替换为空格后（若空白符前面有`\`则不被替换），作为CMD的参数
    * -n  ：一次给予多少个参数
    * -p  ：每个单词都提醒用户
    * -e  ：直到遇到此单词停止
    * -0  ：保留空白符不被替换
<!-- entry end -->

<!-- entry begin: tee -->
* tee FILE
    * -a：追加而非截断
<!-- entry end -->

<!-- entry begin: sort -->
* sort
    * -f    ：忽略大小写
    * -b    ：忽略行前空白
    * -M    ：月份名称排序
    * -n    ：数值排序
    * -r    ：降序
    * -u    ：删除相同行
    * -t    ：指定分隔符
    * -k n,m：指定比较第n-m域
        > field定义：blank开始到non-blank尾结束，从行首到分割符算1
<!-- entry end -->

<!-- entry begin: uniq -->
* uniq
    * -i：忽略大小写
    * -c：计数
    * -f：指定从哪个域开始比较（默认从1开始）
<!-- entry end -->

<!-- entry begin: join paste -->
* join  -1 F1 FILE1 -2 F2 FILE2
    > F1、F2：指定两文件比较的域  
    > FILE1与FILE2其中之一可以为`-`，代表此文件从管道读取
    * -t：指定分隔符
    * -i：忽略大小写

* paste FILE1  FILE2
    * -t：指定分隔符
<!-- entry end -->

<!-- entry begin: cut -->
* cut
    * -d    ：指定分隔符
    * -f    ：指定截取的域，可使用逗号与减号
    * -c n-m：指定取出第n-m个字符
<!-- entry end -->

## 编辑三剑客
### grep
<!-- entry begin: grep -->
* grep
    * -E：扩展正则表达式
    * -A：显示之后几行
    * -B：显示之前几行
    * -C：显示之前和之后几行
    * -i：忽略大小写
    * -n：显示行号
    * -c：计数
    * -v：显示不匹配的
    * -q：静默模式
<!-- entry end -->

### awk
<!-- entry begin: awk -->
* awk '模式{语句;} 模式{语句;}' FILE
    * 语句语法类似C语言，额外有语句`for ( var in map )`；
    * 使用变量时无需前置声明，初值为空，参与运算时置0；
    * 参与运算时map下标自动建立，故检查下标是否存在用in；
    * 关系运算符要两边都是数字才进行数值比较，否则以字典序比较字符串；

* 选项
    * -F    ：分隔符，支持[]正则
    * -f    ：读取脚本文件
    * -v var=val

* 模式：
    * BEGIN
    * END
    * /pattern/
    * !/pattern/
    * /pattern1/&&/pattern2/
    * /pattern1/||/pattern2/
    * 表达式

* 变量：C风格变量
    * 定义：
        * var=val
        * map[key]=val
    * 内置变量
        * ARGC
        * ARGV
        * RS        ：行分隔符(默认`<CR>`)
        * FS        ：字段分隔符(默认`<Space>`)
        * ORS       ：输出的行分隔符(默认`<CR>`)
        * OFS       ：输出的字段分隔符(默认`,`)
        * NR        ：总行数
        * FNR       ：在当前文件中的行数
        * NF        ：字段数
        * OFMT      ：数字输出格式(默认%.6g)
        * IGNORECASE：为true则忽略大小写
        * `$0`        ：整行内容
        * `$N`        ：第N个字段

* 运算符
    * `+` `-` `*` `/` `%` `^`
    * `>` `>=` `<` `<=` `==` `!=`
    * `~`           ：子串匹配
    * `~!`          ：子串不匹配
    * `空格`        ：字符连接
    * `in`          ：确认数组键值
    * `|`           ：管道连接符
    * `"cmd" |`     ：管道连接，读取`cmd`输出
    * `| "cmd"`     ：管道连接，输出到`cmd`

* 关键字
    * 输出
        > 注意：`>`与`>>`可在以上两个关键字最后作重定向，若需要关系运算，应该用圆括号包围
        * `print var1,var2"str"`        ：标准输出，逗号替换为OFS，末尾自动添加换行符
        * `printf "$1:%s\n",$1`         ：标准输出，类似C中的printf()函数
        * `print或printf语句 | "cmd"`   ：管道输出
        * `print或printf语句 > "file"`  ：文件输出（截断）
        * `print或printf语句 >> "file"` ：文件输出（追加）
    * 输入
        * `"cmd" | getline var`         ：管道输入，若无`var`则直接赋值给`$0`
        * `getline var < "file"`        ：文件输入，若无`var`则直接赋值给`$0`
    * 其他
        * `continue`
        * `break`
        * `next`                            ：读取下一行并回到脚本开始处重新执行
        * `delete`                          ：删除map中的元素

* 函数
    * srand(); rand()
        > 前者帮助后者用时间作种，后者返回随机值
    * length(string); length(map)
        > 检查字符串与数组的长度
    * sub(/pattern/, "string", var); gsub(...)
        > 将var中匹配的pattern替换为string，后者全部替换
    * index(var, "string")
        > string在var中出现的位置(1开始)，无则返回0
    * substr(var, pos, length)
        > 截取子串，从pos开始的length个字符(1开始)，无length则到结尾
    * tolower(s); toupper(s)
<!-- entry end -->

### sed
<!-- entry begin: sed -->
* sed '模式{语句;}; 模式{语句;}' FILE
* sed -e '模式 语句块' -e '模式 语句块' FILE
    * a; i; c; r; w;操作只能单独存在，不能在块中，故它们只能用`-e`选项

* 选项
    > 选项要分开
    * -n：仅显示处理结果，只有a i c r p操作才输出
    * -r：支持扩展正则表达式
    * -i：修改原文件
    * -f：指定脚本文件

* 模式
    * /pattern/
    * !/pattern/
    * /pattern/I
    * /pattern/,m
    * n,m

* 操作
    * s/pattern/replace/g
        * replace中&代表整个匹配到的字符串
        * 替换标记：无g全部，Ng第N个
    * a或i      ：行后或行前添加字符串
    * c         ：会把所有匹配行转换成一个字符串
    * d         ：删除匹配行
    * r/w file  ：读取file到行后，写入匹配行到file
    * n         ：跳转到下一行，执行后续操作
    * =         ：行前打印行号
    * q         ：退出
    * N         ：读取下一行至模板块，形成多行组，并将行号改为新加行的行号
    * P/D操作   ：分别用于多行组，打印与删除第一行
    * h         ：拷贝模板块到缓冲区(模板块即当前处理的行)
    * H         ：追加模板块到缓冲区
    * g         ：获得内存缓冲区的内容替代当前行
    * G         ：获得内存缓冲区的内容追加到当前行后
<!-- entry end -->

# 文件与目录
## 文件信息
<!-- entry begin: stat -->
* stat
    * `默认`    ：显示文件详细信息
    * -f        ：显示文件所处文件系统信息
<!-- entry end -->

<!-- entry begin: ls -->
* ls
    * -d        ：显示目录本身
    * -i        ：显示inode
    * -L        ：显示符号链接的目标
    * -n        ：显示uid与gid
    * -R        ：目录递归显示
    * -s        ：显示文件占用block大小
    * -S        ：按大小排序（降序）
    * -t        ：按modify时间排序（降序）
    * -Z        ：安全上下文
<!-- entry end -->

<!-- entry begin: du -->
* du
    * -sh       ：显示目标占用block大小
<!-- entry end -->

<!-- entry begin: file -->
* file
    * -i        ：文件的格式信息
<!-- entry end -->

## 目录文件
<!-- entry begin: mkdir rmdir chdir pwd -->
* mkdir
    * -p        ：递归
    * -m        ：设置权限

* rmdir

* chdir

* pwd
    * -P        ：显示真实路径而非软连接

* chroot
<!-- entry end -->

## 符号链接
<!-- entry begin: readlink ln -->
* readlink
* ln *SRC* *TAG*
    * `默认`    ：硬连接
    * -s        ：软连接
    * -f        ：强制覆盖
<!-- entry end -->

## 硬链接
<!-- entry begin: rm -->
* rm
    * -r        ：目录递归
    * -v        ：详述
    * -f        ：强制覆盖
<!-- entry end -->

<!-- entry begin: mv -->
* mv *SRC*  *TAG*
    * -v        ：详述
    * -i        ：询问是否覆盖
    * -u        ：只更新
    * -f        ：强制覆盖
    * -n        ：直接不覆盖
<!-- entry end -->

<!-- entry begin: cp -->
* cp  *SRC*  *TAG*
    * -a        ：尽可能复制所有信息
    * -r        ：目录递归
    * -v        ：详述
    * -i        ：询问是否覆盖
    * -u        ：只更新
    * -f        ：强制覆盖
    * -n        ：直接不覆盖
<!-- entry end -->

<!-- entry begin: mktemp -->
* mktemp：创建并打印临时文件
    * -d    ：创建临时目录而非普通文件
<!-- entry end -->

## 权限与属主
<!-- entry begin: linux 权限信息 -->
* 权限信息
    > 只有owner与root能修改，使用stat命令可以查看详细信息  
    > `umask`即默认权限掩码，设置后会掩盖默认权限，默认为022，从而目录默认权限为755，文件默认644
    * Uid与Gid      ：指明文件所属的owner与group
    * `r w x`       ：**读/写/执**权限，以八进制数字表示时从高位到低位依次代表是否具有**读/写/执**权限
    * `sS sS tT`    ：**SUID/SGID/SBIT**特殊权限，以八进制表示时从高位到低位依次表示是否具有**SUID/SGID/SBIT**权限
    * 7777          ：4位八进制数，第一个表示特殊权限，后三个分别表示owner/group/other的读/写/执权限

* 对普通文件
    * r/w表示可读/写其对应block
    * x表示可以执行该文件形成进程
    * SUID表示执行时环境变量EUID改为owner
    * SGID表示执行时环境变量EGID改为group

* 对目录
    * r/w表示可读/写其对应block内存储的entry
    * x表示能否对目录下文件进行访问，即使没有rw也可以“摸黑访问”
    * SGID表示所有创建在此目录的普通文件的gid默认为目录的gid
    * SBIT表示该目录下的文件只有其owner与目录owner能删除或更名
<!-- entry end -->

<!-- entry begin: setfacl getfacl -->
* ACL权限
    * setfacl
        > 设置ACL权限，优先级在owner和group之后
    * getfacl
<!-- entry end -->

<!-- entry begin: chattr lsattr -->
* 额外属性
    * chattr
        > 设置文件额外属性
        * -R：    递归目录
        * 以下选项的前缀，-设置，+取消
            * a   ：    只能追加
            * i   ：    无法变更
            * A   ：    不更新atime
            * S   ：    同步存储文件
            * d   ：    不被dump
    * lsattr
<!-- entry end -->

<!-- entry begin: su -->
* su
    * `-`         ：转为root
    * `- user`    ：转为user
    * `-c`        ：用对应目标用户执行一条命令
<!-- entry end -->

<!-- entry begin: sudo -->
* sudo CMD
    * -u        ：使用目标用户权限(仅root可用)
    * -l        ：列出本用户sudo信息
    * -b        ：后台执行
<!-- entry end -->

<!-- entry begin: visudo -->
* visudo
    > /etc/sudoers与/etc/sudoers.d  
    > user host=(root) cmd，!cmd  
    > %grp host=(%root) NOPASSWD:ALL
<!-- entry end -->

<!-- entry begin: umask chmod -->
* test
    > [见](bash.md)
* umask
* chmod [+- ] PERM PATH
    * -R        ：递归目录
* chown OWNER:GROUP PATH
    * -R        ：递归目录
<!-- entry end -->

## 文件日期
<!-- entry begin: touch -->
* touch
    * `默认`    ：修改a/m/ctime
    * -a        ：只改atime
    * -m        ：只改mtime
    * -d        ：指定touch的时间，date模式
<!-- entry end -->

# 系统数据文件和信息
<!-- entry begin: linux 登录文件 -->
* 登录文件
    > 由PAM模块控制登录验证
    * /etc/nologin   ：若存在则只允许root登陆
    * /etc/issue     ：本地控制台登陆提示
    * /etc/motd      ：远程登录提示
    * /etc/login.defs：用户登陆设置
<!-- entry end -->

<!-- entry begin: 用户文件 -->
* 用户文件
    * /etc/passwd
        * `用户名:密码:UID:GID:描述信息:主目录:默认Shell`
    * /etc/shadow
        * `用户名:加密密码:最后一次修改时间:最小修改时间间隔:密码有效期:密码需要变更前的警告天数:密码过期后的宽限时间:账号失效时间:保留`
        > 在密码前加上 "!"、"*" 或 "x" 使密码暂时失效
    * /etc/group
        * `组名:密码:GID:组附加用户列表`
    * /etc/gshadow
        * `组名:加密密码:组管理员:组附加用户列表`
    * /etc/skel/：建立用户主目录时拷贝此目录
<!-- entry end -->

<!-- entry begin: last lastlog lastb w journalctl -->
* 系统日志
    * w         ：系统现在的登录情况
        > /var/run/utmp
    * last      ：系统的启动与用户登陆日志
        > /var/log/wtmp
    * lastlog   ：每个用户最后一次登陆时间
        > /var/log/lastlog
    * lastb     ：上次错误登录记录
        > /var/log/btmp
    * journalctl：系统管理日志
        > /var/log/journal/*
<!-- entry end -->

<!-- entry begin: 主机配置 -->
* 主机配置
    * /etc/shells           ：可用shell
    * /etc/services         ：服务名与端口对照
    * /etc/hosts            ：已知主机名
    * /etc/hostname         ：主机名
    * /etc/resolv.conf      ：DNS服务器
    * /etc/locale.conf      ：语系与字符集
    * /etc/localtime        ：本地时区/usr/share/zoneinfo/
    * /etc/adjtime          ：系统时间校准类型
<!-- entry end -->

<!-- entry begin: uname -->
* uname -a
<!-- entry end -->

<!-- entry begin: 字体 fontconfig mkfontdir mkfontscale fc-cache fc-list -->
* 字体服务
    * fc-list       ：字体缓存查看
    > 以下三条命令为字体安装三部曲
    * mkfontdir
    * mkfontscale
    * fc-cache -f
<!-- entry end -->

<!-- entry begin: hostnamectl -->
* hostnamectl
    * set-hostname
<!-- entry end -->

<!-- entry begin: locale  localectl -->
* locale [-a]
* localectl
<!-- entry end -->

<!-- entry begin: timedatectl -->
* timedatectl
    * set-timezone
    * set-local-rtc
    * set-ntp     ：chronyd服务
<!-- entry end -->

# 日期与时间
<!-- entry begin: date -->
* date
    * +timeformat
        > * `%Y %y`         ：年份、年份后两位
        > * `%m %b %B`      ：月份、月份单词缩写、月份单词
        > * `%d %j %s`      ：一月中第几天、一年中第几天、从epoch开始的秒数
        > * `%H %M %S`      ：时（24小时值）、分、秒
        > * `%a %A`         ：礼拜单词缩写、礼拜单词
    * -d *timeformat*       ：将参数转换为时间
    * -d @`N`               ：epoch之后N秒
    * -d '19700101  Ndays'  ：epoch之后N天
<!-- entry end -->

<!-- entry begin: cal -->
* cal [MONTH] [YEAR]
<!-- entry end -->

<!-- entry begin: time -->
* time
<!-- entry end -->

# UEFI BIOS
## 规范
* 主要规范
    * 读取GPT分区表
    * 读取ESP
        > 设置GPT类型，文件系统类型为FAT家族
    * 能够执行EFI可执行文件

* 启动管理器
    * 添加、删除、修改启动项
        > 默认启动项位于每个ESP的/EFI/BOOT/BOOT{x64,IA32,IA64,ARM,A64}.EFI  
        > 用于那些没有指定硬盘上具体目标，而只是让固件自行搜索的启动项
    * 设置启动项顺序
    * 设置启动项模式（UEFI or BIOS）

## 安全启动
UEFI 规范规定固件可以包含一系列签名，并拒绝运行未签名或签名与固件中包含的签名不一致的 EFI 可执行文件。

**关于Microsoft Windows**：

* 限制了需要获得预装Windows的批量许可（即“微软认证”）的在售PC：
    * 默认启用安全启动 (Secure Boot)（服务器除外）
    * 在其信任密钥列表中包含微软的密钥
    * 启用安全启动 (Secure Boot) 时，必须禁用 BIOS 兼容模式（如果没记错的话，UEFI 规范也有此要求）
    * 支持签名黑名单

* 符合微软认证要求的 x86 计算机 还必须满足以下附加条件：
    * 允许 自然人禁用安全启动 (Secure Boot)
    * 允许 自然人启用自定义模式，以及修改固件的信任密钥列表

* 符合微软认证要求的 ARM 计算机 还必须满足以下附加条件：
    * 不允许 自然人禁用安全启动 (Secure Boot)
    * 不允许 自然人启用自定义模式，以及修改固件的信任密钥列表


# 硬盘与文件系统
<!-- entry begin: GPT 分区表 -->
* GPT分区表组成：
    * MBR保护
        > 防止不识别GPT的程序误读
    * GPT头  
        > 磁盘GUID、GPT版本、校验、分区项信息等
    * 分区项  
        > 分区GUID、类型、属性、标签、分区位置
    * 分区
        > 存储文件系统数据的硬盘区块
    * 备份区  
        > GPT头与分区项的备份
<!-- entry end -->

<!-- entry begin: xfs 文件系统 -->
* XFS文件系统：
    * 结构：
        * 分配组
            * 超级块(只在第一个分配组)
            * 空闲空间索引(B+tree)
            * Inode索引(B+tree)
        * Inodes
        * Blocks
        * Log
        * Realtime
    * XFS特性
        * 分配组的并行性
        * 日志与恢复
        * 延迟分配
        * 扩展属性
<!-- entry end -->

## 分区工具
<!-- entry begin: gdisk 分区 -->
* gdisk  DEV [-l]
    * m ：普通模式
    * x ：专家模式
    * r ：恢复模式
    * ? ：帮助

* partprobe：让内核重载硬盘信息
<!-- entry end -->

<!-- entry begin: parted 分区 -->
* parted DEV
    * p
    * mktable
    * mkpart *LABLE* *START* *END*
    * resizepart *NUM* *END*

    * rm *NUM*
    * name *NUM* *NAME*
    * set *NUM* *FLAG* on/off
        * FLAG       ：raid  lvm  boot  hidden  diag

* partprobe：让内核重载硬盘信息

* 分区标签
```
    parted-Flag      Type                     Attr
        hidden         -                        0
        boot        EFI System                  -
        diag        Windows RE                  -
        msftres     Micorsoft reserved          -
```
<!-- entry end -->

## LVM
<!-- entry begin: lvm pv -->
* LVM-PV阶段
    * pvs
    * pvdisplay  *PV*
    * pvcreate  *DEV*
    * pvremove  *DEV*
<!-- entry end -->

<!-- entry begin: lvm vg -->
* LVM-VG阶段
    * vgs
    * vgdisplay  *VG*
    * vgcreate *VG*  *PV*
        * -s ：指定PE大小
    * vgextend *VG*  *PV*
    * vgreduce *VG*  *PV*
    * vgchange  -a `y/n`
    * vgremove  *VG*
<!-- entry end -->

<!-- entry begin: lvm lv -->
* LVM-LV阶段
    * lvs
    * lvdisplay  *LV*
    * lvcreate
        * -L *SIZE* -n *LV*                 ：普通LV
        * -L *SIZE* -T *VG/LVpool*          ：建立LVpool
        * -V *SIZE* -T *VG/LVpool* -n *LV*  ：在指定LVpool中建立LV
        * -s -L *SIZE* -n *LVS* *VG/LV*     ：建立快照
    * lvresize
        * -L +|-*SIZE*
    * lvchange  -a y|n
    * lvremove  *LV*
<!-- entry end -->

## XFS
<!-- entry begin: mkfs.xfs -->
* mkfs.xfs  *DEV*
    * -f        ：强制
    * -L        ：设置文件系统标签
    * -b size=
    * -i  size=
    * -d agcount=
    * -d file
    * -l  external,logdev=  ,size=
<!-- entry end -->

<!-- entry begin: xfs_admin -->
* xfs_admin
    * -l        ：查看文件系统标签
    * -L        ：修改标签
    * -u        ：查看文件系统uuid
    * -U        ：修改uuid
<!-- entry end -->

<!-- entry begin: xfsdump xfsrestore -->
* xfsdump *MP*
    > 注 ：挂载点*MP*末不能带/号
    * -l        ：备份级别0为全量，其他在前一级基础上增量
    * -L        ：备份文件标签
    * -M        ：备份设备标签
    * -f        ：指定备份文件
    * -e        ：排除属性含d的文件
    * -s        ：指定目录（无增量备份）
* xfsrestore *MP*
    * -f        ：指定使用的备份文件
    * -s        ：只还原指定的文件或目录
    * -I        ：查询基础数据库/var/lib/xfsdump/inventory/
<!-- entry end -->

<!-- entry begin: xfs_repair -->
* xfs_repair
    * -f        ：对image-file修复
    * -n        ：只检测
    * -l        ：指定logdev
    * -d        ：用于单用户模式强制恢复以ro挂载的/
<!-- entry end -->

<!-- entry begin: xfs_info xfs_growfs -->
* xfs_info
* xfs_growfs
<!-- entry end -->

## 挂载
> 文件系统抽象由内核的VFS模块提供，Unix一切皆文件。  
> 其中包括：  
> 硬盘文件系统（文件系统驱动）  
> 内存文件系统（内核接口）  
> 硬件设备（udev映射）

<!-- entry begin: lsblk -->
* lsblk
    * -f        ：显示文件系统类型
    * -m        ：权限及所有者
<!-- entry end -->

<!-- entry begin: blkid  -->
* blkid [DEV]
    * 文件系统标签与类型
    * 文件系统UUID（fstab所用）
    * 分区标签
    * 分区UUID
<!-- entry end -->

<!-- entry begin: df -->
* df
    * -T        ：显示文件系统类型
    * -h        ：人性化size
    * -i        ：显示inode使用情况
<!-- entry end -->

<!-- entry begin: mount -->
* mount DEV MP
    * -a        ：忽略其他参数，按/etc/fstab挂载
    * --bind    ：转移挂载点
    * --rbind   ：递归转移挂载点
    * -t        ：指定文件系统类型(sysfs,proc,devtmpfs,tmpfs)
    * -o        ：
        * remount       ：重新挂载
        * default       ：默认属性
        * loop          ：挂载loop设备
        * logdev=       ：挂载xfs时指定logdev
        * nouuid        ：用于挂载LVM快照
        * iocharset=    ：字符集
        * ro/rw
        * sync/async
        * atime/noatime
        * suid/nosuid
        * exec/exec
        * userquota/grpquota/quota/noquota
* umount
<!-- entry end -->

## 归档包
制作归档包时，应该让解包出来的文件都在一个目录中，故一般在要打包的目录的父目录进行操作从而将整个目录打包

<!-- entry begin: zip -->
* zip ZIPFILE  FILES
    * -[1-9]        ：压缩等级，越大压缩比越高
    * -r            ：打包目录时指定递归
    * -e或-P密码    ：加密
* unzip ZIPFILE [LIST]
    > `LIST`指定提取ZIPFILE中的哪些文件
    * -p            ：解压文件到stdout
<!-- entry end -->

<!-- entry begin: gzip bzip2 xz -->
* gzip/bzip2/xz
    * -[1-9]    ：压缩等级，越大压缩比越高
    * -k        ：保存原文件不删除
    * -l        ：查看压缩包信息
<!-- entry end -->

<!-- entry begin: tar -->
* tar
    * -[z|j|J]    ：gzip | bzip2 | xz
    * -[c|x|t|u]  ：打包|解包|查询|更新
    * --delete    ：删除
    * -f          ：指定压缩文件名
    * -v          ：详述
    * -p          ：保留权限等信息
    * --exlcude   ：排除，pattern
    * -C          ：解包时指定路径
<!-- entry end -->

<!-- entry begin: dd -->
* dd
    * if=
    * skip=
    * of=
    * seek=
    * bs=
    * count=
    * conv=
        * lcase      ：小写
        * ucase      ：大写
        * notrunc    ：不截断，覆盖
> 例：dd if=*manjaro.iso* of=*usb-dev* bs=8M oflag=sync status=progress
<!-- entry end -->

<!-- entry begin: losetup -->
* losetup */dev/loop0*  *loopfile*
    > 制作loop设备
* losetup -d */dev/loop0*
    > 解除loop设备
<!-- entry end -->

# 用户与登录
<!-- entry begin: id groups -->
* id    ：该用户信息
* groups：该用户参与的group
<!-- entry end -->

<!-- entry begin: useradd -->
* useradd
    * -u        ：UID
    * -g        ：GID
    * -G        ：附加组
    * -c        ：描述信息
    * -d        ：主目录绝对路径
    * -m        ：自动创建主目录
    * -s        ：默认shell
    * -o        ：允许用户UID相同
    * -r        ：系统用户(1-499)
    * -D        ：查看或修改默认配置/etc/default/useradd
<!-- entry end -->

<!-- entry begin: usermod -->
* usermod
    * -l        ：用户名
    * -u        ：UID
    * -g        ：GID
    * -G        ：附加组
    * -c        ：描述信息
    * -d        ：主目录绝对路径
    * -s        ：默认shell
<!-- entry end -->

<!-- entry begin: userdel -->
* userdel
    * -r        ：删除主目录，邮箱需要手动删除
<!-- entry end -->

<!-- entry begin: passwd -->
* passwd
    * -S        ：查看信息
    * -l        ：锁定用户
    * -u        ：解锁用户
    * --stdin   ：指明从管道读取密码
<!-- entry end -->

<!-- entry begin: chage -->
* chage
    * -l        ：详情
    * -d        ：最后一次修改YYYY-MM-DD，为0强制修改
    * -m        ：最小修改间隔天数
    * -M        ：最大修改间隔天数(密码过期后强制修改)
    * -W        ：密码到期提前警告天数
    * -I        ：宽限天数，这段时间用户可以登录，但会强制其修改密码
    * -E        ：失效日期YYYY-MM-DD，-1则无
<!-- entry end -->

<!-- entry begin: groupadd -->
* groupadd
    * -g        ：GID
    * -r        ：系统群组
<!-- entry end -->

<!-- entry begin: groupmod -->
* groupmod
    * -g        ：GID
    * -n        ：组名
<!-- entry end -->

<!-- entry begin: gpasswd -->
* gpasswd
    * -A        ：管理员
    * -r        ：移除群组密码
    * -R        ：密码失效
    * -M        ：将用户加入群组(root)
    * -a        ：将用户加入群组
    * -d        ：移除用户
<!-- entry end -->

<!-- entry begin: groupdel      ：不能删除初始组 -->
* groupdel      ：不能删除初始组
<!-- entry end -->

<!-- entry begin: newgrp        ：启动新shell并修改GID -->
* newgrp        ：启动新shell并修改GID
<!-- entry end -->

# 系统资源
## CGroup
<!-- entry begin: cgroup -->
* 基础组件：
    * hierarchy     ：/sys/fs/cgroup/*
    * cgroup        ：/sys/fs/cgroup/*/**/*
* 接口文件：
    * cgroup.clone_children ：是否继承父cgroup（默认0）
    * cgroup.procs          ：加入该cgroup的GID
    * task                  ：加入该cgroup的PID

系统创建hierarchy后，所有进程都会加入该hierarchy的根cgroup；  
同一进程可同时加入多个不同hierarchy中的cgroup；  
新建的cgroup可以继承父cgroup的配置；
<!-- entry end -->


## 进程管理
<!-- entry begin: 进程标识 -->
* 进程标识
    * STATE
    * CMD
    * TTY
    * UID、EUID、GID、EGID
    * PID、PPID、PGID、SID、TPGID
    * SELinux-Context
<!-- entry end -->

<!-- entry begin: CPU使用时间 -->
* CPU使用时间
    * real：运行期间流逝的时间
    * sy  ：内核进程
    * us  ：用户进程(un-niced)
    * ni  ：用户进程(niced)
    * id  ：空闲资源
    * wa  ：等待I/O
    * hi  ：硬中断请求服务
    * si  ：软中断请求服务
    * st  ：虚拟机偷取的时间，即虚拟CPU等待实际CPU
<!-- entry end -->

<!-- entry begin: 进程优先级 -->
* 进程优先级
    * PRI(Priority)与NI(Nice)
        * PRI (最终值) = PRI (原始值) + NI
        * PRI值是由内核动态调整的，用户不能直接修改，只能通过修改 NI 值来影响 PRI 值，间接地调整进程优先级
    * NI 值越小，进程的 PRI 就会降低，该进程就越优先被 CPU 处理
    * NI 范围是 -20~19。
    * 普通用户调整 NI 值的范围是 0~19，而且只能调整自己的进程。
    * 普通用户只能调高 NI 值，而不能降低
    * 只有 root 用户才能设定进程 NI 值为负值，而且可以调整任何用户的进程
<!-- entry end -->

<!-- entry begin: signal -->
* signal
<!--  -->
| 信号编号 | 信号名称          | 信号描述                   | 备注                                                         | 默认处理方式     |
|----------|-------------------|----------------------------|--------------------------------------------------------------|------------------|
| 1        | SIGHUP            | 会话挂断                   | 连接断开时发射给会话首进程、会话首进程终止时发射给前台进程组 | 终止             |
| 2        | SIGINT            | 终端中断                   | 一般由`Ctrl+C`发射给前台进程组                               | 终止             |
| 3        | SIGQUIT           | 终端退出                   | 一般由`Ctrl+\`发射给前台进程组                               | 终止、core dump  |
| 4        | SIGILL            | 非法指令                   | 执行非执行页中指令、堆栈溢出                                 | 终止、core dump  |
| 5        | SIGTRAP           | 跟踪陷入                   | 一般用来将控制转移至调试程序                                 | 终止、core dump  |
| 6        | SIGABRT           | 异常终止(abort)            | 异常终止                                                     | 终止、core dump  |
| 7        | SIGBUS            | 非法地址                   | 地址对齐问题                                                 | 终止、core dump  |
| 8        | SIGFPE            | 算术异常                   | 如除以0、浮点溢出等                                          | 终止、core dump  |
| 9        | SIGKILL           | 强制终止进程               | 不可捕获                                                     | 终止             |
| 10       | SIGUSR1           | 用户自定义信号1            |                                                              | 终止             |
| 11       | SIGSEGV           | 无效内存引用（段错误）     | 访问未分配的地址                                             | 终止、core dump  |
| 12       | SIGUSR2           | 用户自定义信号2            |                                                              | 终止             |
| 13       | SIGPIPE           | 写至无读进程的管道         |                                                              | 终止             |
| 14       | SIGALRM           | 定时器超时(alarm)          | 调用alarm函数发射此信号给调用进程                            | 终止             |
| 15       | SIGTERM           | 终止进程                   |                                                              | 终止             |
| 16       | SIGSTKFLT         | 协处理器栈故障             |                                                              | 终止             |
| 17       | SIGCHLD           | 子进程停止或终止           |                                                              | 忽略             |
| 18       | SIGCONT           | 使暂停进程继续             | 可发送给同一会话的任一进程而无视UID与EUID的匹配              | 若停止则继续执行 |
| 19       | SIGSTOP           | 强制停止进程               | 不可捕获                                                     | 暂停执行         |
| 20       | SIGTSTP           | 终端停止                   | 一般由`Ctrl+Z`发射给前台进程组                               | 暂停执行         |
| 21       | SIGTTIN           | 后台读取                   | 后台进程试图读取终端输入                                     | 暂停执行         |
| 22       | SIGTTOU           | 后台写入                   | 后台进程试图进行终端输出                                     | 暂停执行         |
| 23       | SIGURG            | 紧急IO数据                 | out-of-band数据到达socket时产生                              | 忽略             |
| 24       | SIGXCPU           | 突破对cpu时间的限制        |                                                              | 终止、core dump  |
| 25       | SIGXFSZ           | 突破对文件大小的限制       |                                                              | 终止、core dump  |
| 26       | SIGVTALRM         | 虚拟定时器超时(setitimer)  |                                                              | 终止             |
| 27       | SIGPROF           | 性能分析定时器超时（弃用） |                                                              | 终止             |
| 28       | SIGWINCH          | 终端窗口尺寸发生变化       |                                                              | 忽略             |
| 29       | SIGIO             | 异步I/O                    | 文件描述符准备就绪                                           | 终止             |
| 30       | SIGPWR            | 电量行将耗尽               |                                                              | 终止             |
| 31       | SIGSYS            | 错误的系统调用             |                                                              | 终止、core dump  |
| 34~64    | SIGRTMIN~SIGRTMAX | 实时信号                   |                                                              | 终止             |
> 参考至[linux信号表](https://blog.csdn.net/xuyaqun/article/details/5338563)
<!-- entry end -->

<!-- entry begin: jobs -->
* jobs
    * -l        ：显示PID
    * -r        ：显示running jobs
    * -s        ：显示suspended jobs
<!-- entry end -->

<!-- entry begin: fg/bg  *%JID* -->
* fg/bg  *%JID*
<!-- entry end -->

<!-- entry begin: ps pstree -->
* ps
    * -l        ：只显示当前shell的进程
    * -le       ：显示所有进程
* pstree -Uup
<!-- entry end -->

<!-- entry begin: kill killall pkill -->
* kill -(signal) PID
    PID为负，表示其绝对值为进程组号
* killall -(signal) Pname
    * -I        ：忽略大小写
* pkill (-signal)
    * -ce       ：显示匹配进程的数量并显示每个进程信息
    * -u        ：EUID
    * -U        ：UID
    * -G        ：GID
    * -P        ：PPID
    * -s        ：PSID
    * -t        ：TTY
<!-- entry end -->

<!-- entry begin: nice renice -->
* nice -n NI CMD
* renice -n PID
<!-- entry end -->

<!-- entry begin: top -->
* top
    * -bn       ：指定刷新次数并手动重定向到文件
    * -d        ：指定刷新周期
    * -u        ：指定监视用户
    * -p        ：指定监视PID
<!-- entry end -->

<!-- entry begin: free -->
* free
    * -wh       ：人性化输出
    * -s        ：刷新周期
    * -c        ：刷新次数
<!-- entry end -->

<!-- entry begin: vmstat -w -->
* vmstat -w  [周期]  [次数]
<!-- entry end -->

<!-- entry begin: iostat -h -->
* iostat -h
<!-- entry end -->

<!-- entry begin: lspci lsusb lspci -->
* lspci
    * -s        ：显示指定设备
    * -vv       ：显示详情
* lsusb -t
* lscpu
<!-- entry end -->

<!-- entry begin: nohup -->
* nohup CMD [&]
<!-- entry end -->

<!-- entry begin: lsof -->
* lsof
    > 列出打开的文件
    * -i        ：打开的端口
    * -a        ：and条件逻辑
    * -u        ：UID
    * -p        ：PID
    * -c        ：指定进程cmd的开头字符串
    * +d        ：列出目录下被打开的文件，+D递归
<!-- entry end -->

<!-- entry begin: fuser -->
* fuser -uv FILE/DIR
    > 列出打开目标文件的进程
<!-- entry end -->

<!-- entry begin: ulimit -->
* ulimit  -a -HS
    > /etc/security/ulimits.d/
* ipcs -l
<!-- entry end -->


# 日志
<!-- entry begin: syslog.h 日志规范类型 -->
* syslog.h规范类型
    * 0      ：kern(kernel)：内核日志，大都为硬件检测与内核功能加载
    * 1      ：user：用户层信息(如logger)
    * 2      ：mail：邮件服务有关
    * 3      ：daemon：系统服务(如systemd)
    * 4      ：auth：认证与授权(如login,ssh,su)
    * 5      ：syslog：rsyslogd服务
    * 6      ：lpr：打印
    * 7      ：news：新闻组
    * 8      ：uucp：全名"Unix to Unix Copy Protocol"，早期用于unix系統间的程序资料交换
    * 9      ：cron：计划任务(如cron,at)
    * 10     ：authpriv：与auth类似，但记录较多账号私人信息，包括PAM模块
    * 11     ：ftp：与FTP协议有关
    * 16~23  ：local0 ~ local7：本地保留
<!-- entry end -->

<!-- entry begin: syslog.h 日志规范级别 -->
* syslog.h规范级别
    * 7      ：debug：除错
    * 6      ：info：基本信息说明；
    * 5      ：notice：正常通知
    * 4      ：warning(warn)：警告，可能有问题但还不至与影响到daemon
    * 3      ：err (error)   ：错误，例如配置错误导致无法启动
    * 2      ：crit：严重错误
    * 1      ：alert：警报
    * 0      ：emerg(panic)：疼痛等級，意指系統几乎要死机，通常大概只有硬件出问题导致内核无法运行
<!-- entry end -->

<!-- entry begin: logger -->
* logger -p user.info
<!-- entry end -->

<!-- entry begin: journalctl -->
* journalctl
    * -b        ：开机启动日志
    * -n        ：最近的几行日志
    * -r        ：反向，由新到旧
    * -f        ：监听
    * -t        ：类型
    * -p        ：级别
    * -S、-U    ：since与until某时刻的日志(date格式时间)
    * 指定范围：
        * -u        ：指定unit
        * `_PID=`
        * `_UID=`
        * `_COMM=`
<!-- entry end -->

# systemd
<!-- entry begin: systemd -->
* systemd-units
    * unit配置目录   ：(优先级降序)
        > /etc/systemd/system/  
        > /run/systemd/system/  
        > /lib/systemd/system/
    * unit类型       ：
        * service    ：一般服务类型
        * socket     ：监听端口
        * timer      ：定时器
        * target     ：一系列unit的集合
        > 包括        ：graphical，multi-user，basic，sysinit，rescue，emerge，shutdown，getty
    * 启动流程
        * 根据配置目录优先级，寻找default.target
        * 读取并执行default.target，递归启动各依赖units
        > * systemd无法管理手动执行启动的服务  
        > * 只有在配置目录的unit才在systemd视线里  
        > * 是否开机启动取决于满足上述的unit是否在default.target的依赖链中
<!-- entry end -->

## units配置
<!-- entry begin: unit配置 -->
* unit配置
    * 选项可重复设置，后面覆盖前面
    * bool值可为     ：1/0，yes/no，ture/false，on/off
    * #与;开头为注释
    * foo.service.wants/requires依赖目录
    * 若unit名字为foo@bar.service，且目标不存在，则使用foo@.service(模板)，配置中%I为bar
<!-- entry end -->

### [Unit]
<!-- entry begin: [Unit] -->
* [Unit]
    > 指定条件下开启或关闭自己或其他Units
    * Description    ：简介
    * Documentation  ：提供详细信息，接受 "http://", "https://", "file:///", "info:///", "man:///" 五种URI类型
    * After/Before   ：同时开启/关闭时确定先后顺序，待所有状态完全确定后才算完成
        > 启动先则关闭后，但当一启动一关闭则先关后开(无先后顺序关系则独立启动与关闭)
    * 同时启动依赖Units
        * Wants(fail、dead都行)
        * Requires(fail不行、dead行)
        * BindsTo(fail、dead都不行)
        > 依赖关系是同时启动而非先后顺序启动  
        > 当条件检测失败时为dead而非fail
    * Requisite(只检查，不启动，检查失败则fail)
    * PartOf(跟着同时关闭或重启)
    * Conflicts(不能同时存在)
<!-- entry end -->

### [Install]
<!-- entry begin: [Install] -->
* [Install]
    > 只在enable与disable时使用
    * Alias  ：实质便是在递归启动链中创建符号链接
        > 别名会随着单元的enable与disable而生效和失效
    * WantedBy/RequiredBy    ：空格分隔的unit列表
        > 加入到.wants/.requires目录中
    * Also   ：空格分隔单元列表
        > enable与disable它时也对列表中单元进行相同操作
<!-- entry end -->

### [Service]
<!-- entry begin: [Service] -->
* [Service]
    * Type
        * simple
        * exec
            > simple表示当fork()返回时即算启动完成，而 exec 则表示仅在 fork()与exec()都执行成功时才算是启动完成；  
            > 这意味着对于exec类型的服务来说， 如果不能成功调用主服务进程(例如User=不存在或者二进制可执行文件不存在)， 那么systemctl start将会执行失败
        * oneshot(主进程退出后才算完成，直接从activating到inactive)
        * forking(父进程退出且至少有一个子进程才算完成，应该设置PIDFile=以跟踪主进程)
        * notify(要等待返回状态信息后才算完成)
        * dbus(从D-Bus获得名称后算完成，需设置BusName选项)
        * idle(所有任务完成后才启动，最多延迟5秒)
    * ExecStart
        * 只有Type=oneshot才能有多个命令行；
        * 绝对路径前-        ：失败退出被视为成功
        * 绝对路径前+        ：root用户权限
    * ExecPre/ExecPost
        * 可设置多个，顺序执行，若未设置-，则失败后后续不执行
    * ExecStop/ExecReload
    * RemainAfterExit        ：正常退出后仍为active，并无法再次启动
    * Restart
        * no，on-success，on-failure，on-abnormal，on-watchdog，on-abort，always
        * 正常退出、异常退出、被杀死、超时的时候是否重启，不包括手动关闭
    * RestartSec       ：默认单位秒
    * SuccessExitStatus：设置额外正常退出状态，可包括信号
    * KillMode
        * process      ：仅杀死主进程
        * control-group：杀死cgroup中所有进程
        * none         ：仅执行ExecStop
    * User
    * Group
<!-- entry end -->

### [Timer]
<!-- entry begin: [Timer] -->
* [Timer]
    * OnBootSec              ：相对内核启动
    * OnStartupSec           ：相对systemd启动
    * OnActiveSec            ：相对该timer启动
    * OnUnitActiveSec        ：相对匹配unit最后一次启动
    * OnUnitInactiveSec      ：相对匹配单元最后一次关闭
    * OnCalendar=(星期) 年-月-日 时-分-秒 (时区)
        * 星期       ：Mon，Tue，Wed，Thu，Fri，Sat，Sun
        * 主子部分   ：
            * `,`表示指定
            * `*`表示任一
            * `/N`表示指定的间隔为N跳跃，后面的数也算
            * `..`表示范围
            * `月~日`表示月中倒数第几天
            * 秒可以为小数
    * AccuracySec    ：设置精度，默认一分钟
    * Persistent     ：是否操作OnCalendar不错过
    * WakeSystem     ：是否到时唤醒系统
    * Unit           ：指定匹配unit，默认同名.service
    * 注             ：时间单位us ms s m h d w M y
<!-- entry end -->

### [Socket]
<!-- entry begin: [Socket] -->
* [Socket]
    * ListenStream：监听的port或`(IP/)port`，TCP
    * ListenDatagram：监听的port或`(IP/)port`，UDP
    * ListenSequentialPacket：监听UNIX socket
    * FreeBind=yes：在指定IP可用前监听它， 出于健壮性考虑， 当你希望将套接字绑定到特定的IP地址时应该设置此项，否则网络未及时启动时socket将无法启动
    * Accept=yes：为每个连接都产生实例
    * 注：可能希望在foo.service中[Unit]设置Conflicts=foo.socket
<!-- entry end -->

## 命令
<!-- entry begin: systemctl -->
* systemctl
    * 信息查看
        * status
        * show
        * cat
        * edit
    * unit查看
        * --state=
        * -t                ：指定unit类型
        * -a                ：所有
        * list-units        ：已加载的
        * list-unit-files   ：所有
        * list-dependencies [--reverse]
    * 手动控制
        * start
        * stop
        * restart
        * reload
    * 开机管理
        * enable|disable    ：可使用--now同时执行start/stop
        * static            ：只能作为依赖被启动
        * mask              ：禁止启动，unmask解禁
    * 主机状态
        * suspend
        * hibernate
        * rescue
        * emergency
    * 主机target
        * get-default
        * set-default
        * isolate
    * systemd重新读取unit
        * daemon-reload
<!-- entry end -->

<!-- entry begin: systemd-analyze systemd-escape -->
* systemd-analyze
    * systemd-analyze blame
    * systemd-analyze plot > plot.svg
* systemd-escape --path ：路径转义
<!-- entry end -->

# GRUB
## GRUB配置
<!-- entry begin: grub配置 -->
* /etc/default/grub
```
    GRUB_DEFAULT=0
    GRUB_TIMEOUT=5
    GRUB_GFXMODE=auto
    GRUB_GFXPAYLOAD_LINUX=keep
    GRUB_CMDLINE_LINUX-DEFAULT=
```
* /etc/grub.d/
    * 00_header
    * 01_user       #自定义环境
    * 10_linux      #确定linux选单
    * 20_os-prober  #确定其他OS选单
    * 40_custom     #自定义选单
<!-- entry end -->

## GRUB Shell
* 模块   ：默认自动加载command.lst与crypto.lst
* 命令规则       ：
    * 分区       ：(hd0，gpt1)
    * 文件       ：(hd0，gpt1)/path/to/file
    * 扇区       ：(hd0，gpt1)0+1

<!-- entry begin: grub 特殊变量 -->
* 特殊变量
    * prefix     ：grub安装目录
    * root       ：根设备，未指定设备名的文件的默认设备
    * cmdpath    ：core.image所在目录
    * superusers ：超级用户，逗号分隔
<!-- entry begin: grub命令 -->

<!-- entry end -->
* grub命令
    * ls                    ：列出已知设备/设备中的文件/目录的内容
    * cat                   ：显示文件内容，--dos选项处理换行符
    * echo                  ：与bash用法一样
    * normal                ：执行命令脚本
    * source                ：将文件内容插入当前位置
    * configfile            ：将文件做配置加载，不会保留其设置的环境变量
    * set var=val           ：设置变量
    * export var            ：导出至环境变量，使其对configfile命令载入的配置文件可见
    * lsmod                 ：列表已加载模块
    * insmod/rmmod          ：加载/卸载模块
    * loopback dev isofile  ：建立loop设备，-d删除
    * halt/reboot           ：关机/重启
<!-- entry begin: GRUB安全 -->

<!-- entry end -->
* GRUB安全
    * 设置超级用户：`set superusers="root"`
        > 注 ：设置后只有超级用户才能修改选单
    * 设置加密密码：`password_pbkdf2 root grub.pbkdf2.sha512...`
        > 注 ：使用grub-mkpasswd-pbkdf2命令产生密码
    * 设置明文密码：`password user ...`
    * menuentry选项
        * --unrestricted    ：所有人可执行
        * --users ""        ：仅超级用户
        * --users "user"    ：仅user与超级用户
<!-- entry end -->

## GRUB选单
<!-- entry begin: menuentry -->
* menuentry
    * "title"    ：选单名
    * --class    ：选单主题样式
    * --id       ：赋值chosen，覆盖原来的"title"
    * 语句块
        * linux       ：加载内核
        * initrd      ：加载内核映像
        * boot        ：启动已加载的os或loader，选单结束时隐含
        * chianloader ：链式加载文件
<!-- entry end -->

## 系统救援
<!-- entry begin: 系统救援 -->
* 系统救援
    * 修改选单内核参数为rd.break，并chroot
        > 注 ：rd.break模式下无SELinux，修改密码会导致其安全上下文失效而导致无法登陆
    * 修改选单内核参数为systemd-unit=rescu.target
<!-- entry end -->

<!-- entry begin: grub-install grub-mkconfig -->
* grub-install
    * --target=x86_64-efi
    * --efi-directory=/boot/efi
    * --bootloader-id=GRUB

* grub-mkconfig  -o  /boot/grub/grub.cfg
<!-- entry end -->

# 主机配置
# 计算机网络
## 网络配置
<!-- entry begin: ip -->
* ip [--color]
    * link/l
        * set IF up/down
        * set name NAME
        * set promisc
    * addr/a
        * add IP/MASK  broadcast +  dev IF
        * del IP/MASK  dev IF
    * route/r
        * list table TBL_NAME|TBL_NR
        * add IP/MASK  via GWIP  dev IF  src IP  metric 600
        * add [throw|unreachable|prohibit|blackhole] IP/MA
        * del IP/MASK  via GWIP  dev IF
    * neigh/n
        * add/del IP  lla MAC  dev IF
<!-- entry end -->

## NetworkManager
<!-- entry begin: nmcli nmtui -->
* nmcli
    * radio/r
        * wifi on|off
    * connection/c
        * up  CON-NAME
        * add type . ifname . ssid . con-name .
        * del SSID
        * reload
    * device/d
        * wifi
        * dis IF
        * wifi c SSID  password PASSWD  (hidden yes)
* nmtui
<!-- entry end -->

## 其它网络命令
<!-- entry begin: nmap -->
* nmap
![图片来自网络](../images/nmap.png)
<!-- entry end -->

<!-- entry begin: ss -->
* ss
    * -atp       ：TCP端口
    * -atpn      ：TCP端口，指定端口号
    * -aup       ：UDP端口
    * -axp       ：UNIX类型socket
* ping
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

<!-- entry begin: sendEmail -->
* sendEmail
    * -s        ：SMTP服务器
    * -f        ：发送者的邮箱
    * -t        ：接收者的邮箱
    * -cc       ：表示抄送发给谁
    * -bcc      ：表示暗抄送给谁
    * -xu       ：SMTP验证的用户名
    * -xp       ：SMTP验证的密码
    * -u        ：标题
    * -m        ：内容
    * -a        ：附件
    * -o message-content-type=*html*/*text* ：邮件的格式
    * -o message-charset=utf8               ：邮件的编码
<!-- entry end -->

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

# 其他命令
<!-- entry begin: bc -->
* bc
    * -l        ：可以使用数学库函数 s(sin x)，c(cos x)，a(arctan x)，l(ln x)，e(e^x)
    * 特殊变量  ：scale，last，ibase，obase，支持^运算符求幂
<!-- entry end -->


<!-- entry begin: diff patch -->
```sh
diff -Naur OLD NEW > patch
patch -p`n`  < patch
```
> * new和old不要在同一目录下
> * n为去掉的/个数
<!-- entry end -->

stty -a
stty echo
stty -echo
`stty </dev/tty`
