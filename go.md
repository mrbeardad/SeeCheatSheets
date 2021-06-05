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
import .  "path/to/pkg" // 直接导入包中导出的实体，无需额外添加包限定符
import as "path/to/pkg"
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
* 关于作用域，golang中全局作用域中的实体顺序无关，即可在实体定义语句的前面使用实体
* 局部作用域中的实体一般无法在其外访问，除非函数返回局部变量指针


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

// 包初始化器，在当前文件所有变量初始化以及导入包的初始化都完成后才调用该文件init函数
func init() {
    /*...*/
}
```
* 关于指针与值类型的参数
    * 传递指针从而函数可修改传递的变量本身
    * 传递值防止函数修改变量本身


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

// 类型选择
    switch t := t.(type) {
    case RealType:
    case OtherIF:
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
func AsyncFunc(i int) {
    cnt := 0

}
func main() {
    go AsyncFunc(args)
}
```


# 类
* **封装**（结构）
```go
// 类型别名
type double = float64

// 定义新类型
type Int     int    // 只能使用潜在类型的构造方式：字面值构造、结构体构造
type Integer int    // 潜在类型相同的类型变量之间可相互转换；潜在类型可以传递但不能复合，即[]Int与[]Integer的潜在类型不同
// 利用类型转换模拟构造
var i = Int(1)

type Class struct { // 结构体为值语义
    ExportMem T     // 导出包外
    inlineMem T     // 只能在包内访问
}
// 构造结构体
var (
    obj1 = Type{}               // 默认构造为全零值
    obj2 = Type{v1, v2}         // 必须全部显式初始化
    obj3 = Type{ExportMem: val2}// 支持部分显式初始化
    pObj = &Type{}              // 直接返回指针
)

// 访问属性
obj.ExportMem
(&obj).ExportMem

// 方法定义，只能为同一“包”中的“类”定义方法
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

// 调用方法
obj.MethodA()       // 引用语义
(&obj).MethodB()    // 引用语义
obj.MethodA()       // 值语义
(&obj).MethodB()    // 值语义
```


* **继承**（内嵌）
```go
// Reader与Writer俩接口必须具有不相交的方法集
type ReadWriter interface {
    Reader
    Writer
}

// 继承Reader与Writer的方法
type ReaderWriter struct {
    *Reader // 或值语义 Reader
    *Writer // 或值语义 Writer
}
```


* **多态**（接口）
> 底层实现猜测：当把某类型变量传递给接口变量时，传递变量指针和方法表指针
```go
// 任何完整实现了接口定义的方法的类对象/对象指针均可赋值给接口对象
type Base interface {
    Method1(T) RetT
    Method1(T) RetT
}
var i Base          // 零值为nil
i = &DerivedPtr{}   // DerivedPtr至少存在一个接口方法实现为引用语义
i = DerivedVal{}    // DerivedVal所有接口方法均实现为值语义

// 类型断言，i仅能成功转换为其真实类型或某个符合的接口类型
i     = i.(T)       // 尝试将接口类型i转换为T类型，若失败则触发恐慌
i, ok = i.(T)       // 尝试将接口类型i转换为T类型，若失败则t为零值且ok为false

// 类型选择
switch t := t.(type) {
case RealType:
case OtherIF:
default:
}
```


# 基础类型
* 所有类型都有确定的初始状态——零值，如`false`、`0`、`""`、`nil`
* 指针在golang中的用途仅限于：
    * 实现引用语义避免拷贝
    * 传引用参数使被调用者函数可修改“实参”
* golang支持的静态类型转换仅限于：
    * 数值类型之间
    * 整型、`[]byte`、`[]rune`转换为`string`
    * `string`转换为`[]byte`、`[]rune`
    * 潜在类型相同的类

## 数值类型
```go
bool
int8  int16  int32  int64  int  // 根据机器字长int32或int64
uint8 uint16 uint32 uint64 uint // 根据机器字长uint32或uint64
byte // int8
rune // int32
float32
float64
string

*T      // 指针
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
val = mapped[key]               // 若不存在则返回对应零值
val, ok = mapped[key]
```

## 信道
```go
var ch chan
ch = make(chan[, bufsize])

read = <-ch
```

