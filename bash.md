# 目录
<!-- vim-markdown-toc GFM -->

- [目录](#目录)
- [bash参数](#bash参数)
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

<!-- vim-markdown-toc -->

# bash参数
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
cmd [args...] [Redirection]
# Pipelines
[time] [!] Simple-Command [[| or |&] ...]
# Lists
Pipeline [[; or || or && or &] ...]
# Compound-Commands
(list)              # 子shell中执行
{ list; }           # 当前shell执行
((expression))      # C风格表达式，true返回0，false返回非零
[[ expression ]]    # test，内部禁用字词分割与路径扩展
```

| Redirection重定向 | 功能                                                                               |
| ----------------- | ---------------------------------------------------------------------------------- |
| `[fd]<file`       | 重定向fd到file，fd默认为0                                                          |
| `[fd]<<EOF`       | 重定向fd到（接下来内容直到一行只有EOF的行，不包括结束符EFO），fd默认为0            |
| `[fd]<<-EOF`      | 重定向fd到（接下来内容直到一行只有EOF的行，不包括行首的tab与结束符EOF），fd默认为0 |
| `[fd]<<<string`   | 重定向fd到（string内容），fd默认为0                                                |
| `[fd]>file`       | 重定向fd到file（截断），fd默认为1                                                  |
| `[fd]>>file`      | 重定向fd到file（追加），fd默认为1                                                  |
| `&>file`          | 重定向stdout与stderr到file（截断）                                                 |
| `&>>file`         | 重定向stdout与stderr到file（追加）                                                 |
| `fd1>&fd2`        | 重定向fd1到fd2                                                                     |
| `fd<>file`        | 打开文件描述符n指向file                                                            |

学习内建getopts命令有助于理解命令选项参数，unix系统提供了底层C函数用于解析命令行，所以大部分unix工具都遵从该规范
<!-- entry begin: bash getopts -->
```sh
getopts optstring name [args]

# optstring 示例：":op:t::"
# 开头冒号 : 表示开启silent-mode（不打印错误消息）
# 无参数选项 "o"
# 可选参数选项 "p:"
# 必有参数选项 "t::" bash不支持

# name 存储当前处理选项字符，无效选项时为`?`。若为silent-mode则当需参选项无参数时返回`:`而非`?`
# OPTARG 存储当前选项的参数
# OPTIND 存储下次getopts要解析的命令参数的索引。${!OPTIND}即下次要处理的word
```
将组成一个Simple Command的words分为四类：
| words类型 | 描述                                                                                                           |
| --------- | -------------------------------------------------------------------------------------------------------------- |
| 执行命令  | 第一个word                                                                                                     |
| 选项词    | 以`-`开头的word。分为三种：`-o`单选项、`-opt`多选项（`o`与`p`选项必须为无参选项）、`-tfile`选项`t`加参数`file` |
| 参数词    | 跟在必有参数选项词后面的word                                                                                   |
| 主体词    | 其他words均为该类型，特殊word`--`后面的所有words也是主体词                                                     |
<!-- entry end -->

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
| `${#paramter}`                | 字符长度（支持`array[@]`与`array[*]`扩展为数组长度）               |
| `"${array[*]}"`               | 扩展为`"v1 v2 v3"`                                                 |
| `"${array[@]}"`               | 扩展为`"v1" "v2" "v3"`                                             |
| `${!prefix*}`                 | 扩展前缀为prefix的环境变量名                                       |
| `${!prefix@}`                 | 扩展前缀为prefix的环境变量名                                       |
| `${!array[*]}`                | 扩展为数组keys                                                     |
| `${!array[@]}`                | 扩展为数组keys                                                     |
| `${paramter:-word}`           | 若param为空则默认值为word                                          |
| `${paramter:=word}`           | 若param为空则赋值其为word                                          |
| `${paramter:?word}`           | 若param为空则打印错误消息word                                      |
| `${paramter:+word}`           | 若param不为空则替换为word                                          |
| `${paramter:offset:length}`   | 子串展开，可省略`:length`（支持array）                             |
| `${paramter#pattern}`         | 删除最短的匹配头部（支持array）                                    |
| `${paramter##pattern}`        | 删除最长的匹配头部（支持array）                                    |
| `${paramter%pattern}`         | 删除最短的匹配尾部（支持array）                                    |
| `${paramter%%pattern}`        | 删除最长的匹配尾部（支持array）                                    |
| `${paramter^pattern}`         | 转换为大写，仅替换首个匹配，pattern默认为`?`（支持array）          |
| `${paramter^^pattern}`        | 转换为大写，替换全部匹配， pattern默认为`?`（支持array）           |
| `${paramter,pattern}`         | 转换为小写，仅替换首个匹配，pattern默认为`?`（支持array）          |
| `${paramter,,pattern}`        | 转换为小写，替换全部匹配，pattern默认为`?`（支持array）            |
| `${paramter/pattern/string}`  | 模式替换，仅替换首个匹配（支持array）                              |
| `${paramter//pattern/string}` | 模式替换，替换全部匹配（支持array）                                |
| `$(command)`                  | 命令替换，替换为命令的标准输出                                     |
| `` `command` ``               | 命令替换，替换为命令的标准输出（旧式）                             |
| `$(<file)`                    | 命令替换，替换为文件内容，更快速的`$(cat file)`                    |
| `$((expression))`             | 算数扩展                                                           |

## 进程扩展
| 扩展前    | 备注                                         |
| --------- | -------------------------------------------- |
| `<(list)` | 替换为FIFO文件名，list标准输出重定向到该文件 |
| `>(list)` | 替换为FIFO文件名，list标准输入重定向到该文件 |

## 字词分割
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
name() Compound-Command [Redirection]
```
