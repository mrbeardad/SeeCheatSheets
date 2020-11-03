# 目录
<!-- vim-markdown-toc GFM -->

- [安装](#安装)
  - [Linux](#linux)
  - [运维](#运维)
  - [数据库](#数据库)
  - [C++](#c)
  - [编程](#编程)
  - [网络](#网络)
  - [底层](#底层)
  - [其它](#其它)

<!-- vim-markdown-toc -->
# 安装
***仓库中提供了一支C++程序`see`用于快速查看笔记：***  
它会搜索`~/.cheat/`中的markdown文档中的entry，查找匹配的entry并打印  
每个entry以`<!-- entry begin:.*-->`的形式开头，以`<!-- entry end -->`结尾。
其中`.*`表示关键字，see便会搜索其中的关键子决定是否匹配  

安装步骤：
```sh
git clone https://github.com/mrbeardad/SeeCheatSheets ~/.cheat
# 国内访问github不太友好的话，可以用这条命令代替
# git clone https://gitee.com/mrbeardad/SeeCheatSheets ~/.cheat
g++ -O3 -std=c++17 -o ~/.local/bin/see ~/.cheat/see.cpp
```
## Linux
* [Linux系统架构](boot.md)
    > 从启动电源到用户登录整个过程的简短总结
* [UNIX环境高级编程](apue.md)
    > 未完待续。。。
* [Bash](bash.md)
    > 包含bash语法、正则技巧
* [Linux基础](linux.md)
    > 相当于命令手册，外加点基础概念
* [开发者工具](devtool.md)
    > 包含gcc、gdb、ssh、tmux、zsh、git、docker等工具使用
* [VIM](vim.md)
    > [定制版SpaceVim](https://github.com/mrbeardad/SpaceVim)的使用手册

## 运维
* [Docker](docker.md)

## 数据库
* [MySQL](mysql.md)

## C++
* [C++语法特性](cpp.md)
    > 一份语法清单，适合有基础的同学
* [C++编码风格](cppstyle.md)
    > 对代码风格进行规范，拒绝选择困难症
* [C++编程指导](cppguide.md)
    > 编写健壮、可移植、可扩展的C++代码，部分参考自Scott Meyers作品
* [C++程序库](cppman.md)
    > C++标准库、Boost库、MySQL++、JSON
* [CMake构建系统](cmake.md)
    > CMake系统构建工具语法

## 编程
* [逻辑表达与简化](logic.md)
    > 代码的基本逻辑表达方式
* [数据结构与算法](dsaa.md)
    > 基础数据结构与算法

## 网络
* [计算机网络](network.md)
    > 互联网整体架构与协议栈细节
* [比特币与区块链](bitcoin.md)
    > 整理了一些加密技术与区块链原理

## 底层
* [计算机系统导论](csapp.md)

## 其它
* [Markdown概念](markdown.md)
* [ANSI终端转义码](ansi.md)
* [Manjaro安装](manjaro.md)
* [小鹤双拼](xhup.md)
* [英语音标](english.md)

