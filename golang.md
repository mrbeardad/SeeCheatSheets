# 系统构建
```txt
├── src/
│   ├── path/to/pkgA/
│   │   ├── file1.go    # package pkgA
│   │   ├── file2.go    # package pkgA
│   │   └── xxx_test.go # package pkgA_test
│   │
│   └── path/to/pkgB/
│       └── main.go     # package main; import "path/to/pkgA"
│
├── pkg/
│   └── linux_amd64/
│       └── path/to/pkgA/
│           └── libpkgA.a
│
└── bin/
    └── main
```

```sh
go test
go build
go install
go get
```


# 包
```go
// go源文件第一条语句必须是定义包名
// 一般一个包中的文件都在一个与包同名的目录下
// main包中的main函数为程序入口，main包会被编译为可执行文件，其他包会被编译为静态库
package pkgname

// 导入指定包（指定路径下所有源文件），同属一个包的其他文件直接可见
// 路径可以为URL，如"github/user/repo/pkgA"
// 路径也可为系统路径，前缀为/lib/go/src/或$GOPATH/src
import "path/to/pkg"

import (
    "path/to/pkg1"
    "path/to/pkg2"
)

// 导出全局作用域中任何以大写字母开头的函数、变量、常量、类、类成员、类方法
func Func(arg T) {  }
var Var
const VAR
type TypeAlias Type
type Class struct { Mem Type }
func (self *Class) Method(arg T) {  }
```


# 函数
```go
// 一般函数示例
func f(arg1 T1, arg2 T2) RetT {
    return RetT{arg1 + arg2}
}

// 形参类型简写 + 多返回类型
func f(arg1, arg2 T) (RetT1, RetT2) {
    return RetT1{arg1}, RetT2{arg2}
}

// 具名返回值
func f(arg1 T1, arg2 T2) (x RetT1, y RetT2) {
    x = RetT1{}
    y = RetT2{}
    return
}

// 闭包
func fibonacci() func() int {
    im1, i := 0, 1
    return func() int {
        ret := im1
        ip1 := im1 + i
        im1 = i
        i = ip1
        return ret
    }
}
```


# 语句
* **分支**
```go
    if init; cond {

    } else if cond {

    } else {

    }

// init; 可省略，golang中并未限制label为整形常量
    switch init; variable {
    case label1:
        break       // 退出switch语句
    case label2, label3:
        fallthrough // 继续执行下个标签块
    default:
        /* 若无匹配项则执行该段 */
    }

// 一种if-elseif-else的简洁写法
    switch {
    case cond1:
    case cond2:
    default:
    }

// 类型断言
    switch t := t.(type) {
    case int:
    case *int:
    default:
    }
```

* **循环**
```go
    for init; cond; iter {
        for cond {
            for {
                continue
            }
        }
    }

    for idx, val := range array {
        for idx := range array {
            for key, val := range mapped {
                for key := range mapped {
                    break
                }
            }
        }
    }
```

* 延迟调用
```go
// defer将函数地址及其参数（当场捕获）“压栈”，延迟到当前函数返回时按出栈顺序调用
defer call_last()
defer call_3rd()
defer call_2nd()
defer call_1st()
```

* 并发模型
```go
func AsyncFunc() {
    cnt := 0

}
go AsyncFunc(args)
```


# 变量
```go
// 全局变量声明
var GlobalVar Type

func f() {
// 局部变量声明
    var localVar   Type
    var var1, var2 Type

// 变量初始化，Type可忽略而由初始化器推断
    var var0       Type = val
    var var1, var2 Type = val, val

// 多变量声明/初始化
    var (
        var5 Type
        var6 = val
    )

// 短变量初始化，只能在函数内部使用
    var3       := val
    var3, var4 := val1, val2    // var3在此处为赋值而非再声明

// 结构化绑定
    ret1, ret2 := f()    // func() (int, int)
    ret1, _    := f()    // 忽略第二个返回值

// 常量
    const BIGINT = 1 << 511 // 常量整数一般由512位表示，故赋值给变量时很可能造成溢出
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


## 数组
* 值语义的固定长度数组
```go
var array [128]int              // 元素值初始化为零值
array = [128]int{0, 1, /*...*/} // 数组字面值
array = [...]int{0, 1, /*...*/} // 由编译器自动推断长度

v = array[idx]
s = array[beg:end]
s = array[beg:]
s = array[:end]
s = array[:]
```

## 切片
* 引用语义的动态数组
```go
var slice []int                 // 零值为nil，无底层数组
slice = []int{0, 1, /*...*/}    // 切片字面值
slice = make([]int, len[, cap]) // 构建动态长度切片
slice = array[:]                // 数组切片

v = slice[idx]
s = slice[beg:end]
s = slice[beg:]
s = slice[:end]
s = slice[:]

append(slice, newElem)
```

## 映射
* 引用语义的哈希表
```go
var mapped map[Key]Val          // 零值为nil，无key且不能添加key
mapped = map[Key]Val{key1: val1, /*...*/}
mapped = make(map[Key]Val)      // 空map，无key但可以添加key

mapped[key] = val
delete(mapped, key)
val = mapped[key]
val, in = mapped[key]
```


# 类
## 封装
```go
// 类型别名
type TypeAlias Type

// 类定义，值语义
type Class struct {
    ExportMem T // 可在包外访问
    inlineMem T // 只能在包内访问
}

// 方法定义，只能为同一“包”中的“类/类型别名”定义方法
func (self *Class) MethodA(arg T) RetT { // 引用语义
    self.inlineMem = arg
    self.ExportMem = arg
    return RetT{"A"}
}
func (self Class) MethodB(arg T) RetT { // 值语义
    self.inlineMem = arg
    self.ExportMem = arg
    return RetT{"B"}
}
v = obj.MethodA()    // 引用语义
v = (&obj).MethodB() // 引用语义
v = (&obj).MethodB() // 值语义
v = obj.MethodA()    // 值语义


// 对象构造
var (
    obj1 = Type{}
    obj2 = Type{v1, v2} // 必须全部显式初始化
    obj3 = Type{ExportMem: val2, inlineMem: val1} // 支持部分显式初始化
    pObj = &Type{}
)
// 访问属性
obj.ExportMem
(&obj).ExportMem
```
## 动态绑定
```go
type Base interface {
    Method(T) RetT
}

var i Base = Derived{}  // 实现值语义的接口方法
var i Base = &Derived{} // 实现引用语义的接口方法
t     = i.(T)           // 强制转换接口类型为T类型，若失败则触发恐慌
t, ok = i.(T)           // 强制转换接口类型为T类型，若失败则ok为false
t     = i.(type)        // 转换为运行时真实类型的对象
```
