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
- [常用命令](#常用命令)

<!-- vim-markdown-toc -->
# BASH基础
<!-- entry begin: bash set -->
* bash：
    * -x            ：调试信息
    * -n            ：语法检测
    * -c            ：执行命令
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

# 特殊字符
<!-- entry begin: wildchar 通配符 -->
* 通配符：
    * `*`       ：任意长度的任意字符
    * `?`       ：一个任意字符
    * `[  ]`    ：序列中一个可能的字符
    * `[ - ]`   ：序列范围中一个可能的字符
    * `[^ ]`    ：非序列中一个字符
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
* `$扩展`：
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
    * `[fd]<file`       ：重定向fd到file，fd默认为0
    * `[fd]<<EOF`       ：重定向fd到（接下来内容直到一行只有EOF的行，不包括结束符EFO），fd默认为0
    * `[fd]<<-EOF`      ：重定向fd到（接下来内容直到一行只有EOF的行，不包括行首的tab与结束符EFO），fd默认为0
    * `[fd]<<<string`   ：重定向fd到（string内容），fd默认为0
    * `<(cmd)`          ：将cmd的stdout定向到临时文件并替换为该文件名
    * `fd>file`         ：重定向fd到file（截断），fd默认为1
    * `fd>>file`        ：重定向fd到file（追加），fd默认为1
    * `&>file`          ：重定向stdout与stderr到file（截断）
    * `&>>file`         ：重定向stdout与stderr到file（追加）
    * `fd1>&fd2`        ：重定向fd1到fd2
    * `fd<>file`        ：打开文件描述符n指向file
<!-- entry end -->

<!-- entry begin: escape 转义字符 -->
* 转义字符
    * `\`   ：转义所有特殊字符
    * `" "` ：其中只保留`$扩展`、`!扩展`、`"`、`\`的功能
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
- `+cpp`代表C++ regex库支持
- `+vim`代表Vim支持（Vim默认的正则模式除了`.*^$`外的其它所有元字符需要添加`\`）
- `+pos`代表POSIX扩展正则支持

## 零宽断言
| 符号            | 含义         | 备注           |
|-----------------|--------------|----------------|
| `^`             | 行首         | (+cpp+vim+pos) |
| `$`             | 行尾         | (+cpp+vim+pos) |
| `\b`            | 单词边界     | (+cpp+pos)     |
| `\B`            | 非单词边界   | (+cpp+pos)     |
| `\<`            | 单词左边界   | (+vim+pos)     |
| `\>`            | 单词右边界   | (+vim+pos)     |
| `(?=pattern)`   | 顺序预查     | (+cpp)         |
| `(?!pattern)`   | 顺序否定预查 | (+cpp)         |
| `(pattern)\@=`  | 顺序预查     | (+vim)         |
| `(pattern)\@!`  | 顺序否定预查 | (+vim)         |
| `(pattern)\@<=` | 逆序预查     | (+vim)         |
| `(pattern)\@<!` | 逆序否定预查 | (+vim)         |

## 实体模式
| 符号          | 含义                                               | 备注           |
|---------------|----------------------------------------------------|----------------|
| `.`           | 匹配除`\n`外任意字符                               | (+cpp+vim+pos) |
| `[...]`       | 匹配序列中的一个字符                               | (+cpp+vim+pos) |
| `[ - ]`       | 匹配序列中的一个字符                               | (+cpp+vim+pos) |
| `[^ ]`        | 匹配序列中未出现的一个字符                         | (+cpp+vim+pos) |
| `*`           | 匹配前一个字符0 - ∞次                              | (+cpp+vim+pos) |
| `*?`          | 懒惰匹配版`*`                                      | (+cpp)         |
| `\{-}`        | 懒惰匹配版`*`                                      | (+vim)         |
| `+`           | 匹配前一个字符1 - ∞次                              | (+cpp+vim+pos) |
| `+?`          | 懒惰匹配版`+`                                      | (+cpp)         |
| `?`           | 匹配前一个字符0或1次                               | (+cpp+vim+pos) |
| `{n}`         | 匹配前一个字符n次                                  | (+cpp+vim+pos) |
| `{n,m}`       | 匹配前一个字符n-m次                                | (+cpp+vim+pos) |
| `{n,}`        | 匹配前一个字符至少n次                              | (+cpp+vim+pos) |
| `...|...`     | 匹配左或右的模式，用`(...|...)`将其限制在局部      | (+cpp+vim+pos) |
| `(...)`       | 匹配组，将括号内的模式视作一个单位                 | (+cpp+vim+pos) |
| `(?:...)`     | 匹配组，但不支持反向引用                           | (+cpp)         |
| `\%(...)`     | 匹配组，但不支持反向引用                           | (+vim)         |
| `\1, ..., \N` | 反向引用，替换为第N个匹配组（按`(`出现的顺序编号） | (+cpp+vim+pos) |

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
| 转义字符                 | 含义                          | 备注                          |
|--------------------------|-------------------------------|-------------------------------|
| `\d` `\D`                | `[[:digit:]]、[^[:digit:]]`   | (+cpp+vim+pos)                |
| `\s` `\S`                | `[[:space:]]、[^[:space:]]`   | (+cpp+vim+pos)                |
| `\w` `\W`                | `[[:alnum:]_]、[^[:alnum:]_]` | (+cpp+vim+pos)，pos还包含中文 |
| `\n` `\r` `\t` `\v` `\f` |                               | (+cpp+vim)                    |
| `\0` `\xhh` `\uhhhh`     | null字符、ansi、unicode       | (+cpp)                        |
| `\a` `\b` `\e`           | 警报、退格、escape            |                               |
| `\x` `\X`                | `[[:xdigit:]]、[^[:xdigit:]]` | (+vim)                        |
| `\l` `\L`                | `[[:lower:]]、[^[:lower:]]`   | (+vim)                        |
| `\u` `\U`                | `[[:upper:]]、[^[:upper:]]`   | (+vim)                        |
| `\a` `\A`                | `[[:alpha:]]、[^[:alpah:]]`   | (+vim)                        |

## regex技巧
* `[^y]x*[^y]`：企图避免在连续x前后马上接y（错误）。考虑匹配`yxxxy`时，`x*`匹配`x`，前一个`[^y]`匹配第一个`x`，后一个`[^y]`匹配第三个`x`，
    因为`x*`的连续匹配提前结束，故应该这样写regex：`[^yx]x*[^yx]`

* `[^x]*y`：企图避免在y前面出现x（错误）。因为`[^x]*`可以匹配nothing，即什么也不用匹配也算作匹配成功。
    故应该这样写regex：`^[^x]*y`

* `[^x]y`：企图避免在y前面相邻位置接x（错误）。因为未考虑y前面"没有字符"的情况。
    故应该这样写regex：`([^x]|^)y`

* 利用逆向思维，将**不应该在某位置出现什么**，转换为**应该在某位置出现什么**，注意**该位置**是否可以为空
<!-- entry end -->

# 变量
<!-- entry begin: 变量 bash -->
注：zsh中数组下标从1开始，且无需`${arr[i]}`中的花括号  
注：shell变量默认都是字符串  
注：字符串变量所有空白符等特殊字符，使用时应该用双引号包含

* 定义或取消变量
    > 支持数组，不支持字典（映射）
    * var=val
    * array[0]=valA
    * array=([0]=valA [1]=valB [2]=valC)
    * array=(valA valB valC)
    * unset var

* 读取变量
    * `${+var}`                   ：变量存在为1，否则为0
    * `${var}`                    ：读取变量值
    * `${array[i]}`               ：取得数组中的元素
    * `${array[@]}`               ：取得数组中所有元素
    * `${#var}`                   ：字符串长度
    * `${#array[@]}`              ：取得数组的长度
    * `${#array[i]}`              ：取得数组中某个变量的长度
    * `${varname:-word}`          ：若不为空则返回变量，否则返回 word
    * `${varname:=word}`          ：若不为空则返回变量，否则赋值成 word 并返回
    * `${varname:?message}`       ：若不为空则返回变量，否则打印错误信息并退出
    * `${varname:+word}`          ：若不为空则返回 word，否则返回空串
    * `${varname:offset}`         ：取得字符串的子字符串（若varname为数组，则len为idx）
    * `${varname:offset:len}`     ：取得字符串的子字符串（若varname为数组，则len为idx）

* 修改变量
    > 通配符匹配
    * `${variable#pattern}`       ： 如果变量头部匹配 pattern，则删除最小匹配部分返回剩下的
    * `${variable##pattern}`      ： 如果变量头部匹配 pattern，则删除最大匹配部分返回剩下的
    * `${variable%pattern}`       ： 如果变量尾部匹配 pattern，则删除最小匹配部分返回剩下的
    * `${variable%%pattern}`      ： 如果变量尾部匹配 pattern，则删除最大匹配部分返回剩下的
    * `${variable/pattern/str}`   ： 将变量中第一个匹配 pattern 的替换成 str，并返回
    * `${variable//pattern/str}`  ： 将变量中所有匹配 pattern 的地方替换成 str 并返回
    * `eval \$$var`               ： eval为关键字，整个表达式会替换成最终的`${$var}`（假设花括号中的变量会先被替换）
    * `eval var1=\$$var2`         ： 将`eval \$$var2`的替换结果复制给`var1`
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
PS3=
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

# 常用命令
<!-- entry begin: bash builtin -->
* bash部分内建命令
    * :             ：空白命令，返回零
    * alias         ：命令别名
    * source        ：读取指定配置
    * history       ：命令历史
    * reset         ：重置因读取二进制文件导致的乱码
    * exit          ：退出bash界面
    * exec          ：调用exec()加载执行程序
    * wait          ：等待指定的子进程退出
    * sleep         ：休眠指定时间
<!-- entry end -->

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


