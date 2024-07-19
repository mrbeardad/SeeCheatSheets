# 编程语言

- [编程语言](#编程语言)
  - [依赖](#依赖)
  - [变量](#变量)
  - [枚举](#枚举)
  - [运算符](#运算符)
  - [控制流](#控制流)
  - [函数](#函数)
  - [面向对象](#面向对象)
    - [封装](#封装)
    - [多态](#多态)
    - [继承](#继承)
  - [泛型](#泛型)
  - [工程能力](#工程能力)
  - [基础类库与框架](#基础类库与框架)
    - [基础类型](#基础类型)
      - [C++](#c)
      - [Rust](#rust)
      - [Go](#go)
      - [Dart](#dart)
      - [TypeScript](#typescript)
      - [Python](#python)
    - [数字相关](#数字相关)
    - [字符相关](#字符相关)
    - [数据结构与算法](#数据结构与算法)
    - [系统相关](#系统相关)
    - [异步框架](#异步框架)
    - [UI](#ui)
    - [Game](#game)
  - [命名规范](#命名规范)

## 依赖

- C++

  - 程序入口：main 函数
  - 依赖单元：一个头文件 + 一个源文件（可选）
  - 依赖导入：符号限定于命名空间
  - 依赖导出：头文件中所有宏与符号
  - 依赖初始化：global-constructor
  - 依赖管理：git-submodule + cmake

  ```cpp
  #include <iostream>
  #include "3rd_party/path/to/header_only.hpp"
  #include "path/to/header.h"

  using namespace std;
  using qualifier::foo, qualifier::bar;

  namespace ascpp {
    // ...
  }
  ```

- Rust

  - 程序入口：`src/main.rs` 或 `src/bin/*.rs` 中的 main 函数
  - 依赖单元：一个源文件（项目内部依赖才需手动导入，外部依赖由 Cargo.toml 导入）
  - 依赖导入：符号限定于模块路径
  - 依赖导出：`pub`声明
  - ~~依赖初始化~~
  - 依赖管理：cargo

  ```rust
  mod fileordir;

  use create::fileordir::foo;
  use fileordir::{self, foo, bar};
  use super::*;
  ```

- Go

  - 程序入口：`package main` 中的 main 函数
  - 依赖单元：一个目录（不包括子目录）
  - 依赖导入：符号限定于包名
  - 依赖导出：大写字母开头的符号
  - 依赖初始化：包中所有 init 函数
  - 依赖管理：go-mod

  ```go
  package main

  import (
    "fmt"
    as "github.com/path/to/package"
    . "github.com/path/to/package"
    _ "github.com/path/to/package"
  )
  ```

- Dart

  - 程序入口：main 函数
  - 依赖单元：一个源文件
  - 依赖导入：符号默认无限定
  - 依赖导出：非`_`开头的符号
  - ~~依赖初始化~~
  - 依赖管理：dart-pub

  ```dart
  import 'dart:io';
  import 'package:libname/path/to/file.dart';
  import 'package:path/to/file.dart';
  import 'package:path/to/file.dart' as qualifier;
  import 'package:path/to/file.dart' show foo;
  import 'package:path/to/file.dart' hide bar;
  ```

- TypeScript

  - 程序入口：任意源文件顺序执行
  - 依赖单元：一个源文件
  - 依赖导入：符号默认无限定
  - 依赖导出：`export`声明
  - 依赖初始化：脚本导入即执行
  - 依赖管理：npm

  ```ts
  import defaultExport, { foo, bar as alias } from "path/to/module.ts";
  import * as qualifier from "path/to/module.ts";
  import "path/to/side_effect.ts";
  ```

- Python

  - 程序入口：任意源文件顺序执行（`__init__ == "__main__"`）
  - 依赖单元：一个源文件
  - 依赖导入：符号限定于模块路径
  - 依赖导出：非`_`开头的符号
  - 依赖初始化：脚本导入即执行
  - 依赖管理：pip

  ```py
  import module
  import package.module
  from module import *
  from module import foo, bar
  from package import *
  from package import module
  from package.module import state as alias
  ```

## 变量

> 引用语义的目的：
>
> 1. 避免拷贝或移动
> 2. 修改原值
> 3. 多态

- C++

  - 变量类型：强类型、结构型
  - 生命周期：声明时构造，退出作用域时销毁
  - 拷贝控制：左值深拷贝，右值浅拷贝

  ```cpp
  auto foo = other;
  const auto foo = other;
  auto& foo = other;
  const auto& foo = other;

  Type foo = other;
  const Type foo = other;
  Type& foo = other;
  const Type& foo = other;

  constexpr auto FOO = other;
  ```

- Rust

  - 变量类型：强类型、结构型
  - 生命周期：声明时构造，移动或退出作用域时销毁
  - 拷贝控制：`Copy` 默认浅拷贝，`Clone` 手动深拷贝

  ```rust
  let foo = other;
  let mut foo = other;
  let foo = &other;
  let foo = &mut other;

  let foo: Type = other;
  let mut foo: Type = other;
  let foo: &Type = &other;
  let foo: &mut Type = &mut other;

  const FOO: Type = other;
  ```

- Go

  - 变量类型：强类型、结构型
  - 生命周期：声明时构造，不再引用后被 GC 回收
  - 拷贝控制：浅拷贝

  ```go
  foo := other
  foo := &other

  var foo Type = other
  var foo *Type = &other

  const FOO = 1 << 511
  ```

- Dart

  - 变量类型：强类型、引用型
  - 生命周期：声明时构造，不再引用后被 GC 回收
  - 拷贝控制：浅拷贝

  ```dart
  var foo = other;
  final foo = other;
  const foo = other;

  late Type foo;
  Type? foo = other;
  final Type foo = other;
  const Type FOO = other;
  ```

- TypeScript

  - 变量类型：强类型、引用型
  - 生命周期：声明时构造，不再引用后被 GC 回收
  - 拷贝控制：浅拷贝

  ```ts
  let foo = other;
  const foo = other;

  let foo: Type = other;
  const foo: Type = other;
  ```

- Python

  - 类型系统：弱类型、引用型
  - 生命周期：第一次赋值时构造，不再引用后被 GC 回收
  - 拷贝控制：浅拷贝

  ```python
  global GLOBAL_VAR
  nonlocal outside_var
  foo = other
  foo: Type = other
  ```

## 枚举

- C++

  ```cpp
  enum class Enum {
    ZERO = 0,
    ONE,
    TWO,
  };

  Enum e = Enum::ZERO;
  ```

- Rust

  ```rust
  enum Enum {
    ZERO,
    ONE,
    TWO,
  }

  let e: Enum = Enum::ZERO;
  ```

- Go

  ```go
  const (
    ZERO = iota
    ONE
    TWO
  )

  var e int = ZERO
  ```

- Dart

  ```dart
  enum Enum {
    ZERO,
    ONE,
    TWO,
  }

  Enum e = Enum.ZERO;
  ```

- TypeScript

  ```ts
  enum Enum {
    ZERO = 0,
    ONE,
    TWO,
  }

  let e: Enum = Enum.ZERO;
  ```

- Python

  ```python
  from enum import Enum

  class EnumT(Enum):
      ZERO = 0
      ONE = 1
      TWO = 2

  for ev in EnumT:
      print(ev, ev.value)
  ```

## 运算符

> 优先级：单目后缀 > 单目前缀 > 算术 > 关系 > 逻辑

- C++

  - 单后：`::`, `.`, `->`, `()`, `[]`, `++`, `--`
  - 单前：`&`, `*`, `+`, `-`, `++`, `--`
  - 算数：`*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：`=`
  - 其他：`? :`

- Rust

  - 单后：`::`, `.`, `()`, `[]`
  - 单前：`&`, `*`, `+`, `-`
  - 算数：`*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：`=`
  - 其他：`!`, `?`

- Go

  - 单后：`.`, `()`, `[]`, `++`, `--`
  - 单前：`&`, `*`, `+`, `-`
  - 算数：`*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：`=`, `:=`

- Dart

  - 单后：`!`, `.`, `?.`, `..`, `?..`, `()`, `[]`, `?[]`, `++`, `--`
  - 单前：`+`, `-`, `++`, `--`
  - 算数：`*`, `/`, `~/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`, `is`, `is!`
  - 逻辑：`!`, `&&`, `||`, `??`
  - 赋值：`=`
  - 其他：`? :`

- TypeScript

  - 单后：`.`, `?.`, `[]`, `()`, `++`, `--`
  - 单前：`+`, `-`, `++`, `--`
  - 算数：`**`, `*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`, `===`, `!==`, `in`
  - 逻辑：`!`, `&&`, `||`, `??`
  - 赋值：`=`
  - 其他：`? :`

- Python

  - 单后：`.`, `()`, `[]`
  - 单前：`+`, `-`
  - 算数：`**`, `*`, `/`, `//`, `%`, `+`, `-`
  - 关系：`a < b <= c`, `a > b >= c`, `a == b != c`, `in`
  - 逻辑：`not`, `and`, `or`
  - 赋值：`a = b = c`

## 控制流

> 模式匹配：常量绑定、变量绑定、解构、逻辑与、逻辑或

- C++

  - 条件

    ```cpp
    if (condition) {
      // ...
    } else {
      // ...
    }

    switch (int_or_enum) {
      case fallthrough:
      case constant:
        // ...
        break;
      default:
        // ...
    }
    ```

  - 循环

    ```cpp
    while (true) {
      // ...
    }

    while (condition) {
      // ...
    }

    // for 循环中增删元素可能引发错误，因为迭代器失效
    for (const auto& elem : iterabal) {
      // ...
    }

    continue;
    break;
    label:
    goto label;
    ```

  - 异常

    ```cpp
    // try | function try
    [auto func()] try {
    // throw | rethrow
      throw std::exception();
    // catch | catch all
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    } catch (...) {
      throw;
    }
    ```

- Rust

  > Rust 的控制流全是表达式而非语句，因为块作用域符号`{}`本身就是表达式

  - 条件

    ```rust
    if condition {
      // ...
    } else {
      // ...
    }

    if let pattern = expr {
      // ...
    } else {
      // ...
    }

    match expr {
      pattern => {...},
      pattern => expr,
      _ => expr
    }
    ```

  - 循环

    ```rust
    loop {
      // ...
    }

    while condition {
      // ...
    }

    while let pattern = expr {
      // ...
    }

    // for 循环中无法增删元素，因为只能有一个可变借用
    for elem in iterator {
      // ...
    }

    'label:
    continue;
    continue 'label;
    break;
    break 'label;
    break expr;
    ```

- Go

  - 条件

    ```go
    if condition {
      // ...
    } else {
      // ...
    }

    switch {
      case condition:
        fallthrough
      default:
        // ...
    }

    switch expr {
      case expr1, expr2:
        fallthrough
      default:
        // ...
    }

    switch rt := intf.(type) {
      case Type:
        fallthrough
      default:
        // ...
    }
    ```

  - 循环

    ```go
    for {
      // ...
    }

    for condition {
      // ...
    }

    // for 循环中增删元素，对于 slice 不影响，因为会拷贝 slice 结构，对于 map 会影响后续元素访问
    for elem := range iterabal {
      // ...
    }

    label:
    continue;
    continue label;
    break;
    break label;
    ```

- Dart

  - 条件

    ```dart
    if (condition) {
      // ...
    } else {
      // ...
    }

    switch (expr) {
      case pattern:
        // ...
      default:
        // ...
    }
    ```

  - 循环

    ```dart
    while (true) {
      // ...
    }

    while (condition) {
      // ...
    }

    // for 循环中增删元素，会影响后续元素的访问
    for (final elem in iterabal) {
      // ...
    }

    label:
    continue;
    continue label;
    break;
    break label;
    ```

  - 异常

    ```dart
    try {
      throw FormatException('exception');
    } on FormatException {
      // ...
    } on FormatException catch (e) {
      // ...
    } catch (e, s) {
      print('Exception details:\n $e');
      print('Stack trace:\n $s');
      rethrow;
    } finally {
      cleanup();
    }
    ```

- TypeScript

  - 条件

    ```ts
    if (condition) {
      // ...
    } else {
      // ...
    }

    switch (expr) {
      case expr:
        // ...
        break;
      default:
      // ...
    }
    ```

  - 循环

    ```ts
    while (true) {
      // ...
    }

    while (condition) {
      // ...
    }

    // for 循环中增删元素，会影响后续元素的访问
    for (const key_or_index in iterable) {
      // ...
    }

    for (const element of iterable) {
      // ...
    }

    continue;
    continue label;
    break;
    break label;
    ```

  - 异常

    ```ts
    try {
      throw new Error("error message");
    } catch (e) {
      // ...
    } finally {
      // ...
    }
    ```

- Python

  - 条件

    ```python
    if condition:
        pass
    elif condition:
        pass
    else:
        pass

    match expr:
        case pattern:
            pass
        case _:
            pass
    ```

  - 循环

    ```python
    while True:
        pass

    while condition:
        pass
    else:
        pass

    # for 循环中增删元素，会影响后续元素的访问
    for element in iterable:
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

    with expr as target:
        pass
    ```

## 函数

- C++

  - 函数重载
  - 默认实参
  - 参数包：`...T`、`args...`

  ```cpp
  void return_nothing() {
    return;
  }

  int normal_function(int a1, const std::string& a2, int& a3) {

    auto closure = [v, &r, m = std::move(l)] (auto& elem) mutable {};

  }
  ```

- Rust

  - ~~函数重载~~
  - ~~默认实参~~
  - ~~参数包~~

  ```rust
  fn return_nothing() {
    ()
  }

  fn normal_function(a1: i32, a2: &str, a3: &mut i32) -> i32 {

    let closure_v1 = |x|               x + 1  ;
    let closure_v2 = |x|             { x + 1 };
    let closure_v3 = |x: i32| -> i32 { x + 1 };
    let closure_v4 = move |x: i32| -> i32 { x + outter_cap };
  }
  ```

- Go

  - ~~函数重载~~
  - ~~默认实参~~
  - 参数包：`args...`

  ```go
  func ReturnNothing() {
    return
  }

  func NormalFunction(a1 int, a2 string, a3 *int) (r1, r2 int) {

    closure := func(a int) int {
      return a * a
    }

    return
  }
  ```

- Dart

  - ~~函数重载~~
  - 默认实参
  - 参数包：`...args`

  ```dart
  void returnNothing() {
    return;
  }

  String positionalParameter(String r1, String? r2, [String o1 = "optional", String? o2]) {
    // ...
  }

  String namedParameter({required String r1, required String? r2, String o1 = "optional", String? o2}) {

    final closure_v1 = (args) => expression;
    final closure_v2 = (args) { statements; };
  }
  ```

- TypeScript

  - ~~函数重载~~
  - 默认实参
  - 参数包：`...args`

  ```ts
  function returnNothing(): void {
    return;
  }

  function normalFunction(a1: number, a2: string) {
    let closure_v1 = (args) => expression;
    let closure_v2 = (args) => {
      statements;
    };
  }
  ```

- Python

  - ~~函数重载~~
  - 默认实参
  - 参数包：`*posargs`、`**kwargs`

  ```python
  def return_nothing() -> None:
      return

  def normal_function(pos: str, /, pos_or_name: str, *, name: str) -> str:
      def closure_v1():
          nonlocal pos # capture variable if you want to assign it
      closure_v2 = lambda x, y: x + y
  ```

## 面向对象

### 封装

> - 构造函数初始化的缺点：无法返回错误，且无法安全调用虚函数（final 类除外）
> - 工厂方法初始化的场景：解决构造函数初始化的问题
> - Init 方法初始化的场景：仅在接口天然就适合两段式初始化时使用，因为在构造与初始化间存在不可用的中间态

- C++

  - 访问控制：`public`, `protected`, `private`
  - 构造控制：构造函数
  - 析构控制：析构函数
  - 拷贝控制：拷贝构造函数、移动构造函数、拷贝赋值函数、移动赋值函数
  - 比较操作：重载相等比较操作符与三路比较操作符（深度值比较）
  - 语言扩展：操作符重载，STL 迭代器

  ```cpp
  class MyClass {
    std::string _state;
    static const std::string _class_state;

   public:
    // 默认构造函数使用值初始化，自定义构造函数未出现在成员初始化列表中的成员使用默认初始化
    MyClass() = default;
    explicit MyClass(std::string state): _state(std::move(state)) {}
    MyClass(MyClass&&) = default;
    MyClass(const MyClass&) = default;
    auto operator=(MyClass&&) -> MyClass& = default;
    auto operator=(const MyClass&) -> MyClass& = default;
    ~MyClass() = default;

    friend auto operator<=>(const myclass&, const myclass&) -> std::strong_ordering = default;
    explicit operator bool() const;

    auto method() const -> void;
    static auto class_method() -> void;
  };

  auto inst = MyClass("");  rinst.method();
  auto& rinst = inst;       rinst.method();
  auto* pinst = &inst;      pinst->method();
                            MyClass::class_method();
  ```

- Rust

  - 访问控制：`pub`
  - 构造控制：一般通过工厂函数构造实例
  - 析构控制：`Drop`
  - 拷贝控制：`Copy`, `Clone`
  - 比较操作：`Eq`, `Ord`, `PartialEq`, `PartialOrd`（比较字段）
  - 语言扩展：通过 trait 和宏实现

  ```rust
  #[derive(Copy, Clone, PartialEq, PartialOrd)]
  struct MyClass {
    state: String;
  }

  impl MyClass {
    fn new(state: &str) -> MyClass {
      MyClass { state: String::from(state) }
    }

    fn method(&self) {/*...*/}
    fn class_method() {/*...*/}
  }

  let inst = MyClass::new("");  inst.method();
  let rinst = &inst;            rinst.method();
                                MyClass::class_method();
  ```

- Go

  - 访问控制：大写首字母公开到包外，包内代码可随意访问
  - ~~构造控制~~
  - ~~析构控制~~
  - ~~拷贝控制~~
  - 比较操作：默认实现比较字段
  - ~~语言扩展~~

  ```go
  type MyClass struct {
    state string
  }

  func (self *MyClass) Method() {
    // ...
  }

  inst := MyClass{state: ""}; inst.Method();
  pinst := &inst;             pinst.Method();
  ```

- Dart

  - 访问控制：非`_`开头导出
  - 构造控制：构造函数
  - ~~析构控制~~
  - ~~拷贝控制~~
  - 比较操作：默认实现比较引用，可重载比较操作符
  - 语言扩展：操作符重载

  ```dart
  class MyClass {
    String _state;
    static String _classState = "";

    // constructor likes function but no return type
    MyClass(String state) { this._state = state; }
    // named constructor beacase no funtion overload
    MyClass.named(String state) { this._state = state; }
    // using this in constructor, a syntax sugar
    MyClass.useThis(this._state);
    // initializer lists: final fields must have values before the constructor body executes
    MyClass.initList(String state) : _state = state;
    // redirect to another constructor
    MyClass.redirecting1(String state) : this(state);
    MyClass.redirecting2(String state) : this.named(state);

    String get property => _state;
    set property(String state) => _state = state;
    void method() {/*...*/}

    static String get classProperty => _classState;
    static set classProperty(String state) => _classState = state;
    static void classMethod() {/*...*/}
  }

  var inst = MyClass("");
  inst.method();
  MyClass.classMethod();
  ```

- TypeScript

  - 访问控制：非`#`开头导出
  - 构造控制：构造函数
  - ~~析构控制~~
  - ~~拷贝控制~~
  - ~~比较操作~~
  - ~~语言扩展~~

  ```ts
  class MyClass {
    state: string;
    static classState = "";

    constructor(state: string) {
      this.state = state;
    }

    get property(): string {
      return this.state;
    }

    set property(state: string): void {
      this.state = state;
    }

    method(): void {
      // ...
    }

    static get classProperty(): string {
      return this.classState;
    }

    static set classProperty(state: string) {
      this.classState = state;
    }

    static classMethod(): void {
      // ...
    }
  }

  let inst = new MyClass("");
  inst.method();
  MyClass.classMethod();
  ```

- Python

  - ~~访问控制~~
  - 构造控制：`__init__`
  - 析构控制：`__del__`
  - ~~拷贝控制~~
  - 比较操作
  - 语言集成：通过实现名如`__getitem__`的方法

  ```python
  class MyClass:
      class_state = ""

      def __init__(self, state: str) -> None:
          self._state = state

      def __del__(self) -> None:
          pass

      @property
      def aproperty(self) -> str:
          return self._state

      @aproperty.setter
      def aproperty(self, state) -> None:
          self._state = state

      def method(self):
          pass

      @property
      @classmethod
      def aclass_peoperty(cls) -> str:
          return cls.class_state

      @aclass_peoperty.setter
      @classmethod
      def aclass_peoperty(cls, state: str) -> None:
          cls.class_state = state

      @classmethod
      def class_method(cls) -> None:
          pass

  inst = MyClass("")
  inst.method()
  MyClass.class_method()
  ```

### 多态

- C++

  ```cpp
  class Abstract {
   public:
    virtual ~Abstract() = defatul;
    virtual auto abstract_method() -> void = 0;
  };

  class MyClass: public Abstract {
   public:
    auto abstract_method() -> void override;
  };

  Abstract* pintf = new MyClass();
  Abstract& rintf = *pintf;
  ```

- Rust

  ```rust
  trait Abstract {
    fn abstract_method(&self);
    fn abstract_class_method();
  }

  impl Abstract for MyClass {
    fn abstract_method(&self);
    fn abstract_class_method();
  }

  let intf: Box<dyn Abstract> = MyClass::new();
  ```

- Go

  ```go
  type Abstract interface {
    abstractMethod()
  }

  func (this *MyClass) abstractMethod() {
    // ...
  }

  var intf Abstract = &MyClass{}
  ```

- Dart

  ```dart
  abstract class Abstract {
    String get abstractProperty;
    set abstractProerty(String state);
    void abstractMethod();
    // no abstract class property or abstract class method support
  }

  class MyClass implements Abstract {
    String _state;

    @override
    String get abstractProperty => _state;

    @override
    set abstractProperty(String state) {
      _state = state
    }

    @override
    void abstractMethod() {/*...*/}
  }
  ```

- TypeScript

  ```ts
  interface Abstract {
    get abstractProperty(): string;
    set abstractProperty(state: string);
    abstractMethod(): void;
    // no abstract class property or abstract class method support
  }

  class MyClass implements Abstract {
    get abstractProperty(): string {
      // ...
    }
    set abstractProperty(state: string) {
      // ...
    }
    abstractMethod(): void {
      // ...
    }
  }
  ```

- Python

  ```python
  from abc import ABC, abstractmethod

  class Abstract(ABC):
      def __init__(self) -> None:
          pass

      @property
      @abstractmethod
      def abstract_property(self) -> None:
          pass

      @abstract_property.setter
      @abstractmethod
      def abstract_property(self, state: str) -> None:
          pass

      @abstractmethod
      def abstract_method(self) -> None:
          pass

      @property
      @classmethod
      @abstractmethod
      def abstract_class_property(self) -> None:
          pass

      @abstract_class_property.setter
      @classmethod
      @abstractmethod
      def abstract_class_property(self, state: str) -> None:
          pass

      @classmethod
      @abstractmethod
      def abstract_class_method(cls) -> None:
          pass

  class MyClass(Abstract):
      def __init__(self) -> None:
          super().__init__()

      @property
      def abstract_property(self) -> None:
          pass

      @abstract_property.setter
      def abstract_property(self, state: str) -> None:
          pass

      def abstract_method(self) -> None:
          pass

      @property
      @classmethod
      def abstract_class_property(self) -> None:
          pass

      @abstract_class_property.setter
      @classmethod
      def abstract_class_property(self, state: str) -> None:
          pass

      @classmethod
      def abstract_class_method(cls) -> None:
          pass

  inst: Abstract = MyClass()
  ```

### 继承

- C++

  ```cpp
  class Base {
    virtual ~Base() = default;
    virtual base_method() {/*...*/}
  };

  class MyClass: public Base {
   public:
    MyClass() : Base() {}

    void base_method() override {
      Base::base_method();
    }
  };
  ```

- Rust

  ```rust
  pub trait Base {
    fn base_method(&self) -> Self {/*...*/}
    fn base_class_method() {/*...*/}
  }

  impl Base for MyClass {
    fn base_method(&self) {
      Base::base_method(self);
      <MyClass as Base>::base_class_method();
    }
  }
  ```

- Go

  ```go
  type Base struct {
    state string
  }
  func (self *Base) BaseMethod() {
    // ...
  }

  type MyClass struct {
    Base    // Base 的方法集可由 MyClass 与 *MyClass 继承
    *Base   // *Base 的方法集仅可由 *MyClass 继承而 MyClass 不行
  }
  func (self *MyClass) BaseMethod() {
    self.Base.BaseMethod()
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
    }
  }
  ```

- TypeScript

  ```ts
  class Base {
    baseMethod(): void {
      // ...
    }
  }

  class MyClass extends Base {
    baseMethod(): void {
      super.baseMethod();
    }
  }
  ```

- Python

  ```python
  class Base:
      def base_method(self) -> None:
          pass

  class MyClass(Base):
      def __init__(self) -> None:
          super().__init__()

      def base_method(self) -> None:
          super(Base, self).base_method()
  ```

## 泛型

- 泛型约束
- 泛型函数
- 泛型类

## 工程能力

- 异常处理
- 日志记录
- 性能剖析
- 单元测试
- 文档生成

## 基础类库与框架

### 基础类型

- 布尔
- 整数
- 浮点数
- 字符
- 日期时间
- 集合类型
  - tuple
  - struct
  - array
  - map
  - set
  - linked list
  - ring queue
  - heap
- 联合

---

> 日期时间的存储或传递格式（消除歧义）：
>
> - Unix Timestamp，即 UTC 时间距离 1970-01-01 00:00:00 的带正负号的秒数（或毫秒、纳秒等）
> - ISO 8601，例如 `2000-07-27T21:30:59+08:00`

#### C++

```cpp
// 布尔
bool b = true || false;

// 整数：字面量的类型根据数值大小推断，最小为 int
int i = 9'000'000 + 0b1010 + 017 + 0xff;
size_t z = 10uz;

// 浮点数：字面量的类型默认为 double
double d = 1.0 + 2.5e3;
float f = 2.5f;

// 字符：索引元素为字节
char byte = 'A';
char32_t unicode = U'文';
const char* cstr = "hello world\n";
const char* cstr = R"(raw\n)";
std::string str = std::format("interpolation {}", cstr);

// 日期时间：sys_time 与 local_time 无时区，zoned_time 有时区
using namespace std::chrono;
auto zone = get_tzdb().current_zone();
auto now = system_clock::now();
auto datetime = zoned_time(zone, local_days(27d / 7 / 2000) + 21h + 30min + 59s).get_sys_time();
auto duration = years(1) + months(2) + days(3) + weeks(4) + 5h + 6min + 7s + 8ms + 9us + 10ns;
auto to_unix = duration_cast<seconds>(datetime.time_since_epoch()).count();
auto from_unix = system_clock::time_point(seconds(to_unix));
auto to_iso = std::format("{0:%F}T{0:%T%Ez}", zoned_time(zone, datetime));
system_clock::time_point from_iso;
std::stringstream(to_iso) >> parse("%FT%T%Ez", from_iso);

// 元组
std::tuple<int, double> t = {1, 2.5}; // std::get<0>(t)

// 结构
struct Struct {
  int f1;
  double f2;
};
Struct s = {1, 2.5};

// 集合类型
std::array<int, 3> arr = {1, 2, 3};
std::vector<int> vec = {1, 2, 3};
std::map<std::string, int> bmap = {{"one", 1}, {"two", 2}};
std::set<int> bset = {1, 2, 3};
std::unordered_map<std::string, int> hmap = {{"one", 1}, {"two", 2}};
std::unordered_set<int> hset = {1, 2, 3};
std::list<int> list = {1, 2, 3};
std::deque<int> deq = {1, 2, 3};
std::priority_queue<int> heap;

// 联合
auto u = std::variant<int, double>(1);
```

#### Rust

```rust
// 布尔
let b: bool = true || false;

// 整数：字面量的类型根据使用变量的上下文推断，默认为 i32
let i: i32 = 9_000_000 + 0b1010 + 0o17 + 0xff;

// 浮点数：字面量的类型根据使用变量的上下文推断，默认为 f64
let f: f64 = 1.0 + 2.5e3;

// 字符：不允许索引，但允许切片（运行时检测切片字节边界的 UTF-8 有效性）
let byte: u8 = b'A';
let unicode: char = '文';
let s: &str = "hello world\n"
let s: &str = r"raw\n";
let s: String = format!("interpolation {s}");

// 日期时间：SystemTime 无时区，OffsetDateTime 有时区
use time::format_description::well_known::Iso8601;
use time::macros::datetime;
use time::{Duration, OffsetDateTime};
let now = OffsetDateTime::now_local().unwrap();
let datetime = datetime!(2000-07-27 21:30:59 +8);
let duration = Duration::new(secs, nanos) + Duration::seconds(30);
let to_unix = datetime.unix_timestamp();
let from_unix = OffsetDateTime::from_unix_timestamp(to_unix).unwrap().to_offset(time::UtcOffset::current_local_offset().unwrap());
let to_iso = datetime.format(&Iso8601::DEFAULT).unwrap();
let from_iso = OffsetDateTime::parse(&to_iso, &Iso8601::DEFAULT);

// 元组
let tuple: (i32, f64, &str) = (1, 2.0, "three"); // tuple.0
let (a, .., c) = tuple;

// 结构
struct Struct {
  f1: i32,
  f2: f64,
  f3: bool,
}
let st = Struct{f1: 1, f2, ..other};
let Struct{f1: alia, f2, ..} = st;

// 静态数组
let array: [i32; 3] = [0; 3];
let array: [i32; 3] = [1, 2, 3];

// 动态数组
let vec: Vec<i32> = vec![1, 2, 3];

// 范围
let range: Range            = begin..end;
let range: RangeFrom        = begin..;
let range: RangeTo          = ..end;
let range: RangeFull        = ..;
let range: RangeInclusive   = begin..=end;
let range: RangeToInclusive = ..=end;

// 切片：可用于字符串、静态数组、动态数组
let slice: &str = "string slice";
let slice: &[i32] = &array[range];

// 其它集合
let bmap = std::collections::BTreeMap::new();
let bset = std::collections::BTreeSet::new();
let hmap = std::collections::HashSet::new();
let hset = std::collections::HashMap::new();
let heap = std::collections::BinaryHeap::new();
let deq = std::collections::VecDeque::new();
let list = std::collections::LinkedList::new();

// 联合（枚举）
enum Union {
  None,
  Tuple(i32, f64),
  Struct { f1: i32, f2: f64 },
}
let u = Union::Tuple(1, 2.5);
if let Union::Tuple(f1, 2.5) = u { };
```

#### Go

```go
// 布尔
var b bool = true || false

// 整数：字面量的类型为高精度常量，int 根据平台大小可能是 int32 或 int64
var i int = 9_000_000 + 0b1010 + 0o17 + 0xff;

// 浮点数：字面量的类型根据平台大小可能是 float32 或 float64
var f float64 = 1.0 + 2.5e3;

// 字符：索引元素为字节
var r rune = '文'
var s string = "hello world\n" + `raw\n`
bytes := []byte(s)
runes := []rune(s)
s = string(bytes)
s = string(runes)

// 日期时间：有时区
now := time.Now()
datetime := time.Date(2000, 7, 27, 21, 30, 59, 00, time.Local)
duration, _ := time.ParseDuration("1h30m59s")
toUnix := datetime.Unix()
fromUnix := time.Unix(toUnix, 0)
toIso := datetime.Format("2006-01-02T15:04:05-07:00")
fromIso, _ := time.Parse("2006-01-02T15:04:05-07:00", toIso)

// 结构
type Struct struct {
  int f1
  float64 f2
  bool f3
}
st := Struct{
  f1: 1,
  f2: 2.5,
  // f3 default zero-value
}

// 静态数组
array := [3]int{1, 2, 3}
array = [...]int{1, 2, 3}

// 动态数组（切片）
slice := []int{1, 2, 3}
slice = make([]int, len[, cap])
slice = array[begin:end]
slice = array[begin:]
slice = array[:end]
slice = array[:]
slice = append(slice, 4, 5)
slice = append(slice, other_slice...)
copy(dst_slice, slice)

// 映射
hmap := map[string]int{"one": 1}
val, ok := hmap["one"]
delete(hmap, "one")
```

#### Dart

```dart
// 布尔
bool b = true || false;

// 整数
int i = 9000000 + 0xff;

// 浮点数
double d = 1.0 + 2.5e3;

// 字符：索引元素为 Unicode
String str = 'hello ' + "world\n" + r'raw\n';
String inter = '$state or ${state}';
String multiline = '''
multiline
string
''';

// 日期时间：有时区
var now = DateTime.now();
var datetime = DateTime(2000, 7, 27, 21, 30, 59);
var duration = Duration(hours: 1, minutes: 30, seconds: 59);
var toUnix = datetime.millisecondsSinceEpoch;
var fromUnix = DateTime.fromMillisecondsSinceEpoch(toUnix);
String toIso8601StringWithOffset(DateTime datetime) {
  final offset = datetime.timeZoneOffset;
  return "${datetime.toIso8601String()}${offset.isNegative ? "-" : "+"}${offset.inHours.abs().toString().padLeft(2, "0")}:${(offset.inMinutes - offset.inHours * 60).abs().toString().padLeft(2, "0")}";
}
var toIso = toIso8601StringWithOffset(datetime);
var fromIso = DateTime.tryParse(toIso)?.toLocal();

// 元组
int i = (1);
(int,) r = (1,);
(int, double, {String a, String b}) record = (1, a: 'a', 2.5, b: 'b');
var t = (int, String); // No record type literals
var (x, y, a: av, :b) = record;
int one = record.$1; // 1
String a = record.a; // 'a'

// 列表
List<int> list = <int>[];
list = [1, 2, 3];
list = [0, ...list, ...?nullable_list];
list = [0, if (i.isOdd) i];
list = [0, for (final i in list) i];
var [a, ...rest, c] = list;

// 映射
Map<String, int> map = <String, int>{};
map = {'one': 1, 'two': 2, 'three': 3};
map = {...map, ...?nullable_map, 'one': 0};
map = {if (i.isOdd) 'i': i, 'one': 0};
map = {for (final e in map.entries) e.key: e.value, 'one': 0};
var {'zero': nullable_zero, ...} = map;

// 数集
Set<int> set = <int>{};
set = {1, 2, 3};
set = {0, ...set, ...?set};
set = {0, if (i.isOdd) i};
set = {0, for (final i in set) i};
```

#### TypeScript

```ts
// 布尔
let b = true || false;

// 整数：用浮点数表示整数
let i = 9_000_000 + 0b1010 + 0o17 + 0xff;

// 浮点数
let f = 1.0 + 2.5e3;
let nan = NaN + Infinity;

// 大数
let n = 9007199254740991n;

// 字符：索引元素为 UTF-16 编码！
let s = "hello " + "world\n" + String.raw`raw\n but ${state}`;
let inter = `${state}`;
let regexp = /pattern/i;

// 日期时间：有时区
let now = new Date();
let datetime = new Date(2000, 6, 27, 21, 30, 59); // month index start from 0!
let duration = 30_000; // 30s
let toUnix = datetime.getTime(); // Unix ms
let fromUnix = new Date(toUnix);
function toLocalISOString(date) {
  const offset = date.getTimezoneOffset();
  const offsetAbs = Math.abs(offset);
  const isoString = new Date(date.getTime() - offset * 60 * 1000).toISOString();
  return `${isoString.slice(0, -1)}${offset > 0 ? "-" : "+"}${String(
    Math.floor(offsetAbs / 60)
  ).padStart(2, "0")}:${String(offsetAbs % 60).padStart(2, "0")}`;
}
let toIso = toLocalISOString(datetime);
let fromIso = new Date(toIso);

// 联合
let u: null | number = 1;

// 数组
let arr = [1, 2, 3];
arr = [0, ...arr, 4];
let [a, , c=default, ...rest] = arr;
arr[0]

// 对象
let obj = { one: 1, two: 2, three };
obj = { ...obj, one: 0 };
let { one, two: alias2, ["three"]: alias3, ...rest } = obj;
obj.one;
obj["two"];
```

#### Python

```python
# 布尔
b: bool = True or False

# 整数: 32-bit or big-number
i: int = 9_000_000 + 0b1010 + 0o17 + 0xff

# 浮点数: 64-bit
f: float = 1.0 + 2.5e3

# 字符：索引元素为 Unicode
s: str = 'single' + "double"
s = r'raw\n'
s = f'interpolation {s}'
s = '''\
multiline
string
'''
add_s = s + s
mul_s = s * 3

# 日期时间：默认无时区，使用 astimezone() 方法转换为当地时区
now = datetime.now().astimezone()
dt = datetime(2000, 7, 27, 21, 30, 59).astimezone()
duration = timedelta(hours=1, minutes=30, seconds=59)
to_unix = dt.timestamp()
from_unix = datetime.fromtimestamp(to_unix).astimezone()
to_iso = dt.isoformat()
from_iso = datetime.strptime(to_iso, "%Y-%m-%dT%H:%M:%S%z").astimezone()

# 元组
tup: tuple[int] = (1,)
tup: tuple[int, float, str] = (1, 2.0, "three")
x, y, z = tup = 1, 2.0, "three"
add_t = tup + tup
mul_t = tup * 3

# 数组（列表）
ls: list[int] = [1, 2, 3]
ls = [i for i in ls if i % 2]
add_ls = ls + ls
mul_ls = ls * 3

# 范围
rg = range(stop)
rg = range(start, stop)
rg = range(start, stop, step)

# 切片：可用于线性序列如元组、列表、范围
ls[i]               # 返回浅拷贝：
ls[i:j]             #   i, j 为负时表示倒数下标，默认 0, len(ls)
ls[i:j:k]           #   k 为负时表示倒叙，默认为 1
ls[i] = 0           # 覆盖列表中的一个元素
ls[i:j] = [0]       # 覆盖列表中的若干个连续元素
ls[i:j:k] = [0, 0]  # 用右边列表的对应元素覆盖列表中的若干个元素

# 映射
hmap: dict[str, int] = {"one": 1, "two": 2, "three": 3}
hmap = {k: v for k, v in hmap.items() if k % 2}
hmap["zero"]     # 若键不存在则异常
hmap["zero"] = 0 # 赋值时若键不存在则自动创建
hmap |= map1     # map1 覆盖合并到 hmap

# 集合
hset: set[int] = {1, 2, 3}
hset = {i for i in hset if i % 2}
hset < set1 <= set2
hset > set1 >= set2
hset == set1 != set2
hset |= set1
hset &= set1
hset -= set1
hset ^= set1

# 联合
u: None | int | float = 1

```

### 数字相关

- 大数
- 分数
- 数学库
- 随机数

### 字符相关

- 正则表达式
- 格式化
- 序列化

### 数据结构与算法

- 容器：数组，栈，队列，循环队列，堆，链表，树，哈希表
- 算法：搜索，集合，数值，变序，更易

### 系统相关

- 环境变量
- 命令行参数
- 标准输入输出
- 文件系统

### 异步框架

- Task (async/await)
- Timer
- IO
  - 平台信息
  - 系统事件
  - 本地文件
  - 网络文件
  - 网络通讯
  - 进程通讯
  - 对象通讯
- Lock
- Channel

### UI

- 窗口

  - 位置
  - 大小
  - Z-Order
  - 显示/隐藏状态
  - 最大化/最小化状态
  - 活动状态
  - 禁用状态

- 内容

  - 文本（国际化）
  - 图标（矢量图）
  - 图像（多分辨率）
  - 动画

- 呈现

  - 布局：位置、大小、方向、伸缩（一维布局）、网格（二维布局）、图层（三维布局）
  - 样式：形状、颜色、特效等
  - 动画：
    - 随时间改变布局和样式从前一个状态平滑过渡到后一个状态
      - 复合多动画
      - 顺序多动画

- 交互

  - 鼠标事件：enter, leave, move, down, up
  - 高级手势：轻触、长按、右键、双击、三击、拖拽、选择、滚动、缩放
  - 键盘事件：down, up
  - 高级按键：焦点切换、快捷键

> 高 DPI 缩放问题：
>
> - 缩放逻辑像素 = dpi_scale × 物理像素
>   （优点是可以随系统缩放变化）
> - 设置基础分辨率 width × height，屏幕宽高不对应时自动缩放或扩充画面
>   （优点是在不同屏幕上的画面元素显示的相对屏幕大小的比例相近）
>
> 多分辨率布局问题：
>
> - 相对位置与大小
> - 响应式布局
>
> UI 框架类型：
>
> - 声明式 UI，不区分初始化和更新渲染，通过设置组件属性来监听事件
> - 命令式 UI，区分初始化和更新渲染，通过初始化时绑定事件或继承基类并覆盖方法来监听事件

### Game

- Scene
  - Camera
  - World
    - Objects

---

- Render
  - 2D
    - Sprites
    - Tilemaps
  - 3D
    - Models
    - Terrain
  - Color
    - Linear & Gamma
    - HDR
  - Textures
  - Materials
  - Meshes
  - Animations
  - Lighting & Shadows
  - Shaders
- Transform
  - Position
  - Rotation
  - Scale
- Physic
  - Force
    - Gravity
    - Bounciness
    - Friction
    - Drag
    - User Apply
  - Velocity
    - Linear
    - Angular
  - Body
    - Collider
    - Joint
- Particles
  - Emission (count per time)
  - Lifetime
  - Transform
  - Velocity
  - Gravity
- Audio
- Input

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

- 布尔：`is`，`not`，`any`，`all`，`none`，`has`，`enable`, `disable`
- 位置：`idx`，`pos`，`prev`，`next`，`lhs`，`rhs`，`head`，`tail`，`mid`，`begin`，`end`
- 计数：`count`，`size`，`length`，`width`，`height`，`depth`
- 时间：`new`，`old`，`orig`，`cur`，`before`，`after`
- 循环：`idx`，`pos`
- 序数：`number`，`1st`，`2nd`，`3rd`
- 介词：`in`，`on`，`at`，`of`，`2`，`4`
- 用途：`ret`，`val`，`need`，`temp`，`deal`，`src`，`dest`
