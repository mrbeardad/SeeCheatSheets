# 目录

- [目录](#目录)
  - [面向过程](#面向过程)
    - [预处理](#预处理)
    - [命名空间](#命名空间)
    - [符号链接](#符号链接)
    - [类型别名与符号引入](#类型别名与符号引入)
    - [变量](#变量)
      - [声明与定义](#声明与定义)
      - [静态变量](#静态变量)
    - [表达式](#表达式)
      - [类型转换](#类型转换)
      - [左值与右值](#左值与右值)
      - [运算符](#运算符)
      - [求值顺序](#求值顺序)
      - [字面值](#字面值)
    - [语句](#语句)
      - [分支](#分支)
      - [循环](#循环)
      - [异常](#异常)
    - [函数](#函数)
      - [参数修饰](#参数修饰)
      - [关键字](#关键字)
      - [默认实参](#默认实参)
      - [重载函数](#重载函数)
      - [Lambda 闭包](#lambda-闭包)
      - [协程](#协程)
    - [特殊结构类](#特殊结构类)
      - [字面值类](#字面值类)
      - [聚合类](#聚合类)
      - [枚举类](#枚举类)
      - [联合类](#联合类)
      - [嵌套类](#嵌套类)
  - [面向对象](#面向对象)
    - [封装](#封装)
      - [声明与定义](#声明与定义-1)
      - [友元](#友元)
      - [访问控制](#访问控制)
      - [特种成员](#特种成员)
      - [其他成员](#其他成员)
    - [继承](#继承)
      - [作用域](#作用域)
      - [基类访问控制](#基类访问控制)
      - [多重继承](#多重继承)
    - [多态](#多态)
      - [实现原理](#实现原理)
      - [虚函数](#虚函数)
      - [运行时类型识别](#运行时类型识别)
    - [设计](#设计)
  - [泛型编程](#泛型编程)
    - [模板参数](#模板参数)
    - [变参](#变参)
    - [变量模板](#变量模板)
    - [函数模板](#函数模板)
    - [类模板](#类模板)
    - [概念](#概念)
    - [模板特例化](#模板特例化)
    - [其它](#其它)
    - [编译器类型推断](#编译器类型推断)
    - [元编程](#元编程)
  - [其他特性](#其他特性)
    - [内存分配](#内存分配)
    - [成员指针](#成员指针)
    - [volatile](#volatile)
    - [属性](#属性)
    - [基本概念](#基本概念)
    - [内存顺序](#内存顺序)

因为 C++的特性很多且复杂，所以对一些特性的描述会故意略去风格不太好的部分，以简化记忆。本文更适合有一点 C++基础的同学阅读，若文中有的特性并未细说，则读者可自行查阅[cppreference](https://zh.cppreference.com/w/cpp)

> Effective C++：学习 C++时可以将该语言分为四大部分：
>
> - [面向过程](#面向过程)
> - [面向对象](#面向对象)
> - [泛型](#泛型编程)
> - [STL](./cpplib.md#容器库)

## 面向过程

### 预处理

- 头文件引入
  - `#include < >` ：一般用来引入系统接口和标准库
  - `#include " "` ：一般用来引入第三方库和项目库
  - `__has_include(< >)`
  - `__has_include(" ")`
- 宏替换
  - `#define`
  - `#undef`
  - `defined(identifier)`
- 条件编译
  - `#if`
  - `#ifdef`
  - `#ifndef`
  - `#elif`
  - `#elifdef`
  - `#elifndef`
  - `#else`
  - `#endif`
  - `#pragma once`
- 编译器预定义宏
  - 通用信息：`__func__`、`__LINE__`、`__FILE__`、`__TIME__`、`__DATE__`
  - 操作系统：`_WIN32`、`__linux__`、`TARGET_OS_MAC`
  - 编译平台：`_MSC_VER`、`__GUNC__`、`__clang__`
  - 标准版本：`__cplusplus`

| `__cplusplus` | value   |
| ------------- | ------- |
| pre-C++98     | 1       |
| C++98         | 199711L |
| C++11         | 201103L |
| C++14         | 201402L |
| C++17         | 201703L |
| C++20         | 201709L |

### 命名空间

- 头文件中的符号都应放在命名空间中，实现文件代码最好也放入同一命名空间
- 头文件中的符号，要么只提供接口声明，要么 inline 声明的定义

```cpp
// 命名空间：第一次声明为定义，以后为打开
namespace std {

}

// 嵌套命名空间
namespace A::B { // A无需提前定义，此处即可作为A的定义
namespace C {

}
}

// 内联命名空间
inline namespace inline_ns { // 只需定义时指出inline
    /* 内部成员名字暴露在外 */
}

// 无名命名空间
namespace {
    /* 内部成员名字暴露在外，且不对外符号链接 */
}

// 全局命名空间
::foo // 明确限定引用全局符号，会与内联命名空间中同名符号冲突，但不会与无名命名空间冲突

// 命名空间别名
namespace ABC = A::B::C
```

### 符号链接

- 无修饰

  - 无论是否对其初始化，都被当作定义
  - 符号定义对外可链接

- static

  - 无论是否对其初始化，都被当作定义
  - 符号定义不可对外链接
  - 使用目的：
    - 只在本翻译单元中使用该符号而不暴露出去

- extern

  - 未初始化则不会被当作定义
  - 符号定义对外可链接
  - 使用目的：
    - 希望引用外部符号
    - `extern "C"`：符号限定为标准 C ABI，即禁用名称篡改和明确调用约定

- inline

  - 必须初始化且被当做定义
  - 符号定义对外链接，且可多重定义
  - 使用目的：
    - inline 声明最主要作用是允许符号多重定义而不会链接报错，如此以来可在头文件中定义符号，因为头文件可在多个翻译单元中被引入
    - 次要作用是提示编译器针对 inline 函数进行内联展开，节省函数调用开销

- const/constexpr

  - 必须初始化且被当作定义
  - 符号定义默认不对外链接，可以使用 extern 强制对外
  - 使用目的：
    - 提高程序性能

### 类型别名与符号引入

- 类型别名：

  ```cpp
  using TypeAlias = OrigType;
  ```

- 模板类型别名：

  ```cpp
  template<typename T>
  using Alias = TempType<T, int>      // 别名涉及到模板
  ```

- using 声明：<span id="using声明"></span>

  ```cpp
  using ns::var, ns::func, ns::cls;   // 使命名空间成员直接可见，符号冲突则报错
  ```

- using 指示

  ```cpp
  using namespace ns;                 // 时命名空间中所有成员直接可见，符号冲突可使用ns::tag解决
  ```

- using 继承

  ```cpp
  template<typename ...T>
  class D : B1, B2, T... {
      using T::operator();
      using B1::B1, B2::B2;
  }
  ```

  - 类似 using 声明，将基类部分的成员引入派生类的作用域

    - 改变**优先级**和**访问控制**
    - 引入不同基类的同名函数可将**冲突关系变为重载关系**
    - 引入被派生类**遮掩的函数**（基类方法函数即使签名不同也会被派生类的同名方法遮掩）

  - 继承基类的构造函数，提示编译器根据基类构造函数合成派生类的构造函数
    - 不能继承虚基类的构造函数
    - 不能使用变参继承构造函数

### 变量

#### 声明与定义

```cpp
int foo0;       // 默认初始化
int foo1{};     // 值初始化，初始化为 0
int foo2{args}; // 调用构造函数，且防止数据窄化
```

- 对于**内置类型**或**聚合类**，默认初始化后的值未定义，值初始化后的值为 0
- 对于自定义类型，默认初始化与值初始化相同，都相当于调用默认构造函数

```cpp
vector<int> v0{1, 2};   // 调用(initializer_list<int>)
vector<int> v1(1, 2);   // 调用(size_t size, int elem)
```

- 花括号初始化优先调用参数为 initializer_list 的构造函数，并模拟聚合类初始化且防止数据窄化
- 圆括号初始化则按正常重载函数匹配规则来调用

```cpp
auto            v0 = {1, 2};    // 初始列构造为initializer_list
vector<int>     v1 = {1, 2};    // 初始列构造为vector<int>
pair<int, int>  v3 = {1, 2};    // 初始列构造为pair<int, int>
```

- 根据上下文，initializer_list 会自动调用 non-explicit 构造函数来构造临时对象

```cpp
auto  [j, k] = f(); // 值绑定
auto& [x, y] = f(); // 引用绑定
```

- 被解构类型的所有 public 非静态数据成员必须为直接成员或相同基类的直接成员，不能绑定 union

#### 静态变量

**全局静态**变量即声明于命名空间中的变量，此处特点也适用于类的静态数据成员

- 其构造函数会在进入 main 函数前调用，可用于初始化程序
- 其析构函数会在退出 main 函数后调用，可用于程序结束处理
- 注意：多个翻译单元中全局静态变量相互之间的构造顺序并未定义

**局部静态**变量即声明于函数内部的 static 变量

```cpp
// reference-return
T& Instance() {
    static T{};
    return T;
}
```

利用 reference-return 技术代替全局静态变量，优点如下

- 延迟构造：在第一次引用时才 **线程安全** 地构造静态变量
  - 保证不使用则无性能开销
  - 保证引用其它翻译单元中的静态变量时该静态变量已构造完成
- 利用函数包裹，实现可有更多操作空间

### 表达式

- C/C++语言的表达式有逻辑严谨的规则属性
  - 每个表达式都有一个返回值
  - 每个值都有一个类型
  - 利用返回值可以将类型一样的多个表达式组合在一起
- 初等表达式
  > 任何运算符的操作数都可以是其他的表达式或初等表达式
  - 字面量
  - 标识表达式
  - lambda 表达式
  - requires 表达式
  - 折叠表达式
  - 括号中的任何表达式
- 不求值表达式
  > 顾名思义
  - typeid（不为多态泛左值）
  - sizeof（sizeof(T)要求 T::~T 可访问）
  - noexcept
  - decltype
- 弃值表达式
  > 仅用于实施其副作用的表达式
  - 标志表达式
  - 数组下标表达式
  - 类成员访问表达式
  - 间接寻址
  - 成员指针操作
  - 条件表达式（其第二、三操作数是弃值表达式）
  - 逗号表达式（其右操作数是弃值表达式）

#### 类型转换

> 不鼓励使用旧式的 C-style-cast，下面的新式转换更容易在文件中搜索，且更容易由编译器帮助检测

- `static_cast<>()` ：一般用于隐式转换规则允许的转换
- `const_cast<>()` ：一般用于取消底层 const 限定
- `reinterpret_cast<>()`：一般用于转换指针类型
- `dynamic_cast<>()` ：一般用于在继承体系中转换它们实例的指针或引用

- 有关指针类型的转换

  - nullptr_t 可以转换为任意指针类型，反之不成立
  - 任意指针类型可以转换为`void*`，反之只能显式转换
  - `const char*`与`char const*`均为底层 const；`char* const`才是顶层 const
  - 默认的 C-Style 字符串类型为`const char*`，标准提供了一个特殊转换来向后兼容：可以将 C-Style-String 转换为`char*`

#### 左值与右值

- 需要左值：赋值`=`、取地址`&`、自增减`--` `++`
- 返回左值：赋值`=`、解引用`*`、箭头`->`、下标`[]`、前置自增减`++a`与`--a`
- 可能返回左值：
  - 条件`? :`，若`:`号两边都为左值才会返回左值
  - 成员`.`，若`.`左边的对象时左值才会返回左值

#### 运算符

![cpp](images/cppman.png)

#### 求值顺序

- 有序：
  > 注意，逻辑与和逻辑或的实质就是分支，同样会有分支处罚；而条件表达式可能为条件传送，可能避免分支
  - 逻辑与
  - 逻辑或
  - 条件运算符
  - 逗号
- C++17 新规则：在下列表达式中，a 的求值和所有副作用先序于 b，同一个字母的顺序不定。
  - `a.b`
  - `a->b`
  - `a->*b`
  - `a(b1, b2, b3)`
  - `a[b]`
  - `a << b`
  - `a >> b`
  - `b @= a`
- 在一句复合表达式中，若两个子表达式无父子关系，则它们的求值顺序未定

#### 字面值

> 定义于：`inline namespace std::literals`

- `chrono::duration`：
  - 后缀：h、min、s、ms、us、ns
- 数字：
  - 前缀：`0b或0B`、`0`、`0x或0X`
  - 后缀：`U`、`L`、`UL`、`F`
  - 无后缀的整数字面值，其类型为：
    - 若为十进制，为`int` `long` `long long`中的最小者
    - 若不为十进制，为`int` `unsigned int` `long` `unsigned long` `long long` `unsigned long long`
      中的最小者
- 字符：
  - 原始字符串 ：`R"del(string)del"`
  - 前缀（C-char 或 C-string） ：`u8" UTF-8 "`、`u" UTF-16 "`、`U" UTF-32 "`、`L" wchar_t "`
  - 后缀（仅限于 C-string） ：`s`、`sv`
- [自定义](#zdyzmv)

### 语句

#### 分支

```cpp
if constexpr ( init; cond ) { // `init;`部分可忽略
    /* ... */
} else if ( init; cond ) { // `init;`部分可忽略
    /* ... */
} else {
    /* ... */
}

switch ( init; int ) { // `init;`部分可忽略
    case INT:
        /* 此处声明变量可在接下来分支中可见 */
        break;
    default:
        /* 不能初始化变量以防止跳过初始化 */
}
```

#### 循环

```cpp
do {
    /* ... */
} while ( cond )    // 不能使用循环体中声明的变量

while ( cond ) {
    /* ... */
}

for ( init; cond; iter ) {
    /* ... */
}

for( init; auto obj : container ) { // `init;`部分可忽略
    /* ... */
}
```

#### 异常

```cpp
try {
    throw exception{}; // 必定进行一次拷贝
} catch (exception& excep) { // 仅允许多态转换
    /* ... */
}

void func() try
{
    /* ... */
} catch (...) {
    /* ... */
    throw; // 只能出现在catch语句中
}
```

### 函数

#### 参数修饰

| src\dest | `T`  | `const T&` | `T&&` | `T&` |
| -------- | ---- | ---------- | ----- | ---- |
| rvalue   | Move | Ref        | Ref   | X    |
| lvalue   | Copy | Ref        | X     | Ref  |

> 注意：
>
> 1. `T&&` 作为函数返回类型时被视作右值，作为变量声明类型时被视作左值
> 2. `T&&` 并不会延长右值的生命周期，右值仍然会在语句结束时被析构，所以几乎任何时候都不应该**声明右值引用变量**或**返回右值引用类型**

**形参列表修饰**：

- 只读：
  - `const T&`
  - `T`（内置类型/或需要拷贝而避免重载 `T&&`）
- 可写：
  - `T&`（考虑重载 `T&&`）
- 泛型：
  - `template T&&`
  - `template T&&...`
  - 注意使用`std::decay_t<T>`与`std::forward<T>(t)`

**函数签名修饰**：

- `noexcept`：当函数抛出异常是不可能或不可接受的时候
- `inline`：当在头文件中定义函数时应该声明为内联

#### 关键字

- noexcept：<span id="noexcept"></span>

  - 用法：

    - `noexcept`
    - `noexcept(bool)`
    - `noexcept(func)`
      > 只要 func 声明了 noexcept，或没有调用 non-noexcept 函数且无 throw 语句则为 ture
      > 一般嵌套再第二种用法中`noexcept(noexcept(func))`

  - 限定了**函数指针**与[**虚函数**](#xhu)对 noexcept 属性的传递：
    non-noexcept 可接受 noexcept，反之则不行

  - **合成的**特种成员函数会有合成的 noexcept 声明，合成的声明即相当于添加了`noexcept(noexcept(func))`。
    自定义的[析构函数](#xghu)若不显式指定也会合成 noexcept 声明，

  - 提示编译器不抛出异常以进行优化，若最终还是抛出了异常则直接终止程序

- [inline](#inline语义)

- constexpr：<span id="constexprhu"></span>

  - 隐式 inline
  - 提示编译器：若调用实参是 constexpr 对象（如果有参数的话），**且将返回值传递给一个 constexpr 对象**，
    则此函数**应该**进行编译期计算
  - 使用限制：<u>不太精准但实用的简化版</u>
    - 参数与返回值：必须是字面值类型（如果有的话）
    - 定义的变量：字面值类型，且不能有 static、thread_local、未初始化变量
    - 调用的函数：constexpr 函数

- =delete：

  - 删除合成的特种成员：default 构造、析构、copy 操作、move 操作
  - 删除成员函数或非成员函数，拒绝从某一类型进行类型转换而调用该函数
  - 删除一个模板实例，拒绝实例化出不希望的模板实例（也可使用`enable_if<>`）
    > `template<> void func(bool) = delete`相当于声明一个特化版本并将其删除

- auto：

  > 此时的 auto 就像是无名的模板参数，而要获取其类型时需要对实参调用`decltype(t)`

  - 返回类型推断：
    > `auto func()`
    - 所有 return 语句的返回类型均要相同（可以使用`if-constexpr`绕过）
    - 递归调用之前必须有 return 以确定返回类型
  - 实参类型推断：
    > `void func(auto&& t)`
    - 调用时也可以指定 auto 的类型`func<int>(1)`

- return：

  - **return 的本质就是将返回值赋值给调用者栈帧中的一个（临时）变量**
  - 对于 RVO 情况的按值返回：若被调用方返回局部对象，且其类型与返回值类型相同，则直接在调用方栈帧中构造该对象
  - 对于非 RVO 情况的按值返回：对于右值调用 move 操作，对于左值调用 copy 操作
  - 在此谈谈按值传参的情况：
    - 形参永远都是左值
    - 实参可能是左值或右值，这也决定了由实参构造形参是 copy 还是 move

- operator：

  - 重载运算符
    > `double operator()(int);`
  - 自定义字面值：<span id="zdyzmv"></span>

    > `double operator""_i2d(unsigned long long t);`

    - 重载符名称只能以下划线`_`开头
    - 形参限制：

      | 参数列表                | 对应字面值         |
      | ----------------------- | ------------------ |
      | `(unsigned long long)`  | 整数`250_i`        |
      | `(long double)`         | 浮点数`2.5_d`      |
      | `(const char)`          | 字符`'2'_c`        |
      | `(const char*, size_t)` | 字符串`"string"_s` |

#### 默认实参

- 若某一形参有默认实参，则其后的形参也必须有
  > 故大概率不会用到默认实参的形参放前边
- 若声明时已指出默认实参，则定义时不能重复指出
- 虚函数的默认实参由静态类型决定

#### 重载函数

- 不允许定义形参列表相同的同名函数，即使返回类型不同

- 使用[using 声明](#using声明)可以引入所有重载函数

<!-- need reread: 函数匹配 -->

- 类型转换级别(优先级降序)

  1. 精准匹配、数组与函数蜕化、顶层 const 转 non-const
  2. 非底层 const 转底层 const
  3. 整型提升
  4. 其他内置类型转换
  5. 与类有关的类型转换

- 函数查找

  - 常规作用域查找：从当前作用域向前、向外查找，最终到全局命名空间起始处结束
    > 注：若形参为内置类型，则只会常规作用域查找，因为内置类型未定义于任何命名空间中
  - 实参依赖查找(ADL 或 Koenig)：根据实参的类型还会到到其所在的命名空间中查找匹配的函数
    > 注：显示指定模板参数的模板函数的 ADL 在 C++20 中才引入

- 函数匹配
  - 先进行函数查找，找出所有（实参转换后）形参合适的函数或推断成功的函数模板（候选集）
  - 然后计算出每个函数（模板）的每个参数进行优先级（具有默认实参的参数位可以不必计算）
  - 对候选集中成员的优先级进行字典序比较，选出优先级最高的（同等优先级下非模板优先）

#### Lambda 闭包

- 形式：`[cap]<temp>(arg){sta}`
  > C++17 支持 constexpr 修饰
- 捕获列表：
  - 捕获类型：非 static 的值或引用（static 值可直接使用）
  - 显式捕获：
    > 非本地使用的 lambda 应该使用值捕获（如 return lambda）；
    > 本地使用的 lambda 应该使用引用捕获（避免拷贝）
    - `[x, &y]`：按值捕获 x，按引用捕获 y
    - `[this]`：捕获 this，从而可以使用其成员且可省略`this->`
  - 隐式捕获：
    > 应该避免使用隐式捕获已丧失可读性
    - `[=]`：按值进行隐式捕获
    - `[=, &x]`：按引用且只能按引用捕获 x，其它进行隐式值捕获
    - `[&]`：按引用进行隐式捕获
    - `[&, x]`：按值且只能按值捕获 x，其它进行隐式值捕获
  - 初始化捕获：
    - `[a_m = std::move(x)]`：a_m 为该 lambda 对象的数据成员，x 为外部对象
- 完整的泛型模板支持
- 参数列表修饰：
  - 可以使用 auto 推断参数类型，且其返回类型默认就使用 auto 推断
  - `[](auto&& t) mutable {}`：修饰`mutable`可以在 lambda 中修改值捕获的对象（默认值捕获对象只读）

#### 协程

想要理解协程，得先知道为什么需要协程，而这就要求了解异步编程与回调函数。

C++20 无栈协程：

**原理**

- 协程有两个栈帧用于存储状态：
  - 一个是分配在栈上的普通栈帧，在 恢复/暂停 协程时就像调用普通函数一样 创建/销毁 栈帧。
    若编译器判断一些局部变量的生命周期不超过协程的一次连续执行周期，则将其分配在该栈帧上。
    在协程启动时会将“普通栈帧”上的函数参数 copy/move 到协程栈帧上。
    > 当协程调用普通函数时，就会下移栈指针来扩充栈，在普通函数调用过程中，协程无法返回，
    > 因为普通函数的栈帧在协程普通栈帧下面，若每次协程返回销毁普通栈帧则就会连同普通函数栈帧一起销毁
  - 另一个是分配在堆上协程栈帧，该栈帧保存了伴随整个协程生命周期的数据以及协程 Promise 对象。

**特点**

- 协程的返回类型`Feature`需要定义一个成员类型`promise_type`来 **_定义协程的行为_**，其成员包括

  - `Feature get_return_object()` ：协程第一次返回时的返回值
  - `Awaitable yield_value(exp)` ：`co_yield exp`相当于`co_awiat Promise.yield_value(exp)`在暂停时返回值给 caller
  - `Value return_value(value)` ：`co_return non_void_exp`在协程结束返回时调用
  - `void return_void()` ：以下情形调用该函数 1.`co_return;`2.`co_return void_exp`3.控制流出返回 void 协程结尾
  - `Awaitable initial_suspend()` ：在协程开始处插入该段代码
  - `Awaitable final_suspend()` ：在协程结尾出插入该段代码

- 协程第一次返回类型`Feature`一般具有成员对象`coroutine_handle<promise_type>`用来 **_恢复协程_**，其成员包括：

  - `resume`或`operator()`：恢复协程
  - `done` ：是否处于 final_suspend 阶段
  - `destroy` ：销毁 Promise 对象
  - `promise` ：返回 Promise 对象引用
  - `static coroutine_handle from_promise(Promise)`：从 Promise 对象返回其 coroutine_handle

- 语句`co_await Awaitable;`即产生一个暂停点 **_等待事件完成后返回结果_**，协程暂停的行为依赖 Awiatable 的定义，其成员包括：
  - `bool awiat_ready()` ：若返回 true 则不暂停直接执行下条协程语句
  - `? await_suspend(coroutine_handle<>)` ：根据返回值类型不同而具有不同行为
    - 若返回 void 则直接暂停挂起
    - 若返回 bool 为 true 则直接暂停挂起
    - 若返回`coroutine_handle<>`则暂停挂起并调用其`resume`恢复那个协程
  - `T await_resume()` ：协程恢复时`co_awiat`语句的运算结果

### 特殊结构类

<!-- need reread -->

#### 字面值类

- 数据成员为字面值类型的[聚合类](#聚合类)
- 若非聚合类，则：
  - 数据成员均为字面值类型
  - 类内初始值为 constexpr（如果有的话）
  - 至少有一个 constexpr 构造函数
  - 使用默认析构函数

<!-- need reread -->

#### 聚合类

- 定义：
  - 数组
  - 否则：
    - 所有成员与基类都为 public
    - 无 virtual 函数
    - 未定义任何构造函数
    - 注：C++14 之前要求不能有类内初始值
- 聚合初始化： <span id="jhcsh"></span>
  - 默认提供一个特殊构造函数：用花括号按顺序逐个构造**基类**和**数据成员**。
    但是注意，默认构造内置类型的成员时其值未定义，而非使用值初始化
    > `int arr[]{1, 2};`
  - C++20 支持圆括号的聚合初始化从而允许窄化
    > `int arr[](1.0, 2L);`
  - C++20 支持在花括号中使用指示符以初始化确定的成员，但仍然需要保持顺序
    > `pair p{.first = 1, .second{2}}`

#### 枚举类

- 实质：
  - 该类的成员为 constexpr 整数
  - 该类的对象是这些成员中的一个

> `enum Num {ONE, TWO, THREE};`  
> `enum Num {ONE, TWO, THREE};`  
> `enum Num: int {ONE, TWO, THREE};`

- 成员名字对外可见
- 默认类型由编译器推断
- 可以赋值给整型变量
- 不可由整数赋值
- 无名枚举：无对象，可直接使用成员

> `enum class Num {one, two, three};`  
> `enum class Num {one, two, three};`  
> `enum class Num: int {one, two, three};`

- 成员名字对外不可见，需要用`::`运算符访问
- 默认类型为 int
- 不可赋值给整型变量
- 不可由整数赋值

#### 联合类

**_C++17 中用 std::variant<TYPES...>可以完全取代手写 union_**

- 实质：用同一块内存存储几种不同类型中的一种，在某时刻存储的类型由用户保证
- 不能是继承体系中的一员，故不能有虚函数
- 匿名 union：
  > 编译器自动创建一个无名对象，其成员可直接访问
- 若含有的成员类型有自己的默认构造函数和特种成员则合成的构造和拷贝为 delete
  > 原因：联合中存储的值的类型编译时未知，编译器无法调用正确的析构函数来销毁原值
  > 用匿名 union 作用数据成员
  > 用匿名 union 作用数据成员
  > 用普通枚举对象作判别式追踪 union 的状态
- 使用定位 new：用 union 中类成员的地址作定位

#### 嵌套类

- 作为外层类的一个类型成员
- 外层类和嵌套类相互独立，互相之间并没有特殊访问权限，只是外层类成员在嵌套类中可见

## 面向对象

### 封装

#### 声明与定义

- 类的声明：

  - 声明之后，即可使用该类的指针与引用，并可做函数参数与返回值的类型

- 类的定义：

  - 定义之后，即可定义该类的具体对象，并可访问该对象的成员

- 成员定义：
  - 定义在类内的成员函数隐式 inline
  - 定义在类外时，在名字`classname::member`之后进入类的作用域

#### 友元

**友元关系无法传递与继承，但基类的友元可以访问其派生类的基类部分**

- 友元函数 ：若无前置声明，此处即为声明
- 友元类 ：若无前置声明，此处即为声明
- 友元成员函数 ：需要先提供类的前置定义
- 友元模板：

  - 某一实例：需要前置声明模板，并显示指定模板参数<span id="mbyy"></span>

    > `friend func<int>(Test<int> t);`
    > 注意，可声明全特化，但不可声明偏特化

  - 整个模板：若无前置声明，此处即为声明

    > `template <typename T> friend func(T t);`

  - 模板参数：
    > `friend T;`

#### 访问控制

- public：struct 声明默认，所有用户均可访问
- private：class 声明默认，仅类成员与友元可访问
- protected：仅类成员与友元以及派生类成员可访问

#### 特种成员

- 构造函数 <span id="gzhu"></span> > `classname(arglist): initlist {funcbody}`

  - 实现默认构造：
    - =default
    - 无参构造
    - 默认实参
    - 委托构造
      > 注意构造函数会在构造对象时自动调用，
      > 显示调用构造函数的意思是构造并返回一个对象，
      > 在构造函数内部调用其他构造函数以企图初始化数据成员的行为并不正确，
      > 如此请使用委托构造函数
  - explicit：
    - 单参构造可视作隐式类型转换(from)，使用 explicit 拒绝隐式转换
    - 多参数构造也可使用 explicit 用于拒绝[初始列](#initlist)隐式转换
  - constexpr：
    - 必须初始化所有数据成员（未涉及原本会由编译器推断）
    - 该类无虚基类
    - 其它见[constexpr 函数](#constexprhu)
  - 自定义构造方式：
    - 就地构造
    - 类聚合式构造
    - 逐块式构造
    - 成员模板构造
    - 其他

  >

- copy 语义

  - copy 构造：
    > `ClassName(const ClassName&): initlist {funcbody}`
  - copy 自赋值：
    - 先拷贝新值
    - 再销毁旧值
    - 赋值

- move 语义

  - move 构造： > `ClassName(ClassName&&): initlist {funcbody}`
  - 若不为`noexcept`，则标准库容器不会使用移动构造函数
  - move 自赋值：
  - 直接拒绝自赋值

  >

- 析构函数 <span id="xghu"></span> > `~classname() {}`
  - 逆序销毁
  - 会由编译器无条件地合成[noexcept 声明](#noexcept) \* 析构函数若为=delete，则该类对象只能通过动态分配创建该对象，且不能销毁
    >

**合成的特种成员何时 deleted**

- 析构函数：
  - 有成员无法析构
- 默认构造：
  - 定义了其他构造函数（包括 copy 构造与 move 构造）
  - 有成员无法默认构造
  - 有成员无法析构
  - 有 const 或引用成员且无类内初始值
- 拷贝构造：
  - 有成员无法拷贝构造
  - 有成员无法析构
  - 定义了移动操作
- 拷贝赋值
  - 有成员无法拷贝赋值
  - 有 const 或引用成员
  - 定义了移动操作
- 移动构造
  - 有成员无法移动构造
  - 有成员无法析构
  - 定义了析构函数、拷贝操作、移动赋值
- 移动赋值
  - 有成员无法移动赋值
  - 有 const 或引用成员
  - 定义了析构函数、拷贝操作、移动构造

**一言蔽之，若重定义了 copy, move, destructor 中的一个，其它所有特种成员也得重新定义**

#### 其他成员

- 类型成员
  _ [using 类型别名](#using用法)
  _ [using 模板类型别名](#using用法) \* [嵌套类](#嵌套类)

  >

- 数据成员

  - 构造顺序与声明顺序相同
  - 类内初始值：只用于编译器推断默认构造函数，一般只用于聚合类
  - 若有 const 与引用成员则[无默认构造与赋值操作](#合成的特种成员何时deleted)
  - mutable：即使该类对象为 const 也允许修改该数据成员
  - static：<span id="static"></span>
    - 可以使用不完整类型，但定义时需提供完整定义
    - const：若定义在类内则也应该在类外声明以形成实体（链接符号），non-const 的只能定义在类外
    - [inline](#inline语义)：在类内提供定义即可形成实体，利用 inline 语义避免符号多重定义
    - constexpr：隐式 inline

  >

- 成员函数

  - 限定 this：`const`、`&`或`&&`
  - 修饰符的放置顺序：`const && noexcept final override =0`
  - 定义在类内隐式`inline`
  - static：不会隐式传递`this`，故无法使用 non-static 成员

  >

- 三路比较
  > `auto operator<=>(const Type& obj) const =default`；不会自动合成需要手动指定
  - 返回值`>0`表示`>`，`==0`表示`==`，`<0`表示`<`，编译器据此来合成 6 个比较操作
  - 默认依字典序比较类实例的每个基类（从左到右，深度优先）和所有非静态数据成员（按声明顺序）
    > 内置数组成员算作聚合类，也具有三路比较运算符
  - 因为性能问题，你可能提供`operator==()`来完成整个比较操作的自动化生成
    > 旧式的 6 个比较运算符优先级高于三路比较
  - 返回类型 auto 可以显示指定为以下三者之一：
    - std::strong_ordering ：等价的值表示完全相同，如`int`
      > 成员常量：less、equivalent、greater
    - std::weak_ordering ：等价的值也不一定完全相同，如`double`
      > 成员常量：less、equivalent、greater
    - std::partial_ordering ：允许忽略无法比较的成员，如包含`NaN`的`double`
      > 成员常量：less、equivalent、greater、unordered

<!-- need reread: 类型转换 -->

- 类型转换

  - `explicit operator bool()` > 在明确需要 bool 值的地方仍会隐式转换 bool 值
  - `explicit operator Type() { return Type{}; }` > 此形式一般在无法修改转换目标（即 Type）的源码时才使用，即一般都使用第一种形式设计类型转换
  - 二义性问题：A 转换为确定的类型 B，应该调用哪个转换函数？ > 本质上就是重载函数的匹配问题，但是注意类型转换是不具传递性的，即 A 转 B，B 转 C，但 A 不能转 C。
  - 存在一个特殊巧合：C 具有一个单参构造函数用于 B 转 C，`C(const B&);`；
    此时可由 A 构造 C，`C c{A{}};`，因为该初始化语句会调用前面那个单参构造函数，而 A 可通过类型转换为 B 而匹配该函数。
    故此看似进行了类型转换的传递，实则不然。
  - 还有一个特殊存在，类转换为算术类型时会进行真正的传递性转换。A 转 int，int 转 double，A 可直接转 double。
    这是语法层面的规则，而非语法 trick。如果存在多个类 A 存在多个转向算术类型的转换函数，会有一个稍微复杂点的机制进行函数匹配。
    在此不做细说，因为这种做法是 deprecated。只需知道，转向算术类型的转换函数除了 bool 外最多再定义一个，且声明为 explicit。
    让编译器帮助进行传递性转换可更加安全。

  >

- 重载运算符
  - 除了函数调用运算符，其他不能有默认实参
    >

### 继承

- EBO（空白基类优化）：
  若基类部分无数据成员，且派生类第一个数据成员类型不是基类类型，则基类部分不分配空间
  正常情况 C++对于对象的空间分配，至少会分配 1Byte 以区分对象地址，即使类中无数据成员

- 类的 static 成员，对整个程序都唯一，不管它存在于哪个继承体系中

- 声明不能包含基类列表

- 对基类部分的构造函数，可将基类部分看做派生类的数据成员，成员名字即基类类名

- 构造函数、析构函数中调用虚函数，为静态类型版本

#### 作用域

- 派生部分作用域内嵌于基类部分

- 通过基类的引用或指针调用成员，会根据静态类型进入作用域，并开始逐层向外进行名字查找，
  如果查找到的是虚函数，编译器将其绑定到虚指针上，运行时进行动态绑定

- 作用域名称遮掩规则与正常的有些不同，内层（派生类）作用域中的函数会掩盖外层（基类）的所有同名函数，即使它们的函数签名不同，可通过[using 继承](#usingjc)或转换函数解决
  <a href=## title="目的是为了防止不小心继承了疏远的基类的函数">[注]</a>

<!-- need reread -->

- 若基类是未实例化的类模板，则编译器默认不会进入外层（基类）作用域查找符号，
  因为类模板的具体定义需要实例化是才知晓，而其中可能没有你试图调用的符号，
  此时便需要如此显式调用`this->temp_base_func()`

- 访问基类部分需要通过基类接口
  - 派生类：`member`或`base::member`
  - 用户：`derive.member`或`derive.base::member`

#### 基类访问控制

- 基类列表：

  - virtual
  - public
  - protected
  - private

- 派生类到基类的转换，需要调用者有访问该派生类的基类部分的权限，即派生类（派生访问说明符）允许访问，即派生类（派生访问说明符）允许访问，且基类（成员访问说明符）也允许

#### 多重继承

- 构造顺序：
  - 由上至下（从基类到派生类）
  - 从左到右（基类列表中的顺序，虚基类优先）
- 名字查找在直接基类中同时进行，若有重名则需要使用作用域运算符，为了避免该潜在二义性：
  - 使用[using 继承](#usingjc)
  - 或者设计自己的 wrapper，在其中通过`::`作用域运算符进行访问
- 若转换存在多条路径，也会产生二义性错误
  >

### 多态

#### 实现原理

- 只要类中存在虚函数，则就会引入一个隐式数据成员——虚指针
- 虚指针指向一个虚表，每个基类与派生类的虚表都不同，虚表存储类类型信息，以及虚函数指针
- 虚表的合并：子类先复制父类虚表，再在其中各个位置覆盖信息，若子类有新定义的虚函数，则添加在后面

#### 虚函数

- 必要条件：

  - 非 static 非模板的非构造函数
  - 必须提供定义

- 关键字修饰：
  - virtual&emsp; ：声明为虚函数
  - final&emsp;&emsp;：拒绝函数被覆盖（拒绝类被继承）
  - override ：标记为覆盖虚函数，若并未覆盖则报错
  - = 0&emsp;&emsp;&emsp;：声明为纯虚函数，可以不用提供定义(若要定义则定义在类外)，派生类必须覆盖
- 抽象基类：含纯虚函数，不能定义对象

- 默认实参：调用时由静态类型决定（静态绑定）

- 派生类中试图覆盖基类中同名的虚函数，其参数列表和返回类型等各种属性都要相同，否则会隐藏虚函数

  > 例外：基类中返回基类的指针或引用，派生类可以返回派生类的指针或引用，前提派生类与基类之间的转换可访问

- 动态绑定 <span id="dtbd"></span>：当通过基类的指针或引用调用虚函数时，会判断该指针或引用的动态类型，
  并调用其动态类型中设计的的虚函数。但构造函数与析构函数中调用的虚函数不会发生动态绑定，即调用静态类型的虚函数版本

#### 运行时类型识别

- `dynamic_cast<>()`：
  - 转换为目标类型的指针、引用
- `typeid()`：
  _ 接受目标类型的对象返回 type_info 的 const 引用
  _ type_info 提供成员`.name()`
  >

### 设计

- OOP 与 Template 之间“多态”的区别
  - OOP 通过继承体系的运行时转换(动态多态)
    - 需要设计 virtual 函数并 override
    - 可以调用已编译好的库
  - Template 通过泛型的编译时实例化(静态多态)
    - 需要设计类的成员供 Template 调用
    - 需要重新编译源文件(因为 template 时根据已有的调用实例由编译器自动生成的，而在设计它之前是没有这个实例的)

> 类设计规则

- 取消友元
- 数据成员
  - private
  - pImpl
  - 结构对齐
  - const & 引用
- 构造函数
  - default?
  - explicit?
  - non-inline
  - never-call-virtual
- 析构
  - virutal & definition
  - noexcept & .destroy()
  - non-inline
  - never-call-virtual
- copy? & move?
- operator

  - 单成
  - 算赋
  - 前后
  - explicit bool 1

- 类间关系

  - **is-a**：public 继承
    - 抽象分化
    - 混合类
  - **has-a**：复合
  - **impl-of**：复合或 private 继承
    - virtual
    - protect
    - EBO

- 虚函数接口
  - pure virtual ：无默认定义
  - non-pure virtual ：提供默认定义
  - non-virtual ：提供强制定义

## 泛型编程

### 模板参数

- 模板参数作用域中，不能重用模板参数名

- 用 typename 指出目标模板的成员为类型，避免与静态数据成员相混淆 <span id="typename"></span>

  > 如`T::A* b;`，可能是声明类型为 T::A\*的对象 b，也可能是 static 变量 T::A 乘以 b。

- 类型参数：
  - 普通类型
    > `template<typename T>`
  - 模板类型
    > `template<template<typename T> class C>`
- 非类型参数：必须常量表达式
  - 显示指定
    > `template<int INT>`
  - auto 推断
    > `template<auto INT>`
- 默认模板参数
  - 类型与非类型都可以有默认模板参数
  - 自动实施于偏特化版本

### 变参

> `template <typename... Types, auto... VARS>`

- `sizeof...()`获取参数包中参数数量

<!-- need reread -->

- 模板类型参数解包
  - 类型修饰
    > `(T&&... args)`与`(C<T...> v)`
  - 基类列表 <span id="bcpu"></span>
    > `template <typename... T> class Deriv: T...;`
  - [using 继承](#usingjc)
    > `using T::operator()...;`
- 函数参数解包（也适用于非类型参数）
  - 函数调用
    > 例如`func(arg1, args...);`与`func(forward<Type>(args)...)`
    > 注意参数包可以为空
  - 左右折叠表达式
    - 一元折叠：`((cout << E << endl), ...)`
      > 表示对 E 包中每个元素调用逗号左边表达式
    - 二元右折叠：`(E OP ... OP init)`
      > 表示`E1 OP (... OP (EN-1 OP (EN OP init)))`
    - 二元左折叠：`(init OP ... OP E)`
      > 表示意味着 `(((init OP E1) OP E2) OP ...) OP E2`

### 变量模板

> `template <typename T> obj = get_val<T>()`

- 根据模板参数(类型或非类型)来方便地生成变量
- 可以用于[元编程技术](#元编程)来编译期获取值
- 不能作为模板模板参数(一种非类型参数)
  >

### 函数模板

> `template <typename T> int test(vector<T> t);` > `test(vector<int>{})` > `test(vector<int>{})`
> 注意只会从显式实参推断；
> 注意不会由默认实参推断模板类型参数；
> 注意不会推断返回类型，此时应使用 auto 返回；

- 显式指定模板参数：

  > `test<int>(vector<int>{})`

  - 指出无法由实参推断出的模板参数
  - 用于声明[模板友元](#mbyy)时指定其某一实例为友元
  - 用于模板元编程的分支、循环（递归）

- 函数模板的偏序规则：用于判断重载的模板函数中哪个更特例化 <span id="px"></span>

  ```cpp
  template<typename T>
  T partial_order_func(T t); // #1

  template<typename T>
  T partial_order_func(T* t); // #2

  template<typename T>
  T partial_order_func(const T* t); // #3
  ```

  &emsp;规则就是：将模板参数`T`设为类型`X`，则`#1`就是`X`，`#2`是`X*`，`#3`是`const X*`。
  针对`#1`与`#2`，将后者的类型`X*`带入前者的模板，可以成功推断(忽略类型转换)出`T`为`X*`；
  如此单方面的推断，则表明`#1`的模板参数比`#2`更加泛型，即`#2`比`#1`更加特例化
  如此单方面的推断，则表明`#1`的模板参数比`#2`更加泛型，即`#2`比`#1`更加特例化
  &emsp;**常见的有：**

  - `T*`比`T`更特例化
  - `const T*`比`T*`更特例化
  - `(T1 t1, T1 t2)`比`(T1 t1, T2 t2)`更特例化

### 类模板

- 类外使用类模板名字需要用`template<typename T> ... Myclass<T>:: ...`

  > 若定义的是成员模板，则需要两个 template，一个是类模板的，一个是成员模板的，初学时会感到奇怪

- 在类模板作用域中使用类名可忽略模板实参

- 显示指定模板参数：

  > `pair<int, double> p{1, 1.0};`

- 自动推断模板参数：
  > `pair p{1, 1.0};`
  - 根据构造函数生成**自动推断向导**以推断模板实参
    > `classname(T1) -> classname<T2::type>;` ，或
    > `template <typename... T> classname(T...) -> classname<T...>;` > `template <typename... T> classname(T...) -> classname<T...>;`

<!-- need rereead -->

### 概念

- requires 表达式

  > `requires (可选的形参列表用于使用目标类型的对象，如t) {要求序列;}`

  - 简单要求：要求表达式能够通过编译
    > `t.mem()`
  - 类型要求：要求目标类型合法
    > `typename T::type`
  - 复合要求：表达式结果作为类型约束的最后一个参数
    > `{简单约束的表达式} (可选)noexcept -> 类型约束`
  - 嵌套要求：相当于使用约束子句进行静态断言
    > `requires子句`

- requires 子句：后随常量表达式（限制包括初等表达式、&&、||）

  > requires (概念 && requires 表达式 || 布尔常量)

- 概念：一般定义为**requires 表达式模板**

  > `template<typename T> concept Concept = Boolean`

- 概念的使用
  - `template<Concept T>`
    > T 作 Concept 的第一个模板参数
  - `template <typename T> requires (Concept<T> && sizeof(T) == 4)`
    > requires 子句需要初等表达式（括号内的表达式可强制算作初等表达式）与`&&`、`||`联用
  - `if ( Concept<int> )`

### 模板特例化

- 全特化：

  > `template<> Class<int, int*> {...};`

  > `template<typename T> class Class<int, T*> {...};` > `template<typename T> class Class<int, T*> {...};` > `tempalte<typename C, typename... Args> class Class<C(Args..)> {...}`

  - 只适用于类模板
  - 偏特化模板参数列表是原来的子集或特例化版本

- 作用：
  - 为特殊类型的模板提供特殊的定义
  - 元编程的分支语句

### 其它

- 控制实例化<span id="kzslh"></span>
  - `extern template vector<int>`
  - `template vector<int>`
  - 避免因为声明和定义不能分离导致多个文件重复模板实例
  - 编译器遇到模板的使用就会进行实例，故该声明应该在使用之前
  - 实例化定义会实例化类模板的所有成员，而非只实例化使用的

>

- 转发与移动

```cpp
// 转发的目的即是移动
template <typename T> void wrapper(T&& t) {
  real(forward<T>(t)); // 当 T 为 `t` `t&&`类型时，转发为右值
}
```

### 编译器类型推断

> 注意可能发生推断二义性
> 注意`auto t = {1}`会推断`auto`为`initializer_list<int>`

- 值推断：类似`auto`或`T`

  - 将**引用**视作**对象**
  - 忽略 const 与 volatile（但是保留顶层 const 指针）
  - 数组与函数退化为指针
  - [类型抵消](#lxdx)

- 引用推断：类似`auto&`或`T&`

  - 将**对象**视作**引用**
  - 保留 const 与 volatile
  - 保留数组与函数
  - [类型抵消](#lxdx)

  | 实参                  | 形参       | 推断类型    |
  | --------------------- | ---------- | ----------- |
  | `t`或`t&`             | `T&`       | T = t       |
  | `t`或`t&`             | `const T&` | T = t       |
  | `const t`或`const t&` | `T&`       | T = const t |
  | `const t`或`const t&` | `const T&` | T = t       |
  | `std::move(t)`        | `T&`       | T = t       |

- 万能引用推断
  _ 将左值对象视作引用，右值对象视作对象
  _ 保留 const 与 volatile
  _ 保留数组与函数
  _ [类型抵消](#lxdx)与[引用折叠](#yyvd)

      | 实参                  | 形参  | 推断类型     |
      | --------------------- | ----- | ------------ |
      | `t`或`t&`             | `T&&` | T = t&       |
      | `const t`或`const t&` | `T&&` | T = const t& |
      | `std::move(t)`        | `T&&` | T = t        |

  >

- decltype 型推断：
  - 保留 const 与 volatile
  - 保留数组与函数
  - 保留引用类型
  - 对于[返回左值的表达式](#左值与右值)会推断为引用类型
  - 双层括号显式指定为引用
    > 作用：
    >
    > - 尾式返回类型`auto func(auto t) -> decltype(t)`
    > - 与 auto 合用`decltype(auto) func()`
- 类型抵消<span id="lxdx"></span>
  - 广义上的修饰符：`[]`、`()`、`<>`、`_`、`&`、`const`、`volatile`
  - 广义上的修饰符：`[]`、`()`、`<>`、`\*`、`&`、`const`、`volatile` 工作原理即是：
    > 拿万能引用第二条举例，**修饰的模板参数类型**为 T&&，**假想推断出的实参类型**为 t&
    >
    > 1. （去重）先将两者都有的修饰符抵消掉，`T&&`与`t&`无重叠部分
    > 2. （减多）再忽略前者中比后者中多余的修饰符，即忽略`T&&`中的`&&`
    > 3. （结果）此时前者必定只剩下了`T`，而后者为`t&`，即推断得`T = t&`
    > 4. （合并）将第 2 条中忽略的部分补上，则完整的形参实例即是`t& &&`，进行引用折叠后得`t&`
- 引用折叠 <span id="yyvd"></span>
  - `t& &&` => `t&`
  - `t& &` => `t&`
  - `t&& &` => `t&`
  - `t&& &&`=> `t&&`

### 元编程

元编程：在编译期，利用 **类型推导**与 **类型匹配** 以在编译期对类型进行限定、计算、选择；
数值的编译期计算倒是其次

元函数的调用形式：

```cpp
// 获取类型
MetaFunc<T, N>::type    // 利用using类型别名和using模板类型别名
MetaFunc_t<T, N>        // 利用`using模板类型别名`包装`::type`

// 获取值
MetaFunc<T, N>::value   // 利用static const数据成员
MetaFunc<T, N>          // 直接利用`变量模板`取代`::value`，但无法利用static_assert进行编译期判断
```

- 循环：
  - 变参递归，设计参数个数固定的特化模板结束递归
  - 非变参递归，设计最终结束状态的特化模板结束递归
- 分支：
  - 利用特例化的偏序规则进行分支选择
  - 利用`if constexpr (CONSTEXPR_INT)`来实现编译期分支，作用可看作预处理指令`#if CONDITION`，
    只不过前者如果出现在模板之外，仍然会对被丢弃的语句进行语法检测
- 顺序：
  - using 类型别名的顺序

## 其他特性

### 内存分配

- `new`、`new[]`、`delete`、`delete[]`
- 定位 new：
  - `new (ptr) Type{}`
  - `new (std::nothrow) Type{}`
- new 表达式：
  1. 分配内存
  2. 构造对象
  3. 返回指针
- delete 表达式：
  1. 销毁对象
  2. 释放内存
- 重载 new 与 delete 操作符，重载为类成员隐式 static
  - `void* operator new(size_t)`
  - `void* operator new(size_t，nothrow_t&) noexcept`
  - `void* operator new(size_t，align_val_t)`
- 数据对齐：
  - alignas(32)
  - alignof(obj)

### 成员指针

```cpp
struct S {
  void Foo();
  int bar_;
};

int main() {
  S s{1};
  S* pS{&s};

  void (S::* mp2f)() = &S::Foo;
  int S::* mp2v = &S::bar_;

  (s.*mp2f)();
  sP->*mp2v;
```

### volatile

- 告诉编译器不要进行存取优化，程序始终从内存中取变量的地址，而不是从高速缓存或寄存器中取变量的值

### 属性

- `[[deprecated]]`或`[[deprecated(解释弃用的理由并/或提议代替用实体的文本)]]`

- `[[fallthrough]]`

  > 仅可应用到 switch 语句中的空语句以创建直落语句： `[[fallthrough]];`

- `[[likely]]`与`[[unlikely]]`

  > 用于提醒编译器 switch 与 if-else 语句中更可能执行的分支语句

- `[[nodiscard]]`或`[[nodiscard(解释结果不应被舍弃的理由的文本)]]`

  > 若从并非转型到 void 的弃值表达式中，

  - 调用声明为 nodiscard 的函数，或
  - 调用按值返回声明为 nodiscard 的枚举或类的函数，或
  - 以显式类型转换或 static_cast 形式调用声明为 nodiscard 的构造函数，或
  - 以显式类型转换或 static_cast 形式构造声明为 nodiscard 的枚举或类的对象，

- `[[maybe_unused]]`
  > 抑制针对未使用实体的警告

### 基本概念

- 关键字、标识符、注释、预处理器宏
- 实体：实体包括值、对象、引用、结构化绑定、函数、枚举项、类型、类成员、模板、模板特化、形参包和命名空间。
- 声明与定义：声明可以引入实体，将它们与名字关联起来，并定义其属性。能够定义使用一个实体所需的所有属性的声明是定义。
- 变量：被声明的且不是非静态数据成员的对象和引用是变量。（主要实体）
- 函数：函数的定义通常包括一系列的语句，其中部分会包含表达式。（主要实体）
- 语句与表达式：表达式指定了程序需要进行的运算。（程序核心）

### 内存顺序

- memory_order_relaxed: 无内存屏障
- memory_order_acquire: 读内存屏障，即在此之后的指令不会重排到之前
- memory_order_release: 写内存屏障，即在此之前的指令不会重排到之后
- memory_order_acq_rel: 读写内存屏障（用于 RMW 原子操作），双向保证指令顺序
- memory_order_seq_cst: memory_order_acq_rel 基础上额外保证所有线程可见性（冲刷缓存数据到所有核）
