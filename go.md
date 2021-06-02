# 系统构建
* 模块
    * 一系列包的集合，文件系统中表现为一个目录
    * 模块名由`go.mod`文件中`module mod_name`语句指定
* 包
    * 一系列函数的集合，文件系统中表现为一个文件
    * 包名由`package pkg_name`语句指定
* 程序入口
    * `main`包中的`main`函数，`main`都包会被按照可执行文件来编译构建
* 构建
    * 依赖由go检测所有`main`包的依赖来自动安装
    * 发布时，由go自动安装所有依赖
```txt
mod/
    main.go~
    submodA/
        pkg1.go
        pkg2.go
```

```sh
go init mod_name
go build
```
