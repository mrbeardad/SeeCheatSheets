# 系统构建
```txt
├── src/
│   └── path/to/pkgA/
│       ├── go.mod
│       ├── main.go         # pacagke main
│       └── lib/
│           ├── file1.go    # package pkgA
│           ├── file2.go    # package pkgA
│           └── xxx_test.go # package pkgA
│
├── pkg/
│   └── linux_amd64/
│       └── path/to/pkgA/
│           └── pkgA.a
│
└── bin/
    └── main
```

```sh
go get [-u|-d] [-t]

go test

go build [-x|-n] [-v] [-a]

go install
```


# 包
```go
// go源文件第一条语句必须是定义包名
// 一个目录下的源文件必须属于同一个包
// 一般包名与目录名相同，包名用于在使用包内实体时的限定符，目录名用于导入路径、编译库名
package pkgname
package internal        // 仅能在该模块中访问

// 导入指定包，即指定路径下所有源文件，同属一个包的其他文件直接可见
// 路径可以为URL，如"github.com/user/repo/pkgA"
// 路径也可为系统路径，前缀为$GOROOT/src/或$GOPATH/src
import "path/to/pkg"
import as "path/to/pkg"
import .  "path/to/pkg" // 直接导入包中导出的实体，无需额外添加包限定符
import _  "path/to/pkg" // 指明不会使用该包中的实体，仅利用导入该包的副作用(init函数)
import (
    "path/to/pkg1"
    as "path/to/pkg2"
)

// 导出全局作用域中任何以大写字母开头的函数、变量、常量、接口、类、类成员、类方法
func Func(arg T) {  }
var Var Type
const VAR = 0
type Base interface { Method(T) RetT }
type Class struct { Mem Type }
func (self *Class) Method(arg T) RetT {  }
```


# 函数
* 函数也算一种数据类型
* 关于作用域，golang中全局作用域中的实体顺序无关，即可在实体定义语句的前面使用实体
* 局部作用域中的实体一般无法在其外访问，除非函数返回局部变量指针
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

// 闭包，可隐式引用捕获闭包外的局部变量，一定要注意其并发问题
// 解决方法：1. 外覆函数包装启动goroutine 2. 利用传递值拷贝参数代替引用捕获
func counter(i int) func() int {
    return func() int {
        i++
        return i
    }
}

// 包初始化器，在当前文件所有变量初始化以及导入包的初始化都完成后才调用该文件init函数
func init() {
    /*...*/
}
```


# 变量
```go
// 全局变量声明
var Var Type
var _   Type    // 指明不会使用该变量，仅用于接口检测或调试时临时避免报错

func f() {
// 局部变量声明
    var var0 Type
    var var1 Type = Var
    var var2      = Var
    var (
        var5 Type
        var6 Type = Var
        var7      = Var
    )
    var3         := Var
    var3, var4   := Var, Var
    _, ok        := Var, Var
}
```

# 常量
```go
// 常量只能为数字类型与字符类型
const BIGINT = 1 << 511     // 常量整数一般由512位表示，故赋值给变量时很可能造成溢出

const (
    _  = iota               // 枚举器，初始值为0
    KB = 1 << (10 * iota)   // 此时iota为1，则KB为1 << 10
    MB                      // 隐式延续表达式 1 << (10 * iota)，则MB为1 << 20
    GB
    TB
    EB
)

```


# 语句
* **分支**
```go
    if init; cond {

    } else if cond {

    } else {

    }

// golang中并未限制label为整形常量，但variable与label的类型必须相同
    switch init; variable {
    case label1:    // 每条分支中的变量独立
        break
    case label2, label3:
        fallthrough
    default:
        switch variable {
            // case label从上到下求值
        }
    }

// 一种if-elseif-else的简洁写法
    switch {
    case cond1:
    case cond2:
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

// 支持字符串、数组、数组指针、切片、映射、信道
    for idxOrKey, val := range container { // 注意range表达式会对值进行一次拷贝，对于字符串与数组本身为值语义致迭代数组与原数组不同
        for idxOrKey := range container  {
            break
        }
    }
```


* 延迟调用
```go
// defer将函数地址及其参数（当场捕获）压栈，延迟到当前函数“返回前”按出栈顺序调用
    defer func () {
        retErr = recover()  // 平复恐慌（捕获异常）
        panic(interface{}{})// 触发恐慌（抛出异常）
    }()
    defer call3rd()
    defer call2nd()
    defer call1st()
```

* 异步并发
    * 默认用户层代码都运行在一个线程中
    * 设置环境变量`GOMAXPROCS=NCPU`或在程序中调用函数`runtime.GOMAXPROCS(runtime.NumCPU())`修改
```go
// 利用无缓冲信道同步goroutines
func seq() {
    sem := make(chan struct{})
    for i := 0; i < 10; i++ {
        go func() {
            <-sem               // 抢占
            println(i)
            sem <- struct{}{}   // 唤醒
        }()
    }
    sem <- struct{}{}           // 启动
    <-sem                       // 等待
}
```


# 类
* **封装**（结构）
```go
// 类型别名
type double = float64

// 定义新类型
type Int     int    // 继承潜在类型的构造方式
type Integer int    // 潜在类型相同的类型变量之间可相互转换，类型转换关系可传递，潜在类型可以传递但不能复合
type Class struct { // 结构体为值语义
    ExportMem T     // 导出包外
    inlineMem T     // 只能在包内访问
}
// 利用类型转换模拟构造
var i = Int(1.0)
// 构造结构体
var (
    obj1 = Type{}               // 默认构造为全零值
    obj2 = Type{v1, v2}         // 必须全部显式初始化
    obj3 = Type{ExportMem: val2}// 支持部分显式初始化
    pObj = &Type{}              // 直接返回指针
)

// 方法定义，只能为同一“包”中的自定义类型定义方法
func (self Class) MethodVal(arg T) RetT {   // 值方法，值类型的方法集仅包含其值方法
    self.inlineMem = arg
    self.ExportMem = arg
    return RetT{"B"}
}
func (this *Class) MethodPtr(arg T) RetT {  // 指针方法，指针类型的方法集包含值方法和指针方法
    self.inlineMem = arg
    self.ExportMem = arg
    return RetT{"A"}
}

// 调用方法，指针与值方法的调用区别可被隐藏
obj.MethodPtr()     // 引用语义
(&obj).MethodPtr()  // 引用语义
obj.MethodVal()     // 值语义
(&obj).MethodVal()  // 值语义
```


* **复用**（内嵌）
```go
// 接口组合
type Reader interface {
    Read() string
}
type Writer interface {
    Write(string)
}
type ReadWriter interface { // 注意方法的“冲突”与“覆盖”
    Reader
    Writer
}

// 结构嵌套
type R struct {}    // 实现了Reader接口
type W struct {}    // 实现了Writer接口
type RW struct {    // 匿名字段的方法集会被加入外部类型的方法集
    W               // 例外：RW不包含*R的方法集。
    *R
}
```


* **动态绑定**（接口）
```go
// 任何完整实现了接口定义的方法的类对象均可赋值给接口对象
type Base interface {
    Method1(T) RetT
    Method1(T) RetT
}
var i Base          // 零值为nil。接口对象实际存储为iface数据结构，存储“类型信息”与“动态值”
i = DerivedVal{}    // 值类型的方法集仅包含其值方法
i = &DerivedPtr{}   // 指针类型的方法集包含值方法和指针方法

// 类型断言（运行时），i仅能成功转换为其真实类型或某个符合的接口类型
i     = i.(T)       // 尝试将接口类型i转换为T类型，若失败则触发恐慌
i, ok = i.(T)       // 尝试将接口类型i转换为T类型，若失败则t为零值且ok为false

// 类型选择
switch t := i.(type) {
case RealType:
case OtherIF:
default:
}
```

* 综合
    * 方法一般实现为指针方法
    * 匿名内嵌一般使用值内嵌
    * 赋值接口一般使用指针


# 基础类型
* 所有类型都有确定的初始状态——零值，如`false`、`0`、`""`、`nil`
* golang中指针的用途：
    * 避免拷贝参数
    * 修改参数原值
    * 引用共享数据
* golang支持的静态类型转换仅限于：
    * 数字类型之间
    * 整型、`[]byte`、`[]rune`转换为`string`
    * `string`转换为`[]byte`、`[]rune`
    * 潜在类型相同的类

## 指针类型
```go
uintptr
*Type
```

## 数字类型
```go
// 整型与浮点型字面量的类型会由编译器根据上下文进行定义
bool
int8  int16  int32  int64  int  // 根据机器字长int32或int64
uint8 uint16 uint32 uint64 uint // 根据机器字长uint32或uint64
float32
float64
```

## 字符类型
```go
byte
rune
string
```


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
* 子数组视图
* 共享数组
```go
var slice []int                 // 零值为nil

slice = make([]int, len[, cap]) // 构建动态长度切片
slice = []int{0, 1, /*...*/}    // 切片字面值
slice = array[:]                // 数组切片

v = slice[idx]
s = slice[beg:end]
s = slice[beg:]
s = slice[:end]
s = slice[:]

slice = append(slice, newElem)
```

## 映射
* 引用语义的哈希表
```go
// 键类型不能为函数、切片、字典，数组元素或结构字段也不能为这三种类型
var mapped map[Key]Val          // 零值为nil，无key且不能添加key

mapped = make(map[Key]Val)      // 空map，无key但可以添加key
mapped = map[Key]Val{key1: val1, /*...*/}

mapped[key] = val
delete(mapped, key)
val = mapped[key]               // 若不存在则返回对应零值
val, ok = mapped[key]
```

## 信道
* 引用语义
```go
var ch chan Type                // 零值为nil，不可写入
// 单向信道的作用时限制代码行为，双向信道可隐式转换为单向信道
var wc chan<-Type               // 单向只写信道
var rc <-chan Type              // 单向只读信道

ch = make(chan Type[, bufsize])

ch <- Type{}        // 无缓冲信道写入时若无读端则阻塞，缓冲信道写入时会在缓冲区满时阻塞
read     := <-ch    // 若无数据可读则阻塞
read, ok := <-ch    // ok判断信道是否关闭，若关闭则read为零值，ok为false
close(ch)

select {
case v, ok := <-ch1:
    if !ok {
        ch1 = nil
    }
case v, ok := <-ch2:
    if !ok {
        ch2 = nil
    }
default:
    // 如果所有信道均阻塞，则执行此处代码；若无default则阻塞当前goroutine直到某一信道准备就绪
}
```

