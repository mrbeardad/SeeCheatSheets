# 目录

<!-- vim-markdown-toc GFM -->

- [目录](#目录)
  - [bash 参数](#bash-参数)
  - [基础语法](#基础语法)
  - [命令行扩展](#命令行扩展)
    - [花括号扩展](#花括号扩展)
    - [波浪号扩展](#波浪号扩展)
    - [变量/命令/算数扩展](#变量命令算数扩展)
    - [进程扩展](#进程扩展)
    - [字词分割](#字词分割)
    - [路径扩展](#路径扩展)
    - [引用移除](#引用移除)
  - [语句](#语句)
  - [函数](#函数)

<!-- vim-markdown-toc -->

## bash 参数

<!-- entry begin: bash set -->

| 选项        | 功能                                                      |
| ----------- | --------------------------------------------------------- |
| -c          | 执行给定命令                                              |
| -s          | 从标准输入读取命令                                        |
| -u          | 未初始化变量被视作错误，同`set -u`                        |
| -e          | 遇非零返回值则立即退出，同`set -e`                        |
| -o pipefail | 管道命令只有当所有命令返回零才返回零，同`set -o pipefail` |

<!-- entry end -->

## 基础语法

```sh
# Simple Command
name [args ...] [REDIRECT]

# Pipeline
[time] [!] simple-command [ [| or |&] simple-command2 ...]

# List
pipeline [ [; or || or && or &] pipeline2 ...]

# Compound Command
{ list; }           # 当前shell执行
(list)              # 子shell中执行
((expression))      # C风格表达式，true返回零，false返回非零
[[ expression ]]    # test，内部禁用字词分割与路径扩展
```

| 重定向          | 功能                                                                                      |
| --------------- | ----------------------------------------------------------------------------------------- |
| `[fd]<file`     | 重定向 fd 到 file（读取），fd 默认为 0                                                    |
| `[fd]<<EOF`     | 重定向 fd 到接下来内容直到一行只有 EOF 的行且不包含 EOF（读取），fd 默认为 0              |
| `[fd]<<-EOF`    | 重定向 fd 到接下来内容直到一行只有 EOF 的行且不包含行首的 tab 与 EOF（读取），fd 默认为 0 |
| `[fd]<<<string` | 重定向 fd 到（string 内容），fd 默认为 0                                                  |
| `[fd]>file`     | 重定向 fd 到 file（截断），fd 默认为 1                                                    |
| `[fd]>>file`    | 重定向 fd 到 file（追加），fd 默认为 1                                                    |
| `&>file`        | 重定向 stdout 与 stderr 到 file（截断）                                                   |
| `&>>file`       | 重定向 stdout 与 stderr 到 file（追加）                                                   |
| `fd1>&fd2`      | 重定向 fd1 到 fd2                                                                         |
| `fd<>file`      | 打开文件描述符 n 指向 file                                                                |

## 命令行扩展

### 花括号扩展

| 扩展前      | 扩展后     | 备注                            |
| ----------- | ---------- | ------------------------------- |
| `{a,b,c}`   | `a b c`    | 逐词扩展，可再嵌套逐词扩展      |
| `{a..c}`    | `a b c`    | 字符序列扩展                    |
| `{1..3}`    | `1 2 3`    | 数字序列扩展                    |
| `{1..3..2}` | `1 3`      | 可选设置增量，默认 1 或-1       |
| `{01..9}`   | `01 02 03` | 第一个数字 0 开头则用零补齐宽度 |

### 波浪号扩展

> 仅当`~`为单词开头才展开

| 扩展前  | 扩展后                | 备注                              |
| ------- | --------------------- | --------------------------------- |
| `~user` | `/path/t，/user/home` | ~与下个/之间的所有字符被当做 user |
| `~+`    | `/path/to/PWD`        | `$PWD`                            |
| `~-`    | `/path/to/OLDPWD`     | `$OLDPWD`                         |

### 变量/命令/算数扩展

```sh
# 特殊变量
$0 $1 $2    # 位置参数
$@ $*       # 所有位置参数
$#          # 位置参数个数
$$          # 当前 shell 的 PID
$!          # 上个后台运行命令的 PID
$?          # 上条前台 pipeline 返回值

# 所有字面量均为字符类型；设置变量语句支持变量扩展、命令扩展、算数扩展
export name                 # 将变量导出到 shell 的环境变量
parameter=value             # 设置全局变量，等号左右不能有空格
declare parameter=str       # 设置局部字符变量
declare -i parameter=num    # 设置局部数字变量，将 num 解析为算数表达式
declare [-i] array[0]=value # 设置局部数组变量
declare [-i] array=(v0 v1 v2)
declare [-i] array=([0]=v0 [1]=v1 [2]=v2)

# array 与 parameter 可相互转换
# array 转为 parameter 相当于 parameter=array[0]
# parameter 转为 array 相当于 array[0]=parameter
# pattern 为通配符语法

${parameter}                 # 变量值，无歧义时可省略花括号
${!parameter}                # 间接变量值，大概意思即 ${$parameter}
${!prefix@}                  # 扩展前缀为 prefix 的环境变量名
${#parameter}                # 字符长度
${parameter:+word}           # 若 parameter 非空则扩展为 word
${parameter:-word}           # 若 parameter 为空则扩展为 word
${parameter:=word}           # 若 parameter 为空则将其赋值并扩展为 word
${parameter:?word}           # 若 parameter 为空则打印错误消息 word
${parameter:offset}          # 子字符串
${parameter:offset:length}   # 子字符串
${parameter#pattern}         # 删除最短的匹配头部
${parameter##pattern}        # 删除最长的匹配头部
${parameter%pattern}         # 删除最短的匹配尾部
${parameter%%pattern}        # 删除最长的匹配尾部
${parameter^pattern}         # 将第一个字符转换为大写，pattern 默认为 ?
${parameter^^pattern}        # 将所有匹配字符转换大写，pattern 默认为 ?
${parameter,pattern}         # 将第一个字符转换为小写，pattern 默认为 ?
${parameter,,pattern}        # 将所有匹配字符转换小写，pattern 默认为 ?
${parameter/pattern/string}  # 模式替换，替换首个匹配
${parameter//pattern/string} # 模式替换，替换全部匹配

# array[@] 与 array[*] 形式的区别在于
# "${array[@]}" 扩展为多个单词 "str0" "str1" "str2"
# "${array[*]}" 扩展为一个单词 "str0 str1 str2"
# 以下均以前者为例

${array[idx]}                # 数组元素，idx可以是数字，也可以是变量名
${array[@]}                  # 数组所有元素
${!array[@]}                 # 扩展为数组的所有下标
${#array[@]}                 # 数组长度
${array[@]:offset}           # 子数组
${array[@]:offset:length}    # 子数组
${array[@]#pattern}          # 删除最短的匹配头部，作用于每个元素
${array[@]##pattern}         # 删除最长的匹配头部，作用于每个元素
${array[@]%pattern}          # 删除最短的匹配尾部，作用于每个元素
${array[@]%%pattern}         # 删除最长的匹配尾部，作用于每个元素
${array[@]^pattern}          # 将第一个字符转换为大写，pattern 默认为 ?，作用于每个元素
${array[@]^^pattern}         # 将所有匹配字符转换大写，pattern 默认为 ?，作用于每个元素
${array[@],pattern}          # 将第一个字符转换为小写，pattern 默认为 ?，作用于每个元素
${array[@],,pattern}         # 将所有匹配字符转换小写，pattern 默认为 ?，作用于每个元素
${array[@]/pattern/string}   # 模式替换，替换首个匹配，作用于每个元素
${array[@]//pattern/string}  # 模式替换，替换全部匹配，作用于每个元素

$(command)                   # 命令替换，替换为命令的标准输出，旧式为 `command`
$(<file)                     # 命令替换，替换为文件内容，更快速的 $(cat file)
$((expression))              # 算数扩展
```

### 进程扩展

| 扩展前    | 备注                                            |
| --------- | ----------------------------------------------- |
| `<(list)` | 替换为 FIFO 文件名，list 标准输出重定向到该文件 |
| `>(list)` | 替换为 FIFO 文件名，list 标准输入重定向到该文件 |

### 字词分割

> Linux Shell 命令参数接口是在 Shell 层将参数分隔成字符数组再传入应用程序。

- `Space`、`Tab`、`NewLine`将命令行分割为一个个单词，并将参数数组传入应用程序

### 路径扩展

- 若无匹配路径则默认不扩展
- 扩展后的路径直接分割为一个个 argv，即使路径名包含分隔符，因为路径扩展在字词分割后进行

| 扩展前 | 备注                                              |
| ------ | ------------------------------------------------- |
| `*`    | 扩展为任意长字符来匹配目录项                      |
| `**`   | 匹配任意子目录中的目录项，需要`shopt -s globstar` |
| `?`    | 一个任意字符                                      |
| `[]`   | 同正则表达式的`[]`                                |

### 引用移除

- 当前面所有扩展完成后，自动将多余引用符号删除
- 引用符号包括`\`、`" "`、`' '`用于转义特殊字符
- 其中`" "`中还会保留变量扩展、命令替换、算数扩展

## 语句

- 条件

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

- 循环

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

## 函数

```sh
# 返回值为最后一个命令返回值
name() Compound-Command [Redirection]
```

- `source a.sh`执行在当前 shell
- `func`内部命令执行在当前 shell
- `a.sh`外部命令执行在子 shell
- `(list)`执行在子 shell
- 同一 shell 执行的脚本共享所有变量，子 shell 仅继承父 shell 环境变量
