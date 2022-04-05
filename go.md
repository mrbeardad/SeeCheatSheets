# 安装
```txt
$GOROOT/
└── pkg/
    └── linux_amd64/
        └── path/to/pkg.a

$GOPATH/
├── src/
│   └── path/to/pkg/
└── pkg/
    ├── sumdb/
    └── mod/
        └── path/to/mod/
            └── v2/

$GOBIN/
└── exe

$GOCACHE/
```

# 模块
* 模块名：
  * 版本管理仓库根目录`golang.org/x/net`
  * 版本管理仓库子目录`golang.org/x/tools/gopls`
  * 添加主版本后缀`golang.org/x/repo/sub/v2`：表示主版本为2的版本
* 版本号：
  * 公共接口变更且后向不兼容时，更新主版本号
  * 公共接口添加了后向兼容的功能时，更新次版本号
  * 公共接口未变化时，更新补丁号
  * 预发布版本：可选添加pre-release后缀
  * 伪版本：特殊的预发布版本，go自动根据git-tag, utc-time, commit-hash生成伪版本号
  * v0与预发布版本均视作不稳定版本，即视其与任何版本都不兼容
  * go根据MVS(minimal version selection)算法计算模块依赖树
<!-- entry begin: go -->
```sh
go env [-u] [-w] [var ...]
# 逗号分隔列表，指定proxy-url，direct表示直接从版本管理仓库下载
GOPROXY=https://proxy.golang.org,direct
# 签名文件下载服务器
GOSUMDB=sum.golang.google.cn
# 逗号分隔列表，是GONOPROXY与GONOSUMDB的默认值，指定通配符匹配的模块作为私有模块，不走代理且无需验证
GOPRIVATE=

-mod=mod        # 忽略vendor目录，并自动更新go.mod
-mod=readonly   # 忽略vendor目录，且当需要更改go.mod时报错
-mod=vendor     # 仅从vendor目录加载模块
# 1.14之后，若存在vendor目录则默认-mod=vendor，其他情况默认-mod=readonly

go mod init [module-path]       # 初始化模块
go mod tidy [-v]                # 重新扫描并整理依赖
go mod vendor [-v]              # 同步依赖模块到vendor目录
go mod graph                    # 列出依赖模块
go mod why packages...          # 列出主模块到列出包的最短路径
go mod download [all]           # 下载主模块所有依赖

go get -d golang.org/x/net      # 管理依赖，可选添加@指定版本
# @v1.2.3       # 完整版本号
# @v1.2         # 版本号前缀
# @<v1.2.3      # 版本号比较
# @master       # 标签名或分支名
# @1234abcd     # 提交哈希前缀
# @latest       # 依次：最新release > 最新pre-release > 最新默认分支commit
# @upgrade      # （默认）规则同上，但只升级不降级
# @patch        # 选择当前主版本号与次版本号相同的补丁版本最高的版本
# @none         # 删除依赖，并降级其他模块至不依赖它的版本

go clean -cache                 # 清除构建缓存
go build [-o output] [packages] # 编译当前包或指定包，参数 -gcflags='all=-N -l' 用于调试
go run packages [arguments]     # 编译并运行指定包
go test [packages]              # 编译并测试当前包或指定包，测试文件XXX_test.go
go install [packages]           # 编译并安装当前包或指定包
```
<!-- entry end -->


# 包
* 包是模块下的一个目录所有文件集合（不包括子目录）
* 同一包中的符号可相互引用不依赖定义的先后顺序
* 导入包的搜索路径
    * 当前模块
    * `$GOROOT/src/`
    * `$GOPATH/pkg/mod/`
    * 下载模块
```go
// 源文件第一条语句必须是定义包名
package pkgname
package main                    // 构建可执行文件的包
package internal                // 仅在模块内访问的包

// 导入指定包，即指定路径下所有源文件，同属一个包的其他文件直接可见
import    "Module/path/to/pkg"  // 包限定符为对应包名，所以包名最好与目录名相同
import as "Module/path/to/pkg"  // 包限定符别名为as
import .  "Module/path/to/pkg"  // 直接导入包中导出的符号，无需额外添加包限定符
import _  "Module/path/to/pkg"  // 指明不会使用该包中的符号，仅利用导入该包的副作用(init函数)

// 包初始化：首先根据依赖关系初始化所有全局变量，再调用所有init函数
func init() {   // 一个包甚至一个源文件内可存在多个init函数

}

// 导出全局作用域中任何以大写字母开头的变量、常量、函数、类型、字段、方法、接口
var Var Type
const VAR = 0
func Func(arg T) {  }
type Class struct { Mem Type }
func (this *Class) Method(arg T) RetT {  }
type Base interface { Method(T) RetT }
```


# 变量
* 垃圾回收器：
  * 最小触发GC内存大小 4MB*GOGC/100 （GOGC默认100，`off`关闭GC）
  * 内存分配达到一定比例则触发GC
  * 2分钟没触发过GC则触发GC
  * 手动触发GC`runtime.GC()`
```go
// var variable Type
variable := initializer // 类转：Type(initializer)
a, b, _  := get_values()
```

# 常量
* 常量类型只能为整数、浮点数、复数、字符类型
* 常量具有任意精度（实现可能会有限制），可以无类型
* 无类型常量在表达式中会进行适当类型转换，若可能丢失精度则编译器报错
```go
const INT int = 1 << 31     // 有类型常量，限制精度
const BIGINT = 1 << 511     // 无类型常量，任意精度
// 枚举值
const (
    _, _ = iota, iota       // iota == 0
    KB = 1 << (10 * iota)   // KB == 1 << 10
    MB                      // 隐式延续表达式 1 << (10 * iota)，则MB为1 << 20
    GB                      // 枚举器iota大小与其出现的行数有关
    TB
    EB
)
```


# 表达式
| 操作符                       | 备注               |
| ---------------------------- | ------------------ |
| `.` `()` `[]`                | 成员、函数、下标   |
| `+` `-` `!` `^` `*` `&` `<-` | 单目               |
| `*` `/` `%` `<<` `>>` `&`    | 乘除、位移、位与   |
| `+` `-` `^` `\|`             | 加减、位与、位异或 |
| `==` `!=` `<` `<=` `>` `>=`  | 关系               |
| `&&`                         | 逻辑与             |
| `\|\|`                       | 逻辑或             |

* 赋值`=`与后缀自增减`++``--`为语句而非表达式
* 多元赋值`a, b = b, a`中，先计算等号右边产生副本，再将副本赋值给左边
* 位反操作符为`^`
* string支持`+`拼接字符串
* 指针仅支持`*`、`==`、`!=`


# 语句
* **分支**
```go
if [init;] condition {

} else if cond {

} else {

}

// golang中switch本质就是if-else
switch {
case condition:

default:

}

// variable与label的类型必须相同
switch [init;] variable {
case expr1, expr2:    // 从上到下，从左到右，短路求值
    fallthrough
default:

}
```

* **循环**
```go
for initializer; condition; iterator {
    for condition {
        for {
            break
            continue
        }
    }
}

// 支持字符串、数组、数组指针、切片、映射、通道
for idxOrKey := range container {  // 注意range表达式会浅拷贝container，注意区分值语义与引用语义
    for idxOrKey, val := range container {
        for data := range channel {     // 通道被关闭则退出循环
            for range any {
                break
                continue
            }
        }
    }
}
```


* 延迟调用
```go
// 模拟异常
defer func() {
    panic = recover()
    if panic != nil { // recover返回nil可能是当前无panic，也可能是panic(nil)
        println("catch a panic!")
    } else {
        panic("throw a panic!")
    }
}()

defer call3rd()
defer call2nd()
defer call1st()
```


* 异步并发
    * 默认goroutine运行在多个系统线程中，从而可能并行
    * 一个线程中创建有多个协程，最初始时运行调度器代码，从协程队列中取出协程来运行
        > 每个线程都有自己的协程队列（无锁），另外还有一个全局协程队列（有锁）
    * 占用当前线程控制流的协程阻塞前，会主动交出控制权给下个协程（或许是下个非阻塞用户协程，或许是调度器）
```go
go jobRun(args...)
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

// 可变参数 + 具名返回值
func f(arg1 T, args ...T) (slice []T, ok bool) {
    slice = args        // go提供语法糖将f(arg1 T, args ...T)重写为f(arg1 T, args []T)，将f(arg1, arg2)重写为f(arg1, []T{arg1})
    ok = args == nil    // 若变参为空则args == nil
    fmt.Print(args...)  // go提供语法糖将切片将f(args...)重写为f(args)而非f([]T{args})
    return
}

// 闭包：可隐式引用捕获闭包外的局部变量
func counter(i int) func() int {
    return func() int {
        i++
        return i
    }
}

```


# 类
## 封装
* 结构值底层数据结构：各字段值的聚合类
```go
// 类型别名
type double = float64

// 类型定义
type Class struct { // struct是一种特殊类型
    ExportMem T     // 导出包外
    _         T     // 填充对齐
    inlineMem T     // 只能在包内访问
}

// 方法定义，只能为同一包中的自定义类型定义方法
func (self Class) MethodV(arg T) RetT { }
func (this *Class) MethodP(arg T) RetT { }


// 构造结构体
var (
    obj1 = Type{}               // 默认构造为全零值
    obj2 = Type{v1, v2}         // 必须全部显式初始化
    obj3 = Type{ExportMem: val} // 支持部分显式初始化
    pObj = &Type{}              // 直接返回指针
)

// 访问字段与方法时，
obj.MethodP(arg)    // Class.MethodP(&obj, arg)
p2obj.MethodP(arg)  // Class.MethodP(p2obj, arg)
obj.MethodV(arg)    // Class.MethodV(obj, arg)
p2obj.MethodV(arg)  // Class.MethodV(*p2obj, arg)
```


## 复用
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

// 匿名嵌套
type R struct {}    // 实现了Reader接口
type W struct {}    // 实现了Writer接口
type RW struct {    // 匿名字段的方法集会被加入外部类型的方法集
    W               // 例外：RW不包含*R的方法集。
    *R
}
```


## 多态
* 接口值底层数据结构：含两个指针——一个指向类型信息，一个指向动态值
```go
// 任何完整实现了接口定义的方法的类对象均可赋值给接口对象
var i interface{}   // 零值为nil
// i指向拷贝的副本
i = Impl{}          // 拷贝结构值
i = &Impl{}         // 拷贝指针

// 类型断言（运行时），i仅能成功转换为其真实类型或某个符合的接口类型
i     = i.(T)       // 尝试将接口类型i转换为T类型，若失败则触发恐慌
i, ok = i.(T)       // 尝试将接口类型i转换为T类型，若失败则t为零值且ok为false

// 类型选择：获取动态真实类型
switch realtypeValue := interfaceValue.(type) {
case RealType:
case OtherIF:
default:
}
```


# 基础类型
* 概念
    * 已定义类型：内置类型、type
    * 未定义类型：复合类型，如指针、切片、映射、通道
    * 底层类型：内置类型与复合类型的底层类型为自身，tpye的底层类型为声明式最右的类型（可传递）
* 可赋值性：仅当以下情况时，类型为V的值x可直接赋值给类型为T的值
    * V与T类型相同
    * x为无类型常量且可无损重解释为T类型
    * x为nil且T为函数、接口、指针、切片、映射、通道之一
    * V实现了接口T
    * V是双向通道而T是单向通道，且V与T的元素的类型相同
    * V与T底层类型相同且至少有一个为未定义类型（即其中一个是另一个的底层复合类型）
* 显式类型转换`T(x)`：
    * 数字类型之间
    * 整型、`[]byte`、`[]rune`转换为`string`
    * `string`转换为`[]byte`、`[]rune`
    * 底层类型相同的类/类指针
* 可比较性：
    * 支持完整比较           ：数字、字符、数组
        > 结构的可比较性依赖字段，且不比较空字段
    * 仅支持相等性比较       ：接口、指针、通道
        > 接口比较动态类型的动态值、通道比较是否为同一底层通道
    * 支持与nil进行相等性比较：函数、接口、指针、切片、映射、通道
* golang中指针的用途：
    * 避免拷贝参数
    * 修改参数原值

## 指针类型
```go
var p uintptr = &T{}    // 为了垃圾回收机制而实现为“智能共享指针(shared_ptr)”
```

## 布尔类型
```go
var bool    // 零值为false
b = false
b = true
```

## 数字类型
```go
var i int           // int8  int16  int32  int64  ，int为32或64位
var ui uint         // uint8 uint16 uint32 uint64 ，uint为32或64位
i, ui = 0b11, 0B11
i, ui = 077, 0o77, 0O77
i, ui = 99,   99_999
i, ui = 0xFF 0Xff

var f float32   // 零值0.0
var d float64
f = 1.1         // 整数部分或小数部分之一可省略
f = 1e1         // 小数点或指数部分之一可省略，基数为10
f = 0x1.1p1     // 整数部分或小数部分之一可忽略
f = 0x1p1       // 小数点可忽略，但指数部分不可忽略，基数为2
```

## 字符类型
* 字符串值底层数据结构：元素类型为byte的动态数组管理器`{buf, len}`
* 字符串为immutable的原因是使用场景广泛需要确保其安全性（不被caller并发修改）
* string相比[]byte，会在修改和转换为[]byte时必定出现拷贝
```go
var b byte      // int8别名
var r rune      // int32别名，注意存储Unicode码点而非UTF-8编码，二者均兼容ascii
// 'a' '文'
// '\a' '\b' '\f' '\n' '\r' '\t' '\v' '\\' '\''
// '\000' '\xff' '\u12e4' '\U00101234'

var s string    // 零值""，字符串为只读类型
s = "多个符文"
s = `raw string\
even contain newline`

// 返回字符串子串而非切片
v = str[idx]
s = str[beg:end]
s = str[beg:]
s = str[:end]
s = str[:]

len(s)
```


## 数组
* 数组值底层数据结构：包含所有元素值的聚合类
```go
var array [128]int              // 元素值初始化为零值

array = [128]int{0, 1, /*...*/} // 数组字面值
array = [...]int{0, 1, /*...*/} // 由编译器自动推断长度

// 返回 slice
v = array[idx]
s = array[beg:end]
s = array[beg:]
s = array[:end]
s = array[:]

len(array)
len(&array)
cap(array)
cap(&array)
```

## 切片
* 切片值底层数据结构：动态数组管理器`{buf, len, cap}`
```go
var slice []int                 // 零值为nil

slice = make([]int, len[, cap]) // 构建动态长度切片
slice = []int{0, 1, /*...*/}    // 切片字面值

v = slice[idx]
s = slice[beg:end]
s = slice[beg:]
s = slice[:end]
s = slice[:]

len(slice)
cap(slice)

append([]T, ...T)       // 返回[]T，若切片底层数组容量不足够追加，则重新分配新底层数组并追加
append([]T, ...[]T)
append([]byte, ...string)

copy(dst []T, src[]T)   // 返回int，表示min(len(dst), len(src))即赋值元素个数
copy(dst []byte, src string)
```

## 映射
* **映射值底层数据结构：指向哈希表管理器的指针**
* 意味着即使底层哈希表rehash后，所有旧的映射值仍有效指向rehash后的哈希表，完全封装了内存管理
* 相反的，当切片底层数组重新分配后，旧的切片值仍指向旧的底层数组，需要用户自行管理内存
```go
// 键类型不能为函数、切片、字典，数组元素或结构字段也不能为这三种类型
var mapped map[Key]Val          // 零值为nil，无key且不能添加key否则panic

mapped = make(map[Key]Val)      // 空map，无key但可以添加key
mapped = map[Key]Val{}          // 空map，无key但可以添加key
mapped = map[Key]Val{key1: val1}

len(mapped)
val = mapped[key]               // 若不存在则返回对应零值
val, ok = mapped[key]

mapped[key] = val
delete(mapped, key)
```

## 通道
* 通道值底层数据结构：有锁队列管理器
* 通道还记录了待写者与待读者goroutine队列链表供调度算法使用
```go
var ch chan Type    // 零值为nil，读写阻塞
var wc chan<- Type  // 单向只写通道
var rc <-chan Type  // 单向只读通道

ch = make(chan Type[, bufsize])

ch <- Type{}        // 无缓冲通道发送时若无接收端则阻塞，缓冲通道写入时会在缓冲区满时阻塞
read     := <-ch    // 无缓冲通道接受时若无发送端则阻塞，缓冲通道接受时会在缓冲区空时阻塞
read, ok := <-ch    // ok判断通道是否无缓存数据且已被关闭，若ok == false，则read = 零值
close(ch)           // 通道关闭原则：只能由唯一发送者关闭
len(ch)
cap(ch)
```

* 优雅的使用通道
    * 同步串行化（`make(chan T)`）
    * 异步安全数据管道（`make(chan Future, bufsize)`）
    * 限制并发量（`make(chan struct{}, MAXPARALLEL)`、`maxChan <- struct{}{}; <-maxChan`）
    * 一对一异步事件信号（`make(chan struct{}, 1)`
    * 一对多异步事件信号（`close(done)`）
    * 多对一异步事件信号（`var wg sync.WaitGroup; wg.Add(N)`）
    * 多对一异步事件信号采用首例（使用try-select一对一）
    * 异步定时器（`<-time.After(dur)`）

```go
// 当有多个异步事件信号需要处理时，使用select进行多路复用
select {
case data, ok := <-readChan:// 当想关掉一条分支时，将对应通道赋值为nil
case <-asyncEvent1:
case _, ok := <-asyncEvent3:
case <-timer:
default:// 使用default实现try，注意不应对无缓冲通道进行try，因为可能对等方未准备好导致丢弃信号
}
```

* 优雅的关闭通道（通知所有接受方和发送方交流结束）
    * **只能由通道的唯一发送者关闭通道**，本质上是 **不能向closed通道send，不能关闭closed通道，否则引发panic**
    * 一个sender：sender作主动方，直接`close(dataChan)`即可
        > 应用上述“一对多异步事件信号”模型
    * 多个sender一个receiver：receiver作主动方，`close(stopChan)`通知senders都结束返回，从而`dataChan`被垃圾回收
        > 应用上述“一对多异步事件信号”模型和“select多路复用”模型
    * 多个sender多个receiver：引入中间者
        * 任一sender或receiver尝试发送一对一异步信号给中间者
        ```go
        select {
        case wannaClose <- struct{}{}:
        default:
        }
        ```
        * 中间者发送一对多异步信号给senders和receivers
        ```go
        close(stopCom)
        ```
        * 每个sender或receiver除了串行化读/写数据外，还要监听异步事件`<-stopCom`

