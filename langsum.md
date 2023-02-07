# 编程语言总结

- [编程语言总结](#编程语言总结)
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
  - [基础类库与框架](#基础类库与框架)
    - [基础类型](#基础类型)
    - [数字相关](#数字相关)
    - [字符相关](#字符相关)
    - [数据结构与算法](#数据结构与算法)
    - [异步框架](#异步框架)
    - [系统相关](#系统相关)
    - [工程能力](#工程能力)
    - [UI](#ui)

## 依赖

- C++

  - 程序入口：main 函数
  - 依赖单元：一个头文件 + 一个实现文件（可选）
  - 依赖导出：头文件中所有宏与符号
  - 依赖导入：`#include <path/to/header.hpp>`，符号限定于命名空间
  - 依赖初始化：global-constructor
  - 依赖管理：手动（git-submodule & cmake）

- Go

  - 程序入口：main 包中 main 函数
  - 依赖单元：一个目录（不包括子目录）
  - 依赖导出：大写字母开头的符号
  - 依赖导入：`import "module.github.com/path/to/package"`，符号限定于包名
  - 依赖初始化：包中所有 init 函数
  - 依赖管理：自动（go-mod）

- Dart

  - 程序入口：main 函数
  - 依赖单元：一个源文件
  - 依赖导出：非`_`开头的符号
  - 依赖导入：`import 'package:lib/lib.dart'`
  - 依赖初始化：无
  - 依赖管理：自动（flutter-pub）

- JavaScript

  - 程序入口：任意源文件
  - 依赖单元：一个源文件
  - 依赖导出：`export`声明
  - 依赖导入：`import { name } from 'root/path/to/module'`，符号限定于别名
  - 依赖初始化：脚本导入即执行
  - 依赖管理：自动（npm）

- Python
  - 程序入口：任意源文件（`__init__ == "__main__"`）
  - 依赖单元：一个源文件
  - 依赖导出：非`_`开头的符号
  - 依赖导入：`from package.subpackage import module`，符号限定于文件名
  - 依赖初始化：脚本导入即执行
  - 依赖管理：自动（pip）

## 变量

> - 结构型：变量本身代表一块包含数据结构的内存，拷贝时至少包括该内存
> - 引用型：变量仅仅指向一块包含数据结构的内存，拷贝时仅针对该指针值
> - 强类型：变量类型静态确定且不可随意变
> - 弱类型：变量类型动态确定且可随意变
> - 引用语义的目的：
>   - (Copy) 避免拷贝
>   - (Write) 修改对象
>   - (Dynamic) 动态绑定

- C++

  ```cpp
  // C++ 初始化方法：
  // 构造函数初始化的缺点：无法返回错误，无法安全调用虚函数（除非显式 final）
  // 工厂方法初始化的场景：解决构造函数初始化的问题
  // Init方法初始化的场景：仅在接口天然就适合两段式初始化时使用，因为在构造与初始化间存在不可用的中间态
  auto foo = bar();                     // 默认
  auto foo = bar(args);                 // 构造
  auto foo = initializer;               // 拷贝
  auto foo = static_cast<bar>(other);   // 转换
  auto [a, b]  = aggregation;           // 解构
  ```

  - 变量类型：结构型强类型
  - 生命周期：退出块作用域时销毁
  - 作用域：退出块作用域后不可见

- Go

  ```go
  var foo Bar             // 默认
  foo  := Bar{fd: data}   // 构造
  foo  := initializer     // 拷贝
  foo  := Bar(other)      // 转换
  a, _ := value, list     // 解构
  ```

  - 变量类型：结构型强类型
  - 生命周期：直到引用计数为零才时销毁
  - 作用域：退出块作用域后不可见

- Dart

  ```dart
  var foo = Bar();        // 默认
  var foo = Bar(args);    // 构造
  var foo = initializer;  // 拷贝
  var foo = other as Bar; // 转换
  ```

  - 变量类型：引用型强类型
  - 生命周期：直到引用计数为零时才销毁
  - 作用域：退出块作用域后不可见

- JavaScript

  ```js
  let foo = new Bar(args); // 构造
  let foo = initializer; // 拷贝
  let foo = new Bar(other); // 转换
  let [a, ["1"]: b, ...rest] = array; // 解构
  ```

  - 变量类型：引用型弱类型
  - 生命周期：直到引用计数为零时才销毁
  - 作用域：退出块作用域后不可见

- Python

  ```python
  foo = bar(args)     # 构造
  foo = initializer   # 拷贝
  foo = bar(other)    # 转换
  a, b = iterable     # 解构
  ```

  - 变量类型：引用型弱类型
  - 生命周期：直到引用计数为零时才销毁
  - 作用域：直到退出函数才不可见

## 常量

- C++

  ```cpp
  #define KVALUE 1          // 宏
  const int kvalue = 1;     // 常量
  enum number { one, two }; // 枚举
  ```

- Go

  ```go
  const BigInt = 1 << 511     // 高精度无类型常量
  const (
    _, _ = iota, iota         // 枚举器，从0开始
    KB = 1 << (10 * iota)
    MB
    GB
  )
  ```

- Dart

  ```dart
  final map = {if (i is int) i: 'int'};         // final 不可改变变量本身，但可以改变其字段内容
  const set = {if (list is List<int>) ...list}; // const 两者都不可改变
  enum Color { red, green, blue }               // 支持定义拥有字段的枚举
  ```

- JavaScript

  ```js
  const kvalue = 1; // 仅限制变量禁止被重新赋值绑定或被重新声明
  ```

- Python

  ```python
  KVALUE = 1 # 非强制
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
- Dart
  - 算数：`*`, `/`, `~/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`
  - 逻辑：`!`, `&&`, `||`, `is`, `is!`
  - 赋值：`=`, `??=`
  - 其他：`? :`, `??`, `++`, `--`, `..`, `?..`, `expr!`
- JavaScript
  - 算数：`**`, `*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`, `===`, `!==`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：`=`, `??=`
  - 其他：`? :`, `??`, `++`, `--`
- Python
  - 算数：`**`, `*`, `/`, `//`, `%`, `+`, `-`
  - 关系：`a < b <= c > d >= e`, `x == y != z`
  - 逻辑：`not`, `and`, `or`
  - 赋值：（语句）`a = b = c`
  - 其他：`x if cond else y`

## 语句

- C++

  - 分支

    ```cpp
    // if else
    if (condition) {
      statement;
    } else {
      statement;
    }

    // switch
    switch (int_or_enum) {
      case constant:
        statement;
        break;
      default:
        statement;
    }
    ```

  - 循环

    ```cpp
    // for
    for (declaration; condition; expression) {
      statement;
    }

    // range based for
    for (auto elem : iterabal) {
      statement;
    }
    ```

  - 异常

    ```cpp
    // try and function try
    // throw and rethrow
    // catch and catch all
    [void func()] try {
      throw std::exception();
    } catch (const std::exception& e) {
      statement;
    } catch (...) {
      throw;
    }
    ```

- Go

  - 分支

    ```go
    // if else
    if condition {
      statement
    } else {
      statement
    }

    // switch expr
    switch expr {
      case expr1, expr2:
        fallthrough
      default:
        statement
    }

    // switch condition
    switch {
      case condition:
        fallthrough
      default:
        statement
    }

    // switch runtime type
    switch rt := intf.(type) {
      case Type:
        fallthrough
      default:
        statement
    }
    ```

  - 循环

    ```go
    // for
    for declaration; condition; expression {
      statement
    }

    // for range
    for elem := range iterabal {
      statement
    }
    ```

  - 异常

    ```go
    panic("throw a panic!")

    defer func() {
      panic = recover()
      if panic != nil {
        statement
      }
    }()

    defer call3rd()
    defer call2nd()
    defer call1st()
    ```

- Dart

  - 分支

    ```dart
    // if else
    if (condition) {
      statement;
    } else {
      statement;
    }

    // switch
    switch (comparable) {
      case constant1:
        continue fallthrough;
      fallthrough:
      case constant2:
        statement;
        break;
      default:
        statement;
    }
    ```

  - 循环

    ```dart
    // for
    for (declaration; condition; expression) {
      statement;
    }

    // for in
    for (final elem in iterabal) {
      statement;
    }
    ```

  - 异常

    ```dart
    try {
      throw FormatException('exception');
    } on FormatException catch (e) {
      statement;
    } catch (e, s) {
      print('Exception details:\n $e');
      print('Stack trace:\n $s');
      rethrow;
    } finally {
      cleanup();
    }
    ```

- JavaScript

  - 分支

    ```js
    // if else
    if (condition) {
      statement;
    } else {
      statement;
    }

    // switch
    switch (expr) {
      case expr:
        statement;
        break;
      default:
        statement;
    }
    ```

  - 循环

    ```js
    // for
    for (declaration; condition; expression) {
      statement;
    }

    // for in
    for (const keyOrIndex in iterable) {
      statement;
    }

    // for of
    for (const element of iterable) {
      statement;
    }
    ```

  - 异常

    ```js
    try {
      throw new Error("error message");
    } catch (identifier) {
      statement;
    } finally {
      statement;
    }
    ```

- Python

  - 分支

  ```python
  # if elif else
  if condition:
    pass
  elif condition:
    pass
  else:
    pass
  ```

  - 循环

  ```python
  while condition:
    pass
  else:
    pass

  for val1, val2 in iterable:
    pass
  else:
    pass
  ```

  - 异常

  ```python
  try:
    raise Exception('error0')
  except Exception:
    raise Exception('error1')
  except Exception as excep:
    raise Exception('error2') from Exception('__cause__')
  except:
    raise
  else:
    pass
  finally:
    pass
  ```

## 函数

> - 注意函数对**可重入性**与**线程安全性**的保证

- C++

  ```cpp
  auto return_nothing() -> void {
    return;
  }

  auto normal_function(const string& s, int i) -> char {
    return s[i];
  }

  auto closure = [&r, v, m = std::move(l)] (auto& elem) mutable {}
  ```

  - 签名修饰：`const &|&&`, `inline`, `constexpr`, `noexcept`
  - 默认实参
  - 函数重载
  - 泛型

- Go

  ```go
  func ReturnNothing() {
    return
  }

  func NormalFunction(s string, i int) (x, y string) {
    x, y = s[i], s[i + 1]
    return
  }

  closure := func() {
    return
  }
  ```

  - 类型折叠
  - 切片变参包：`args...`
  - 返回值列表

- Dart

  ```dart
  void returnNothing() {
    return;
  }

  String positionalParameter(String r1, String? r2 [String o1 = "optional", String? o2]) {
    // ...
  }

  String namedParameter({required String r1, required String? r2, String o1 = "optional", String? o2}) {
    // ...
  }

  (args) {statements;}

  (args) => expression;
  ```

  - 位置参数与命名参数
  - 显式指定`optional`或`required`

- JavaScript

  ```js
  function returnNothing() {
    return;
  }

  function normalFunction(s, i) {
    return s[i];
  }

  let closure = (args) => {
    return;
  };

  let closure = (args) => expr;
  ```

  - 默认实参
  - 数组变参包：`...args`

- Python

  ```python
  def return_nothing() -> None:
    pass

  def normal_function(pos: str, /, pos_or_name: str, *, name: str) -> str:
    return pos + pos_or_name + name

  closure = lambda x, y: x + y
  ```

  - 类型注解：`None`、`Any`、`Optional[T]`、`tuple[int, str,...]`、`list[int]`、`set[str]`、`dict[str, int]`
  - 位置参数与键值参数
  - 位置变参包`*posargs`与键值变参包`**kwargs`
  - 默认实参：仅定义时构造一次，注意引用类型

## 面向对象

### 封装

- C++

  - 访问控制：public, protected, private
  - 构造控制：构造函数
  - 析构控制：析构函数
  - 拷贝控制：拷贝与移动函数
  - 比较操作：三相比较运算符
  - 类型转换：内置类型(bool&int&float, c-str&string)，单参构造函数与 operator 转换函数

  ```cpp
  class myclass {
   public:
    myclass() = default;
    myclass(myclass&&) = default;
    myclass(const myclass&) = default;
    auto operator=(myclass&&) -> myclass& = default;
    auto operator=(const myclass&) -> myclass& = default;
    ~myclass() = default;
    auto operator<=>(const myclass&) -> std::strong_ordering = default;
    explicit operator bool() const { return _name.size(); }

    auto name() -> string { return _name; }
    auto set_name(string name) -> void { _name = std::move(name); }
    void method();

   private:
    string _name;
  };
  ```

- Go

  - 访问控制：首字母大写导出包外
  - 构造控制：无
  - 析构控制：无
  - 拷贝控制：拷贝字段
  - 比较操作：比较字段
  - 类型转换：int&float, bytes&string, type, const, nil, chan, interface

  ```go
  type MyClass struct {
    name string
  }

  func (this *MyClass) Method() {
    doSomething(this.name)
  }
  ```

- Dart

  - 访问控制：非`_`开头导出类外
  - 构造控制：构造函数与命名构造函数
  - 析构控制：无
  - 拷贝控制：无
  - 比较操作：重载操作符
  - 类型转换：

  ```dart
  class MyClass {
    String _name;

    // constructor likes function but no return type
    MyClass(String name) {
      this._name = name;
    }

    // named constructor beacase no funtion overload
    MyClass.named(String name) {
      this._name = name;
    }

    // using this in constructor, a syntax sugar
    MyClass.useThis(this._name);

    // initializer lists: final fields must have values before the constructor body executes
    MyClass.initList(String name) : _name = name;

    // redirect to another constructor
    MyClass.redirecting1(String name) : this(name);
    MyClass.redirecting2(String name) : this.named(name);

    @override
    bool operator ==(Object other) => other is MyClass && other._name = name;

    String get name => name;
    set name(String value) => name = value;
  }
  ```

- JavaScript

  - 访问控制：非`#`开头导出类外
  - 构造控制：`constructor()`
  - 析构控制：无
  - 拷贝控制：无
  - 比较操作：无
  - 类型转换：->boolean, ->number, ->string

  ```js
  class MyClass {
    #name;

    constructor(name) {
      this.#name = name;
    }

    get name() {
      return this.#name;
    }

    set name(x) {
      this.#name = x;
    }

    method() {
      doSomething(this.#name);
    }
  }
  ```

- Python

  - 访问控制：非`_`开头导出文件外（非强制）
  - 构造控制：`__init__()`
  - 析构控制：`__del__()`
  - 拷贝控制：无
  - 比较操作：无
  - 类型转换：`__nonzero__()`与`__str__()`

  ```python
  class myclass:
    def __init__(self, name: str):
      self._name = name

    def __del__(self):
      self._name.__del__()

    @property
    def name(self):
      return self._name

    @name.setter
    def name(self, name):
      self._name = name

    def method(self):
      do_something(self._name)
  ```

### 继承

- C++

  ```cpp
  class myclass: public base, public mixin {
   public:
    myclass() : base(), mixin() {}

    virtual ~myclass() override = default;

    virtual void base_method() override {
      base::base_method();
      // ...
    }
  };
  ```

- Go

  ```go
  type MyClass struct {
    Base    // Base 的方法集可由 MyClass 与 *MyClass 继承
    *Mixin  // *Mixin 的方法集仅可由 *MyClass 继承而 MyClass 不行
  }

  func (this *MyClass) baseMethod() {
    // ...
  }
  ```

- Dart

  ```dart
  class Base {
    int key;
    void baseMethod() {
      // ...
    }
  }

  mixin Mixin {
    void mixinMethod() {
      // ...
    }
  }

  class MyClass extends Base with Mixin {
    MyClass(super.key);

    @override
    void baseMethod() {
      super.baseMethod();
      // ...
    }
  }
  ```

- JavaScript

  ```js
  class MyClass extends Base {
    // ...
  }
  ```

- Python

  ```python
  class myclass(base, mixin):
    def __init__(self):
      super(base, self).__init__()
      super(mixin, self).__init__()

    def __del__(self):
      super(base, self).__del__()
      super(mixin, self).__del__()
  ```

### 多态

- C++

  ```cpp
  class abstract {
   public:
    virtual ~abstract() = defatul;
    virtual abstract_method() = 0;
  }

  class myclass: public abstract {
   public:
    virtual ~myclass() override = default;
    virtual abstract_method() override {
      // ...
    }
  };

  abstract* pintf = new myclass();
  abstract& rintf = *pintf;
  ```

- Go

  ```go
  type Abstract interface {
      abstractMethod()
  }

  type MyClass struct {}

  func (this *MyClass) abstractMethod() {
    // ...
  }

  var intf Abstract = MyClass{}
  ```

- Dart

  ```dart
  abstract class Abstract {
    void abstractMethod();
  }

  class MyClass implements Abstract {
    @override
    void abstractMethod() {
      // ...
    }
  }
  ```

- JavaScript
- Python

## 命名规范

- 通用命名规范
  1. 作用域越宽泛，命名要越详细，且避免使用缩写词；反之作用域很狭窄的变量可简写，比如循环变量 `i`
  2. 让代码读起来像完整句子
- 变量、属性命名：
  1. 将最具描述性的名词放在标识符最后
  2. 若为非布尔类型，尽量使用名词短语
  3. 若为布尔类型，尽量使用非祈使动词短语，如使用 be 动词或助动词，且尽量使用“正向”的语义，如`isConnected`而非`isDisconnected`
- 函数、方法命名：
  1. 若有副作用，尽量使用祈使动词短语
  2. 若无副作用，命名参考变量、属性命名

常用命名前后缀：

- 布尔：`is`，`not`，`any`，`all`，`none`，`has`，`can`，`should`，`must`
- 位置：`idx`，`pos`，`prev`，`next`，`lhs`，`rhs`，`head`，`tail`，`mid`，`begin`，`end`
- 计数：`count`，`size`，`length`，`width`，`height`，`depth`
- 时间：`new`，`old`，`orig`，`cur`，`before`，`after`
- 循环：`idx`，`pos`
- 序数：`number`，`1st`，`2nd`，`3rd`
- 介词：`in`，`on`，`at`，`of`，`2`，`4`
- 用途：`ret`，`val`，`need`，`temp`，`deal`，`src`，`dest`

## 基础类库与框架

### 基础类型

- 布尔
- 整数
- 浮点数
- 字符串
- 日期时间
- 基础容器：列表、集合、映射

### 数字相关

- 大数
- 分数
- 数学库
- 随机数

### 字符相关

- 正则表达式
- 格式化
- 国际化
- 序列化

### 数据结构与算法

- 容器：表，栈，队列，堆，链表，树，哈希表
- 算法：搜索，集合，数值，变序，更易

### 异步框架

- 文件 IO
- 网络 IO
- IPC
- IOC
- Timer
- Lock
- Tasks

### 系统相关

- 环境变量
- 命令行参数
- 标准输入输出
- 文件系统

### 工程能力

- 错误处理
- 日志记录
- 数据埋点
- 单元测试
- 性能剖析

### UI

- 窗口：

  - 位置与大小
  - Z 轴顺序
  - 可见状态
  - 最大最小化状态
  - 激活状态
  - 禁用状态

- 内容

  - 文本
  - 链接
  - 图标
  - 图像
  - 视频
  - 音频
  - 2D 绘制
  - 3D 绘制

- 展示

  - 布局：位置、大小、方向、伸缩（一维布局）、网格（二维布局）、图层（三维布局）、列表-表格-树图（常用高级视图）
  - 样式：颜色、形状
  - 动画：随时间更改布局与样式的状态
  - Materail Design

- 输入

  - 鼠标事件：enter, leave, down, up, move
  - 高级手势：轻触、长按、双击、三击、拖拽、选择、右键、滚动
  - 键盘事件：down, up
  - 高级按键：焦点交互、快捷键
  - 系统事件

- MVVM 模式

  - View：负责展示状态数据并响应 UI 事件
  - ViewModel：负责数据绑定并提供响应逻辑
  - Model：负责维护并提供业务数据与业务逻辑

![framework](images/framework.png)
