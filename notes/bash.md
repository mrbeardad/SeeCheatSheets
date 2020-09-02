# 目录
<!-- vim-markdown-toc GFM -->

- [BASH基础](#bash基础)
- [特殊字符](#特殊字符)
- [Test条件检测](#test条件检测)
- [正则表达式](#正则表达式)
  - [零宽断言](#零宽断言)
  - [实体模式](#实体模式)
  - [字符类](#字符类)
  - [转义字符](#转义字符)
  - [regex技巧](#regex技巧)
- [变量](#变量)
- [BASH脚本](#bash脚本)
- [脚本命令](#脚本命令)
- [流处理命令](#流处理命令)

<!-- vim-markdown-toc -->

# BASH基础
<!-- entry begin: bash cmd -->
* bash：
    * -x            ：调试信息
    * -n            ：语法检测
    * -c            ：执行命令
<!-- entry end -->

<!-- entry begin: set -->
* set
    * -u            ：使用不存在的变量则立即停止
    * -e            ：有一条命令返回非零则立即停止
    * -o pipefail   ：若管道命令其中有一个返回非零也立即停止（管道命令行的返回值为最后一个命令的返回值）
<!-- entry end -->

<!-- entry begin: bash shortcut bindkey -->
* shell常见快捷键
    * ^A：行首
    * ^E：行尾
    * ^U：删除前面字符
    * ^K：删除后面字符
    * ^W：删除前面单词
    * ^Y：粘贴之前的删除内容
    * ^D：删除光标前的字符，或者触发`EOF`（这也可能导致shell退出）
    * ^C：结束当前命令(SIGINT 2)
    * ^Z：暂停当前命令(SIGTSTP 20)
    * ^L：清屏并重新显示
    * ^R：历史命令反向搜索
    * ^S：历史命令正向搜索
    * ^G：退出当前编辑或搜索
    * ^V：显示输入字符的按键序列
<!-- entry end -->

<!-- entry begin: bash builtin -->
* bash部分内建命令
    * :             ：空白命令，返回零
    * alias         ：命令别名
    * source        ：读取指定配置
    * history       ：命令历史
    * reset         ：重置因读取二进制文件导致的乱码
    * wait          ：等待指定的子进程退出
    * sleep         ：休眠指定时间
    * exit          ：退出bash界面
    * exec          ：调用exec()加载执行程序
<!-- entry end -->

# 特殊字符
<!-- entry begin: wildchar 通配符 -->
* 通配符：
    * `*`       ：任意长度的任意字符
    * `?`       ：一个任意字符
    * `[^ ]`    ：序列中一个可能的字符
    * `[ - ]`   ：序列中一个可能的字符
    * `{ , }`   ：序列中的字符展开
    * `{ .. }`  ：序列中的字符展开
    * `**`      ：递归目录（zsh）
<!-- entry end -->

<!-- entry begin: parentheses brackets 括号 -->
* 括号
    * `(cmd; cmd)`      ：子shell中执行，
    * `{cmd; cmd}`      ：当前shell中执行
    * `[ ]`             ：test命令别名（不推荐），最后一个参数必须为`]`
    * `[[ ]]`           ：bash关键字（推荐），支持`=~`匹配正则子串（括号内关闭通配符扩展），支持逻辑运算符
    * `((expr, expr))`  ：C风格表达式
<!-- entry end -->

<!-- entry begin: ~扩展 -->
* ~扩展
    * `~` ：家目录
    * `~+`：当前目录
    * `~-`：上次目录
<!-- entry end -->

<!-- entry begin: !扩展 -->
* !扩展：
    * 指定历史命令
        * `!{char}` ：上条以`char`开头的命令
        * `!-n`     ：上第n条命令
        * `!!`      ：上第1条命令
        * `!#`      ：当前命令
    * 指定历史命令参数
        * `命令:n`  ：命令即上述历史命令（其中`!!`可简写为`!`），n即为该命令第n个参数
        * `!^`      ：上第1条命令的第1个参数
        * `!$`      ：上第1条命令的最后一个参数
        * `!:n`     ：上第1条命令的第n个参数
        * `!:n-$`   ：上第1条命令的第n个参数到最后一个参数
<!-- entry end -->

<!-- entry begin: $扩展 -->
* $扩展：
    * `$$`                            ：扩展为当前shell的PID
    * `$?`                            ：扩展为上次命令返回值
    * `$!`                            ：扩展为上次后台进程的进程号
    * `$*`                            ：扩展为所有参数（聚合）字符串
    * `$@`                            ：扩展为所有参数（分离）数组
    * `$#`                            ：扩展为参数个数
    * `$N`                            ：扩展为bash脚本或bash函数的第N个参数，`$0`为执行的命令
    * `$(<file)`                      ：扩展为file文件的内容
    * `$(cmd)`                        ：扩展为cmd的标准输出，同`` `cmd` ``
    * `$((expr, expr))、$[expr, expr]`：扩展为C风格表达式结果
    * `$'escap'`                      ：扩展为anscii转义后的字符
    * `$varname、${varname}`          ：扩展为变量的值
<!-- entry end -->

<!-- entry begin: 其他特殊符 逻辑符 连接符 -->
* `#`           ：注释直到换行符
* `空白符`      ：空格为参数分隔符，换行为执行命令行
* `|`           ：管道连接符，**连接后的命令视作一条命令**
* `!`           ：逻辑非执行，放在一条命令前
* `&&`          ：逻辑与执行，放在一条命令后
* `||`          ：逻辑或执行，放在一条命令后
* `;`           ：顺序执行，放在一条命令后
* `&`           ：后台执行，放在一条命令后
> 使用`(cmd; cmd)`与`{cmd; cmd}`将命令连接成一条命令
<!-- entry end -->

<!-- entry begin: 重定向 -->
* I/O重定向
    > 重定向符号放在一条命令最后，或管道符之前
    * `< file`          ：重定向stdin到file
    * `<< EOF`          ：读取stdin直到EOF（不读取EOF）
    * `<<< string`      ：将string内容作为stdin
    * `<(cmd)`          ：将cmd的stdout定向到临时文件并替换为该文件名
    * `> file`          ：重定向stdout到file（截断）
    * `>> file`         ：重定向stdout到file（追加）
    * `2> file`         ：重定向stderr到file（截断）
    * `2>> file`        ：重定向stderr到file（追加）
    * `&> file`         ：重定向stdout与stderr到file（截断）
    * `&>> file`        ：重定向stdout与stderr到file（追加）
    * `1>&2`            ：重定向stdout到stderr
    * `2>&1`            ：重定向stderr到stdout
<!-- entry end -->

<!-- entry begin: escape 转义字符 -->
* 转义字符
    * `\`   ：转义所有特殊字符
    * `" "` ：其中只保留$扩展、!扩展、`"`、`\`的功能
    * `' '` ：其中只保留 `'` 的功能
<!-- entry end -->

# Test条件检测
<!-- entry begin: test operator -->
* 逻辑运算符：
    * `!`
    * `&&`
    * `||`

* 字符串比较：
    * `=~` `==` `!=` `<` `>`
    * -n    ：不为空
    * -z    ：为空

* 算术比较
    * -eq、-ne
    * -gt、-ge
    * -lt、-le

* 文件判断
    * -e    ：存在
    * -s    ：存在且size不为0
    * -N    ：修改时间新于读取时间
    * -nt   ：file1比file2新（mtime）
    * -ot   ：file1比file2旧（mtime）
    * -ef   ：两文件为同一文件的硬连接

* 文件类型
    * -b    ：块文件
    * -c    ：字符文件
    * -d    ：目录
    * -f    ：普通文件
    * -L    ：符号链接
    * -S    ：socket
    * -p    ：pipe

* 文件权限
    * -r    ：可读
    * -w    ：可写
    * -x    ：可执行
    * -u    ：SUID
    * -g    ：SGID
    * -k    ：SBIT
    * -O    ：onwer为EUID
    * -G    ：group为EGID
<!-- entry end -->

# 正则表达式
<!-- entry begin: regex 正则 -->
正则表达式的标准并不统一，此处为大多数标准都支持的，具体还得查看使用的正则引擎的文档  
笔者使用的版本一般为：C++ regex库、Vim、POSIX扩展正则工具(如sed, awk, grep)  
故以下后缀代表：
- `无`  代表都支持
- `+cpp`代表C++ regex库支持
- `+vim`代表Vim支持（very magic模式）
- `+pos`代表POSIX扩展正则支持

## 零宽断言
| 符号           | 含义         | 备注       |
|----------------|--------------|------------|
| `^`            | 行首         |            |
| `$`            | 行尾         |            |
| `\b`           | 单词边界     | (+cpp+pos) |
| `\B`           | 非单词边界   | (+cpp+pos) |
| `<`            | 单词左边界   | (+vim)     |
| `>`            | 单词右边界   | (+vim)     |
| `\<`           | 单词左边界   | (+pos)     |
| `\>`           | 单词右边界   | (+pos)     |
| `(?=pattern)`  | 顺序预查     | (+cpp)     |
| `(?!pattern)`  | 顺序否定预查 | (+cpp)     |
| `(pattern)@=`  | 顺序预查     | (+vim)     |
| `(pattern)@!`  | 顺序否定预查 | (+vim)     |
| `(pattern)@<=` | 逆序预查     | (+vim)     |
| `(pattern)@<!` | 逆序否定预查 | (+vim)     |

## 实体模式
| 符号          | 含义                                               | 备注                      |
|---------------|----------------------------------------------------|---------------------------|
| `.`           | 匹配除`\n`外任意字符                               |                           |
| `[^ - ]`      | 匹配序列中的一个字符                               |                           |
| `*`           | 匹配前一个字符0 - ∞次                              |                           |
| `*?`          | 懒惰匹配版`*`                                      | (+cpp)                    |
| `{-}`         | 懒惰匹配版`*`                                      | (+vim)                    |
| `+`           | 匹配前一个字符1 - ∞次                              |                           |
| `+?`          | 懒惰匹配版`+`                                      | (+cpp)                    |
| `?`           | 匹配前一个字符0或1次                               |                           |
| `{n}`         | 匹配前一个字符n次                                  |                           |
| `{n,m}`       | 匹配前一个字符n-m次                                |                           |
| `{n,}`        | 匹配前一个字符至少n次                              |                           |
| `...|...`     | 匹配左或右的模式，用`(...|...)`将其限制在局部      |                           |
| `(...)`       | 匹配组，将括号内的模式视作一个单位                 |                           |
| `(?:...)`     | 匹配组，但不支持反向引用                           | (+cpp)                    |
| `%(...)`      | 匹配组，但不支持反向引用                           | (+cpp)                    |
| `\1, ..., \N` | 反向引用，替换为第N个匹配组（按`(`出现的顺序编号） |                           |

## 字符类
| 字符类     | 内容                         |
|------------|------------------------------|
| [:alnum:]  | 字母＋数字                   |
| [:alpha:]  | 字母                         |
| [:upper:]  | 大写字母                     |
| [:lower:]  | 小写字母                     |
| [:digit:]  | 数字                         |
| [:xdigit:] | 十六进制数字                 |
| [:punct:]  | 符号                         |
| [:blank:]  | ` `与`\t`                    |
| [:space:]  | ` ` `\t` `\n` `\r` `\v` `\f` |
| [:cntrl:]  | `\x00`-`\x1F` 与 `\x7F`      |
| [:graph:]  | `[[:alnum:][:punct:]]`       |
| [:print:]  | `[[:alnum:][:punct:] ]`      |

## 转义字符
| 转义字符                 | 含义                          | 备注                    |
|--------------------------|-------------------------------|-------------------------|
| `\d` `\D`                | `[[:digit:]]、[^[:digit:]]`   |                         |
| `\s` `\S`                | `[[:space:]]、[^[:space:]]`   |                         |
| `\w` `\W`                | `[[:alnum:]_]、[^[:alnum:]_]` | 对于`\w`，pos还包含中文 |
| `\n` `\r` `\t` `\v` `\f` |                               | (+cpp+vim)              |
| `\0` `\xhh` `\uhhhh`     | null字符、ansi、unicode       | (+cpp)                  |
| `\a` `\b` `\e`           | 警报、退格、escape            | (都不支持)              |
| `\x` `\X`                | `[[:xdigit:]]、[^[:xdigit:]]` | (+vim)                  |
| `\l` `\L`                | `[[:lower:]]、[^[:lower:]]`   | (+vim)                  |
| `\u` `\U`                | `[[:upper:]]、[^[:upper:]]`   | (+vim)                  |
| `\a` `\A`                | `[[:alpha:]]、[^[:alpah:]]`   | (+vim)                  |

## regex技巧
* `[^y]x*[^y]`：企图避免在连续x前后马上接y（错误）。考虑匹配`yxxxy`时，`x*`匹配`x`，前一个`[^y]`匹配第一个`x`，后一个`[^y]`匹配第三个`x`，
    因为`x*`的连续匹配提前结束，故应该这样写regex：`[^yx]x*[^yx]`

* `[^x]y`：企图避免在y前面相邻位置接x（错误）。因为未考虑y前面"没有字符"的情况。
    故应该这样写regex：`([^x]|^)y`

* `[^x]*y`：企图避免在y前面出现x（错误）。因为`[^x]*`可以匹配nothing，即什么也不用匹配也算作匹配成功。
    故应该这样写regex：`^[^x]*y`

* 利用逆向思维，将**不应该在某位置出现什么**，转换为**应该在某位置出现什么**，注意**该位置**是否可以为空
<!-- entry end -->

# 变量
<!-- entry begin: 变量 -->
注：zsh中数组下标从1开始，且无需${arr[i]}中的花括号  
注：shell变量默认都是字符串  
注：字符串变量所有空白符等特殊字符，使用时应该用双引号包含

* 定义或取消变量
    * var=val
    * array[0]=valA
    * array=([0]=valA [1]=valB [2]=valC)
    * array=(valA valB valC)
    * unset var

* 读取变量
    * ${+var}                   ：变量存在为1，否则为0
    * ${var}                    ：读取变量值
    * ${array[i]}               ：取得数组中的元素
    * ${array[@]}               ：取得数组中所有元素
    * ${#var}                   ：字符串长度
    * ${#array[@]}              ：取得数组的长度
    * ${#array[i]}              ：取得数组中某个变量的长度
    * ${varname:-word}          ：若不为空则返回变量，否则返回 word
    * ${varname:=word}          ：若不为空则返回变量，否则赋值成 word 并返回
    * ${varname:?message}       ：若不为空则返回变量，否则打印错误信息并退出
    * ${varname:+word}          ：若不为空则返回 word，否则返回空串
    * ${varname:offset:len}     ：取得字符串的子字符串

* 修改变量
    * ${variable#pattern}       ： 如果变量头部匹配 pattern，则删除最小匹配部分返回剩下的
    * ${variable##pattern}      ： 如果变量头部匹配 pattern，则删除最大匹配部分返回剩下的
    * ${variable%pattern}       ： 如果变量尾部匹配 pattern，则删除最小匹配部分返回剩下的
    * ${variable%%pattern}      ： 如果变量尾部匹配 pattern，则删除最大匹配部分返回剩下的
    * ${variable/pattern/str}   ： 将变量中第一个匹配 pattern 的替换成 str，并返回
    * ${variable//pattern/str}  ： 将变量中所有匹配 pattern 的地方替换成 str 并返回
    * eval \$$var               ： eval为关键字，整个表达式会替换成最终的"${$var}"（假设花括号中的变量会先被替换）
    * eval var1=\$$var2         ： 将`eval \$$var2`的替换结果复制给`var1`
<!-- entry end -->

<!-- entry begin: export 环境变量 -->
* export **VAR**
* shell环境变量
    * HOME
    * USER
    * LOGNAME
    * UID、EUID、GID、EGID
    * PATH
    * MAIL
    * PWD
    * SHELL
    * TERM
    * HISTSIZE
    * HOSTNAME：zsh中为HOST
    * RANDOM：0 ~ 32767
    * FUNCNAME
    * LINENO
<!-- entry end -->

# BASH脚本
<!-- entry begin: function 函数 -->
* 定义函数
```sh
function funcname()
{
    statements
    return val
}
```

* 调用函数
```sh
    funcname  arg1  arg2
```
<!-- entry end -->

<!-- entry begin: 语句 -->
* if分支语句
```sh
    if cmd ;then
        #...
    elif [[ test ]] ;then
        #...
    else
        #...
    fi
```

* 循环语句
```sh
for i in List ;do
    for (( ; ; )) ;do
        while cmd ;do
            until [[ test ]] ;do
                #...
            done
        done
    done
done
```

* select选择语句
```sh
select i in List ;do
    #...
done
# 死循环选择，一般嵌套case语句
# 输出PS3提示，选择数字并将对应List中字符串赋值给i
```

* case多分支
```sh
case $i in
v1)     statement ;;    # 直接结束
v2|v3)  statement ;;&   # 测试下个子句是否匹配
*)      statement ;&    # 直接执行下个子句
esac
```

* 控制跳转
    * continue
    * break
<!-- entry end -->

# 脚本命令
<!-- entry begin: getopts -->
* getopts optstring name
    > * 命令行参数    ：在shell中执行一条命令的所有字符，即脚本中的变量`$0` `$1`等。类型有：执行命令、选项、选项参数、命令参数  
    > * 执行命令      ：即shell脚本中的`$0`  
    > * 选项          ：即以`-`开头的命令行参数。选项分为三种类型，`-o`单选项、`-opt`多选项（`o`与`p`选项必须为无参选项）、`-tfile`选项`t`及其参数`file`
    > * 选项参数      ：若某选项必有参数，则跟在该选项后面的同一命令行参数的字符，或下个命令行参数即为该选项的参数  
    > * 命令参数      ：不属于上面三种情况的命令行参数，作为该命令本身的主要参数。**getopts只要解析到命令参数就会停止并返回**
    * optstring
        * `:o`      ：开头`:`表示开启silent-mode，默认为print-mode
        * `o`       ：代表选项`o`没有参数  
        * `o:`      ：代表选项`o`必有参数, 紧跟`-oarg`或间隔`-o arg`中的`arg`都被视为`-o`的参数  
    * print-mode
        > 由getopts帮助自动打印错误信息
        * name      ：存储`optstring`中的选项字符，若为无效选项则为`?`。无效选项即选项字符不在于`optstring`中或本该需要参数的选项却没有参数
        * OPTARG    ：存储当前选项的参数，无则为`空`
        * OPTIND    ：存储下一次getopts要解析的命令行参数的索引，`eval \$$OPTIND`即可获取值
    * silent-mode
        > 关闭自动打印错误
        * name      ：存储`optstring`中的选项字符。该选项字符不存在于`optstring`则为`?`，需参选项无参数则为`:`
        * OPTARG    ：同print-mode
        * OPTIND    ：同print-mode
<!-- entry end -->

<!-- entry begin: read -->
* read  VAR
    * -p    ：提示符
    * -t    ：限时
    * -n    ：读取字符数
    * -s    ：关闭回显
<!-- entry end -->

<!-- entry begin: trap signal -->
* trap
    * `cmd signal1 signal2`    ：在脚本中设置信号处理命令
    * `""  signal1 signal2`    ：在脚本中屏蔽某信号
    * `-   signal1 signal2`    ：恢复默认信号处理行为
<!-- entry end -->

# 流处理命令
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

<!-- entry begin: wc -->
* wc
    * -l：行数
    * -w：单词数
    * -m：字符数
<!-- entry end -->

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
        * $0        ：整行内容
        * $N        ：第N个字段

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
        * `"cmd" | getline var`         ：管道输入，若无`var`则直接赋值给$0
        * `getline var < "file"`        ：文件输入，若无`var`则直接赋值给$0
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

