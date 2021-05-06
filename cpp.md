# 目录
<!-- vim-markdown-toc GFM -->

- [基于C](#基于c)
  - [预处理](#预处理)
  - [属性](#属性)
  - [命名空间](#命名空间)
  - [静态变量](#静态变量)
    - [local static](#local-static)
    - [non-local static](#non-local-static)
    - [inline语义](#inline语义)
  - [函数](#函数)
    - [关键字](#关键字)
    - [默认实参](#默认实参)
    - [重载函数](#重载函数)
    - [可调用对象](#可调用对象)
    - [协程](#协程)
    - [表达式](#表达式)
      - [初始列](#初始列)
      - [类型转换](#类型转换)
      - [左值与右值](#左值与右值)
      - [运算符优先级](#运算符优先级)
      - [求值顺序](#求值顺序)
      - [结构化绑定](#结构化绑定)
      - [Lambda表达式](#lambda表达式)
      - [字面值](#字面值)
  - [类](#类)
    - [声明与定义](#声明与定义)
    - [友元](#友元)
    - [成员](#成员)
    - [合成的特种成员何时deleted](#合成的特种成员何时deleted)
  - [特殊的类](#特殊的类)
    - [字面值类](#字面值类)
    - [聚合类](#聚合类)
    - [枚举类](#枚举类)
    - [联合类](#联合类)
    - [嵌套类](#嵌套类)
  - [using用法](#using用法)
  - [语句](#语句)
- [面向对象](#面向对象)
  - [OOP核心思想](#oop核心思想)
  - [继承机制](#继承机制)
  - [作用域](#作用域)
  - [基类列表与访问控制](#基类列表与访问控制)
  - [构造函数与特种成员](#构造函数与特种成员)
  - [虚函数](#虚函数)
  - [多重继承](#多重继承)
  - [运行时类型识别](#运行时类型识别)
  - [设计](#设计)
- [泛型编程](#泛型编程)
  - [模板参数](#模板参数)
  - [变参](#变参)
  - [函数模板](#函数模板)
  - [类模板](#类模板)
  - [变量模板](#变量模板)
  - [概念](#概念)
  - [模板特例化](#模板特例化)
  - [其它](#其它)
  - [编译器类型推断](#编译器类型推断)
  - [元编程](#元编程)
- [其他特性](#其他特性)
  - [内存分配](#内存分配)
  - [成员指针](#成员指针)
  - [volatile](#volatile)
- [附](#附)

<!-- vim-markdown-toc -->

&emsp;混合了C++11/14/17/20的标准，C++20的Big-Four(模块、~~概念~~、~~协程~~、范围)并没有涉及，等C++23吧，期待。。
文中会有标记写出哪版标准支持该特性，但无标记可不代表每个版本都支持哦，因为。。我忘了时哪版支持的了  

&emsp;因为C++的特性很多且复杂，所以对一些特性的描述会故意略去风格不太好的部分，以简化记忆。
本文更适合有一点C++基础的同学阅读，若文中有的特性并未细说，则读者可自行查阅[cppreference](#https://zh.cppreference.com/w/cpp)

**学习C++时，可以将C++语言分为四大部分**
* 基于C
* 面向对象
* 泛型模板
* STL

# 基于C
此部分按照C++代码中的作用域范围顺序来进行整理排序的，由外到内。
但是注意，基于C不代表等于C，现代C++与C有了很大的不同

## 预处理
* 宏替换
    * `#define`：未明确值的宏会替换为0
    * `#undef`
    * `defined(标识符)`
* 条件编译
    * `#if`：判断是否定义且非零
    * `#ifdef`
    * `#ifndef`
    * `#elif`
    * `#else`
    * `#endif`
* 文件引入
    * `#include < >`
    * `#include " "`
    * `__has_include(< >)`
    * `__has_include(" ")`
* 错误指令
    * `#error 错误消息`
* 编译器预定义宏
    * 通用信息：`__func__`、`__LINE__`、`__FILE__`、`__TIME__`、`__DATE__`
    * 操作系统：`_WIN32`、`__linux__`、`TARGET_OS_MAC`
    * 编译平台：`_MSC_VER`、`__GUNC__`、`__clang__`
    * 标准版本：`__cplusplus`
        * `pre-C++98 定义为 1`
        * `C++98     定义为 199711L`
        * `C++11     定义为 201103L`
        * `C++14     定义为 201402L`
        * `C++17     定义为 201703L`
        * `C++20     定义为 201709L`

## 属性
<!-- entry begin: 属性 attribution -->
* `[[deprecated]]`或`[[deprecated(解释弃用的理由并/或提议代替用实体的文本)]]`

    | 目标                      | 实例                                                             |
    |---------------------------|------------------------------------------------------------------|
    | class/struct/union        | `struct [[deprecated]] S;`                                       |
    | typedef名，也包括别名声明 | `[[deprecated]] typedef S* PS;`、`using PS [[deprecated]] = S*;` |
    | 变量，包括静态数据成员    | `[[deprecated]] int x;`                                          |
    | 非静态数据成员            | `union U { [[deprecated]] int n; };`                             |
    | 函数                      | `[[deprecated]] void f();`                                       |
    | 命名空间                  | `namespace [[deprecated]] NS { int x;}`                          |
    | 枚举                      | `enum [[deprecated]] E {};`                                      |
    | 枚举项                    | `enum { A [[deprecated]], B [[deprecated]] = 42 };`              |
    | 模板特化                  | `template<> struct [[deprecated]] X<int> {};`                    |

*  `[[fallthrough]]`
    > 仅可应用到switch语句中的空语句以创建直落语句： `[[fallthrough]];`

* `[[likely]]`与`[[unlikely]]`

    | 目标        | 实例                                        |
    |-------------|---------------------------------------------|
    | switch语句  | `switch (i) {[[likely]] case 1: return 1;}` |
    | if-else语句 | `if ( i < 0 ) [[unlikely]] {return 0;}`     |

* `[[nodiscard]]`或`[[nodiscard(解释结果不应被舍弃的理由的文本)]]`
    > 若从并非转型到 void 的弃值表达式中，
    * 调用声明为 nodiscard 的函数，或
    * 调用按值返回声明为 nodiscard 的枚举或类的函数，或
    * 以显式类型转换或 static_cast 形式调用声明为 nodiscard 的构造函数，或
    * 以显式类型转换或 static_cast 形式构造声明为 nodiscard 的枚举或类的对象，

* `[[maybe_unused]]`
    > 抑制针对未使用实体的警告

    | 目标                  | 实例                                                               |
    |-----------------------|--------------------------------------------------------------------|
    | class/struct/union    | `struct [[maybe_unused]] S;`                                       |
    | typedef 包括别名声明  | `[[maybe_unused]] typedef S* PS;，using PS [[maybe_unused]] = S*;` |
    | 变量 包括静态数据成员 | `[[maybe_unused]] int x;`                                          |
    | 非静态数据成员        | `union U { [[maybe_unused]] int n; };`                             |
    | 函数                  | `[[maybe_unused]] void f();`                                       |
    | 枚举                  | `enum [[maybe_unused]] E {};`                                      |
    | 枚举项                | `enum { A [[maybe_unused]], B [[maybe_unused]] = 42 };`            |

* `[[no_unique_address]]`
    > 空类空间优化：`struct [[no_unique_address]] X {int i_m; Empty e1_m, e2_m;};`
<!-- entry end -->

## 命名空间
* 普通命名空间
    > `namespace std {  }`
    * 第一次声明为定义，以后为打开

* 嵌套命名空间
    > 在命名空间中使用`#include`要注意嵌套错误，可在进入命名空间前提前`#include`从而利用头文件保护宏来避免嵌套
    * 在全局命名空间中声明：`namespace A::B {  }`
        > 外层命名空间不用提前声明
    * 在外层命名空间中声明：`namespace A { namespace B{  } }`

* 内联命名空间
    > `inline namespace std_v1 {  }`
    * 只需要第一次声明时（定义时）指出inline
    * 其内成员名字暴露在外

* 无名命名空间
    * 其内成员不对外链接
    * 其内成员名字暴露在外

* 全局命名空间
    * 通过`::member`引用时，不与**无名命名空间**中成员冲突，但仍会与**内联命名空间**成员冲突

* 命名空间别名
    > `namespace AB = A::B`

* 声明与定义：  
头文件中非模板的函数与全局变量，为避免符号重定义
要么只提供声明并在lib中提供定义，要么提供inline定义

<!-- need reread: 版本库 -->
* 设计版本库：
```cpp
// test.hpp 第一个版本
namespace std { /* ... */ }

// test.hpp 第二个版本
#include <test_v1.hpp> // test_v1.hpp即第一个版本的test.hpp，其中的`namespace test`改为了`namespace test::v1`
namespace std { /* ... */}
```

## 静态变量
### local static
local static对象即声明在函数内部的静态声明周期变量，它们一样存在与目标文件数据段，但一般只随着函数的引用而被引用。
local static对象的构造发生在初次调用该函数时，且构造过程为线程安全的。
几乎任何时候需要使用静态变量的地方，都可以用**static reference return**技术来取代，优点如下：
* 在第一次调用时才构造（线程安全），不使用时无需承担构造开销
* 多文件编译时不能保证全局变量按着期望的顺序来构造，使用该技术保证对象在使用前已被构造
* 利用函数包裹，实现可有更多操作空间

### non-local static
全局对象的构造函数会在进入main函数前调用。
当多文件编译时，有时希望利用全局变量的构造函数自动执行一些任务，
这就需要你将该全局变量的定义所在源文件（或目标文件）纳入到最终目标的依赖中去。
需要注意几点：
1. 直接将定义了全局变量符号的目标与主目标一同编译即可强行引入你的符号
2. 定义在类内的static数据成员是不会产生符号的（除非使用inline）


**关于链接**：

* 无修饰的默认情况：
    * 无论是否对其初始化，都被当作定义
    * 符号定义对外可链接

* extern：
    * 未初始化则不会被当作定义，从而可以引用外部符号
    * 符号定义对外可链接

* const与constexpr：
    * 必须初始化，且被当作定义
    * 符号定义默认不对外链接，可以使用**extern**强制对外

### inline语义
* 对函数：
    * 提示编译器内联展开以优化性能
    * 若展开失败则保证所有编译单元（TU）中的**同名inline函数**链接到同一定义
        > 存在多个不同的定义时链接不会报错，而是会选其中一个进行链接

* 对全局变量：
    * 将所有TU的同名inline变量链接到同一定义
        > 作用是可以在头文件中定义`inline static`变量而不引起符号重定义的链接错误

* 对函数模板与变量模板：
    * 它们具有隐式`inline`属性，但是对于函数模板其作用仅限于“链接”而非“内联展开”
    * 编译工具链可能会自动消除多个TU中重复的模板实例

* 对类的static数据成员：
    * 同全局变量，将所有**TU**的同名`inline`变量链接到同一定义，
        可以在类内定义`static inline`数据成员，而无需再在类外定义一次以形成实体
    * 为模板类提供了定义static数据成员的机制，避免需要将模板的声明与定义分离的麻烦

## 函数
### 关键字
* inline：[见inline语义](#inline语义)

* constexpr：<span id="constexprhu"></span>
    * 隐式inline
    * 提示编译器：若调用实参是constexpr对象（如果有参数的话），**且将返回值传递给一个constexpr对象**，
        则此函数**应该**进行编译期计算
    * 使用限制：<u>不太精准但实用的简化版</u>
        * 参数与返回值：必须是字面值类型（如果有的话）
        * 定义的变量：字面值类型，且不能有static、thread_local、未初始化变量
        * 调用的函数：constexpr函数

* auto：
    > 此时的auto就像是无名的模板参数，而要获取其类型时需要对实参调用`decltype(t)`
    * 返回类型推断：
        > `auto func()`
        * 所有return语句的返回类型均要相同（可以使用`if-constexpr`绕过）
        * 递归调用之前必须有return以确定返回类型
    * 实参类型推断：
        > `void func(auto&& t)`
        * 调用时也可以指定auto的类型`func<int>(1)`

* operator：
    * 重载运算符
        > `double operator()(int);`
    * 自定义字面值：<span id="zdyzmv"></span>
        > `double operator""_i2d(unsigned long long t);`
        * 重载符名称只能以下划线`_`开头
        * 形参限制：

            | 形参类型                | 字面值             |
            |-------------------------|--------------------|
            | `(unsigned long long)`  | 整数`250_i`        |
            | `(long double)`         | 浮点数`2.5_d`      |
            | `(const char)`          | 字符`'2'_c`        |
            | `(const char*, size_t)` | 字符串`"string"_s` |

* noexcept：<span id="noexcept"></span>
    * 用法：
        * `noexcept`
        * `noexcept(bool)`
        * `noexcept(func)`
            > 只要func声明了noexcept，或没有调用non-noexcept函数且无throw语句则为ture  
            > 一般嵌套再第二种用法中`noexcept(noexcept(func))`

    * 限定了**函数指针**与[**虚函数**](#xhu)对noexcept属性的传递：
        non-noexcept可接受noexcept，反之则不行

    * **合成的**特种成员函数会有合成的noexcept声明，合成的声明即相当于添加了`noexcept(noexcept(func))`。
        自定义的[析构函数](#xghu)若不显式指定也会合成noexcept声明，  

    * 提示编译器不抛出异常以进行优化，若最终还是抛出了异常则直接终止程序

<!-- need reread: =delete技巧 -->
* =delete：
    * 删除合成的特种成员：default构造、析构、copy操作、move操作
    * 删除成员函数或非成员函数，拒绝从某一类型进行类型转换而调用该函数
    * 删除一个模板实例，拒绝实例化出不希望的模板实例（也可使用`enable_if<>`）
        > `template<> void func(bool) = delete`相当于声明一个特化版本并将其删除

<!-- need reread: 非RVO的move -->
* return：
    * 对于RVO情况的按值返回：若被调用方返回局部对象，且其类型与返回值类型相同，则直接在调用方栈帧中构造该对象
    * 对于非RVO情况的按值返回：对于右值调用move操作，对于左值调用copy操作
    * 在此谈谈按值传参的情况：
        * 形参永远都是左值
        * 实参可能是左值或右值，这也决定了由实参构造形参是copy还是move

### 默认实参
* 若某一形参有默认实参，则其后的形参也必须有
    > 故大概率不会用到默认实参的形参放前边
* 若声明时已指出默认实参，则定义时不能重复指出
* 虚函数的默认实参由静态类型决定

### 重载函数
* 不允许定义形参列表相同的同名函数，即使返回类型不同

* 使用[using声明](#using声明)可以引入所有重载函数

<!-- need reread: 函数匹配 -->
* 类型转换级别(优先级降序)
    1. 精准匹配、数组与函数蜕化、顶层const转non-const
    2. 非底层const转底层const
    3. 整型提升
    4. 其他内置类型转换
    5. 与类有关的类型转换

* 函数查找
    * 常规作用域查找：从当前作用域向前、向外查找，最终到全局命名空间起始处结束
        > 注：若形参为内置类型，则只会常规作用域查找，因为内置类型未定义于任何命名空间中
    * 实参依赖查找(ADL或Koenig)：根据实参的类型还会到到其所在的命名空间中查找匹配的函数
        > 注：显示指定模板参数的模板函数的ADL在C++20中才引入

* 函数匹配
    * 先进行函数查找，找出所有（实参转换后）形参合适的函数或推断成功的函数模板（候选集）
    * 然后计算出每个函数（模板）的每个参数进行优先级（具有默认实参的参数位可以不必计算）
    * 对候选集中成员的优先级进行字典序比较，选出优先级最高的（同等优先级下非模板优先）


### 可调用对象
* 调用形式：
    * 函数、函数指针
    * 成员函数指针
    * 成员函数
* 传递形式：
    * 函数指针
    * 成员函数指针
    * 类对象
* 对比优劣：
    * 函数对象相比函数指针：
        * 多个实例可存储独立的内部状态
        * 利用类型系统而更细致的区分
    * Lambda相比函数对象：
        * 便捷、直观
        * 无default构造函数
>

* 函数参数 <span id="cuxu"></span>

| 类型          | 特性                             |
|---------------|----------------------------------|
| T             | 拷贝                             |
| T&            | 引用、左值(非常量)               |
| T&&           | 引用、右值                       |
| const T&      | 引用、左值、右值、类型转换、只读 |
| temp T&       | 引用、左值、泛型                 |
| temp const T& | 引用、左值、右值、泛型、只读     |
| temp T&&      | 引用、左值、右值、泛型、转发     |

> `const T&`接受右值时，该右值临时量会在调用语句结束时便销毁。所以编写异步/延迟调用的函数时注意这点

### 协程
想要理解协程，得先知道为什么需要协程，而这就要求了解异步编程与回调函数。

C++20无栈协程：

**原理**
* 协程有两个栈帧用于存储状态：
    * 一个是分配在栈上的普通栈帧，在 恢复/暂停 协程时就像调用普通函数一样 创建/销毁 栈帧。
        若编译器判断一些局部变量的生命周期不超过协程的一次连续执行周期，则将其分配在该栈帧上。
        在协程启动时会将“普通栈帧”上的函数参数copy/move到协程栈帧上。
        > 当协程调用普通函数时，就会下移栈指针来扩充栈，在普通函数调用过程中，协程无法返回，
        > 因为普通函数的栈帧在协程普通栈帧下面，若每次协程返回销毁普通栈帧则就会连同普通函数栈帧一起销毁
    * 另一个是分配在堆上协程栈帧，该栈帧保存了伴随整个协程生命周期的数据以及协程Promise对象。

**特点**
* 协程的返回类型`Feature`需要定义一个成员类型`promise_type`来 ***定义协程的行为***，其成员包括
    * `Feature   get_return_object()`   ：协程第一次返回时的返回值
    * `Awaitable yield_value(exp)`      ：`co_yield exp`相当于`co_awiat Promise.yield_value(exp)`在暂停时返回值给caller
    * `Value     return_value(value)`   ：`co_return non_void_exp`在协程结束返回时调用
    * `void      return_void()`         ：以下情形调用该函数1.`co_return;`2.`co_return void_exp`3.控制流出返回void协程结尾
    * `Awaitable initial_suspend()`     ：在协程开始处插入该段代码
    * `Awaitable final_suspend()`       ：在协程结尾出插入该段代码

* 协程第一次返回类型`Feature`一般具有成员对象`coroutine_handle<promise_type>`用来 ***恢复协程***，其成员包括：
    * `resume`或`operator()`：恢复协程
    * `done`                ：是否处于final_suspend阶段
    * `destroy`             ：销毁Promise对象
    * `promise`             ：返回Promise对象引用
    * `static coroutine_handle from_promise(Promise)`：从Promise对象返回其coroutine_handle

* 语句`co_await Awaitable;`即产生一个暂停点 ***等待事件完成后返回结果***，协程暂停的行为依赖Awiatable的定义，其成员包括：
    * `bool awiat_ready()`                      ：若返回true则不暂停直接执行下条协程语句
    * `?    await_suspend(coroutine_handle<>)`  ：根据返回值类型不同而具有不同行为
        * 若返回void则直接暂停挂起
        * 若返回bool为true则直接暂停挂起
        * 若返回`coroutine_handle<>`则暂停挂起并调用其`resume`恢复那个协程
    * `T    resume()`                           ：协程恢复时`co_awiat`语句的运算结果

### 表达式
* 初等表达式
    > 任何运算符的操作数都可以是其他的表达式或初等表达式
    * 字面量
    * 标识表达式
    * lambda表达式
    * requires表达式
    * 折叠表达式
    * 括号中的任何表达式
* 不求值表达式
    > 顾名思义
    * typeid（不为多态泛左值）
    * sizeof（sizeof(T)要求T::~T可访问）
    * noexcept
    * decltype
* 弃值表达式
    > 仅用于实施其副作用的表达式
    * 标志表达式
    * 数组下标表达式
    * 类成员访问表达式
    * 间接寻址
    * 成员指针操作
    * 条件表达式（其第二、三操作数是弃值表达式）
    * 逗号表达式（其右操作数是弃值表达式）

#### 初始列
<span id="initlist"></span>
* 作用：
    * 用于调用构造函数（**initializer_list优先** ） `string s{"str"};`
    * 用于隐式构造为需要的类型（**需要non-explicit构造函数**） `string s = {"str"}`
        > 若无法推断构造目标的类型，则构造initializer_list
* 对于内置标准类型
    * 空的花括号可进行值初始化
    * 拒绝丢失精度的转换（窄化）

#### 类型转换
> 不鼓励使用旧式的C-style-cast，下面的新式转换更容易在文件中搜索，且更容易由编译期帮助检测
* `static_cast<>()`     ：一般用于隐式转换规则允许的转换
* `const_cast<>()`      ：一般用于取消底层const限定
* `reinterpret_cast<>()`：一般用于转换指针类型
* `dynamic_cast<>()`    ：一般用于在继承体系中转换它们实例的指针或引用

**有关指针类型的转换**
* nullptr_t可以转换为任意指针类型，反之不成立
* 任意指针类型可以转换为`void*`，反之只能显式转换
* `const char*`与`char const*`均为底层const；`char* const`才是顶层const
* 默认的C-Style字符串类型为`const char*`，标准提供了一个特殊转换来向后兼容：可以将C-Style-String转换为`char*`

#### 左值与右值
* 需要左值：赋值`=`、取地址`&`、自增减`--` `++`
* 返回左值：赋值`=`、解引用`*`、箭头`->`、下标`[]`、前置自增减`++a`与`--a`
* 可能返回左值：
    * 条件` ? : `，若`:`号两边都为左值才会返回左值
    * 成员`.`，若`.`左边的对象时左值才会返回左值

#### 运算符优先级
![cpp](images/cppman.png)

#### 求值顺序
* 有序：
    > 注意，逻辑与和逻辑或的实质就是分支，同样会有分支处罚
    * 逻辑与
    * 逻辑或
    * 条件运算符
    * 逗号
* C++17新规则：在下列表达式中，a的求值和所有副作用先序于b，同一个字母的顺序不定。
    * `a.b`
    * `a->b`
    * `a->*b`
    * `a(b1, b2, b3)`
    * `b @= a`
    * `a[b]`
    * `a << b`
    * `a >> b`
* 在一句复合表达式中，若两个子表达式无父子关系，则它们的求值顺序未定

#### 结构化绑定
* t的所有public非静态数据成员必须为直接成员或相同基类的直接成员，不能绑定union
* 值绑定：`auto [x, ignore, z] = t`
* 引用绑定：`auto& [x, y, z] = t`

#### Lambda表达式
* 形式：`[cap]<temp>(arg){sta}`
    > C++17支持constexpr修饰
* 捕获列表：
    * 捕获类型：非static的值或引用（static值可直接使用）
    * 显式捕获：
        * `[x, &y]`：按值捕获x，按引用捕获y
        * `[this]`：捕获this，从而可以使用其成员且可省略`this->`
    * 隐式捕获：
        > 应该避免使用隐式捕获已丧失可读性
        * `[=]`：按值进行隐式捕获
        * `[=, &x]`：按引用且只能按引用捕获x，其它进行隐式值捕获
        * `[&]`：按引用进行隐式捕获
        * `[&, x]`：按值且只能按值捕获x，其它进行隐式值捕获
    * 初始化捕获：
        * `[a_m = std::move(x)]`：a_m为该lambda对象的数据成员，x为外部对象
* 完整的泛型模板支持
* 参数列表修饰：
    * 可以使用auto推断参数类型，且其返回类型默认就使用auto推断
    * `[](auto&& t) mutable {}`：修饰`mutable`可以在lambda中修改值捕获的对象（默认值捕获对象只读）

#### 字面值
> 定义于：`inline namespace std::literals`
* `chrono::duration`：
    * 后缀：h、min、s、ms、us、ns
* 数字：
    * 前缀：`0b或0B`、`0`、`0x或0X`
    * 后缀：`U`、`L`、`UL`、`F`
    * 无后缀的整数字面值，其类型为：
        * 若为十进制，为`int` `long` `long long`中的最小者
        * 若不为十进制，为`int` `unsigned int` `long` `unsigned long` `long long` `unsigned long long`
           中的最小者
* 字符：
    * 原始字符串                ：`R"del(string)del"`
    * 前缀（C-char或C-string）  ：`u8" UTF-8 "`、`u" UTF-16 "`、`U" UTF-32 "`、`L" wchar_t "`
    * 后缀（仅限于C-string）    ：`s`、`sv`
* [自定义](#zdyzmv)

## 类
### 声明与定义
* 类的声明：
    * 声明之后，即可使用该类的指针与引用，并可做函数参数与返回值的类型

* 类的定义：
    * 定义之后，即可定义该类的具体对象，并可访问该对象的成员

* 成员：
    * 定义在类内的成员函数隐式inline
    * 定义在类外时，在名字`classname::member`之后进入类的作用域

### 友元
* 友元函数      ：若无前置声明，此处即为声明
* 友元类        ：若无前置声明，此处即为声明
* 友元成员函数  ：需要先提供类的前置定义
* 友元模板：
    * 某一实例：需要前置声明模板，并显示指定模板参数<span id="mbyy"></span>
        > `friend func<int>(Test<int> t);`  
        > 注意，可声明全特化，但不可声明偏特化

    * 整个模板：若无前置声明，此处即为声明
        > `template <typename T> friend func(T t);`

    * 模板参数：
        > `friend T;`

* 友元关系无法传递与继承，但基类的友元可以访问其派生类的基类部分

### 成员
* 类型成员
    * [using类型别名](#using用法)

    * [using模板类型别名](#using用法)
    * [嵌套类](#嵌套类)
>

* 数据成员
    * 构造顺序与声明顺序相同

    * 类内初始值：只用于编译器推断默认构造函数
    * 若有const与引用成员则[无默认构造与赋值操作](#合成的特种成员何时deleted)
    * mutable：即使该类对象为const也允许修改该数据成员
    * static：<span id="static"></span>
        * 可以使用不完整类型，但定义时需提供完整定义
        * const：若定义在类内则也应该在类外声明以形成实体（链接符号），non-const的只能定义在类外
        * [inline](#inline语义)：在类内提供定义即可形成实体，利用inline语义避免符号多重定义
        * constexpr：隐式inline
>

* 成员函数
    * 限定this：`const`、`&`或`&&`

    * 修饰符的放置顺序：**const&emsp;&/&&&emsp;noexcept&emsp;final&emsp;override&emsp;=0**
    * 定义在类内隐式`inline`
    * static：不会隐式传递`this`，故无法使用non-static成员
>

* 构造函数 <span id="gzhu"></span>
    > `classname(arglist): initlist {funcbody}`
    * 实现默认构造：
        * =default
        * 无参构造
        * 默认实参
        * 委托构造
            > 注意构造函数会在构造对象时自动调用，
            > 显示调用构造函数的意思是构造并返回一个对象，
            > 在构造函数内部调用其他构造函数以企图初始化数据成员的行为并不正确，
            > 如此请使用委托构造函数
    * explicit：
        * 单参构造可视作隐式类型转换(from)，使用explicit拒绝隐式转换
        * 多参数构造也可使用explicit用于拒绝[初始列](#initlist)隐式转换
    * constexpr：
        * 必须初始化所有数据成员（未涉及原本会由编译器推断）
        * 该类无虚基类
        * 其它见[constexpr函数](#constexprhu)
    * 自定义构造方式：
        * 就地构造
        * 类聚合式构造
        * 逐块式构造
        * 成员模板构造
        * 其他
>

* 析构函数 <span id="xghu"></span>
    > `~classname() {}`
    * 逆序销毁
    * 会由编译器无条件地合成[noexcept声明](#noexcept)
    * 析构函数若为=delete，则该类对象只能通过动态分配创建该对象，且不能销毁
>

* copy语义
    * copy构造：
        > `ClassName(const ClassName&): initlist {funcbody}`
    * copy自赋值：
        * 先拷贝新值
        * 再销毁旧值
        * 赋值
* move语义
    * move构造：
        > `ClassName(ClassName&&): initlist {funcbody}`
        * 若不为`noexcept`，则标准库容器不会使用移动构造函数
    * move自赋值：
        * 直接拒绝自赋值
>

* 三路比较
    > `auto operator<=>(const Type& obj) const =default`；不会自动合成需要手动指定
    * 返回值`>0`表示`>`，`==0`表示`==`，`<0`表示`<`，编译器据此来合成6个比较操作
    * 默认依字典序比较类实例的每个基类（从左到右，深度优先）和所有非静态数据成员（按声明顺序）
        > 内置数组成员算作聚合类，也具有三路比较运算符
    * 因为性能问题，你可能提供`operator==()`来完成整个比较操作的自动化生成
        > 旧式的6个比较运算符优先级高于三路比较
    * 返回类型auto可以显示指定为以下三者之一：
        * std::strong_ordering  ：等价的值表示完全相同，如`int`
            > 成员常量：less、equivalent、greater
        * std::weak_ordering    ：等价的值也不一定完全相同，如`double`
            > 成员常量：less、equivalent、greater
        * std::partial_ordering ：允许忽略无法比较的成员，如包含`NaN`的`double`
            > 成员常量：less、equivalent、greater、unordered

<!-- need reread: 类型转换 -->
* 类型转换
    * `explicit operator bool()`
        > 在明确需要bool值的地方仍会隐式转换bool值
    * `explicit operator Type() { return Type{}; }`
        > 此形式一般在无法修改转换目标（即Type）的源码时才使用，即一般都使用第一种形式设计类型转换
    * 二义性问题：A转换为确定的类型B，应该调用哪个转换函数？
        > 本质上就是重载函数的匹配问题，但是注意类型转换是不具传递性的，即A转B，B转C，但A不能转C。
        * 存在一个特殊巧合：C具有一个单参构造函数用于B转C，`C(const B&);`；
        此时可由A构造C，`C c{A{}};`，因为该初始化语句会调用前面那个单参构造函数，而A可通过类型转换为B而匹配该函数。
        故此看似进行了类型转换的传递，实则不然。
        * 还有一个特殊存在，类转换为算术类型时会进行真正的传递性转换。A转int，int转double，A可直接转double。
        这是语法层面的规则，而非语法trick。如果存在多个类A存在多个转向算术类型的转换函数，会有一个稍微复杂点的机制进行函数匹配。
        在此不做细说，因为这种做法是deprecated。只需知道，转向算术类型的转换函数除了bool外最多再定义一个，且声明为explicit。
        让编译器帮助进行传递性转换可更加安全。
>

* 重载运算符
    * 除了函数调用运算符，其他不能有默认实参
>

### 合成的特种成员何时deleted
* 析构函数：
    * 有成员无法析构
* 默认构造：
    * 定义了其他构造函数（包括copy构造与move构造）
    * 有成员无法默认构造
    * 有成员无法析构
    * 有const或引用成员且无类内初始值
* 拷贝构造：
    * 有成员无法拷贝构造
    * 有成员无法析构
    * 定义了移动操作
* 拷贝赋值
    * 有成员无法拷贝赋值
    * 有const或引用成员
    * 定义了移动操作
* 移动构造
    * 有成员无法移动构造
    * 有成员无法析构
    * 定义了析构函数、拷贝操作、移动赋值
* 移动赋值
    * 有成员无法移动赋值
    * 有const或引用成员
    * 定义了析构函数、拷贝操作、移动构造

**一言蔽之，若重定义了除default构造函数的其它特种成员中的一个，其它所有特种成员也得重新定义**

## 特殊的类
<!-- need reread -->
### 字面值类
* 数据成员为字面值类型的[聚合类](#聚合类)
* 若非聚合类，则：
    * 数据成员均为字面值类型
    * 类内初始值为constexpr（如果有的话）
    * 至少有一个constexpr构造函数
    * 使用默认析构函数

<!-- need reread -->
### 聚合类
* 定义：
    * 数组
    * 否则：
        * 所有成员与基类都为public
        * 无virtual函数
        * 未定义任何构造函数
        * 注：C++14之前要求不能有类内初始值
* 聚合初始化： <span id="jhcsh"></span>
    * 默认提供一个特殊构造函数：用花括号按顺序逐个构造**基类**和**数据成员**。
        但是注意，默认构造内置类型的成员时其值未定义，而非使用值初始化
        > `int arr[]{1, 2};`
    * C++20支持圆括号的聚合初始化从而允许窄化
        > `int arr[](1.0, 2L);`
    * C++20支持在花括号中使用指示符以初始化确定的成员，但仍然需要保持顺序
        > `pair p{.first = 1, .second{2}}`

### 枚举类
* 实质：
    * 该类的成员为constexpr整数
    * 该类的对象是这些成员中的一个
* 普通枚举：
    > `enum Num {ONE, TWO, THREE};`  
    > `enum Num: int {ONE, TWO, THREE};`
    * 成员名字对外可见
    * 默认类型由编译器推断
    * 可以赋值给整型变量
    * 不可由整数赋值
    * 无名枚举：无对象，可直接使用成员
* 领域枚举：
    > `enum class Num {one, two, three};`  
    > `enum class Num: int {one, two, three};`
    * 成员名字对外不可见，需要用`::`运算符访问
    * 默认类型为int
    * 不可赋值给整型变量
    * 不可由整数赋值

### 联合类
***C++17中用std::variant<TYPES...>可以完全取代手写 union***

* 实质：用同一块内存存储几种不同类型中的一种，在某时刻存储的类型由用户保证
* 不能是继承体系中的一员，故不能有虚函数
* 匿名union：
    > 编译器自动创建一个无名对象，其成员可直接访问
* 若含有的成员类型有自己的默认构造函数和特种成员则合成的构造和拷贝为delete
    > 原因：联合中存储的值的类型编译时未知，编译器无法调用正确的析构函数来销毁原值
* 通常用类来管理这种union
    > 用匿名union作用数据成员  
    > 用普通枚举对象作判别式追踪union的状态
* 使用定位new：用union中类成员的地址作定位


### 嵌套类
* 作为外层类的一个类型成员
* 外层类和嵌套类相互独立，互相之间并没有特殊访问权限，只是外层类成员在嵌套类中可见
>

## using用法
* 类型别名：
    > `using NewType = OldType;`
    * 针对内置类型、类、模板类实例
* 模板类型别名：
    > `template<typename T> using newtype = typename oldtype<T>::Type`
    * 作用目标为模板类或模板类的类型成员（可能需要[typename](#typename)）
    * 不能在块作用域中使用
* using声明<span id="using声明"></span> ：
    > `using std::member1, std::member2;`
    * 将命名空间或类中的成员引入当前作用域而直接可见
    * 引入函数时不需要函数签名，故可引入完整的重载函数集
* using指示
    > `using namespace std;`
    * 将命名空间所有成员引入当前作用域，若发生名字冲突可以再使用`::`运算符解决(using声明不行)

<!-- need reread -->
* using继承： <span id="usingjc"></span>
    > `using D1::D1, D2::D2;`  
    > `using T::operator()...` 此形式用于[变参派生](#bcpu)的派生类
    * 类似using声明，将基类部分的成员引入派生类的作用域
        * 改变**优先级**和[**访问控制**](#基类列表与访问控制)
        * 引入不同基类的同名函数可将冲突关系变为重载关系
        * 引入被派生类遮掩的函数（这些函数即使签名不同也会被派生类的同名函数遮掩）

    * 继承基类的构造函数，提示编译器根据基类构造函数合成派生类的构造函数
        * 不能继承虚基类的构造函数
        * 不能使用变参继承构造函数
>

## 语句
* 分支判断
    ```cpp
    if ( init; condition ) { // `init;`部分可忽略
        /* ... */
    } else if constexpr ( init; constexpr_bool ) { // `init;`部分可忽略
        /* ... */
    } else {
        /* ... */
    }
    ```
* 开关多分支
    ```cpp
    switch ( init; integer ) { // `init;`部分可忽略
        case constexpr_integer:
            /* ... */
            break;
        default:
            /* ... */
            break;
    }
    ```
    * 每个case分支的声明的变量并非分支独有的
    * 不能初始化变量，避免跳过初始化，可以在块作用域中初始化（即限制其作用域）
    * 注意break的使用
* for循环
    ```cpp
    for ( init; condition; iterate ) { // `init;`部分可忽略
        /* ... */
    }
    ```
* while循环
    ```cpp
    while ( condition ) {
        /* ... */
    }
    ```
* do循环
    ```cpp
    do {
        /* ... */
    } while ( condition )
    ```
    * 条件检测仍然不能使用循环体中声明的变量
* range-based-for
    ```cpp
    for( init; obj : container ) {// `init;`部分可忽略
        /* ... */
    }
    ```
* 异常
    ```cpp
    void func() try
    {
        /* ... */
    } catch (...) {
        /* ... */
        throw;
    }

    try {
        /* ... */
        throw exception{};
    } catch (exception&) {
        /* ... */
    }
    ```
    * 异常的抛出`throw exception{}`必定会有一次拷贝构造被调用
    * 重新抛出`throw;`：只能直接或间接出现在catch语句中
    * catch允许的类型转换：
        * 精准匹配、数组与函数蜕化、顶层const转non-const
        * 继承体系的转换
* 控制转移
    * continue
    * break
    * ~~goto~~

# 面向对象
## OOP核心思想
* <div title="这一点其实在之前便涉及了，即是类的设计和封装">数据抽象</div>
* 继承
* 动态绑定
>

## 继承机制
* 多态实现：
    * 只要类或其基类中存在虚函数，则就会引入一个隐式数据成员（虚指针）
    * 虚指针存在与每个类部分中（派生类的数据可分为两部分，派生类部分与基类部分）
        > 如D继承自B，在D实例对象中，B部分中有个虚指针，D部分中也有个虚指针，如此一来D才能当做B来用
    * 虚指针指向一个同类通用虚表，虚表存储类类型信息，以及虚函数指针
        > D实例对象中的B部分的虚指针指向D的虚表
    * 当通过B的指针或引用调用虚函数时，会发生动态绑定，即去虚指针指向的虚表中寻找当前调用的虚函数的地址
    * 虚表的合并，子类先复制父类虚表，再在其中各个位置覆盖信息，若子类有新定义的虚函数，则添加在后面

* EBO（空白基类优化）：  
    若基类部分无数据成员，且派生类第一个数据成员类型不是基类类型，则基类部分不分配空间  
    正常情况C++对于对象的空间分配，至少会分配1Byte，即使类中无数据成员

* 类的static成员，对整个程序都唯一，不管它存在于哪个继承体系中

* 声明不能包含基类列表

## 作用域
* 派生部分作用域内嵌于基类部分

* 通过基类的引用或指针调用成员，会根据静态类型进入作用域，并开始逐层向外进行名字查找，
    如果查找到的是虚函数，编译器将其绑定到虚指针上，运行时进行动态绑定

* 作用域名称遮掩规则与正常的有些不同，内层（派生类）作用域中的函数会掩盖外层（基类）的所有同名函数，
    即使它们的函数签名不同，可通过[using继承](#usingjc)或转换函数解决
    <a href=## title="目的是为了防止不小心继承了疏远的基类的函数">[注]</a>

<!-- need reread -->
* 若基类是未实例化的类模板，则编译器默认不会进入外层（基类）作用域查找符号，
    因为类模板的具体定义需要实例化是才知晓，而其中可能没有你试图调用的符号，
    此时便需要如此显式调用`this->temp_base_func()`

* 访问基类部分需要通过基类接口
    * 派生类：`member`或`base::member`
    * 用户：`derive.member`或`derive.base::member`

## 基类列表与访问控制
* 基类列表：virtual，public、proteble、private
* 访问控制：public、proteble、private

* 派生类到基类的转换，需要调用者有访问该派生类的基类部分的权限，  
    即派生类（派生访问说明符）允许访问，  
    且基类（成员访问说明符）也允许

## 构造函数与特种成员
* 对基类部分的构造函数，可将基类部分看做派生类的数据成员，成员名字即基类类名
* 构造函数、析构函数中调用虚函数，为静态类型版本

## 虚函数
* 必要条件：
    * 非static非模板的非构造函数
    * 必须提供定义

* 关键字修饰：
    * virtual&emsp; ：声明为虚函数
    * final&emsp;&emsp;：拒绝函数被覆盖（拒绝类被继承）
    * override ：标记为覆盖虚函数，若并未覆盖则报错
    * = 0&emsp;&emsp;&emsp;：声明为纯虚函数，可以不用提供定义(若要定义则定义在类外)，派生类必须覆盖
* 抽象基类：含纯虚函数，不能定义对象

* 默认实参：调用时由静态类型决定（静态绑定）

* 派生类中试图覆盖基类中同名的虚函数，其参数列表和返回类型等各种属性都要相同，否则会隐藏虚函数
    > 例外：基类中返回基类的指针或引用，派生类可以返回派生类的指针或引用，前提派生类与基类之间的转换可访问

* 动态绑定 <span id="dtbd"></span>：当通过基类的指针或引用调用虚函数时，会判断该指针或引用的动态类型，
    并调用其动态类型中设计的的虚函数。但构造函数与析构函数中调用的虚函数不会发生动态绑定，即调用静态类型的虚函数版本

## 多重继承
* 构造顺序：
    * 由上至下（从基类到派生类）
    * 从左到右（基类列表中的顺序，虚基类优先）
* 名字查找在直接基类中同时进行，若有重名则需要使用作用域运算符，为了避免该潜在二义性：
    * 使用[using继承](#usingjc)
    * 或者设计自己的wrapper，在其中通过`::`作用域运算符进行访问
* 若转换存在多条路径，也会产生二义性错误
>

## 运行时类型识别
* `dynamic_cast<>()`：
    * 转换为目标类型的指针、引用
* `typeid()`：
    * 接受目标类型的对象返回type_info的const引用
    * type_info提供成员`.name()`
>

## 设计
* OOP与Template之间“多态”的区别
    * OOP通过继承体系的运行时转换(动态多态)
        * 需要设计virtual函数并override
        * 可以调用已编译好的库
    * Template通过泛型的编译时实例化(静态多态)
        * 需要设计类的成员供Template调用
        * 需要重新编译源文件(因为template时根据已有的调用实例由编译器自动生成的，而在设计它之前是没有这个实例的)
>
# 泛型编程
## 模板参数
* 模板参数作用域中，不能重用模板参数名

* 用typename指出目标模板的成员为类型，避免与静态数据成员相混淆 <span id="typename"></span>
    > 如`T::A* b;`，可能是声明类型为T::A*的对象b，也可能是static变量T::A乘以b。

* 类型参数：
    * 普通类型
        > `template<typename T>`
    * 模板类型（此处将模板归为广义的类型，方便下文区分类型于非类型参数）
        > `template<template<typename, auto> class T>`  
        > `template<template<typename...> class T>`  
        > `template<template<auto...> class T>`
* 非类型参数：必须常量表达式
    * 显示指定
        > `template<int INT>`
    * auto推断
        > `template<auto INT>`
* 默认模板参数
    * 类型与非类型都可以有默认模板参数
    * 自动实施于偏特化版本

## 变参
> `template <typename... Types, auto... VARS>`
* `sizeof...()`获取参数包中参数数量

<!-- need reread -->
* 模板类型参数解包
    * 类型修饰
        > `(T&&... t)`与`(C<T...> t)`
    * 基类列表 <span id="bcpu"></span>
        > `template <typename... T> class Deriv: T...;`
    * [using继承](#usingjc)
        > `using T::operator()...;`
* 函数参数解包（也适用于非类型参数）
    * 函数调用
        > 例如`func(arg1, args...);`与`func(forward<Type>(args)...)`
        > 注意参数包可以为空
    * 左右折叠表达式
        * 一元折叠：`((cout << E << endl), ...)`
            > 表示对E包中每个元素调用逗号左边表达式
        * 二元右折叠：`(E OP ... OP init)`
            > 表示`E1 OP (... OP (EN-1 OP (EN OP init)))`
        * 二元左折叠：`(init OP ... OP E)`
            > 表示意味着 `(((init OP E1) OP E2) OP ...) OP E2`

## 函数模板
> `template <typename T> int test(vector<T> t);`
* 自动推断模板参数：具体规则见[auto推断](#autolxtd)
    > `test(vector<int>{})`  
    > 注意只会从显式实参推断；
    > 注意不会由默认实参推断模板类型参数；
    > 注意不会推断返回类型，此时应使用auto返回；
* 显式指定模板参数：
    > `test<int>(vector<int>{})`
    * 指出无法由实参推断出的模板参数
    * 用于声明[模板友元](#mbyy)时指定其某一实例为友元
    * 用于模板元编程的分支、循环（递归）

* 函数模板的偏序规则：用于判断重载的模板函数中哪个更特例化 <span id="px"></span>
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
    而反过来将`X`带入`#2`的模板，无法成功推断，因为`T*`要求传入的实参必须为指针；
    如此单方面的推断，则表明`#1`的模板参数比`#2`更加泛型，即`#2`比`#1`更加特例化  
    &emsp;**常见的有：**
    * `T*`比`T`更特例化
    * `const T*`比`T*`更特例化
    * `(T1 t1, T1 t2)`比`(T1 t1, T2 t2)`更特例化

## 类模板
* 类外使用类模板名字需要用`template<typename T> ... Myclass<T>:: ...`
    > 若定义的是成员模板，则需要两个template，一个是类模板的，一个是成员模板的，初学时会感到奇怪

* 在类模板作用域中使用类名可忽略模板实参

* 显示指定模板参数：
    > `pair<int, double> p{1, 1.0};`

* 自动推断模板参数：
    > `pair p{1, 1.0};`
    * 根据构造函数生成**自动推断向导**以推断模板实参
    * 编译器无法推断嵌套类型或基类类型，需要手写自动推断向导，[详见](https://arne-mertz.de/2017/06/class-template-argument-deduction/)
        > `classname(T1) -> classname<T2::type>;` ，或  
        > `template <typename... T> classname(T...) -> classname<T...>;`  

## 变量模板
> `template <typename T> obj = get_val<T>()`
* 根据模板参数(类型或非类型)来方便地生成变量
* 可以用于[元编程技术](#元编程)来编译期获取值
* 不能作为模板模板参数(一种非类型参数)
>

<!-- need rereead -->
## 概念
* requires表达式
    > `requires (可选的形参列表用于使用目标类型的对象，如t) {要求序列;}`
    * 简单要求：要求表达式能够通过编译
        > `t.mem()`
    * 类型要求：要求目标类型合法
        > `typename T::type`
    * 复合要求：表达式结果作为类型约束的最后一个参数
        > `{简单约束的表达式} (可选)noexcept -> 类型约束`
    * 嵌套要求：相当于使用约束子句进行静态断言
        > `requires子句`

* requires子句：后随常量表达式（限制包括初等表达式、&&、||）
    > requires (概念 && requires表达式 || 布尔常量)

* 概念：一般定义为**requires表达式模板**
    > `template<typename T> concept Concept = Boolean`

* 概念的使用
    * `template<Concept T>`
        > T作Concept的第一个模板参数
    * `template <typename T> requires (Concept<T> && sizeof(T) == 4)`
        > requires子句需要初等表达式（括号内的表达式可强制算作初等表达式）与`&&`、`||`联用
    * `if ( Concept<int> )`

## 模板特例化
* 全特化：
    > `template<> Class<int, int*> {...};`

* 偏特化：
    > `template<typename T> class Class<int, T*> {...};`  
    > `tempalte<typename C, typename... Args> class Class<C(Args..)> {...}`
    * 只适用于类模板
    * 偏特化模板参数列表是原来的子集或特例化版本

* 作用：
    * 为特殊类型的模板提供特殊的定义
    * 元编程的分支语句

## 其它
* 控制实例化<span id="kzslh"></span>
    * 避免因为声明和定义不能分离导致多个文件重复模板实例
    * 实例化声明：
        > `extern  template vector<int>`  
        > 编译器遇到模板的使用就会进行实例，故该声明应该在使用之前
    * 实例化定义
        > `template vector<int>`  
        > 实例化定义会实例化类模板的所有成员，而非只实例化使用的
>

* 转发与移动
    ```cpp
    // 转发的目的即是移动
    template <typename T>
    void wrapper(T&& t)
    {
        real(forward<T>(t));    // 当T为`t` `t&&`类型时，转发为右值
    }
    ```
>

## 编译器类型推断
* auto类型推断与模板类型参数推断： <span id="autolxtd"></span>
    > 注意可能发生推断二义性  
    > 注意`auto t = {1}`会推断`auto`为`initializer_list<int>`
    * 值推断：类似`auto`或`T`
        * 将**引用**视作**对象**
        * 忽略const与volatile
        * 数组与函数退化为指针
        * [类型抵消](#lxdx)

        | 实参类型            | 推断出的实参类型 | 修饰的模板参数 | 左边两者类型抵消后得T的类型 |
        |---------------------|------------------|----------------|-----------------------------|
        | `t或t&`             | t                | T              | T = t                       |
        | `const t或const t&` | t                | T              | T = t                       |
        | `std::move(t)`      | t                | T              | T = t                       |

    * 引用推断：类似`auto&`或`T&`
        * 将**对象**视作**引用**
        * 保留const与volatile
        * 保留数组与函数
        * [类型抵消](#lxdx)

        | 实参类型            | 推断出的实参类型 | 修饰的模板参数 | 左边两者类型抵消后得T的类型 |
        |---------------------|------------------|----------------|-----------------------------|
        | `t或t&`             | t&               | T&             | T = t                       |
        | `t或t&`             | t&               | const T&       | T = t                       |
        | `const t或const t&` | const t&         | T&             | T = const t                 |
        | `std::move(t)`      | t&               | T&             | T = t                       |

    * 万能引用推断
        * 将左值对象视作引用，右值对象视作对象
        * 保留const与volatile
        * 保留数组与函数
        * [类型抵消](#lxdx)与[引用折叠](#yyvd)

        | 实参类型            | 推断出的实参类型 | 修饰的模板参数 | 左边两者类型抵消后得T的类型 |
        |---------------------|------------------|----------------|-----------------------------|
        | `t或t&`             | t&               | T&&            | T = t&                      |
        | `const t或const t&` | const t&         | T&&            | T = const t&                |
        | `std::move(t)`      | t                | T&&            | T = t                       |
>
* decltype型推断：
    * 保留const与volatile
    * 保留数组与函数
    * 保留引用类型
    * 对于[返回左值的表达式](#左值与右值)会推断为引用类型
    * 双层括号显式指定为引用
    > 作用：
    > * 尾式返回类型`auto func(auto t) -> decltype(t)`
    > * 与auto合用`decltype(auto) func()`
>
* 类型抵消<span id="lxdx"></span>  
    * 广义上的修饰符：`[]`、`()`、`<>`、`*`、`&`、`const`、`volatile`  
    工作原理即是：
        > 拿万能引用第二条举例，**修饰的模板参数类型**为T&&，**假想推断出的实参类型**为t&
    1. （去重）先将两者都有的修饰符抵消掉，`T&&`与`t&`无重叠部分
    2. （减多）再忽略前者中比后者中多余的修饰符，即忽略`T&&`中的`&&`
    3. （结果）此时前者必定只剩下了`T`，而后者为`t&`，即推断得`T = t&`
    4. （合并）将第2条中忽略的部分补上，则完整的形参实例即是`t& &&`，进行引用折叠后得`t&`
>
* 引用折叠 <span id="yyvd"></span>
    * `t& &&` => `t&`
    * `t& &`  => `t&`
    * `t&& &` => `t&`
    * `t&& &&`=> `t&&`

## 元编程

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
* 循环：
    * 变参递归，设计参数个数固定的特化模板结束递归
    * 非变参递归，设计最终结束状态的特化模板结束递归
* 分支：
    * 利用特例化的偏序规则进行分支选择
    * 利用`if constexpr (CONSTEXPR_INT)`来实现编译期分支，作用可看作预处理指令`#if CONDITION`，
        只不过前者如果出现在模板之外，仍然会对被丢弃的语句进行语法检测
* 顺序：
    * using类型别名的顺序

# 其他特性

## 内存分配
* `new`、`new[]`、`delete`、`delete[]`
* 定位new：
    * `new (ptr) Type{}`
    * `new (std::nothrow) Type{}`
* new表达式：
    1. 分配内存
    2. 构造对象
    3. 返回指针
* delete表达式：
    1. 销毁对象
    2. 释放内存
* 重载new与delete操作符，重载为类成员隐式static
    * `void* operator new(size_t)`
    * `void* operator new(size_t，nothrow_t&) noexcept`
    * `void* operator new(size_t，align_val_t)`
* 数据对齐：
    * alignas(32)
    * alignof(obj)

## 成员指针
* `Type Class::* mp {Class.member}`
* `Class.*mp或Classptr->*mp`

## volatile
* 告诉编译器不要进行存取优化，程序始终从内存中取变量的地址，而不是从高速缓存或寄存器中取变量的值

# 附
* 关键字、标识符、注释、预处理器宏
* 实体：实体包括值、对象、引用、结构化绑定、函数、枚举项、类型、类成员、模板、模板特化、形参包和命名空间。
* 声明与定义：声明可以引入实体，将它们与名字关联起来，并定义其属性。能够定义使用一个实体所需的所有属性的声明是定义。
* 变量：被声明的且不是非静态数据成员的对象和引用是变量。（主要实体）
* 函数：函数的定义通常包括一系列的语句，其中部分会包含表达式。（主要实体）
* 语句与表达式：表达式指定了程序需要进行的运算。（程序核心）
