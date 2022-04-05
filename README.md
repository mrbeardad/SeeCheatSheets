- [安装](#安装)
- [编程之道](#编程之道)
- [编程语言](#编程语言)
  - [C++](#c)
  - [Go](#go)
  - [Python](#python)
- [Web](#web)
- [Server](#server)
- [Linux](#linux)
- [底层原理](#底层原理)
- [其他](#其他)

# 安装
```sh
# 国内访问github不太友好的话，可以将URL中的github.com改为gitee.com
git clone --depth=1 https://github.com/mrbeardad/SeeCheatSheets ~/.cheat
mkdir ~/.cheat/build
cd ~/.cheat/build
cmake -D CMAKE_BUILD_TYPE=Release ..
cmake --build . -t see
cmake --install .
```
***本仓库中提供了一支C++程序`see`用于快速查看笔记：***
它会搜索`~/.cheat/*.md`中匹配的笔记条目并进行语法高亮后打印到终端。
每个条目以`<!-- entry begin: .* -->`的形式开头，并以`<!-- entry end -->`结尾；
其中`.*`表示关键字，see便会搜索其中的关键子决定是否匹配；
详情请使用`see -h`查看。

![see](images/see.jpg)


**注**：部分笔记插入了Tex公式，~~介于鄙人的懒惰~~ 介于GFM不支持Tex，诸位就自己下个浏览器插件吧。
[github-mathjax](https://chrome.google.com/webstore/detail/mathjax-3-plugin-for-gith/peoghobgdhejhcmgoppjpjcidngdfkod/related?hl=en "如果从Web Store安装失败则尝试手动下载并安装其github源码")
另外，各篇文章中的图片几乎都来自网络，来源繁多复杂且大多未表明出处。。。侵删啊，乖乖。

# 编程之道
* [计科八股](interview.md):star:
    > 计算机科学基础知识
* [编码技术](coding.md):star:
    > 解题技巧与编码经验
* [设计模式](dspt.md):star:
    > 应用框架系统的设计套路

# 编程语言
* [语言总结](langsum.md):star:

## C++
* [C++语法特性](cpp.md)
    > 一份C++语法清单，适合有基础的同学
* [C++编程指导](cppguide.md)
    > 优雅的编写C++代码
* [C++程序库](cppman.md)
    > 应该掌握的基础C++库
* [CMake构建系统](cmake.md)
    > 依赖管理与项目构建

## Go
* [Go语言基础](go.md)
    > 语言基础
* [Go程序库](goman.md)
    > 常用程序库

## Python
* [Python语言基础](python.md)
    > 基础语法与内置类型操作

# Web
* [HTTP](http.md)
    > 超文本传输协议
* [HTML](html.md)
    > 超文本标记语言
* [CSS](css.md)
    > 层叠样式表
* [JaveScript](js.md)
    > 前端编程语言
* [React](react.md)
    > 前端UI框架

# Server
* K8S
    > 用于动态编排容器集群
* [Docker](docker.md)
    > 提供进程隔离的运行环境
* [Nginx](nginx.md)
    > 用于路由与负载均衡
* [MySQL](sql.md)
    > 关系型数据库
* [Redis](redis.md)
    > 数据结构化内存存储
* Kafka
    > 消息队列

# Linux
* [Linux知识体系](linux.md)
    > 从Linux启动流程开始了解整个Linux系统知识体系
* [Linux系统接口](apue.md)
    > 大部分都符合POSIX、XSI或USU规范
* [Shell命令行](bash.md)
    > bash基础语法与一些内建命令
* [开发者工具](devtool.md)
    > 一些常用的linux命令与工具

# 底层原理
> 精华版可阅读[计科八股](interview.md)
> * [处理器设计](cpu.md)
>     > 逻辑电路与处理器设计，以及CPU高速缓存机制
> * [计算机组成](bus.md)
>     > 主要是总线接口与存储技术，还有显卡与显示屏工作原理
> * [信息编码](code.md)
>     > 整数之无符码与补码，浮点数之IEEE 754标准，字符之Unicode与UTF-8
> * [汇编语言](asm.md)
>     > x86-64汇编语言
> * [操作系统](os.md)
>     > 链接、异常控制流、虚拟内存
> * [数据库存储](innodb.md)
>     > MySQL中innodb存储引擎的设计架构，包括索引与事务原理
> * [计算机网络](network.md)
>     > 自顶向下展示互联网架构
> * [信息学应用](security.md)
>     > 差错校验与加密技术
> * [数据结构与算法](dsaa.md)

# 其他
* [Markdown语法](markdown.md)
* [多种正则语法](regex.md)
* [比特币系统](bitcoin.md)
* [英文读音](english.md)
* [色彩理论](color.md)