# 目录
<!-- vim-markdown-toc GFM -->

- [安装](#安装)
  - [C++](#c)
  - [编程](#编程)
  - [技术](#技术)
  - [Linux](#linux)
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
[github-mathjax](https://chrome.google.com/webstore/detail/mathjax-plugin-for-github/ioemnmodlmafdkllaclgeombjnmnbima/related)  
另外，各篇文章中的图片几乎都来自网络，来源繁多复杂且大多未表明出处。。。侵删啊，乖乖。

## C++
* [C++语法特性](cpp.md)
    > 一份语法清单，适合有基础的同学
* [C++编码风格](cppstyle.md)
    > 对代码风格进行规范，拒绝选择困难症
* [C++编程向导](cppguide.md)
    > 编写高效、健壮、可移植、可扩展的C++代码
* [C++程序库](cppman.md)
    > C++标准库以及某些第三方库
* [CMake构建系统](cmake.md)
    > CMake系统构建工具语法

## 编程
* [逻辑表达与简化](logic.md)
    > 代码的基本逻辑表达方式
* [数据结构与算法](dsaa.md)
    > 基础数据结构与算法
* [正则表达式](regex.md)
    > 目前包含的正则语法版本有Grep、Vim、C++

## 技术
* [Docker](docker.md)
    > 容器，用于组建微服务架构
* [MySQL](mysql.md)
    > 关系型数据库，存储于磁盘

## Linux
* [Linux体系架构](linux.md)
    > Linux的开机流程，以及整个系统的内核态与用户态架构
* [Unix系统接口](apue.md)
    > 大部分都符合POSIX、XSI或USU规范
* [Bash脚本](bash.md)
    > bash语法、内置命令以及一些linux基础命令
* [开发者工具](devtool.md)
    > 包含gcc、gdb、ssh、tmux、zsh、git、pacman等工具使用
* [Vim](vim.md)
    > 此为[魔改版SpaceVim](https://github.com/mrbeardad/SpaceVim)的使用手册

## 网络
* [计算机网络](network.md)
    > 自顶向下展示互联网架构
* [比特币与区块链](bitcoin.md)
    > 理解大名顶顶的比特币
* [差错校验与加密技术](security.md)
    > 这些技术在网络中有重要应用

## 底层
* [信息编码](code.md)
    > 整数之无符码与补码，浮点数之IEEE 754标准，字符之Unicode与UTF-8
* [机器代码](asm.md)
    > x86-64汇编语言
* [处理器设计](cpu.md)
    > 逻辑电路与处理器设计，以及CPU高速缓存机制
* [计算机组成](bus.md)
    > 主要是总线接口与存储技术，还有显卡与显示屏工作原理
* [操作系统](os.md)
    > 链接、异常控制流、虚拟内存

## 其它
* [Markdown](markdown.md)
* [English](english.md)

