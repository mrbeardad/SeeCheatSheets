# 目录
- [笔记](#笔记)
  - [Linux](#linux)
  - [C++](#c)
  - [底层](#底层)
  - [网络](#网络)
  - [其它](#其它)

# 安装
***仓库中提供了一支C++程序`see`用于快速查看笔记***  
它会搜索`~/.cheat/notes`中的markdown文档中的entry，查找匹配的entry并打印  
每个entry以`<!-- entry begin:.*-->`的形式开头，其中`.*`表示关键字，see便会搜索其中的关键子决定是否匹配  
每个entry又以`<!-- entry end -->`结尾，see会打印头不到尾部的内容

安装步骤：
```sh
git clone https://github.com/mrbeardad/learning-notes-and-cheat-sheets ~/.cheat
# 国内访问github不太友好的话，可以用这条命令代替
# git clone https://gitee.com/mrbeardad/learning-notes-and-cheat-sheets ~/.cheat
g++ -O3 -std=c++17 -o ~/.local/bin/see ~/.cheat/see.cpp
```

## Linux
* [学习资源](notes/learning-resource.md)
    > 很久没更新了，希望还有效
* [Linux基础](notes/linux.md)
    > 相当于命令手册，外加点基础概念
* [Linux系统启动](notes/boot.md)
    > 从启动电源到用户登录整个过程的简短总结
* [Bash](notes/bash.md)
    > 包含bash语法、正则技巧与流处理命令
* [ANSI终端颜色](notes/ansi.md)
    > 终端渲染
* [开发者工具](notes/devtool.md)
    > 包含gcc、gdb、ssh、tmux、git等工具使用
* [Vim](notes/vim.md)
    > [vim配置](https://github.com/mrbeardad/SpaceVim)的使用手册

## C++
* [C++程序库](notes/cppman.md)
    > 中文版C++标准库手册，主要用于终端查询
* [C++语法特性](notes/cpp.md)
    > 一份语法清单，适合有基础的同学
* [C++风格与经验](notes/cppstyle.md)
    > 列出了非常实用经验，来自Scott Meyers与我自己的经验
* [编程技巧&数据结构与算法](notes/dsaa.md)
    > 包含了基础的循环、分支、递归的设计步骤与代码简化经验

## 底层
* [深入了解计算机系统](notes/csapp.md)
    > 看神书，做笔记

## 网络
* [比特币与区块链](notes/bitcoin.md)
    > 整理了一些加密技术与区块链原理

## 其它
* [Markdown概念](notes/markdown.md)
    > 解决Markdown恼人的区块结束与连接问题
* [RIME小鹤双拼](notes/double_pinyin.md)
    > RIME输入法默认配置为小鹤双拼
* [Manjaro安装](notes/manjaro.md)
    > 简洁的Manjaro安装教程

