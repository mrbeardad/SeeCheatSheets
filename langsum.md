# 编程语言

- [编程语言](#编程语言)
  - [依赖](#依赖)
  - [变量](#变量)
  - [枚举](#枚举)
  - [操作符](#操作符)
  - [控制流](#控制流)
  - [函数](#函数)
  - [面向对象](#面向对象)
    - [封装](#封装)
    - [多态](#多态)
    - [继承](#继承)
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
    - [异步框架](#异步框架)
    - [系统相关](#系统相关)
    - [工程能力](#工程能力)
    - [UI](#ui)
  - [命名规范](#命名规范)

## 依赖

- C++

  - 程序入口：main 函数
  - 依赖单元：一个头文件 + 一个实现文件（可选）
  - 依赖导入：符号限定于命名空间
  - 依赖导出：头文件中所有宏与符号
  - 依赖初始化：global-constructor
  - 依赖管理：git-submodule + cmake

  ```cpp
  #include <iostream>
  #include "3rd_party/path/to/header.hpp"
  #include "path/to/header.h"

  using namespace std;
  using qualifier::foo, qualifier::bar;

  namespace ascpp {
    // ...
  }
  ```

- Rust

  - 程序入口：`main.rs` 中的 main 函数
  - 依赖单元：内部依赖单元是一个源文件，外部依赖单元是一个 crate
  - 依赖导入：符号限定于模块路径
  - 依赖导出：`pub`声明
  - ~~依赖初始化~~
  - 依赖管理：cargo

  ```rust
  pub mod module;

  use create::module::foo;
  use module::{self, foo, bar};
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
  import 'path/to/file.dart';
  import 'path/to/file.dart' as qualifier;
  import 'path/to/file.dart' show foo;
  import 'path/to/file.dart' hide bar;
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
  - 依赖导出：非`_`开头的符号
  - 依赖导入：符号限定于模块路径
  - 依赖初始化：脚本导入即执行
  - 依赖管理：pip

  ```py
  import module
  import package.module
  from module import *
  from module import foo, bar
  from package import *
  from package import module
  from package.module import name as alias
  ```

## 变量

> - 存储结构
>   - 结构型：变量本身代表一块包含数据结构的内存，拷贝时至少包括该内存
>   - 引用型：变量仅仅指向一块包含数据结构的内存，拷贝时仅针对该指针值
> - 类型系统
>   - 强类型：变量类型、函数签名、类属性与方法等，均静态确定且不可随意变
>   - 弱类型：变量类型、函数签名、类属性与方法等，均动态确定且可随意变
> - 生命周期：
>   - 构造
>   - 移动
>   - 销毁
> - 引用的目的
>   - 避免拷贝/移动
>   - 修改原值
>   - 多态

- C++

  - 变量类型：结构型强类型
  - 作用域：块作用域`{}`
  - 所有权：默认一个实例只能有一个所有者
  - 生命周期：右值移动所有权，所有者退出作用域时销毁变量

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

  - 变量类型：结构型强类型
  - 作用域：块作用域`{}`
  - 所有权：默认一个实例只能有一个所有者
  - 生命周期：non-`Copy` trait 移动所有权，所有者退出作用域时销毁变量

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

  - 变量类型：结构型强类型
  - 作用域：块作用域`{}`
  - 所有权：一个实例可能有多个所有者
  - 生命周期：直到不再被引用时才会被 GC 回收

  ```go
  foo := other
  foo := &other

  var foo Type = other
  var foo *Type = &other

  const FOO = 1 << 511
  ```

- Dart

  - 变量类型：引用型强类型
  - 作用域：块作用域`{}`
  - 所有权：一个实例可能有多个所有者
  - 生命周期：直到不再被引用时才会被 GC 回收

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

  - 变量类型：引用型弱类型
  - 作用域：块作用域`{}`
  - 所有权：一个实例可能有多个所有者
  - 生命周期：直到不再被引用时才会被 GC 回收

  ```ts
  let foo = other;
  const foo = other;

  let foo: Type = other;
  const foo: Type = other;
  ```

- Python

  - 变量类型：引用型弱类型
  - 作用域：函数作用域
  - 所有权：一个实例可能有多个所有者
  - 生命周期：直到不再被引用时才会被 GC 回收

  ```python
  global GLOBAL_VAR
  nonlocal outside_var
  foo = other
  ```

## 枚举

- C++

  ```cpp
  enum class EnumValue {
    ONE = 1,
    TWO,
    THREE,
  };
  ```

- Rust

  ```rust
  enum EnumValue {
    ONE = 1,
    TWO,
    THREE,
  }

  enum EnumType {
      NoMem,
      Tuple(i32, i32),
      Struct { x: i32, y: i32 },
  }
  ```

- Go

  ```go
  const (
    _ = iota
    KB = 1 << (10 * iota)
    MB
    GB
  )
  ```

- Dart

  ```dart
  enum EnumValue {
    ZERO,
    ONE,
    TWO,
  }

  enum EnumValueWithField {
    car(tires: 4, passengers: 5, carbonPerKilometer: 400),
    bus(tires: 6, passengers: 50, carbonPerKilometer: 800),
    bicycle(tires: 2, passengers: 1, carbonPerKilometer: 0);

    final int tires;
    final int passengers;
    final int carbonPerKilometer;

    const Vehicle({
      required this.tires,
      required this.passengers,
      required this.carbonPerKilometer,
    });
  }
  ```

- TypeScript

  ```ts
  enum EnumValue {
    ONE = 1,
    TWO,
    THREE,
  }

  enum EnumType {
    Type1,
    Type2,
  }
  ```

- Python

## 操作符

> 优先级：单元后缀 > 单元前缀 > 算术 > 关系 > 逻辑

- C++

  - 单后：`::`, `.`, `->`, `[]`, `()`, `++`, `--`
  - 单前：`&`, `*`, `+`, `-`, `++`, `--`
  - 算数：`*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：`=`
  - 其他：`? :`

- Rust

  - 单后：`::`, `.`, `[]`, `()`
  - 单前：`&`, `*`, `+`, `-`
  - 算数：`*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：`=`
  - 其他：`!`, `?`, `..`, `..=`

- Go

  - 单后：`.`, `[]`, `()`, `++`, `--`
  - 单前：`&`, `*`, `+`, `-`
  - 算数：`*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：`=`, `:=`

- Dart

  - 单后：`!`, `.`, `?.`, `[]`, `?[]`, `()`, `++`, `--`
  - 单前：`+`, `-`, `++`, `--`
  - 算数：`*`, `/`, `~/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`, `is`, `is!`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：`=`, `??=`
  - 其他：`? :`, `??`, `..`, `?..`

- TypeScript

  - 单后：`!`, `.`, `?.`, `[]`, `()`, `++`, `--`
  - 单前：`+`, `-`, `++`, `--`
  - 算数：`**`, `*`, `/`, `%`, `+`, `-`
  - 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`, `===`, `!==`, `in`
  - 逻辑：`!`, `&&`, `||`
  - 赋值：`=`, `??=`
  - 其他：`? :`, `??`

- Python

  - 单后：`.`, `[]`, `()`
  - 单前：`+`, `-`
  - 算数：`**`, `*`, `/`, `//`, `%`, `+`, `-`
  - 关系：`a < b <= c > d >= e`, `x == y != z`, `in`
  - 逻辑：`not`, `and`, `or`
  - 赋值：`a = b = c`

## 控制流

- C++

  - 分支

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
    for (declaration; condition; expression) {
      // ...
    }

    for (const auto& elem : iterabal) {
      // ...
    }
    ```

  - 异常

    ```cpp
    // try / function try
    [auto func()] try {
    // throw / rethrow
      throw std::exception();
    // catch / catch all
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    } catch (...) {
      throw;
    }
    ```

- Rust

  > Rust 的控制流全是表达式而非语句，甚至块作用域符号`{}`本身也是表达式

  - 分支

  ```rust
  if condition {
    // ...
  } else {
    // ...
  }

  match expr {
    PATTERN => expr1,
    PATTERN => expr2,
    _ => expr3
  }

  if let PATTERN = expr {
    // ...
  } else {
    // ...
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

  while let PATTERN = expr {
    // ...
  }

  for element in iterator {
    // ...
  }

  break expr;
  break 'labeled;
  ```

- Go

  - 分支

    ```go
    if condition {
      // ...
    } else {
      // ...
    }

    switch expr {
      case expr1, expr2:
        fallthrough
      default:
        // ...
    }

    switch {
      case condition:
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
    for declaration; condition; expression {
      // ...
    }

    for elem := range iterabal {
      // ...
    }
    ```

- Dart

  - 分支

    ```dart
    if (condition) {
      // ...
    } else {
      // ...
    }

    switch (comparable) {
      case fallthrough:
      case constant1:
        // ...
        break;
      case constant2:
        // ...
        continue fallthrough;
      default:
        // ...
    }
    ```

  - 循环

    ```dart
    for (declaration; condition; expression) {
      // ...
    }

    for (final elem in iterabal) {
      // ...
    }
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

  - 分支

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
    for (declaration; condition; expression) {
      // ...
    }

    for (const keyOrIndex in iterable) {
      // ...
    }

    for (const element of iterable) {
      // ...
    }

    break labeled;
    continue labeled;
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

  - 分支

  ```python
  if condition:
    pass
  elif condition:
    pass
  else:
    pass

  match expr:
    case PATTER:
      pass
    case _:
      pass
  ```

  - 循环

  ```python
  while condition:
    pass
  else:
    pass

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

> - 注意函数对**可重入性**与**线程安全性**的保证

- C++

  - 默认实参
  - 函数重载
  - 泛型
  - 参数包（`...T`与`arg...`）

  ```cpp
  void return_nothing() {
    return;
  }

  auto normal_function(int a1, const std::string& a2, int& a3) -> char {
    // ...
  }

  auto closure = [&r, v, m = std::move(l)] (auto& elem) mutable {}
  ```

- Rust

  - ~~默认实参~~
  - ~~函数重载~~
  - 泛型
  - ~~参数包~~

  ```rust
  fn return_nothing() {
    ()
  }

  fn normal_function(a1: int, a2: &str, a3: &mut int) -> char {
    // ...
  }

  let closure_v1 = |x: u32| -> u32 { x + 1 };
  let closure_v2 = |x|             { x + 1 };
  let closure_v3 = |x|               x + 1  ;
  let closure_v4 = move |x: u32| -> u32 { x + outter_cap };
  ```

- Go

  - ~~默认实参~~
  - ~~函数重载~~
  - 泛型
  - 参数包（`args...`）

  ```go
  func ReturnNothing() {
    return
  }

  func NormalFunction(a1 int, a2 string, a3 *int) (x, y string) {
    // ...
    return
  }

  closure := func() {
    return
  }
  ```

- Dart

  - 默认实参
  - ~~函数重载~~
  - 泛型
  - 参数包（`...args`）

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

  final closure1 = (args) => expression;
  final closure2 = (args) {statements;}
  ```

- TypeScript

  - 默认实参
  - ~~函数重载~~
  - ~~泛型~~
  - 参数包（`...args`）

  ```ts
  function returnNothing(): void {
    return;
  }

  function normalFunction(s: string, i: int) {
    // ...
  }

  let closure1 = (args) => expression;
  let closure2 = (args) => {
    statements;
  };
  ```

- Python

  - 默认实参
  - ~~函数重载~~
  - ~~泛型~~
  - 参数包（`*posargs`与`**kwargs`）

  ```python
  def return_nothing() -> None:
    return

  def normal_function(pos: str, /, pos_or_name: str, *, name: str) -> str:
    return pos + pos_or_name + name

  closure = lambda x, y: x + y
  ```

## 面向对象

### 封装

> - 构造函数初始化的缺点：无法返回错误，且无法安全调用虚函数（final 类除外）
> - 工厂方法初始化的场景：解决构造函数初始化的问题
> - Init 方法初始化的场景：仅在接口天然就适合两段式初始化时使用，因为在构造与初始化间存在不可用的中间态

- C++

  - 访问控制：`public`, `protected`, `private`
  - 构造控制：`ClassName(): ... {...}`
  - 析构控制：`~ClassName() {...}`
  - 拷贝控制：`ClassName(const ClassName&): ... {...}`
  - 比较操作：`... operator<=>(const ClassName&) {...}`
  - 语言集成：运算符重载，STL 迭代器

  ```cpp
  class MyClass {
   public:
    MyClass() = default;
    MyClass(MyClass&&) = default;
    MyClass(const MyClass&) = default;
    auto operator=(MyClass&&) -> MyClass& = default;
    auto operator=(const MyClass&) -> MyClass& = default;
    ~MyClass() = default;

    auto operator<=>(const myclass&) -> std::strong_ordering = default;
    explicit operator bool() const { return _name.size(); }

    auto property() const -> const std::string& { return _name; }
    auto set_property(std::string name) { _name = std::move(name); }

    void method() const;
    static cls_method();

   private:
    std::string _name;
  };
  ```

- Rust

  - 访问控制：`pub`
  - ~~构造控制~~
  - 析构控制：`Drop`
  - 拷贝控制：`Clone`, `Copy`
  - 比较操作：`Eq`, `Ord`, `PartialEq`, `PartialOrd`
  - 语言集成：通过 trait 实现

  ```rust
  #[derive(Copy, Clone, PartialEq, PartialOrd)]
  pub struct MyClass {
    name: String;
  }

  impl MyClass {
    pub fn new(name: &str) -> String {
      String::from(name)
    }

    pub fn property(&self) -> &str {
      self.name
    }

    pub fn set_property(&mut self, &str name) {
      self.name = String::from(name);
    }

    pub fn method(&self) {
      // ...
    }

    pub fn cls_method() {
      // ...
    }
  }
  ```

- Go

  - 访问控制：大写首字母导出包外，包内代码可随意访问
  - ~~构造控制~~
  - ~~析构控制~~
  - ~~拷贝控制~~
  - 比较操作：自动实现
  - ~~语言集成~~

  ```go
  type MyClass struct {
    name string
  }

  func (this *MyClass) Method() {
    doSomething(this.name)
  }
  ```

- Dart

  - 访问控制：非`_`开头导出
  - 构造控制：`MyClass.constrcutor(...)`
  - ~~析构控制~~
  - ~~拷贝控制~~
  - 比较操作：重载比较操作符
  - 语言集成：运算符重载

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

    bool operator ==(Object other) => other is MyClass && other._name = name;

    String get property => name;
    set property(String value) => name = value;

    void method() {}
  }
  ```

- TypeScript

  - 访问控制：`public`, `protected`, `private`
  - 构造控制：`constructor(...) {...}`
  - ~~析构控制~~
  - ~~拷贝控制~~
  - ~~比较操作~~
  - ~~语言集成~~

  ```ts
  class MyClass {
    private name: string;

    constructor(name: string) {
      this.name = name;
    }

    get property(): string {
      return this.name;
    }

    set property(new_name: string) {
      this.name = new_name;
    }

    method() {
      doSomething(this.name);
    }
  }
  ```

- Python

  - ~~访问控制~~
  - 构造控制：`__init__(self, ...):`
  - 析构控制：`__del__(self):`
  - ~~拷贝控制~~
  - 比较操作
  - 语言集成：通过实现名如`__getitem__`的方法

  ```python
  class myclass:
    def __init__(self, name: str):
      self._name = name

    def __del__(self):
      self._name.__del__()

    @property
    def pproperty(self):
      return self._name

    @pproperty.setter
    def pproperty(self, name):
      self._name = name

    def method(self):
      pass

    def cls_method(cls):
      pass
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

- Rust

  ```rust
  trait Abstract {
    fn abstract_method();
  }

  impl Abstract for MyClass {
    fn abstract_method() {
      // ...
    }
  }

  let intf: Box<dyn Abstract> = MyClass::new();
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

- TypeScript

  ```ts
  interface Abstract {
    abstractMethod(): void;
  }

  class MyClass implements Abstract {
    abstractMethod(): void {
      // ...
    }
  }
  ```

- Python

### 继承

- C++

  ```cpp
  class base {
    base() = default;
    virtual ~base() =default;
    virtual base_method() {}
  };

  class myclass: public base, public mixin {
   public:
    myclass() : base(), mixin() {}
    virtual ~myclass() override = default;

    virtual void base_method() override {
      // ...
      base_method();
      base::base_method();
    }
  };
  ```

- Rust

  ```rust
  pub trait Base {
    fn base_factory() {}
    fn base_method(&self) {}
  }

  #[derive(Mixin)]
  impl Base for MyClass {
    fn base_method(&self) {
      // ...
      self.base_method();
      Base::base_method(self);
      Self::base_factory();
      <MyClass as Base>::basefactory();
    }
  }
  ```

- Go

  ```go
  type Base struct {
    name string
  }
  func (self *Base) BaseMethod() {
    // ...
  }

  type MyClass struct {
    Base    // Base 的方法集可由 MyClass 与 *MyClass 继承
    *Mixin  // *Mixin 的方法集仅可由 *MyClass 继承而 MyClass 不行
  }
  func (self *MyClass) BaseMethod() {
    // ...
    self.BaseMethod()
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
      // ...
      baseMethod();
      super.baseMethod();
    }
  }
  ```

- TypeScript

  ```ts
  class MyClass extends Base {
    // ...
  }
  ```

- Python

  ```python
  class base:
    def __init__(self):
      super(base, self).__init__()
      super(mixin, self).__init__()

    def base_method(self):
      pass

  class myclass(base, mixin):
    def __init__(self):
      super(base, self).__init__()
      super(mixin, self).__init__()

    def base_method(self):
      self.base_method()
      super(base, self).base_method()
  ```

## 基础类库与框架

### 基础类型

- 布尔
- 整数
- 实数
- 字符
- 日期时间
- 集合类型

---

> 日期时间通常存储为 Unix Timestamp，即当前 UTC 时间距离 1970-01-01 00:00:00 的秒数（或毫秒、纳秒等）；
> 想要能正确且无歧义从本地时间构造日期时间对象，和将日期时间对象转换为可读的本地时间，还需要一个时区参数来表示从本地时间相对 UTC 时间的偏移量；
> ISO 8601 格式 `2000-07-27T21:30:59+08:00` 不仅能为当地人展示可读的无歧义的本地时间，同时还让其它时区的人能获取无歧义的 UTC 时间。

#### C++

```cpp
// 布尔
bool b = true || false;

// 整数：字面量的类型根据数值大小推断，最小为 int
int i = 9'000'000 + 0b1010 + 017 + 0xff;
unsigned u = 10u;
size_t uz = 10uz;
ssize_t z = 10z;

// 实数：字面量的类型默认为 double
double d = 1.0 + 2.5e3;
float f = 2.5f;

// 字符：索引元素为字节
char ascii = 'a';
char32_t unicode = U'文';
const char* cstr = "hello world!\n" R"(raw\n)";
std::string str = cstr;
const char* u8 = u8"utf-8 string";
const char16_t* u16 = u"utf-16 string";
const char32_t* u32 = U"utf-32 string";
const wchar_t* u32 = L"utf-16 or utf-32 string";

// 日期时间：sys_time 与 local_time 无时区，zoned_time 有时区
using namespace std::chrono;
auto lz = get_tzdb().current_zone();
auto now = system_clock::now();
auto datetime = zoned_time(lz,
    local_days(27d / 7 / 2000) + 21h + 30min + 59s)
    .get_sys_time();
auto duration = 1h + 30min + 59s;
auto to_unix = duration_cast<seconds>(datetime.time_since_epoch()).count();
auto from_unix = system_clock::time_point(seconds(to_unix));
auto to_iso = std::format("{0:%F}T{0:%T%Ez}", zoned_time(lz, datetime));
system_clock::time_point from_iso;
std::stringstream(to_iso) >> parse("%FT%T%Ez", from_iso);

// 结构
point p1 = {10, 20, 30};
point p2 = {.x = 10, .y = 20, .z = 30};
auto [x, y, z] = p2;

// 元组
tuple<int, double> t = {1, 2.0};

// 其它集合
std::array<int, 3> arr = {1, 2, 3};
std::vector<int> vec = {1, 2, 3};
std::deque<int> deq = {1, 2, 3};
std::priority_queue<int> heap;
std::list<int> list = {1, 2, 3};
std::map<std::string, int> bmap = {{"one", 1}, {"two", 2}};
std::set<int> bset = {1, 2, 3};
std::unordered_map<std::string, int> hmap = {{"one", 1}, {"two", 2}};
std::unordered_set<int> hset = {1, 2, 3};
```

#### Rust

```rust
// 布尔
let b: bool = true || false;

// 整数：字面量的类型根据使用变量的上下文推断，默认为 i32
let i: i32 = 9_000_000 + 0b1010 + 0o17 + 0xff;

// 实数：字面量的类型根据使用变量的上下文推断，默认为 f64
let f: f64 = 1.0 + 2.5e3;

// 字符：不允许索引，但允许切片（运行时检测切片字节边界的 UTF-8 有效性）
let byte: u8 = b'a';
let c: char = '文';
let s: &str = "hello world\n" + r"raw\n";
let inter: String = format!("{name}");
let ss: String = String::from(s);

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

// 枚举
let opt = Some(50);
if let Some(x) = opt {};

// 结构
let point = Point{x: 10, y, ..other_point}; // point.x
let x: i32 = point.x;
let Point{x: x_px, y, ..} = point;

// 元组
let tuple: (i32, f64, &str) = (1, 2.0, "three");
let one: i32 = tuple.0;
let (a, .., c) = tuple;

// 数组
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

// 切片：可用于字符串、数组、动态数组
let slice: [i32] = arr[range];

// 其它集合
let deq = std::collections::VecDeque::new();
let heap = std::collections::BinaryHeap::new();
let list = std::collections::LinkedList::new();
let bmap = std::collections::BTreeMap::new();
let bset = std::collections::BTreeSet::new();
let hmap = std::collections::HashSet::new();
let hset = std::collections::HashMap::new();
```

#### Go

```go
// 布尔
var b bool = true || false

// 整数：字面量的类型为高精度常量，int 根据平台大小可能是 int32 或 int64
var i int = 9_000_000 + 0b1010 + 0o17 + 0xff;

// 实数：字面量的类型根据平台大小可能是 float32 或 float64
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
point := Point{
  x: x,
  y: y,
  // z default 0
}

// 数组
array := [3]int{1, 2, 3}
array = [...]int{1, 2, 3}

// 切片
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

// 实数
double d = 1.0 + 2.5e3;

// 字符：索引元素为 Unicode
String str = 'hello ' + "world\n" + r'raw\n';
String inter = '$name or ${name}';
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

// 集合
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

// 整数
let i = 9_000_000 + 0b1010 + 0o17 + 0xff;

// 实数
let f = 1.0 + 2.5e3;
let nan = NaN + Infinity;

// 字符：索引元素为 Unicode
let s = "hello " + "world\n" + String.raw`raw\n but ${name}`;
let inter = `${name}`;
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

// 数组
let arr = [1, 2, 3];
arr = [0, ...arr, 4];
let [a, ...rest] = arr;

// 对象
let obj = { one: 1, two: 2, three };
obj = { ...obj, one: 0 };
let { one: varone, ["two"]: two, three, ...rest } = obj;
obj.one;
obj["two"];
```

#### Python

```python
# 布尔
b: bool = True or False

# 整数: 32-bit or big-number
i: int = 9_000_000 + 0b1010 + 0o17 + 0xff

# 实数: 64-bit
f: float = 1.0 + 2.5e3

# 字符：索引元素为 Unicode
s: str = 'hello ' + "world\n" + r'raw\n'
s = f'inter {s}'
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
i1: int = 1
t1: tuple[int] = (1,)
tup: tuple[int, float, str] = (1, 2.0, "three")
x, y, z = tup = 1, 2.0, "three"
add_t = tup + tup
mul_t = tup * 3

# 列表
ls: list[int] = [1, 2, 3]
ls = [i for i in ls if i % 2]
add_ls = ls + ls
mul_ls = ls * 3

# 范围
r = range(stop)
r = range(start, stop)
r = range(start, stop, step)

# 切片：可用于线性序列如元素、列表、范围
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

- 容器：表，栈，队列，堆，链表，树，哈希表
- 算法：搜索，集合，数值，变序，更易

### 异步框架

- 文件 IO
- 网络 IO
- IPC
- IOC
- Lock
- Timer
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

  - 文本：国际化
  - 图标
  - 图像：分辨率
  - 视频
  - 音频
  - 2D 绘制
  - 3D 绘制

- 展示

  - 布局：位置、大小、方向、伸缩（一维布局）、网格（二维布局）、图层（三维布局）
  - 样式：颜色、形状等
  - 动画：随时间变化而更改布局与样式
  - Materail Design Components

- 输入

  - 鼠标事件：enter, leave, move, down, up
  - 高级手势：轻触、长按、右键、双击、三击、拖拽、选择、缩放、滚动
  - 键盘事件：down, up
  - 高级按键：焦点交互、快捷键
  - 系统事件

- MVVM 模式

  - View：负责展示状态数据并响应 UI 事件
  - ViewModel：负责数据绑定并提供响应逻辑
  - Model：负责维护并提供业务数据与业务逻辑

![framework](images/framework.png)

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
