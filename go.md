# 系统构建
```txt
src/
└── path/to/mod/
    ├── go.mod          # 模块依赖的版本信息
    ├── go.sum          # 模块依赖的哈希校验
    ├── executable.go   # pacagke main
    └── otherPkg/
        ├── file1.go    # package pkgA
        ├── file2.go    # package pkgA
        └── xxx_test.go # package pkgA

$GOROOT/pkg/
└── linux_amd64/
    └── path/to/pkgA/
        └── pkgA.a

$GOPATH/pkg/
├── sumdb/
└── mod/
    ├── path/to/modA/
    │   ├── v1/
    │   └── v2/         # 新版本子目录，后向兼容，增量迁移
    └── path/to/modB/

$GOCACHE/

$GOBIN/
└── executable
```

```sh
go mod init module              # 初始化模块信息，module一般为项目URL

go list -m all                  # 列出所有直接或间接依赖的模块

go mod tidy                     # 检查模块内所有包并安全删除不再依赖的包
go get URL/to/pkg@latest        # 添加或更新依赖

go test                         # 跑测试代码
go build [-x|-n]                # 构建系统

go install                      # 安装bin或pkg
go install URL@latest
```


# 包
```go
// go源文件第一条语句必须是定义包名
// 一个目录根下的源文件必须属于同一个包
// 一般包名与目录名相同，目录名用于导入路径，包名用于在使用包内符号时的限定符
package pkgname
package internal        // 仅能在该模块中访问

// 导入指定包，即指定路径下所有源文件，同属一个包的其他文件直接可见
// 首先搜索当前模块是否匹配
// 其次缓存目录$GOPATH/pkg/mod/
// 最后本地无则下载最新版：1. 最新被标签为stable的版本 2. 最新被标签为prerelease版本 3. 最新未被标签的版本
import "Module/pkg"
import as "Module/pkg"
import .  "Module/pkg"  // 直接导入包中导出的符号，无需额外添加包限定符
import _  "Module/pkg"  // 指明不会使用该包中的符号，仅利用导入该包的副作用(init函数)

// 导出全局作用域中任何以大写字母开头的函数、变量、常量、接口、接口、字段、方法
func Func(arg T) {  }
var Var Type
const VAR = 0
type Base interface { Method(T) RetT }
type Class struct { Mem Type }
func (self *Class) Method(arg T) RetT {  }
```


# 函数
* 函数也算一种数据类型
* 关于作用域，golang中全局作用域中的符号顺序无关，即可在符号定义语句的前面使用符号
* 局部作用域中的符号一般无法在其外访问，除非函数返回局部变量指针
```go
// 一般函数示例
func f(arg1 T1, arg2 T2) RetT {
    return RetT{arg1 + arg2}
}

// 形参类型简写 + 多返回类型
func f(arg1, arg2 T) (RetT1, RetT2) {
    return RetT1{arg1}, RetT2{arg2}
}

// 可变参数 + 具名返回值
func f(arg1 T, args ...T) (slice []T, ok bool) {
    slice = args        // go提供语法糖将f(arg1 T, args ...T)重写为f(arg1 T, args []T)，将f(arg1, arg2)重写为f(arg1, []T{arg1})
    ok = args == nil    // 若变参为空则args == nil
    print(args...)      // go提供语法糖将切片将f(args...)重写为f(args[0], args[1:])
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
var _   Type = val  // 指明不会使用该变量，仅用于接口检测或调试时临时避免报错

func f() {
// 局部变量声明
    var var0 Type
    var var1 Type = Var
    var var2      = Var
    var3         := Var
    var3, var4   := Var, Var
    _, var5      := Var, Var
}
```

# 常量
* 常量类型只能为整数、浮点数、复数、字符类型
* 常量具有任意精度（实现可能会有限制），可以无类型
* 常量由字面量或其它常量初始化
* 无类型常量在表达式中会进行适当类型转换，若可能丢失精度则编译器报错
```go
const INT int = 1 << 31     // 无类型常量，任意精度
const BIGINT = 1 << 511     // 有类型常量，限制精度
// 枚举值
const (
    _, _ = iota, iota       // iota == 0
    KB = 1 << (10 * iota)   // KB == 1 << 10
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
    case label1:        // case label，从上到下，从左到右，短路求值
        break
    case label2, label3:// 每条分支中的变量独立
        fallthrough
    default:
        switch variable {
        default:
        }
    }

// golang中switch本质就是if-else
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
    for idxOrKey, val := range container {  // 注意range表达式会对值进行一次拷贝，区分引用语义与值语义类型
        for idxOrKey := range container {
            for data := range channel {     // 信道被关闭则退出循环
                break
            }
        }
    }
```


* 延迟调用
```go
// defer将函数地址及其参数（当场捕获）压栈，延迟到当前函数“返回前”按出栈顺序调用
// 一般用于恢复panic
    defer lastCall()
    defer call3rd()
    defer call2nd()
    defer call1st()
```

* 异步并发
    * 一个线程中创建有多个协程，每个线程有自己的协程队列，还有一个全局协程队列
    * 占用当前线程控制流的协程阻塞前，会主动交出控制权给下个协程（或许是下个用户协程，或许是调度器）
```go
// 利用无缓冲信道同步goroutines
    for i := 0; i < 10; i++ {
        go func() {
            println(i)
        }()
    }
```


# 表达式
* 操作符优先级

| 操作符                       | 备注               |
|------------------------------|--------------------|
| `.` `()` `[]`                | 成员、函数、下标   |
| `+` `-` `!` `^` `*` `&` `<-` | 单目               |
| `*` `/` `%` `<<` `>>` `&`    | 乘除、位移、位与   |
| `+` `-` `^` `\|`             | 加减、位与、位异或 |
| `==` `!=` `<` `<=` `>` `>=`  | 关系               |
| `&&`                         | 逻辑与             |
| `\|\|`                       | 逻辑或             |

* 赋值`=`与后缀自增减`++``--`为语句而非表达式
* 指针仅支持`*`、`==`、`!=`
* 位反操作符为`^`
* string支持`+`拼接字符串


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
    _         T     // 填充对齐
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
* 可赋值性（隐式转换）：仅当以下情况类型为V的值x可直接赋值给类型为T的值
    * V与T类型相同
    * x为无类型常量且可重解释为T类型
    * x为nil，T为指针、函数、切片、映射、信道、接口
    * T是V的最底层类型
    * V实现了接口T
    * T是双向信道，且V与T的元素的类型相同
* 显式类型转换`T(x)`：
    * 数字类型之间
    * 整型、`[]byte`、`[]rune`转换为`string`
    * `string`转换为`[]byte`、`[]rune`
    * 底层类型相同的类/类指针
* 可比较性：
    * 支持完整比较           ：结构、整数、浮点数、字符串、数组
        > 结构不比较空字段
    * 仅支持相等性比较       ：接口、指针、信道
        > 接口比较动态类型的动态值、信道比较是否为同一底层信道
    * 支持与nil进行相等性比较：函数、接口、指针、切片、映射、信道
* golang中指针的用途：
    * 避免拷贝参数
    * 修改参数原值
    * 引用共享数据

## 指针类型
```go
var p uintptr = &T{}
```

## 数字类型
```go
var b bool  // 零值false
b = false
b = true

var i int           // int8  int16  int32  int64  int为32或64位
var ui uint         // uint8 uint16 uint32 uint64 uint为32或64位
i, ui = 0b11, 0B11
i, ui = 0o77, 0O77
i, ui = 99,   99_999
i, ui = 0xEE. 0Xee

var f float64   // 零值0.0
var d float64
f = 1.1         // 整数部分或小数部分之一可省略
f = 1e1         // 小数点或指数部分之一可省略，基数为10
f = 0x1.1p1     // 整数部分或小数部分之一可忽略
f = 0x1p1       // 小数点可忽略，但指数部分不可忽略，基数为2
```

## 字符类型
```go
var b byte      // int8别名
var r rune      // int32别名
// 'a' 'ä' '文'
// '\a' '\b' '\f' '\n' '\r' '\t' '\v' '\\' '\''
// '\000' '\xff' '\u12e4' '\U00101234'

var s string    // 零值""
s = "多个符文"
s = `raw string\
even contain newline`
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

v = slice[idx]
s = slice[beg:end]
s = slice[beg:]
s = slice[:end]
s = slice[:]

append([]T, ...T)       // 返回[]T，若切片底层数组容量不足够追加，则重新分配新底层数组并追加
append([]T, ...[]T)
append([]byte, ...string)

copy(dst []T, src[]T)   // 返回int，表示min(len(dst), len(src))即赋值元素个数
copy(dst []byte, src string)
```

## 映射
* 引用语义的哈希表
```go
// 键类型不能为函数、切片、字典，数组元素或结构字段也不能为这三种类型
var mapped map[Key]Val          // 零值为nil，无key且不能添加key

mapped = make(map[Key]Val)      // 空map，无key但可以添加key
mapped = map[Key]Val{key1: val1, /*...*/}

val = mapped[key]               // 若不存在则返回对应零值
val, ok = mapped[key]

mapped[key] = val
delete(mapped, key)
```

## 信道
* 引用语义的有锁并发循环数组，并记录待写者与待读者goroutine队列链表
```go
var ch chan Type                // 零值为nil，读写阻塞
var wc chan<- Type              // 单向只写信道
var rc <-chan Type              // 单向只读信道

ch = make(chan Type[, bufsize])

ch <- Type{}        // 无缓冲信道写入时若无读端则阻塞，缓冲信道写入时会在缓冲区满时阻塞
read     := <-ch    // 若无数据可读则阻塞
read, ok := <-ch    // ok判断信道是否无缓存数据且已被关闭，若ok == false，则read = 零值
close(ch)
```

* 优雅的使用信道
    * 无缓冲信道实现同步，缓冲信道实现异步
```go
func Sync() {
    sync := make(chan struct{})
    for i := 0; i < MAXPROC; i++ {
        go func() {
            <-sync              // 抢占
            println("goroutine ", i, " finish!")
            sync <- struct{}{}  // 释放-唤醒-阻塞
        }
    }
    sync <- struct{}{}          // 启动
}

func Async() {

}
```

* 优雅的关闭信道
    * **在sender方关闭信道，不能在存在多个sender的情况下关闭信道**（表现）
    * **不能向closed信道send，不能关闭closed信道**（本质）
```go
// 当只有一个sender，直接由sender直接关闭即可
func OnlyOneSender() {
    dataChan := make(chan int, BUFSIZE)
    // sender
    go func() {
        for i := 0; i < 10; i++ {
            dataChan<- i
        }
        close(dataChan)
    }()
    // receiver
    for i := 0; i < PROCCPUNT; i++ {
        go func() {
            for data := range dataChan {
                println(data)
            }
        }()
    }
}
// 当有多个sender且只有一个receiver，由receiver通知是否结束
func MultiSenderAndOneReceiver() {
    dataChan := make(chan int, BUFSIZE)
    quitChan := make(chan struct{})
    // sender
    for i := 0; i < PROCCOUNT; i++ {
        go func() {
            select {
            case dataChan<- 1:
            case <-quitChan:
        }()
    }
    // receiver
    go func() {
        count := 0
        for data := range dataChan {
            if count < MAX {
                println(data)
            } else {
                close(quitChan)
                // 
            }
        }
    }()
}
// 当有多个sender且有多个receiver，由
```

```go
select {
case v, ok := <-ch1:
case v, ok := <-ch2:
    if !ok {
        ch2 = nil   // 读写阻塞
    }
default:
    // 如果所有信道均阻塞，则执行此处代码；若无default则阻塞当前goroutine直到某一信道准备就绪
}
```

# 内建函数
```go
len(C)                  // 返回int，接受string, [N]T, *[N]T, []T, map[K]T, chan T
cap(C)                  // 返回int，接受[N]T, *[N]T, []T, chan T
panic(interface{})      // 除法恐慌
recover() interface{}   // 若返回nil则说明当前goroutine未恐慌，或panic(nil)
```
