# 目录
<!-- vim-markdown-toc GFM -->

- [安装](#安装)
  - [C++](#c)
  - [Linux](#linux)
  - [底层](#底层)
  - [网络](#网络)
  - [技术](#技术)
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

## C++
* [C++语法特性](notes/cpp.md)
    > 一份语法清单，适合有基础的同学
* [C++风格与经验](notes/cppstyle.md)
    > 列出了一些实用经验，部分参考自Scott Meyers作品
* [编程技巧&数据结构与算法](notes/dsaa.md)
    > 包含了基础的逻辑表达技巧、数据结构与算法
* [C++程序库](notes/cppman.md)
    > 目前包含了C++标准库、部分Boost库、MySQL++
* [CMake构建系统](notes/cmake.md)

## Linux
* [Linux基础](notes/linux.md)
    > 相当于命令手册，外加点基础概念
* [开发者工具](notes/devtool.md)
    > 包含gcc、gdb、ssh、tmux、zsh、git等工具使用
* [Bash](notes/bash.md)
    > 包含bash语法、正则技巧与流处理命令
* [Vim](notes/vim.md)
    > [定制版SpaceVim](https://github.com/mrbeardad/SpaceVim)的使用手册
* [Linux系统启动](notes/boot.md)
    > 从启动电源到用户登录整个过程的简短总结

## 底层
* [深入了解计算机系统](notes/csapp.md)

## 网络
* [比特币与区块链](notes/bitcoin.md)
    > 整理了一些加密技术与区块链原理

## 技术
* [MySQL](notes/mysql.md)
* [Docker](notes/docker.md)

## 其它
* [Markdown概念](notes/markdown.md)
* [ANSI终端颜色](notes/ansi.md)
* [Manjaro安装](notes/manjaro.md)
* [小鹤双拼](notes/xhup.md)
* [英语音标](notes/english.md)

