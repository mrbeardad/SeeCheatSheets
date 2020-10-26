# 目录
<!-- vim-markdown-toc GFM -->

- [安装](#安装)
  - [技术](#技术)
  - [C++](#c)
  - [编程](#编程)
  - [Linux](#linux)
  - [底层](#底层)
  - [网络](#网络)
  - [其它](#其它)

<!-- vim-markdown-toc -->
# 安装
***仓库中提供了一支C++程序`see`用于快速查看笔记：***  
它会搜索`~/.cheat/notes`中的markdown文档中的entry，查找匹配的entry并打印  
每个entry以`<!-- entry begin:.*-->`的形式开头，以`<!-- entry end -->`结尾。
其中`.*`表示关键字，see便会搜索其中的关键子决定是否匹配  

安装步骤：
```sh
git clone https://github.com/mrbeardad/learning-notes-and-cheat-sheets ~/.cheat
# 国内访问github不太友好的话，可以用这条命令代替
# git clone https://gitee.com/mrbeardad/learning-notes-and-cheat-sheets ~/.cheat
g++ -O3 -std=c++17 -o ~/.local/bin/see ~/.cheat/see.cpp
```
## 技术
* [Docker](notes/docker.md)
* [MySQL](notes/mysql.md)

## C++
* [C++语法特性](notes/cpp.md)
    > 一份语法清单，适合有基础的同学
* [C++编码风格](notes/cppstyle.md)
    > 对代码风格进行规范，拒绝选择困难症
* [C++编程向导](notes/cppguide.md)
    > 编写健壮、可移植、可扩展的C++代码，部分参考自Scott Meyers作品
* [C++程序库](notes/cppman.md)
    > C++标准库、Boost库、MySQL++、JSON
* [CMake构建系统](notes/cmake.md)
    > CMake系统构建工具语法

## 编程
* [逻辑表达与简化](notes/logic.md)
    > 代码的基本逻辑表达方式
* [数据结构与算法](notes/dsaa.md)
    > 基础数据结构与算法

## Linux
* [Linux系统启动](notes/boot.md)
    > 从启动电源到用户登录整个过程的简短总结
* [UNIX环境高级编程](notes/apue.md)
    > 未完待续。。。
* [Linux基础](notes/linux.md)
    > 相当于命令手册，外加点基础概念
* [Bash](notes/bash.md)
    > 包含bash语法、正则技巧
* [开发者工具](notes/devtool.md)
    > 包含gcc、gdb、ssh、tmux、zsh、git等工具使用
* [Vim快捷键](notes/vim.md)
    > [定制版SpaceVim](https://github.com/mrbeardad/SpaceVim)的使用手册

## 底层
* [深入了解计算机系统](notes/csapp.md)

## 网络
* [计算机网络](notes/network.md)
    > 互联网整体架构与协议栈细节
* [比特币与区块链](notes/bitcoin.md)
    > 整理了一些加密技术与区块链原理

## 其它
* [Markdown概念](notes/markdown.md)
* [ANSI终端颜色](notes/ansi.md)
* [Manjaro安装](notes/manjaro.md)
* [小鹤双拼](notes/xhup.md)
* [英语音标](notes/english.md)

