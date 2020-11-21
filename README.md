# 目录
<!-- vim-markdown-toc GFM -->

- [安装](#安装)
  - [技术](#技术)
  - [Linux](#linux)
  - [C++](#c)
  - [编程](#编程)
  - [网络](#网络)
  - [底层](#底层)
  - [其它](#其它)

<!-- vim-markdown-toc -->
# 安装
***本仓库中提供了一支C++程序`see`用于快速查看笔记：***  
它会搜索`~/.cheat/*.md`中的entry，查找匹配的entry并打印；
每个entry以`<!-- entry begin: .* -->`的形式开头，并以`<!-- entry end -->`结尾；
其中`.*`表示关键字，see便会搜索其中的关键子决定是否匹配；
详情请使用`see -h`查看。

**安装步骤：**
```sh
git clone --depth=1 https://github.com/mrbeardad/SeeCheatSheets ~/.cheat
# 国内访问github不太友好的话，可以将URL中的github.com改为gitee.com
if [ ! -d ~/.local/bin ] ;then
    mkdir -p ~/.local/bin
fi
g++ -O3 -std=c++17 -o ~/.local/bin/see ~/.cheat/see.cpp
```

**注**：部分笔记插入了Tex公式，介于鄙人的懒惰，诸位就自己下个浏览器插件吧。
[github-mathjax](https://github.com/orsharir/github-mathjax/issues/24#issuecomment-438140315 "下载ZIP解压后通过浏览器开发者模式安装")  
另外，各篇文章中的图片几乎都来自网络，来源繁多复杂且大多未表明出处。。。侵删啊，乖乖。

## 技术
* [Docker](docker.md)
* [MySQL](mysql.md)

## Linux
* [Linux知识框架](linux.md)
    > 讲述Linux的开机过程，以及整个系统的内核态与用户态架构
* [Linux系统接口](apue.md)
    > Linux的API，大部分都符合POSIX、XSI或USU规范
* [bash脚本](bash.md)
    > bash语法与linux基础命令
* [开发者工具](devtool.md)
    > 包含gcc、gdb、ssh、tmux、zsh、git、pacman等工具使用
* [Vim](vim.md)
    > 这是[魔改版SpaceVim](https://github.com/mrbeardad/SpaceVim)的使用手册

## C++
* [C++语法特性](cpp.md)
    > 一份语法清单，适合有基础的同学
* [C++编码风格](cppstyle.md)
    > 对代码风格进行规范，拒绝选择困难症
* [C++编程向导](cppguide.md)
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
* [正则表达式](regex.md)
    > 包含的正则语法版本有posix、vim、c++

## 网络
* [计算机网络](network.md)
    > 互联网整体架构与协议栈细节
* [比特币与区块链](bitcoin.md)
    > 整理了一些加密技术与区块链原理

## 底层
* [信息编码](code.md)
    > 整数之无符码与补码，浮点数之IEEE 754标准，字符之Unicode与UTF-8
* [机器代码](asm.md)
    > x86-64汇编语言
* [处理器](cpu.md)
    > 逻辑电路与处理器设计，以及CPU高速缓存机制
* [计算机组成](bus.md)

* [计算机系统](csapp.md)

## 其它
* [Markdown](markdown.md)
* [English](english.md)

