# 目录

- [目录](#目录)
  - [基础语法](#基础语法)
  - [命令行扩展](#命令行扩展)
    - [变量扩展](#变量扩展)
    - [字词分割](#字词分割)
    - [引用移除](#引用移除)
  - [语句](#语句)
  - [函数](#函数)

## 基础语法

```cmd
# Simple Command
cmd [args ...] [REDIRECTION]

# Pipeline
simple-command [ | simple-command2 ... ]

# List
pipeline [ [& or && or ||] pipeline2 ... ]
```

| 重定向         | 功能                                       |
| -------------- | ------------------------------------------ |
| `<file`        | 重定向 stdin 到 file                       |
| `[fd]>file`    | 重定向 stdout 到 file（截断），fd 默认为 1 |
| `[fd]>>file`   | 重定向 stdout 到 file（追加），fd 默认为 1 |
| `[fd1]>&[fd2]` | 重定向 fd1 到 fd2                          |

## 命令行扩展

### 变量扩展

```batch
:: 特殊变量
%* %0 %1 ... %9
%ERRORLEVEL%

:: 所有字面量均为字符类型；设置变量语句支持变量扩展

:: 开启设置局部变量，否则为全局变量
SETLOCAL

:: 设置字符变量，等号左右不能有空格
set string=str
:: 设置数字变量，等号右边字符被解析为算数表达式
set /A number += 1 + 2 - 3 * 4 / 5 %% 6
:: 设置数组变量
set array[0]=value

:: 结束设置局部变量
ENDLOCAL

:: 路径修改
%~1           :: remove quotes
%~f1          :: full path
%~s1          :: short name wihtout space in DOS 8.3
%~d1          :: drive
%~p1          :: parent directory
%~n1          :: name of file
%~x1          :: extension of file
```

### 字词分割

> Windows CMD 命令参数接口是将整个原始命令行字符串传入应用程序自己解析。

- 一般将`Space`, `Tab`作为分隔符将命令行划分为一个个 argv
- CMD 脚本还会额外将`,`,`;`,`=`,视作参数分割符
- 可以使用`" "`转义分隔符

### 引用移除

- 当前面所有扩展完成后，自动将多余引用符号删除
- 引用符号包括：
  - `^`用于转义命令连接符与重定向符
  - `" "`用于转义字词分隔符

## 语句

- 条件

```batch
if defined variable           compound-command [else ...]
if [/i] string1 <OP> string2  compound-command [else ...]
if [not] string1==string2     compound-command [else ...]
if [not] exist filepath       compound-command [else ...]
if [not] ERRORLEVEL number    compound-command [else ...]
```

- 循环

```batch
:: SET 即字词分隔符分割的字符，支持通配符 ? 和 *

:: * 仅扩展为普通文件名，/d 时仅扩展为目录名
for [/d] %%i in (SET) do compound-command

:: 递归搜索 drive:path 或当前目录，找到匹配 SET 的普通文件名[或目录名]
for [/d] /r [[drive:]path] %%i in (set) do compound-command

:: 遍历数字列表，集合[start,end]
for /l %%i in (start,step,end) do compound-command

:: 遍历文件内容、字符内容、命令输出
for /f ["options"] %%parameter in (filenameset) do command
for /f ["options"] %%parameter in ("text string to process") do command
for /f ["options"] %%parameter in ('cmd') do command
```

- options
  - delims=xxx：字词分隔符，默认`Space`,`Tab`,`,`,`;`,`=`
  - skip=n：跳过前 n 行，默认为 0
  - eol=;：行首注释符，默认为`;`
  - tokens=n：读取每行的第 n 个字符，默认为 1

## 函数

```batch
:caller
call :function_name

:: caller 与 callee 之间通讯可通过全局变量或者函数参数
:function_name
set /a index=2
echo The value of index is %index%
exit /b 0
```

- 命令运行在 cmd 中的 script 里，用 `cmd` 命令创建子 cmd，用 `call` 命令创建子 script
- 同一 cmd 中执行的脚本共享所有变量，子进程（子 cmd）继承父 cmd 调用处的所有变量
- `a.cmd`：执行在当前 cmd 和当前 script，当 `a.cmd` 执行完毕时立即停止当前 script
- `call a.cmd`：执行在当前 cmd 和子 script，当 `a.cmd` 执行完毕后继续执行当前 script 后续命令
- `cmd /c a.cmd`：执行在子 cmd（同一窗口），当 `a.cmd` 里调用 `exit` 时不会退出当前 cmd
- `start /wait cmd /c a.cmd`：执行在子 cmd（新窗口），当 `a.cmd` 里调用 `exit` 时不会退出当前 cmd
  - `/wait` 表示同步等待 `start` 命令执行完毕，默认异步
  - `/c` 表示执行指定脚本且在完成时关闭窗口，`/k` 表示执行指定脚本且在完成时保留窗口
- 第二种调用形式用于内部脚本，后两种调用形式用于外部脚本
