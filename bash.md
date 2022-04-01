# 目录
<!-- vim-markdown-toc GFM -->

- [目录](#目录)
- [bash命令参数](#bash命令参数)
- [基础语法](#基础语法)
- [命令行扩展](#命令行扩展)
  - [花括号扩展](#花括号扩展)
  - [波浪号扩展](#波浪号扩展)
  - [变量/命令/算数扩展](#变量命令算数扩展)
  - [进程扩展](#进程扩展)
  - [字词划分](#字词划分)
  - [路径扩展](#路径扩展)
  - [引用移除](#引用移除)
- [语句](#语句)
- [常用命令](#常用命令)
  - [文件信息](#文件信息)
  - [文件日期](#文件日期)
  - [目录文件](#目录文件)
  - [符号链接](#符号链接)
  - [硬链接](#硬链接)
  - [文件搜索](#文件搜索)
  - [文件I/O](#文件io)
  - [流处理命令](#流处理命令)
  - [编辑三剑客](#编辑三剑客)
    - [grep](#grep)
    - [awk](#awk)
    - [sed](#sed)
  - [其他命令](#其他命令)

<!-- vim-markdown-toc -->

# bash命令参数
<!-- entry begin: bash set -->
| 选项        | 功能                                                      |
| ----------- | --------------------------------------------------------- |
| -c          | 执行给定命令                                              |
| -s          | 从标准输入读取命令                                        |
| -u          | 未初始化变量被视作错误，同`set -u`                        |
| -e          | 遇非零返回值则立即退出，同`set -e`                        |
| -o pipefail | 管道命令只有当所有命令返回零才返回零，同`set -o pipefail` |
<!-- entry end -->

# 基础语法
```sh
# Simple Commands
[!] cmd [args...] [Redirection]
# Pipelines
[time] Simple-Command [[| or |&] ...]
# Lists
Pipeline [[; or || or &&] ...]
# Compound-Commands
(list)
{ list; }
((expression))
[[ expression ]]
```

| Redirection重定向 | 功能                                                                               |
| ----------------- | ---------------------------------------------------------------------------------- |
| `[fd]<file`       | 重定向fd到file，fd默认为0                                                          |
| `[fd]<<EOF`       | 重定向fd到（接下来内容直到一行只有EOF的行，不包括结束符EFO），fd默认为0            |
| `[fd]<<-EOF`      | 重定向fd到（接下来内容直到一行只有EOF的行，不包括行首的tab与结束符EFO），fd默认为0 |
| `[fd]<<<string`   | 重定向fd到（string内容），fd默认为0                                                |
| `[fd]>file`       | 重定向fd到file（截断），fd默认为1                                                  |
| `[fd]>>file`      | 重定向fd到file（追加），fd默认为1                                                  |
| `&>file`          | 重定向stdout与stderr到file（截断）                                                 |
| `&>>file`         | 重定向stdout与stderr到file（追加）                                                 |
| `fd1>&fd2`        | 重定向fd1到fd2                                                                     |
| `fd<>file`        | 打开文件描述符n指向file                                                            |

# 命令行扩展
## 花括号扩展
| 扩展前    | 扩展后     | 备注                          |
| --------- | ---------- | ----------------------------- |
| `{a,b,c}` | `a b c`    | 逐词扩展，可再嵌套逐词扩展    |
| `{a-c}`   | `a b c`    | 字符序列扩展｝                |
| `{1-3}`   | `1 2 3`    | 数字序列扩展                  |
| `{1-3-2}` | `1 3`      | 可选设置增量，默认1或-1       |
| `{01-9}`  | `01 02 03` | 第一个数字0开头则用零补齐宽度 |

## 波浪号扩展
* 波浪号扩展在变量赋值时也有效

| 扩展前  | 扩展后                | 备注                             |
| ------- | --------------------- | -------------------------------- |
| `~user` | `/path/t，/user/home` | ~与下个/之间的所有字符被当做user |
| `~+`    | `/path/to/PWD`        | `$PWD`                           |
| `~-`    | `/path/to/OLDPWD`     | `$OLDPWD`                        |

## 变量/命令/算数扩展
* `paramter=val`
* `array[0]=v0`
* `array=(v0 v1 v2)`
* `array=([A]=v0 [B]=v1 [C]=v2)`

| 扩展前                        | 备注                                                               |
| ----------------------------- | ------------------------------------------------------------------ |
| `${paramter}`                 | 变量名无歧义时可省略花括号，`array`当做`parameter`时值为`array[0]` |
| `${!paramter}`                | 间接变量，大概意思即`${$paramter}`                                 |
| `${#paramter}`                | 字符长度（支持array扩展为数组长度）                                |
| `"${array[@]}"`               | 扩展为`"v1 v2 v3"`                                                 |
| `"${array[*]}"`               | 扩展为`"v1" "v2" "v3"`                                             |
| `${!array[*]}`                | 扩展为数组keys                                                     |
| `${!array[@]}`                | 扩展为数组keys                                                     |
| `${!prefix*}`                 | 扩展前缀为prefix的变量名                                           |
| `${!prefix@}`                 | 扩展前缀为prefix的变量名                                           |
| `${paramter:-word}`           | 若param为空则默认值为word                                          |
| `${paramter:=word}`           | 若param为空则赋值其为word                                          |
| `${paramter:?word}`           | 若param为空则打印错误消息word                                      |
| `${paramter:+word}`           | 若param不为空则替换为word                                          |
| `${paramter:offset:length}`   | 子串展开，可省略`:length`（支持array）                             |
| `${paramter#pattern}`         | 删除最短的匹配头部（支持array）                                    |
| `${paramter##pattern}`        | 删除最长的匹配头部（支持array）                                    |
| `${paramter%pattern}`         | 删除最短的匹配尾部（支持array）                                    |
| `${paramter%%pattern}`        | 删除最长的匹配尾部（支持array）                                    |
| `${paramter/pattern/string}`  | 模式替换，仅替换首个匹配（支持array）                              |
| `${paramter//pattern/string}` | 模式替换，替换全部匹配（支持array）                                |
| `${paramter^pattern}`         | 转换为大写，仅替换首个匹配，pattern默认为`?`（支持array）          |
| `${paramter^^pattern}`        | 转换为大写，替换全部匹配， pattern默认为`?`（支持array）           |
| `${paramter,pattern}`         | 转换为小写，仅替换首个匹配，pattern默认为`?`（支持array）          |
| `${paramter,,pattern}`        | 转换为小写，替换全部匹配，pattern默认为`?`（支持array）            |
| `$(command)`                  | 替换为命令的标准输出                                               |
| `` `command` ``               | 替换为命令的标准输出（旧式）                                       |
| `$(<file)`                    | 替换为文件内容，更快速的`$(cat file)`                              |
| `$((expression))`             | 参数扩展、字符扩展、命令扩展、引号删除                             |

## 进程扩展
| 扩展前    | 备注                                         |
| --------- | -------------------------------------------- |
| `<(list)` | 替换为FIFO文件名，list标准输出重定向到该文件 |
| `>(list)` | 替换为FIFO文件名，list标准输入重定向到该文件 |

## 字词划分
`Space`、`Tab`、`NewLine`将命令行划分为一个个**word**

## 路径扩展
* 若无匹配路径则默认不扩展

| 扩展前 | 备注               |
| ------ | ------------------ |
| `*`    | 任意长的任意字符   |
| `?`    | 一个任意字符       |
| `[]`   | 同正则表达式的`[]` |

## 引用移除
* 当前面所有扩展完成后，自动将多余引用符号删除
* 引用符号包括`\`、`" "`、`' '`用于转义特殊字符
* 其中`" "`中还会保留变量扩展、命令替换、算数扩展

# 语句
* **分支**
```sh
if list; then
    ...
elif list; then
    ...
else
    ...
fi

case word in
pattern)
    list ;&    # fallthrough
pattern|pattern)
    list ;;    # break
esac
```

* **循环**
```sh
while list; do
done

until list; do
done

for name in word... ; do
done

for (( expr1; expr2; expr3 )); do
done

# 打印$PS3提示用户选择，一直循环直到执行break
select name in word ; do
    break
done
```

* **函数**
```sh
# 返回值为最后一个命令返回值
name() compound-command [redirection]

# 复合命令包括：
(list)              # 子shell中执行
{ list; }           # 当前shell执行
((expression))      # C风格表达式，true返回0，false返回非零
[[ expression ]]    # test，内部禁用通配符扩展
```

# 常用命令
<!-- entry begin: getopts -->
* getopts optstring name
    > * 命令行参数    ：在shell中执行一条命令的所有字符，即脚本中的变量`$0` `$1`等。类型有：执行命令、选项、选项参数、命令参数  
    > * 执行命令      ：即shell脚本中的`$0`
    > * 选项          ：即以`-`开头的命令行参数。选项分为三种类型，`-o`单选项、`-opt`多选项（`o`与`p`选项必须为无参选项）、`-tfile`选项`t`及其参数`file`
    > * 选项参数      ：若某选项必有参数，则跟在该选项后面的同一命令行参数的字符，或下个命令行参数即为该选项的参数  
    > * 命令参数      ：不属于上面三种情况的命令行参数，作为该命令本身的主要参数。**getopts只要解析到命令参数就会停止并返回，且不识别"--"。**
    * optstring
        * `:o`      ：开头`:`表示开启silent-mode，默认为print-mode
        * `o`       ：代表选项`o`没有参数  
        * `o:`      ：代表选项`o`必有参数, 紧跟`-oarg`或间隔`-o arg`中的`arg`都被视为`-o`的参数  
    * print-mode
        > 由getopts帮助自动打印错误信息
        * name      ：存储`optstring`中的选项字符，若为无效选项则为`?`。无效选项即选项字符不在于`optstring`中或本该需要参数的选项却没有参数
        * OPTARG    ：存储当前选项的参数，无则为`空`
        * OPTIND    ：存储下一次getopts要解析的命令行参数的索引，`eval array[${#array[@]}]=\$$OPTIND`即可获取值
    * silent-mode
        > 关闭自动打印错误
        * name      ：存储`optstring`中的选项字符。该选项字符不存在于`optstring`则为`?`，需参选项无参数则为`:`
        * OPTARG    ：同print-mode
        * OPTIND    ：同print-mode
<!-- entry end -->

<!-- entry begin: trap signal -->
* trap
    * `cmd signal1 signal2`    ：在脚本中设置信号处理命令
    * `""  signal1 signal2`    ：在脚本中屏蔽某信号
    * `-   signal1 signal2`    ：恢复默认信号处理行为
<!-- entry end -->

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

## 文件日期
<!-- entry begin: touch -->
* touch
    * `默认`    ：修改a/m/ctime
    * -a        ：只改atime
    * -m        ：只改mtime
    * -d        ：指定touch的时间，date模式
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

## 文件搜索
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

## 文件I/O
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

## 流处理命令
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

* 变量：
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
        * NR        ：行数
        * FNR       ：在当前文件中的行数
        * NF        ：字段数
        * OFMT      ：数字输出格式(默认%.6g)
        * IGNORECASE：为true则忽略大小写
        * `$0`      ：整行内容
        * `$N`      ：第N个字段

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
        * 替换标记：默认第一个，g全部，Ng第N个
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

## 其他命令
<!-- entry begin: bc -->
* bc
    * -l        ：可以使用数学库函数 s(sin x)，c(cos x)，a(arctan x)，l(ln x)，e(e^x)
    * 特殊变量  ：scale，last，ibase，obase，支持^运算符求幂
<!-- entry end -->
