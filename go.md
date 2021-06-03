# 系统构建
* 包
    * 一系列文件的集合，文件系统中表现为一个目录
    * `package pkgname`相同的文件同属一个包
* 模块
    * 一系列包的集合，文件系统中表现为一个目录
    * 模块名由`go.mod`文件中`module modname`语句指定
* 程序入口
    * `main`包中的`main`函数即程序入口
* 构建
    * 由go检测所有`main`包的依赖并自动安装
    * 发布时，由go自动安装所有依赖
```txt
mod/
    go.mod
    main.go~
    pkg/
        pkgfile1.go
        pkgfile2.go
```

# go命令
```sh
go init modname
go build
```


# 包
```go
// 导入
import "path/to/pkg" // 导入目录下所有包

import (
    "path/to/pkg1"
    "path/to/pkg2"
)

// 导出
// 任何以大写字母开头的符号都会被导出
var Var
func Func() { }
type Type struct { }
```


# 函数
```go
// 一般函数示例
func funcName(arg1 Type, arg2, Type) RetType {
    return RetType{arg1 + arg2}
}
```
* 参数列表
    * 正常写法：`(arg1 Type, arg2 Type)`
    * 类型简写：`(arg1, arg2 Type)`

* 返回类型
    * 无返回值：` `
    * 单返回值：`RetType`
    * 多返回值：`(RetType1, RetType2)`
    * 具名返回：`(x RetType, y RetType2)`或`(x, y RetType)`
        > 空`return`语句则直接返回函数体内名为x, y的变量，返回值名作为函数声明一部分可作为文档一部分；
        > 应该仅在简单短小的函数中使用，否则降低可读性


# 语句
* 循环
```go
// 三段式写法，各段都可为空
for init; cond; iter {
    // while式写法
    for cond {
        // 死循环写法
        for {

        }
    }
}
```

* 分支
```go
// init可为空
if init; cond {

} else if cond {

} else {

}

// init可为空，init;也可为空
switch init; variable { // golang中并未限制label为整形常量
case label1:
    /* 语句自动break */
case label2:
    /* 除非加fallthrough */
default:
    /* 若无匹配项则执行该段 */
}

// 一种if-elseif-else的简洁写法
switch {
case cond1:
case cond2:
default:
}
```

* 延迟
```go
for i := 0; i < 10; i++ {
// defer将函数地址及其参数（当场捕获）“压栈”，延迟到当前函数返回时按出栈顺序调用
    defer call_func()
}
```


# 变量
**golang中由语言本身管理内存，变量本质是一个指向数据的指针，赋值的本质也即是更改指针指向**
```go
// 全局变量声明
var globalVarname Type

func f() {
// 局部变量声明
    var local_varname Type
    var var1, var2 Type
// 变量初始化，Type可忽略而由初始化器推断
    var vari       Type = val
    var var1, var2 Type = val1, val2
// 短变量初始化，只能在函数内部使用
    vari := val
    var1, var2 := val1, val2
// 多变量声明/初始化
    var (
        var1 Type
        var2 = val
    )
// 常量
    const BigInt = 1 << 511 // 常量整数一般由512位表示，但赋值给变量时很可能造成溢出
}
```



# 类型
## 数值类型
```go
bool
int8  int16  int32  int64  int  // 根据机器字长int32或int64
uint8 uint16 uint32 uint64 uint // 根据机器字长uint32或uint64
uintptr
byte // int8
rune // int32 Unicode码点
float32
float64
string

*T      // 指针
```
* 所有类型都有确定的初始状态——`0`、`false`、`""`
* 只能通过`Type(val)`显式转换而拒绝隐式转换


## 数组与切片
```go
var array = [9]int{0, 1, /* ... */}

var slice = []int{0, 1, /* ... */}

slice = array[0:2]
slice = array[0:]
slice = array[:2]
slice = array[:]
slice = nil // 无底层数组的切片
len(slice)  // 返回切片长度，即切片可见元素数量
cap(slice)  // 返回切片容量，即切片头部所在底层数组位置开始到底层数组尾部的元素数量

```


# 类
```go
// 类定义
type Type struct {
    innerMem Type
    OutterMem Type
}
// 对象构造
var (
    obj1 = Type{}
    obj2 = Type{v1, v2} // 不能部分显式初始化
    obj3 = Type{OutterMem: val2, innerMem: val1} // 支持部分显式初始化
    pObj = &Type{}
)
// 属性访问
obj.OutterMem
(&obj).OutterMem
```
