# Programing Language Summary

- [Programing Language Summary](#programing-language-summary)
  - [依赖](#依赖)
  - [变量](#变量)
  - [表达式](#表达式)
  - [语句](#语句)
  - [函数](#函数)
  - [面向对象](#面向对象)
    - [封装](#封装)
    - [继承](#继承)
    - [多态](#多态)
  - [命名规范](#命名规范)

## 依赖

- C++
  - 程序入口：main 函数
  - 依赖单元：一个头文件 + 一个实现文件（可选）
  - 依赖导入：`#include <path/to/header.hpp>`，初始化：global-constructor
  - 依赖导出：头文件中所有宏与符号，符号限定于命名空间
  - 依赖管理：手动（git-submodule & cmake）
- Go
  - 程序入口：main 包中 main 函数
  - 依赖单元：一个目录（不包括子目录）
  - 依赖导入：`import "module.github.com/path/to/package"`，初始化：所有 init 函数
  - 依赖导出：大写字母开头的符号，符号限定于包名
  - 依赖管理：自动（go-mod）
- Python
  - 程序入口：源码文件均可执行，`__init__ == "__main__"`
  - 依赖单元：一个源文件
  - 依赖导入：`from package.subpackage import module`，初始化：`__init__.py`文件
  - 依赖导出：非`_`开头的符号，符号限定于文件名
  - 依赖管理：半自动（pip-install）

## 变量

- C++

  ```cpp
  Type var{}                          // 待初始化
  Type var{args};                     // 构造
  auto var = initializer;             // 拷贝
  auto var = static_cast<Type>(other) // 转换
  auto [a, b] = aggregation;          // 多元
  ```

  - 变量类型：强类型（变量类型不可变、函数声明不可变、没有动态符号表）
  - 生命周期：退出块作用域时销毁
  - 作用域：退出块作用域后不可见
  - 赋值拷贝：一般为深拷贝，指针为浅拷贝

- Go

  ```go
  var val Type          // 待初始化
  val := Type{fd: data} // 构造
  val := initializer    // 拷贝
  val := Type(other)    // 转换
  a, b, _ := value-list // 多元
  ```

  - 变量类型：强类型
  - 生命周期：直到引用计数为零才时销毁
  - 作用域：退出块作用域后不可见
  - 赋值拷贝：一般为浅拷贝，接口为深拷贝

- Python

  ```python
  var = Type(args)    # 构造
  var = initializer   # 拷贝
  var = Type(other)   # 转换
  a, b = iterable     # 多元
  ```

  - 变量类型：弱类型
  - 生命周期：直到引用计数为零时才销毁
  - 作用域：直到退出函数才不可见
  - 赋值拷贝：全为浅拷贝

## 表达式

> 优先级：成员|函数|下标 > 单目 > 算术 > 关系 > 逻辑

- C++
  - `=`为表达式
  - 支持`? :`、`++`、`--`
- Go
  - `=`与`:=`均为语句而非表达式
  - 支持后缀`++`与`--`且为语句
- Python
  - `=`为语句，`:=`为表达式
  - 支持`x if cond else y`、`a > b > c`、`a**e`

## 语句

- C++
  - 分支
    - if-else
    - switch-case-default-break
  - 循环
    - for
    - **for-range**
  - 异常
    - try-catch-throw
- Go
  - 分支
    - if-else
    - switch-case-default-fallthrough
    - switch-case:condition
    - switch-case:type
  - 循环
    - for
    - **for-range**
  - 延迟
    - defer
    - panic()
    - recover()
- Python
  - 分支
    - if-elif-else
  - 循环
    - while-else
    - **for-in-else**
  - 异常
    - try-except-raise-else-finally
    - with

## 函数

- C++

  ```cpp
  auto example(const string& s, int i=0) -> char {
      return s[i];
  }
  ```

  - 形参修饰：`const T&` ? `T` ? `T&`
  - 函数声明：`inline` ? `noexcept` ? `this`
  - 函数重载：`-> T&` ? `T&&` ? `template<typename T>`
  - 泛型变参包
  - 默认实参
  - 闭包：`[&r, v] (auto& x) mutable {}`

- Go

  ```go
  func example(s string, i int) (x, y int) {
      x, y = s[i], s[i + 1]
      return
  }
  ```

  - 形参指针
  - 返回列表
  - 切片变参包
  - 闭包

- Python

  ```python
  def exmaple(s: str, /, l: list[int], *, d: dict[str,]) -> None:
      return True
  ```

  - 类型注解：`None`、`Any`、`Optional[T]`、`tuple[int, str,...]`、`list[int]`、`dict[str, int]`
  - 位置参数与键值参数
  - 位置变参包与键值变参包：`*posargs`与`**kwargs`
  - 默认实参：仅定义时构造一次，注意引用类型
  - 闭包：`lambda x, y: x * y`

## 面向对象

### 封装

- C++
  - 访问控制：public, private, protected
  - 结构定义：默认实例唯一，static 类唯一
  - 方法定义：默认实例相关，static 类相关
  - 构造控制：构造函数
  - 析构控制：析构函数
  - 拷贝控制：拷贝与移动函数
  - 类型转换：内置类型（bool&Number, c-str&string），单参构造函数与 operator 转换函数
- Go
  - 访问控制：首字母大写导出包外
  - 结构定义：实例唯一
  - 方法定义：实例相关
  - 构造控制：无
  - 析构控制：无
  - 拷贝控制：无
  - 类型转换：Number, bytes&string, type, const, nil, chan, interface
- Python
  - 访问控制：非`_`开头导出文件外
  - 结构定义：默认类唯一，实例可动态增删属性
  - 方法定义：默认实例相关，实例可动态增删方法
  - 构造控制：`__init__()`
  - 析构控制：`__del__()`
  - 拷贝控制：无
  - 类型转换：`__bool__()`与`__str__()`

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
    int name_; // private! p2Impl? construct-order! alignment?
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
    Get() int
}
// 将“接口定义”与“方法继承”分开
type MyClass struct {
    Base1   // MyClass与*MyClass均继承Base1方法集
    *Base2  // MyClass无法继承*Base2的方法集，而*MyClass可以
    name string
}

func (this *MyClass) Get() int {
    return this.name
}
```

- Python

```python
class MyClass(Base):
    def __init__(self, name: str):
        super().__init__()  # super.(Base, self).__init__()
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

| C++命名            | 例子                   | 备注                                                                     |
| ------------------ | ---------------------- | ------------------------------------------------------------------------ |
| Headers            | `lower_with_under.hpp` |                                                                          |
| Implements         | `lower_with_under.cpp` |                                                                          |
| Namespaces         | `lower_with_under`     | 顶级命名空间与项目名匹配，内嵌命名空间与目录名匹配                       |
| Macroes            | `CAPS_WITH_UNDER`      | 尽量使用 inline 或 constexpr 代替                                        |
| Classes            | `CapWords`             |                                                                          |
| Functions          | `CapWords`             |                                                                          |
| Method             | `CapWords`             |                                                                          |
| Constants          | `kCapWords`            |                                                                          |
| Static Variables   | `kCapWords`            | 注意不会调用静态生命周期对象的析构函数，使用 ref-return 代替 global 变量 |
| Local Variables    | `lower_with_under`     |
| struct Data Member | `lower_with_under`     | static 同                                                                |
| class Data Member  | `lower_with_under_`    | static 同                                                                |

| Go 命名  | Public     | Internal   |
| -------- | ---------- | ---------- |
| All Name | `MixedCap` | `mixedCap` |

| Python 命名            | Public                | Internal                          |
| ---------------------- | --------------------- | --------------------------------- |
| Packages               | `lower_with_under`    |                                   |
| Modules                | `lower_with_under.py` | `_lower_with_under.py`            |
| Classes                | `CapWords`            | `_CapWords`                       |
| Functions              | `lower_with_under()`  | `_lower_with_under()`             |
| Method                 | `lower_with_under()`  | `_lower_with_under() (protected)` |
| Constants              | `CAPS_WITH_UNDER`     | `_CAPS_WITH_UNDER`                |
| Global/Class Variables | `lower_with_under`    | `_lower_with_under`               |
| Local Variables        | `lower_with_under`    | `_lower_with_under`               |
| Instance Variables     | `lower_with_under`    | `_lower_with_under (protected)`   |
