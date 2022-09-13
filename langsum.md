# Programing Language Summary

- [Programing Language Summary](#programing-language-summary)
  - [依赖](#依赖)
  - [变量](#变量)
  - [常量](#常量)
  - [表达式](#表达式)
  - [语句](#语句)
  - [函数](#函数)
  - [面向对象](#面向对象)
    - [封装](#封装)
    - [继承](#继承)
    - [多态](#多态)
  - [命名规范](#命名规范)
  - [基础库与框架](#基础库与框架)

## 依赖

- C++

  - 程序入口：main 函数
  - 依赖单元：一个头文件 + 一个实现文件（可选）
  - 依赖导出：头文件中所有宏与符号，初始化：global-constructor
  - 依赖导入：`#include <path/to/header.hpp>`，符号限定于命名空间
  - 依赖管理：手动（git-submodule & cmake）

- Go

  - 程序入口：main 包中 main 函数
  - 依赖单元：一个目录（不包括子目录）
  - 依赖导出：大写字母开头的符号，初始化：包中所有 init 函数
  - 依赖导入：`import "module.github.com/path/to/package"`，符号限定于包名
  - 依赖管理：自动（go-mod）

- JavaScript

  - 程序入口：任意源文件
  - 依赖单元：一个源文件
  - 依赖导出：`export`声明导出，初始化：导入即执行
  - 依赖导入：`import { name } from 'root/path/to/module'`，符号限定于模块名
  - 依赖管理：自动（npm）

- Python
  - 程序入口：任意源文件(`__init__ == "__main__"`)
  - 依赖单元：一个源文件
  - 依赖导出：非`_`开头的符号，初始化：导入即执行
  - 依赖导入：`from package.subpackage import module`，符号限定于文件名
  - 依赖管理：自动（pip）

## 变量

> - 结构型：变量本身代表一块内存结构，赋值拷贝至少包括变量内存结构
> - 引用型：变量仅仅指向一块内存结构，赋值拷贝仅针对指针值
> - 强类型：变量类型不可变，函数签名不可变，无动态符号表
> - 弱类型：变量类型动态可变，函数签名不固定，有动态符号表
> - 引用语义的目的：
>   - (Copy) 避免拷贝
>   - (Write) 修改对象
>   - (Dynamic) 动态绑定

- C++

  ```cpp
  Type foo{};                           // 待初始化
  Type foo{args};                       // 构造
  auto foo = initializer;               // 拷贝
  auto foo = static_cast<Type>(other);  // 转换
  auto [a, b]  = aggregation;           // 解构
  ```

  - 变量类型：结构型强类型
  - 生命周期：退出块作用域时销毁
  - 作用域：退出块作用域后不可见

- Go

  ```go
  var foo Type            // 待初始化
  foo  := Type{fd: data}  // 构造
  foo  := initializer     // 拷贝
  foo  := Type(other)     // 转换
  a, _ := value-list      // 解构
  ```

  - 变量类型：结构型强类型
  - 生命周期：直到引用计数为零才时销毁
  - 作用域：退出块作用域后不可见

- JavaScript

  ```js
  let foo; // 待初始化
  let foo = new Type(args); // 构造
  let foo = initializer; // 拷贝
  let foo = new Type(other); // 转换
  let [a, , ...rest] = array; // 解构
  let { a, ["1"]: b, ...rest } = obj; // 解构
  ```

  - 变量类型：引用型弱类型
  - 生命周期：直到引用计数为零时才销毁
  - 作用域：退出块作用域后不可见

- Python

  ```python
  foo = Type(args)    # 构造
  foo = initializer   # 拷贝
  foo = Type(other)   # 转换
  a, b = iterable     # 解构
  ```

  - 变量类型：引用型弱类型
  - 生命周期：直到引用计数为零时才销毁
  - 作用域：直到退出函数才不可见

## 常量

- C++

  ```cpp
  #define CONST_VALUE 1       // 宏
  const int kConstValue = 1;  // 常量
  enum Number {kOne, kTwo};   // 枚举
  ```

- Go

  ```go
  const BigInt = 1 << 511     // 高精度无类型常量
  const (
    _, _ = iota, iota         // 枚举器
    KB = 1 << (10 * iota)
    MB
    GB
  )
  ```

- JavaScript

  ```js
  const CONST_VALUE = 1; // 仅限制变量禁止被重新赋值绑定或被重新声明
  ```

- Python

  ```python
  CONST_VALUE = 1 # 非强制
  ```

## 表达式

> 优先级：成员|函数|下标 > 单目 > 算术 > 关系 > 逻辑

- C++
  - 算数：`*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：`=`
  - 其他：`? :`, `++`, `--`
- Go
  - 算数：`*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：（语句）`=`, `:=`
  - 其他：（语句）`i++`, `i--`
- JavaScript
  - 算数：`**`, `*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`, `===`, `!==`
  - 逻辑：`!`, `&&`, `||`, `??`
  - 赋值：`=`, `??=`
  - 其他：`? :`, `++`, `--`
- Python
  - 算数：`**`, `*`, `/`, `//`, `%`, `+`, `-`
  - 关系：`a < b <= c > d >= e`, `x == y != z`
  - 逻辑：`not`, `and`, `or`
  - 赋值：（语句）`a = b = c`
  - 其他：`x if cond else y`

## 语句

- C++
  - 分支
    - if-else
    - switch-case-default-break
  - 循环
    - for
    - **for-range**
  - 异常
    - throw-try-catch
- Go
  - 分支
    - if-else
    - switch-case-default-fallthrough
    - switch-case:condition
    - switch-case:type
  - 循环
    - for
    - **for-range**
  - 异常
    - defer
    - panic()
    - recover()
- JavaScript
  - 分支
    - if-else
    - switch-case-default-break
  - 循环
    - labeled
    - for
    - **for-in**
    - **for-of**
  - 异常
    - throw-try-catch
- Python
  - 分支
    - if-elif-else
  - 循环
    - while-else
    - **for-in-else**
  - 异常
    - raise-try-except-else-finally
    - with

## 函数

> - 注意函数对**纯函数性**与**线程安全性**的保证

- C++

  ```cpp
  char example(const string& s, int i, T& t) {
      return t = s[i];
  }

  auto lambda = [&r, v] (auto& elem) mutable {}
  ```

  - 签名修饰：`inline`, `constexpr`, `noexcept`
  - 形参列表
    - 默认实参
  - 函数重载
  - 单返回值
  - 泛型

- Go

  ```go
  func example(s string, i int) (x, y int) {
      x, y = s[i], s[i + 1]
      return
  }
  ```

  - 形参列表
    - 切片变参包：`args...`
  - 返回值列表

- JavaScript

  ```js
  function example(s, i) {
    return s + i;
  }

  // 函数体仅单独return语句则可省略花括号与return
  let lambda = (x, y) => {
    return x + y;
  };
  ```

  - 形参列表
    - 数组变参包`...args`
    - 默认实参
  - 单返回值

- Python

  ```python
  def exmaple(s: str, /, l: list[int], *, d: dict[str,]) -> None:
      return True

  f = lambda x, y: x + y
  ```

  - 形参列表
    - 类型注解：`None`、`Any`、`Optional[T]`、`tuple[int, str,...]`、`list[int]`、`set[str]`、`dict[str, int]`
    - 位置参数`/`与键值参数`*`
    - 位置变参包`*posargs`与键值变参包`**kwargs`
    - 默认实参：仅定义时构造一次，注意引用类型
  - 单返回值

## 面向对象

### 封装

- C++
  - 访问控制：public, protected, private
  - 结构定义：默认实例唯一，static 类唯一
  - 方法定义：默认实例相关，static 类相关
  - 构造控制：构造函数
  - 析构控制：析构函数
  - 拷贝控制：拷贝与移动函数
  - 类型转换：内置类型(bool&int&float, c-str&string)，单参构造函数与 operator 转换函数
- Go
  - 访问控制：首字母大写导出包外
  - 结构定义：实例唯一
  - 方法定义：实例相关
  - 构造控制：无
  - 析构控制：无
  - 拷贝控制：无
  - 类型转换：int&float, bytes&string, type, const, nil, chan, interface
- Python
  - 访问控制：非`_`开头导出文件外（非强制）
  - 结构定义：默认类唯一，实例可动态增删属性
  - 方法定义：默认实例相关，实例可动态增删方法
  - 构造控制：`__init__()`
  - 析构控制：`__del__()`
  - 拷贝控制：无
  - 类型转换：`__nonzero__()`与`__str__()`
- JavaScript
  - 访问控制：非`#`开头导出类外
  - 结构定义：默认实例唯一，static 类唯一
  - 方法定义：默认实例相关，static 类相关
  - 构造控制：`constructor()`
  - 析构控制：无
  - 拷贝控制：无
  - 类型转换：->boolean, ->number, ->string

### 继承

- C++

```cpp
class Base {
   public:
    Base() = default;
    Base(Base&&) = default;
    Base(const Base&) = default;
    Base& operator=(Base&&) = default;
    Base& operator=(const Base&) = default;
    virtual ~Base() = default;

    virtual void InterfaceMethod() = 0;
};

class MyClass: public Base {
   public:
    MyClass();
    MyClass(MyClass&&) = default;
    MyClass(const MyClass&) = default;
    MyClass& operator=(MyClass&&) = default;
    MyClass& operator=(const MyClass&) = default;
    virtual ~MyClass() override;

    explicit operator int() const;

    virtual void InterfaceMethod() override;

   private:
// private! p2Impl? construct-order! alignment?
    int name_;
};
// default? explicit! non-inline! never-call-virtual!
MyClass::MyClass() : Base{}, name_{} {}
// virtual-def! noexcept! non-inline! never-call-virtual!
MyClass::~MyClass() {}
// explicit! bool? only-one-cvt-number!
MyClass::operator int() const {
    return id_;
}
void MyClass::InterfaceMethod() {}
```

- Go

```go
type Interface interface {
    InterfaceMethod() int
}

type MyClass struct {
    Base1   // MyClass与*MyClass均继承Base1方法集
    *Base2  // MyClass无法继承*Base2的方法集，而*MyClass可以
    name string
}

func (this *MyClass) InterfaceMethod() int {
    return this.name
}
```

- JavaScript

```js
class MyClass extends Base {
  #name;

  constructor(name) {
    super(); // 调用Base的constructor，Base的方法可直接通过this调用
    this.#name = name;
  }

  get name() {
    return this.#name;
  }

  set name(x) {
    this.#name = x;
  }
}
```

- Python

```python
class MyClass(Base):
    def __init__(self, name: str):
        super().__init__()  # super(Base, self).__init__()
        self._name = name

    def __del__(self):
        super().__del__()

    @property
    def name(self):
        return self._name

    @name.setter
    def name(self, name):
        self._name = name
```

### 多态

- C++：
  - 派生类的引用或指针可转换为基类的引用或指针
  - 运行时类型：`typeid()` `dynamic_cast<>()`
  - 通过虚指针与虚表实现
- Go：
  - 方法集匹配的类型可转换为相应接口类型
  - 运行时类型：`switch rt := interf.(type) {}` `rt[, ok] = interf.(RT)`
  - 通过静态符号表与反射实现
- JavaScript
  - 弱类型系统可直接转换为接口类型
  - 运行时类型：`typeof`
  - 利用动态符号表与反射实现
- Python
  - 弱类型系统可直接转换为接口类型
  - 运行时类型：`type()`
  - 利用动态符号表与反射实现

## 命名规范

名字规范的要点：

1. 不要节省字母，实现代码自解释
2. 作用域越广越远命名要越详细，反之作用域很小的变量可简写（比如循环变量 idx）

常用命名前后缀：

- 位置：`idx`，`pos`，`prev`，`next`，`lhs`，`rhs`，`head`，`tail`，`mid`，`begin`，`end`
- 计数：`count`，`size`，`length`，`width`，`height`，`depth`
- 时间：`new`，`old`，`orig`，`cur`，`before`，`after`
- 循环：`idx`，`pos`
- 序数：`number`，`1st`，`2nd`，`3rd`
- 布尔：`is`，`not`，`any`，`all`，`none`，`has`
- 介词：`in`，`on`，`at`，`of`，`2`，`4`
- 用途：`ret`，`val`，`need`，`temp`，`deal`，`src`，`dest`

| C++命名            | 形式                | 备注                                                                     |
| ------------------ | ------------------- | ------------------------------------------------------------------------ |
| Namespaces, Files  | `lower_with_under`  | 头文件引入顺序，目录结构命名空间                                         |
| Classes            | `UpperCamelCase`    |                                                                          |
| Functions, Method  | `UpperCamelCase`    |                                                                          |
| Macroes            | `UPPER_WITH_UNDER`  | 尽量使用 inline 或 constexpr 代替                                        |
| Constants          | `kUpperCamelCase`   |                                                                          |
| Static Variables   | `kUpperCamelCase`   | 注意不会调用静态生命周期对象的析构函数，使用 ref-return 代替 global 变量 |
| Local Variables    | `lower_with_under`  |
| struct Data Member | `lower_with_under`  | static 同                                                                |
| class Data Member  | `lower_with_under_` | static 同                                                                |

| Go 命名         | Public           | Internal         |
| --------------- | ---------------- | ---------------- |
| Packages, Files | `lowerword`      | `internal`       |
| All Name        | `UpperCamelCase` | `lowerCamelCase` |

| JavaScript 命名       | 形式               |
| --------------------- | ------------------ |
| Packages, Files       | `lowerCamelCase`   |
| Classes, Component    | `UpperCamelCase`   |
| Functions, Method     | `lowerCamelCase`   |
| Constants             | `UPPER_WITH_UNDER` |
| Variables, Properties | `lowerCamelCase`   |

| Python 命名           | 形式               |
| --------------------- | ------------------ |
| Packages, Files       | `lower_with_under` |
| Classes               | `UpperCamelCase`   |
| Functions, Method     | `UpperCamelCase`   |
| Constants             | `UPPER_WITH_UNDER` |
| Variables, Properties | `lower_with_under` |

## 基础库与框架

- 分数
- 大数
- 数学库
- 随机数

---

- 字符串
- 正则表达式
- 格式化
- 序列化
- 本地化与国际化
- 标准输入输出
- 文件输入输出
- 文件系统

---

- 日期时间

---

- 容器（数组，栈，队列，堆，链表，树，哈希表）
- 算法（搜索，集合，数值，变序，更易）

---

- 并发与同步
- 异步框架（网络 IO、文件 IO、IPC、IOC、Timer、Tasks）

---

- 错误处理
- 日志收集
- 数据埋点
- 性能剖析
- 单元测试
