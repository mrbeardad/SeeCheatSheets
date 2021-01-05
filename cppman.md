***注意，该文档主要用于通过本仓库的see命令进行终端查询，故未太注重渲染后的排版与美观***
# 目录
<!-- vim-markdown-toc GFM -->

- [C++标准库](#c标准库)
  - [错误处理](#错误处理)
    - [异常体系结构](#异常体系结构)
    - [异常对象使用](#异常对象使用)
  - [内存管理](#内存管理)
    - [〈new〉](#new)
    - [〈memory〉](#memory)
  - [工具库](#工具库)
    - [〈cstdlib〉](#cstdlib)
    - [〈functional〉](#functional)
    - [〈chrono〉](#chrono)
    - [〈initializer_list〉](#initializer_list)
    - [〈utility〉](#utility)
    - [〈tuple〉](#tuple)
    - [〈any〉](#any)
    - [〈variant〉](#variant)
    - [〈optional〉](#optional)
    - [〈bitset〉](#bitset)
  - [数值库](#数值库)
    - [〈limits〉](#limits)
    - [〈ratio〉](#ratio)
    - [〈cmath〉](#cmath)
    - [〈numeric〉](#numeric)
    - [〈random〉](#random)
  - [字符处理](#字符处理)
    - [〈cctype〉](#cctype)
    - [〈cwctype〉](#cwctype)
    - [〈string〉](#string)
    - [〈string_view〉](#string_view)
    - [〈format〉](#format)
  - [容器库](#容器库)
    - [构造](#构造)
    - [赋值](#赋值)
    - [访问](#访问)
    - [插入](#插入)
    - [删除](#删除)
    - [容量](#容量)
    - [关联与无序容器](#关联与无序容器)
    - [无序容器](#无序容器)
    - [链表容器](#链表容器)
  - [迭代器库](#迭代器库)
    - [辅助函数](#辅助函数)
    - [流迭代器](#流迭代器)
    - [反向迭代器](#反向迭代器)
    - [移动迭代器](#移动迭代器)
    - [插入迭代器](#插入迭代器)
  - [算法库](#算法库)
    - [执行策略](#执行策略)
    - [非更易算法](#非更易算法)
    - [更易算法](#更易算法)
    - [搜索算法](#搜索算法)
    - [划分算法](#划分算法)
    - [变序算法](#变序算法)
    - [集合算法](#集合算法)
    - [极值算法](#极值算法)
  - [正则表达式库](#正则表达式库)
  - [输入输出库](#输入输出库)
    - [总览](#总览)
    - [基础操作](#基础操作)
    - [非格式输入输出](#非格式输入输出)
    - [预定义操作](#预定义操作)
    - [操作符](#操作符)
    - [文件流](#文件流)
    - [字符流](#字符流)
    - [流缓冲区](#流缓冲区)
    - [本地环境](#本地环境)
  - [并发库](#并发库)
    - [线程启动](#线程启动)
    - [线程控制](#线程控制)
    - [线程同步](#线程同步)
    - [并发实例](#并发实例)
  - [文件系统](#文件系统)
    - [文件信息](#文件信息)
    - [目录](#目录)
    - [符号链接](#符号链接)
    - [硬链接](#硬链接)
    - [权限](#权限)
    - [类](#类)
    - [函数](#函数)
- [BOOST库](#boost库)
  - [字符编码转换](#字符编码转换)
  - [序列化](#序列化)
- [nlohmann-json](#nlohmann-json)
  - [序列化](#序列化-1)
  - [反序列](#反序列)
  - [自定义变量转换](#自定义变量转换)
- [Mysql++](#mysql)
  - [异常](#异常)
  - [连接](#连接)
  - [SQL语句执行](#sql语句执行)

<!-- vim-markdown-toc -->
注：代码块中的示例为伪代码
# C++标准库
## 错误处理
<!-- entry begin: exception stdexcept 标准库异常 异常体系结构 -->
### 异常体系结构
```
exception                        <exception>
├─── bad_typeid                  <typeinfo>      ：typeid()接受解引用的多态类型的空指针
├─── bad_cast                    <typeinfo>      ：dynamic_cast<>()转换多态类型的引用失败
│    └─── bad_any_cast           <any>           ：any_cast<>()转换any类型出错
├─── bad_variant_access          <variant>       ：get<>()转换variant类型出错
├─── bad_weak_ptr                <memory>        ：shared_ptr构造时接受失效的weak_ptr
├─── bad_function_call           <functional>    ：function::operator()()调用时未包装目标
├─── bad_alloc                   <new>           ：内存申请失败
├─── bad_array_new_length        <new>           ：new表达式接受非法长度
├─── logic_error                 <stdexcept>
│    ├─── invalid_argument       <stdexcept>     ：bitset、stoi、stof
│    ├─── domain_error           <stdexcept>     ：一般供第三方数学库使用
│    ├─── length_error           <stdexcept>     ：basic_string、vector::reserve
│    ├─── out_of_range           <stdexcept>     ：bitset、basic_string、stoi、stod、边界检查
│    ├─── bad_optional_access    <optional>      ：optional::value
│    └─── future_error           <future>        ：future、promise
└─── runtime_error               <stdexcept>
     ├─── range_error            <stdexcept>     ：wstring_convert::from_bytes、wstring_convert::to_bytes
     ├─── overflow_error         <stdexcept>     ：bitset::to_ulong、bitset::to_ullong
     ├─── underflow_error        <stdexcept>     ：一般供第三方数学库使用
     ├─── regex_error            <regex>         ：正则表达式库。成员函数code()返回regex_constants::error_type
     ├─── nonexistent_local_time <chrono>
     ├─── ambiguous_local_time   <chrono>
     ├─── format_error           <format>        ：格式化库中函数出错
     └─── system_error           <system_error>  ：操作系统相关异常
          ├─── filesystem_error  <filesystem>    ：文件系统库中函数失败。成员函数path1()与path2()
          └─── ios_base::failure <ios>           ：输入输出库中函数失败（需手动设置）
```
<!-- entry end -->

<!-- entry begin: what code 异常挂起 异常构造 异常成员 -->
### 异常对象使用
```cpp
// 构造异常对象
logic_error(const string&);
logic_error(const char*);
runtime_error(const string&);
runtime_error(const char*);
system_error(error_code);
system_error(error_code, const string&);
system_error(error_code, const char*);
error_code make_error_code(errc);

// 异常成员函数
/*
 * 该函数为异常基类std::exception所声明的虚函数。
 * 返回的字符串指针保证在异常对象销毁前，或在调用异常对象的非静态成员函数前合法
*/
C char*     what();
/*
 * 适用于system_error及其派生类，与future_error
 * error_code是依赖平台的错误码，error_condition是可移植的错误码
*/
error_code& code();
ec.clear()
ec.message()
ec.category().name()
ec.value()
ec.default_error_condition().message()
ec.default_error_condition().category().name()
ec.default_error_condition().value()
#include <cerrno>
ec <=> errc::MEM;
#include <ios>
ec <=> io_errc::MEM;
#include <future>
ec <=> future_errc::MEM;

exception_ptr   current_exception()         // 挂起当前的异常
void            rethrow_exception(exceptr)  // 重抛挂起的异常
```
<!-- entry end -->

## 内存管理
<!-- entry begin: new delete get_new_handler set_new_handler -->
### 〈new〉
```cpp
void*   operator new(size_t);
void*   operator new(size_t, align_val_t);
void*   operator new(size_t, user-def-args...);
void*   operator new(size_t, align_val_t, user-def-args...);
void*   operator new(size_t, nothrow_t);
void*   operator new(size_t, align_val_t, nothrow_t);
void*   operator new(size_t, void*);
// new[]版本参数同上
void    operator delete(void*);
void    operator delete(void*, size_t);
void    operator delete(void*, align_val_t);
void    operator delete(void*, size_t, align_val_t);
void    operator delete(void*, args...);
void    operator delete(void*, nothrow_t);
void    operator delete(void*, align_val_t, nothrow_t);
void    operator delete(void*, void* place);
// delete[]版本参数同上

new_handler get_new_handler();
new_handler set_new_handler(nh);
```
<!-- entry end -->

<!-- entry begin: memory unique_ptr shared_ptr weak_ptr 智能指针 -->
### 〈memory〉
```cpp
class unique_ptr<T, Deleter = default_delete<T> >
{
    // 构造函数：支持move，拒绝copy
    unique_ptr(ptr);
    unique_ptr(ptr, del);
    // 修改器
    pointer     release();
    void        reset(ptr = nullptr);
    // 观察器
    pointer     get();
    Deleter&    get_deleter();
    // OP
    operator bool
    operator*
    operator->
    // 非成员函数
    unique_ptr  make_unique<T>(args...);
};

class shared_ptr<T>
{
    // 构造函数
    shared_ptr(ptr);
    shared_ptr(ptr, del);
    shared_ptr(wptr);
    shared_ptr(uptr);
    // 修改器
    void        reset();
    void        reset(ptr);
    void        reset(ptr, del);
    // 观察器
    T*          get();
    long        use_count();
    bool        owner_before(sptr);
    bool        owner_before(wptr);
    // OP
    operator*
    operator->
    // 非成员函数
    shared_ptr  make_shared<T>(args...);
    shared_ptr  static_pointer_cast<T>(sptr);
    shared_ptr  const_pointer_cast<T>(sptr);
    shared_ptr  reinterpret_pointer_cast<T>(sptr);
    shared_ptr  dynamic_pointer_cast<T>(sptr);
    shared_ptr  enable_shared_from_this<T>::shared_from_this();
    weak_ptr    enable_shared_from_this<T>::weak_from_this();
};

class weak_ptr<T>
{
    // 构造函数
    weak_ptr(sptr);
    // 修改器
    void        reset();
    long        use_count();
    bool        expired();
    shared_ptr  lock();
    bool        owner_before(sptr);
    bool        owner_before(wptr);
};
```
<!-- entry end -->

## 工具库
<!-- entry begin: cstdlib -->
### 〈cstdlib〉
```cpp
// 进程控制
void    abort();                                        // 异常终止进程，不进行清理
void    exit(int exit_code);                            // 正常终止进程，进行清理
void    quick_exit(int exit_code);                      // 正常终止进程，进行非完全清理
void    _Exit(int exit_code);                           // 异常终止进程，不进行清理
int     atexit(void(*func)());                          // 注册在调用exit()时被调用
int     at_quick_exit(void(*func)());                   // 注册在调用quick_exit()时被调用
int     system(const char* cmd);                        // 调用宿主环境命令处理器
char*   getenv(const char* env_var);                    // 访问环境变量
// 内存管理
void*   malloc(size_t size);                            // 分配内存
void*   aligned_alloc(size_t alignment, size_t size);   // 分配对齐的内存
void*   calloc(size_t num, size_t size);                // 分配并清零内存
void*   realloc(void* ptr, size_t new_size);            // 重新分配已分配内存
void    free(void* ptr);                                // 释放已分配内存
// 数值字符串转换
double  atof(const char* str);
int     atoi(const char* str);
long    strtol(const char* str, char** str_end, int base);
double  strtod(const char* str, char** str_end);
```
<!-- entry end -->

<!-- entry begin: functional hash function reference_wrapper ref cref -->
### 〈functional〉
```cpp
struct hash<T>
{
    // 特化包括有：
    // 整型、浮点型、指针、
    // 智能指针、string族、string_view族、bitset、vector<bool>、
    // error_code、error_condition、type_index、thread::id、optional、variant
};

class function<F(Args...)>
{
    // 成员函数
    operator()
    operator bool
};

class reference_wrapper<T>
{
    // 成员函数
    T& get();
    // OP
    operator()  // 调用存储的函数
    operator T&
};

referece_wrapper<T>         ref(T& t);
referece_wrapper<const T>   cref(T& t);
```
<!-- entry end -->

<!-- entry begin: chrono 时间库 -->
### 〈chrono〉
```cpp
class system_clock
{
    // 成员类型
    rep
    period
    duration    = duration<rep, period>
    time_point  = time_point<system_clock>
    // 成员函数
    S time_point    now();
    S time_t        to_time_t(time_point);
    S time_point    frome_time_t(time_t);
};

class system_clock
{
    // 成员类型
    rep
    period
    duration    = duration<rep, period>
    time_point  = time_point<steady_clock>
    // 成员函数
    S time_point    now();
};

class duration<Rep, Period = ratio<1> >
{
    // 成员类型
    rep
    period
    // 成员函数
    rep             count();
    S duration      zero();
    S duration      min();
    S duration      max();
    // Operator
    operator++
    operator--
    operator+
    operator-
    operator*
    operator/
    operator%
    operator""h
    operator""min
    operator""s
    operator""ms
    operator""us
    operator""ns
    operator""d
    operator""y
    // 非成员函数
    duration        duration_cast<D>(d);
    duration        floor(d)
    duration        ceil(d)
    duration        round(d)
    duration        abs(d)
};

// 类型别名
std::chrono::nanoseconds
std::chrono::microseconds
std::chrono::milliseconds
std::chrono::seconds
std::chrono::minutes
std::chrono::hours
std::chrono::days
std::chrono::weeks
std::chrono::months
std::chrono::years

class time_point<Clock, Duration = Clock::duration>
{
    // 成员类型
    rep
    period
    duration
    clock
    // 成员函数
    duration        time_since_epoch();
    S duration      min();
    S duration      max();
    // Operator
    operator+
    operator-
    // 非成员函数
    time_point      time_point_cast<TP>(tp);
    time_point      floor(tp);
    time_point      ceil(tp);
    time_point      round(tp);
};
```
<!-- entry end -->

<!-- entry begin: initializer_list -->
### 〈initializer_list〉
```cpp
class initializer_list<T>
{
    // 构造函数
    initializer_list(); // 语言特性支持的列表初始化的默认类型
    // 成员函数
    size_t      size();
    const T*    begin();
    const T*    end();
};
```
<!-- entry end -->

<!-- entry begin: utility integer_sequence pair -->
### 〈utility〉
```cpp
struct integer_sequence<T, T... INTS>
{
    // 成员函数
    static size_t size();
    // 辅助模板
    index_sequence<INTS>        = integer_sequence<size_t, INTS>
    make_integer_sequence<T, N> = integer_sequence<T, 0..N-1>
    make_index_sequence<N>      = integer_sequence<size_t, 0..N-1>
};

class pair<T1, T2>
{
    // 成员对象
    T1 first;
    T2 second;
    // 构造函数
    pair();                             // 默认构造
    pair(x, y);                         // 类聚合式构造
    pair(pair<U1, U2>);                 // 成员模板构造
    pair(                               // 逐块式构造
        std::piecewise_construct,
        tuple<Args1...> first_args,
        tuple<Args2...> second_args
    );
    // 非成员函数
    pair    make_pair(x, y);            // 被C++17结构化绑定取代
    T&      get<size_t>(p);
    T&      get<T>(p);
    // 辅助类
    tuple_size<pair>::value
    tuple_element<size_t, pair>::type
};
```
<!-- entry end -->

<!-- entry begin: tuple -->
### 〈tuple〉
```cpp
class tuple<Types...>
{
    // 构造函数
    tuple();                            // 默认构造
    tuple(args...);                     // 类聚合式构造
    tuple(tuple<UTypes...>);            // 成员模板构造
    tuple(p);                           // pair转换构造
    // 非成员函数
    tuple   make_tuple(args...);        // 被C++17结构化绑定取代
    tuple&  tie(args...);               // std::ignore作占位符。被C++17结构化绑定取代
    tuple   tuple_cat(tuples...);
    T&      get<size_t>(t);
    T&      get<T>(t);
    // 辅助类
    tuple_size<tuple>::value
    tuple_element<size_t, tuple>::type
};
```
<!-- entry end -->

<!-- entry begin: any -->
### 〈any〉
```cpp
class any
{
    // 构造函数
    any();                          // 默认构造。空对象
    any(value);                     // 类聚合式构造
    any(                            // 就地构造
        std::in_place_type<Type>,
        args...
    );
    any(
        std::in_place_type<Type>,
        il, args...
    );
    // 修改器
    T&      emplace<T>(args...);
    T&      emplace<T>(il, args...);
    void    reset();
    // 观察器
    bool    has_value();
    type_info&  type();
    // 非成员函数
    T       any_cast<T>(any&);
    T*      any_cast<T>(any*);
};
```
<!-- entry end -->

<!-- entry begin: variant -->
### 〈variant〉
```cpp
class variant<Types...>
{
    // 构造函数
    variant();                  // 默认构造第一个类型。可用std::monostate作占位符类型
    variant(t);                 // 匹配构造最佳类型
    variant(                    // 就地构造
        std::in_place_type<T>,  // 也可为std::in_place_index<size_t>
        args...
    );
    variant(
        std::in_place_type<T>,
        il, args...
    );
    // 修改器
    T&      emplace<T>(args...);
    T&      emplace<T>(il, args...);
    T&      emplace<size_t>(args...);
    T&      emplace<size_t>(il, args...);
    // 观察器
    size_t  index();
    bool    valueless_by_exception();   // 发生异常时，index()返回std::variant_npos
    // 非成员函数
    T&      get<size_t>(vrt);           // 转换失败抛出std::bad_variant_access
    T&      get<T>(vrt);
    T*      get_if<size_t>(vrt);        // 转换失败返回空指针
    T*      get_if<T>(vrt);
    R       visit(visitor, vrt);        // visitor内部可利用if constexpr
    // 辅助类
    variant_size<variant>::value
    variant_alternative<size_t, variant>::type
};
```
<!-- entry end -->

<!-- entry begin: optional -->
### 〈optional〉
```cpp
#include <optional>

class optional<T>
{
    // 构造函数
    optional();                             // 默认构造为std::nullopt
    optional(val);                          // 类聚合式构造
    optional(std::in_place, args...);       // 就地构造
    optional(std::in_place, il, args...);
    // 修改器
    T&      emplace(args...);
    T&      emplace(il, args...);
    void    reset();
    // 观察器
    operator->
    operator*
    C T&    value();                        // 若为std::nullopt则抛出std::bad_optional_access
    T       value_or(def_val);
    bool    has_value();
    // OP
    operator bool
};
```
<!-- entry end -->

<!-- entry begin: bitset -->
### 〈bitset〉
```cpp
class bitset<size_t>
{
    // 构造函数
    bitset();
    bitset(ulong);
    bitset(str, pos = 0, nbits = npos, zero = '0', one = '1');
    bitset(Cstr, nbits = npos, zero = '0', one = '1');
    // 元素访问
    bool        test(pos);
    bool        all();
    bool        any();
    bool        none();
    size_t      count();
    size_t      size();
    // 修改器
    bitset&     set();
    bitset&     set(pos, boolean = true);
    bitset&     reset();
    bitset&     reset(pos);
    bitset&     flip();
    bitset&     flip(pos);
    // 转换
    string          to_string(zero = '0', one = '1');
    unsigned long   to_ulong(); // 存储时低位在“左”，显示时低位在“右”（小端）
    // OP
    operator[]                  // 可能返回reference，支持operator=、operator~、operator bool
    operator&
    operator|
    operator^
    operator~
    operator<<
    operator>>
};
```
<!-- entry end -->

## 数值库
<!-- entry begin: limits 数值极限 -->
### 〈limits〉
```cpp
class numeric_limits<T>
{
    // 常用静态成员常量
    S radix             // 给定类型的表示所用的基或整数底
    S digits            // 能无更改地表示的 radix 位数
    S digits10          // 能无更改地表示的十进制位数
    S max_digits10      // 区别所有此类型值所需的十进制位数
    S max_exponent10    // 10 的该数次幂是合法有限浮点值的最大整数
    S min_exponent10    // 10 的该数次幂是合法正规浮点值的最小负数
    // 常用静态成员函数
    S lowest()          // 最低有限值
    S min()             // 最小有限值。浮点数为最小正数
    S max()             // 最大有限值
    S epsilon()         // 返回 1.0 与给定类型的下个可表示值的差
    S round_error()     // 返回给定浮点类型的最大舍入误差
    S infinity()        // 返回给定类型的正无穷大值
    S quiet_NaN()       // 返回给定浮点类型的安静 NaN 值
    S signaling_NaN()   // 返回给定浮点类型的发信的 NaN
    S denorm_min()      // 返回给定浮点类型的最小正非正规值
};
```
<!-- entry end -->

<!-- entry begin: ratio 分数 -->
### 〈ratio〉
```cpp
class ratio<Num, Den = 1>
{
    // 成员对象
    S intmax_t num;
    S intmax_t den;
    // 成员类型
    pico
    nano
    micro
    centi
    deci
    deca
    hecto
    kilo
    mega
    giga
    tera
    peta
    // 辅助类
    ratio_add           <r1, r2>::type
    ratio_subtract      <r1, r2>::type
    ratio_multiply      <r1, r2>::type
    ratio_divide        <r1, r2>::type
    ratio_equal         <r1, r2>::value
    ratio_not_equal     <r1, r2>::value
    ratio_less          <r1, r2>::value
    ratio_less_equal    <r1, r2>::value
    ratio_greater       <r1, r2>::value
    ratio_greater_equal <r1, r2>::value
};
```
<!-- entry end -->

<!-- entry begin: cmath 数学库 -->
### 〈cmath〉
```cpp
// 基本运算
f   abs(f);             // 求绝对值
f   fmod(x, y);         // 求 x/y（向零取整）的余数
f   remainder(x, y);    // 求 x/y（四舍五入）的余数
f   fma(x, y, z);       // 求 x*y+z
f   fmax(x, y);         // 求 x,y 中较大者（忽略NAN）
f   fmin(x, y);         // 求 x,y 中较小者（忽略NAN）
f   dim(x, y);          // 求 max(0,x-y)
fl  nanf(Cstr);         // Cstr设置NAN的尾数
db  nan(Cstr);
ld  nanl(Cstr);
// 线性插值
f   lerp(a, b, t);      // 求 a+t(b-a)
// 指数函数
f   log(x);             // 求 ln(x)
f   log1p(x);           // 求 ln(1+x)
f   log2(x);            // 求 log₂(x)
f   log10(x);           // 求 log₁₀(x)
f   exp(x);             // 求 eˣ
f   expm1(x);           // 求 eˣ-1
f   exp2(x);            // 求 2ˣ
// 幂函数
f   pow(x, y);          // 求 xʸ
f   sqrt(x);            // 求 √x
f   cbrt(x);            // 求 ³√x
f   hypot(x);           // 求 √(x²+y²)
// 三角函数
f   sin(x);             // 求 sin(x)
f   cos(x);             // 求 cos(x)
f   tan(x);             // 求 tan(x)
f   asin(x);            // 求 arcsin(x) 在[-π/2, π/2]
f   acos(x);            // 求 arccos(x) 在[0, π]
f   atan(x);            // 求 arctan(x) 在[-π/2, π/2]
f   atan2(y, x);        // 求 arctan(y/x)在[-π, π]
// 误差与伽马函数
f   erf(arg);           // 误差函数
f   erfc(arg);          // 补误差函数
f   tgamma(arg);        // gamma函数
f   lgamma(arg);        // gamma函数的自然对数
// 浮点取整
f   ceil(f);            // 向上取整
f   floor(f);           // 向下取整
f   trunc(f);           // 向零取整
i   round(f);           // 四舍五入
// 浮点操作
f   frexp(f, iptr);     // 将 f 分解为 n * 2ᵉˣᵖ，n ∈ [0.5, 1)。exp存于iptr并返回n
f   ldexp(f, i);        // 求 f * 2ᵉˣᵖ
f   modf(f, iptr);      // 将 f 分解为整数与小数。整数存于iptr并返回小数
f   scalbn(x, exp);     // 求 x * 2ᵉˣᵖ
i   ilogb(f);           // 求 log₂|f|
f   logb(f);            // 求 log₂|f|
f   nextafter(from, to);// 求 from 趋向 to 的下个可表示的浮点值
f   copysign(x, y);     // 求 x 的模与 y 的符号组成的浮点值
```
<!-- entry end -->

<!-- entry begin: numeric 数值库 算法 -->
### 〈numeric〉
```cpp
void    iota(b, e, v);                                      // e = v++
T       accumulate(b, e, init, bOp = plus);                 // bOp(e)
T       reduce(b, e, init = 0, bOp = plus);                 // bOp(e)。支持policy(默认无序)
T       transform_reduce(b1, e1, init, bOp, uOp);           // bOp(uOp(e))
T       transform_reduce(b1, e1, b2, init,                  // bOp1(bOp2(e1, e2))
    bOp1 = plus, bOp2 = muiltiplies);
T       inner_product(b1, e1, b2, init, bOp1, bOp2);        // bOp1(bOp2(e1, e2))。支持policy(默认无序)
destE   adjacent_difference(b, e, destB, bOp = reduce);     // dE = e - em1
destE   partial_sum(b, e, destB, bOp = plus);               // dE = bOp(e)
destE   inclusive_scan(b, e, destB, bOp = plus);            // dE = bOp(e)
destE   exclusive_scan(b, e, destB, bOp = plus);            // dE = bOp(*p2e--)
destE   transform_inclusive_scan(b, e, destB, bOp = plus);  // dE = bOp(uOp(e))
destE   transform_exclusive_scan(b, e, destB, bOp = plus);  // dE = bOp(uOp(em1))
T       gcd(m, n);                                          // 求最大公因数
T       lcm(m, n);                                          // 求最小公倍数
T       midpoint(a, b);                                     // 求中间值
```
<!-- entry end -->

<!-- entry begin: random 随机数 随即引擎 随机分布 -->
### 〈random〉
```cpp
// 常用引擎：成员函数seed(val)作种
minstd_rand
mt19937_64
ranlux48
knuth_b
default_random_engine
// 常用分布：重载操作operator()(engine)返回符合分布的随机数
uniform_int_distribution(min=0, max=INTMAX) // min-max的均匀整数分布
uniform_real_distribution(min=0, max=1.0)   // min-max的均匀实数分布
bernoulli_distribution(p=0.5)               // 0-1分布，返回bool
binomial_distribution(n=1, p=0.5)           // 二项分布
normal_distribution(u=0, o=1)               // 正态分布
```
<!-- entry end -->

## 字符处理
<!-- entry begin: cctype -->
### 〈cctype〉
```cpp
bool isalnum(c);
bool isalpha(c);
bool islower(c);
bool isupper(c);
bool isdigit(c);
bool isxdigit(c);
bool ispunct(c);
bool isblank(c);
bool isspace(c);
bool iscntrl(c);
bool isgraph(c);
bool isprint(c);
int  toupper(c);
int  tolower(c);
```
<!-- entry end -->

<!-- entry begin: cwctype -->
### 〈cwctype〉
```cpp
bool iswalnum(c);
bool iswalpha(c);
bool iswlower(c);
bool iswupper(c);
bool iswdigit(c);
bool iswxdigit(c);
bool iswpunct(c);
bool iswblank(c);
bool iswspace(c);
bool iswcntrl(c);
bool iswgraph(c);
bool iswprint(c);
int  towupper(c);
int  towlower(c);
```
<!-- entry end -->

<!-- entry begin: string -->
### 〈string〉
```cpp
class string
{
/*
 * 目标：(str, pos = 0, len = npos) (cstr, len = auto) (char) (n, char)
 * 此处不包括STL接口
*/
    // 构造函数
    string(目标)
    // 修改
    string& assign(目标)
    string& append(目标)
    string& operator=(str) (cstr) (char)
    string& operator+(str) (cstr) (char)
    string& operator+=(str) (cstr) (char)
    string& insert(pos, 目标)               // 目标除开(char)
    string& replace(pos, len, 目标)
    // 查找
    bool    starts_with(str) (cstr) (char)
    bool    end_with(str) (cstr) (char)
    bool    contains(str) (cstr) (char)
    size_t  find(str, pos = 0) (cstr, pos = 0, len = auto) (char, pos = 0)
    size_t  rfind(str, pos = 0) (cstr, pos = 0, len = auto) (char, pos = 0)
    size_t  find_first_of(str, pos = 0) (cstr, pos = 0, len = auto) (char, pos = 0)
    size_t  find_first_not_of(str, pos = 0) (cstr, pos = 0, len = auto) (char, pos = 0)
    size_t  find_last_of(str, pos = 0) (cstr, pos = 0, len = auto) (char, pos = 0)
    size_t  find_last_not_of(str, pos = 0) (cstr, pos = 0, len = auto) (char, pos = 0)
    // 转换
    C T*    data();
    C T*    c_str();
    string  to_string(v);
    wstring to_string(v);
    int     stoi(str, size_t* = nullptr, base = 10);
    double  stod(str, size_t* = nullptr, base = 10);
    // 比较
    int     compare(pos, len, 目标);        // 目标除开(char) (n, char)
    // 复制
    size_t  copy(dest, len, pos = 0);
    // 子串
    string  substr(pos = 0, len = npos);
    // 容量
    bool    empty();
    size_t  size();
    size_t  length();
    void    resize(count, char = '\0');
    size_t  capacity();
    void    shrink_to_fit();
    void    reserve(new_cap = 0);       // new_cap小于capacity()则无效
};
```
<!-- entry end -->

<!-- entry begin: string_view -->
### 〈string_view〉
```cpp
class string_view
{
    // 构造函数
    string_view(str);
    string_view(cstr, len = auto);
    string_view(beg, end);
    // 修改
    void    remove_prefix(n);
    void    remove_suffix(n);
    // 转换
    // 查找
    // 比较
    // 复制
    // 子串
    // 容量
};
```
<!-- entry end -->

<!-- entry begin: format 格式化 -->
### 〈format〉
> `"{arg_id:填充与对齐 符号 # 0 宽度 精度 L 类型}"`
* arg_id
    > 要么全部默认按顺序，要么全部手动指定
* 填充与对齐
    > 填充与对齐只能一同出现，无填充则默认为空格
    * `<`：左对齐（非整数与非浮点数默认左对齐）
    * `>`：右对齐（整数与浮点数默认右对齐）
    * `^`：居中
* 符号
    * `+`：显示正负号
    * `-`：显示负号（默认）
    * ` `：非负数前导空格
* #
    * 对整数，showbase
    * 对浮点数，showpoint
* 0
    * 对整数与浮点数，用0填充前导空白，若与对齐符号一同使用则失效
* 宽度与精度
    * 宽度：`{:6}`
    * 精度：`{:.6}`
    * 宽度与精度：`{:6.6}`
* L
    * 对整数：插入合适数位分隔符
    * 对浮点数：插入合适数位分隔符与底分隔符
    * 对bool：boolalpha
* 类型
    * `c`字符
    * `s`字符串
    * 整数：
        * `b`与`B`：二进制
        * `o`：八进制
        * `x`与`X`：十六进制
    * 浮点数：
        * `a`与`A`：十六进制
        * `e`与`E`：科学计数法
        * `f`与`F`：定点表示法
        * `g`与`G`：智能表示
<!-- entry end -->

## 容器库
* a : array
* s : string
* v : vector
* d : deque
* l : list
* A : Assoicated
* U : Unordered
* M : all-kinds-of-Map

### 构造
<!-- entry begin: 容器构造 -->
```cpp
// array为聚合类，支持聚合初始化
// 前三条对于A与U都可加额外参数(..., cmpPred)与(..., bnum, hasher, eqPred)
Container()                     // ALL-a
Container(il)                   // ALL-a
Container(beg, end)             // ALL-a
Container(num)                  // ALL-a-s-A-U
Container(num, val)             // ALL-a-A-U
Container(copy)                 // ALL
Container(move)                 // ALL
```
<!-- entry end -->

<!-- entry begin: 容器赋值 -->
### 赋值
```cpp
C&      operator=()             // ALL
C&      assign(il)              // ALL-a-A-U
C&      assign(beg, end)        // ALL-a-A-U
C&      assign(num, val)        // ALL-a-A-U
void    fill(v)                 // a
```
<!-- entry end -->

<!-- entry begin: 容器访问 -->
### 访问
```cpp
T&      at(idx)                 // ALL-L-A-U
T&      at(key)                 // M
T&      operator[] (idx)        // ALL-L-A-U
T&      operator[] (key)        // M
T&      front()                 // ALL-A-U
T&      back()                  // ALL-A-U
T*      data()                  // a+s+v
itr     begin()                 // ALL
itr     cbegin()                // ALL
itr     end()                   // ALL
itr     cend()                  // ALL
itr     rbegin()                // ALL
itr     crbegin()               // ALL
itr     rend()                  // ALL
itr     crend()                 // ALL
```
<!-- entry end -->

<!-- entry begin: 容器元素插入 -->
### 插入
```cpp
1st_ins insert(pos, val)        // ALL-a
1st_ins insert(pos, il)         // s+v+d+l
1st_ins insert(pos, beg, end)   // s+v+d+l
1st_ins insert(pos, num, val)   // s+v+d+l
T&      emplace(pos, args...)   // v+d+l
T&      emplace_back(v)         // v+d+l
T&      emplace_front(v)        // d+l
void    push_back(v)            // s+v+d+l
void    push_front(v)           // d+l

ins     insert(hint, val)       // ALL-a
ins     insert(val)             // A+U（非multi返回pair<ins, bool>）
void    insert(il)              // A+U
void    insert(beg, end)        // A+U
ins     emplace(args...)        // A+U（非multi返回pair<ins, bool>）
ins     emplace_hint(pos,args..)// A+U
```
<!-- entry end -->

<!-- entry begin: 容器元素删除 -->
### 删除
```cpp
void    clear()                 // ALL
ers_num erase(v)                // A+U
ers+1   erase(pos)              // ALL
ers+1   erase(beg, end)         // ALL
void    pop_back()              // ALL-a-A-U
void    pop_front()             // d+l
```
<!-- entry end -->

### 容量
<!-- entry begin: 容器容量 -->
```cpp
bool    empty()                 // ALL
size_t  size()                  // ALL
void    resize(num)             // ALL-a-A-U
void    resize(num, v)          // ALL-a-A-U
size_t  capacity()              // s+v
void    reserve(num)            // s+v+U（只能扩大）
void    shrink_to_fit()         // s+v+d
size_t  max_size()              // ALL
```
<!-- entry end -->

<!-- entry begin: U A 无序 关联 -->
### 关联与无序容器
```cpp
pos     lower_bound(k)          // A
pos     upper_bound(k)          // A
p<b,e>  equal_range(k)          // A
bool    contains(k)             // A+U
size_t  count(k)                // A+U
pos     find(k)                 // A+U
void    merge(C)                // A+U
void    merge(multiC)           // A+U
node_h  extract(pos)            // A+U
node_h  extract(key)            // A+U
class node_h<>
{
    bool empty();
    T&   value();
};
```
<!-- entry end -->

<!-- entry begin: U 无序 -->
### 无序容器
```cpp
size_t  max_bucket_count()
size_t  bucket_count()
bidx    bucket(key)
size_t  bucket_size(bidx)
itr     begin(bidx)
itr     cbegin(bidx)
itr     end(bidx)
itr     cend(bidx)
npb     load_factor()
float   max_load_factor()
float   max_load_factor(float)
void    rehash(bnum)
void    reserve(num)
```
<!-- entry end -->

<!-- entry begin: l 链表 -->
### 链表容器
```cpp
ers_num remove(v)
ers_num remove_if(uOp)
void    sort(cmp = less)
void    unique(cmp = less)
void    merge(srcL, cmp = less)             // 需要先排序
void    splice(pos, srcL, srcPos)
void    splice(pos, srcL, srcBeg, srcEnd)
void    reverse()
```
<!-- entry end -->

## 迭代器库

<!-- entry begin: 迭代器辅助函数 -->
### 辅助函数
```cpp
#include <iterator>
itr     next(itr, n = 1)
itr     prev(itr, n = 1)
itr     distance(itr1, itr2)
void    iter_swap(itr1, itr2)
```
<!-- entry end -->

<!-- entry begin: 流迭代器 -->
### 流迭代器
```cpp
class istream_iterator<T, CharT = char>
{
    // 构造函数
    istream_iterator()      // 默认构造尾后迭代器
    istream_iterator(istrm)
};

class ostream_iterator<T, CharT = char>
{
    // 构造函数
    ostream_iterator(ostrm, const CharT* delim = "")
};

// 流缓冲区迭代器会自动冲刷缓冲区

class istreambuf_iterator<CharT>
{
    // 构造函数
    istreambuf_iterator()       // 默认构造为尾后迭代器
    istreambuf_iterator(istrm)
    istreambuf_iterator(ibuf_ptr)
};

class ostreambuf_iterator<CharT>
{
    // 构造函数
    ostreambuf_iterator(ostrm)
    ostreambuf_iterator(obuf_ptr)
};
```
<!-- entry end -->

<!-- entry begin: 反向迭代器 -->
### 反向迭代器
```cpp
class reverse_iterator<Iter>
{
    // 构造
    // 通过构造函数构造
    reverse_iterator()
    reverse_iterator(itr)
    // 通过辅助函数构造
    ritr make_reverse_iterator(itr)
    // 通过各容器的成员函数构造
    ritr rbegin()
    ritr crbegin()
    ritr rend()
    ritr crend()

    // 成员函数
    itr  base()
}
```
<!-- entry end -->

<!-- entry begin: 移动迭代器 -->
### 移动迭代器
```cpp
class move_iterator<Iter>
{
    // 构造
    // 通过构造函数构造
    move_iterator()
    move_iterator(itr)
    // 通过辅助函数构造
    mitr make_move_iterator(itr)
};
```
<!-- entry end -->

<!-- entry begin: 插入迭代器 -->
### 插入迭代器
```cpp
class insert_iterator<Container>
{
    // 构造
    // 通过构造函数构造
    insert_iterator()
    insert_iterator(Cont, itr)
    // 通过辅助函数构造
    iitr    inserter(Cont, pos)
};
class front_insert_iterator<Container>
{
    // 构造
    // 通过构造函数构造
    front_insert_iterator()
    front_insert_iterator(Cont)
    // 通过辅助函数构造
    fiitr   front_inserter(Cont)
};
class back_insert_iterator<Container>
{
    // 构造
    // 通过构造函数构造
    back_insert_iterator()
    back_insert_iterator(Cont)
    // 通过辅助函数构造
    biitr   back_inserter(Cont)
};
```
<!-- entry end -->

## 算法库
<!-- entry begin: execution policy -->
### 执行策略
```cpp
#include <execution>
// 执行策略常量
std::execution::seq;
std::execution::par;
std::execution::unseq;
std::execution::par_unseq;
```
```txt
┌──────────────────────┬───────────────┬─────────────────┐
│        Policy        │ Vectorization │ Parallelization │
├──────────────────────┼───────────────┼─────────────────┤
│ sequenced            │      X        │         X       │
├──────────────────────┼───────────────┼─────────────────┤
│ unsequenced          │      V        │         X       │
├──────────────────────┼───────────────┼─────────────────┤
│ parallel             │      X        │         V       │
├──────────────────────┼───────────────┼─────────────────┤
│ parallel_unsequenced │      V        │         V       │
└──────────────────────┴───────────────┴─────────────────┘
```
<!-- entry end -->

<!-- entry begin: 非更易算法 -->
### 非更易算法
```cpp
uOp     for_each(b, e, uOp)                     // 返回uOp(已改动过的)拷贝
uOp     for_each_n(b, n, uOp)
size_t  count(b, e, v)
size_t  count_if(b, e, uOp)
bool    all_of(b, e, uOp)
bool    any_of(b, e, uOp)
bool    none_of(b, e, uOp)
p<i,ci> mismatch(b, e, cmpB, bOp=equal_to)      // 返回pair存储两个区间第一个不同点的迭代器
```
<!-- entry end -->

<!-- entry begin: 更易算法 -->
### 更易算法
```cpp
destE   move(b, e, destB)                       // 支持子区间左移
destE   move_backward(b, e, destE)              // 支持子区间右移
destE   copy(b, e, destB)                       // 支持子区间左移
destE   copy_backward(b, e, destE)              // 支持子区间右移
destE   copy_if(b, e, destB, uOp)
destE   copy_n(b, N, destB)

// 删除算法将被删除元素后面的元素前移，并未更改容器大小
newE    remove(b, e, v)
newE    remove_if(b, e, uOp)
destE   remove_copy(b, e, destB, v)
destE   remove_copy_if(b, e, destB, uOp)
newE    unique(b, e, bOp=equal_to)              // 需要先排序
destE   unique_copy(b, e, destB, bOp=equal_to)  // 需要先排序
newE    shift_left(b, e, cnt)                   // 不回绕
newE    shift_right(b, e, cnt)                  // 不回饶

void    replace(b, e, oldV, newV)
void    replace_if(b, e, uOp, newV)
destE   replace_copy(b, e, destB, oldV, newV)
destE   replace_copy_if(b, e, destB, uOp, newV)

void    fill(b, e, v)
void    fill_n(b, N, v)
void    generate(b, e, generator)
void    generate_n(b, N, generator)
destE   transform(b, e, destB, uOp)
destE   transform(b1, e1, b2, destB, bOp)

void    swap(x, y)
destE   swap_ranges(b, e, destB)
```
<!-- entry end -->

<!-- entry begin: 搜索算法 -->
### 搜索算法
```cpp
// 搜索单个元素
tagItr  find(b, e, v)
tagItr  find_if(b, e, uOp)
tagItr  find_if_not(b, e, uOp)

// 搜索子区间
tagItr  search(b, e, seqB, seqE, bOp=equal_to)
tagItr  search_n(b, e, N, v, bOp=equal_to)              // bOp(elem, v)
tagItr  find_end(b, e, seqB, seqE, bOp=equal_to)
tagItr  adjacent_find(b, e, bOp=equal_to)               // 搜索一对连续相等的元素

// 搜索目标范围中的元素
tagItr  find_first_of(b, e, seqB, seqE, bOp=equal_to)

// 二分搜索，需要先排序
tagItr  binary_search(b, e, v, bOp=lower_to)
tagItr  lower_bound(b, e, v, bOp=lower_to)
tagItr  upper_bound(b, e, v, bOp=lower_to)
p<b,e>  equal_range(b, e, v, bOp=lower_to)
```
<!-- entry end -->

<!-- entry begin: 划分算法 -->
### 划分算法
```cpp
bool    is_partitioned(b, e, uOp)
m       partition(b, e, uOp)                                // 返回划分的前半部分的尾后迭代器
m       stable_partition(b, e, uOp)                         // 返回划分的前半部分的尾后迭代器
p<t,f>  partition_copy(b, e, destTrueB, destFalseB, uOp)    // 返回pair分别保存两个目标区间的尾后迭代器
m       partition_point(b, e, uOp)                          // 返回满足uOp()为true的区间的尾后迭代器
```
<!-- entry end -->

<!-- entry begin: 变序算法 -->
### 变序算法
```cpp
bool    is_sorted(b, e, bOp=lower_to)
sortE   is_sorted_until(b, e, bOp=lower_to)                 // 返回已排序区间的尾后迭代器
void    sort(b, e, bOp=lower_to)
void    stable_sort(b, e, bOp=lower_to)
void    partial_sort(b, m, e, bOp=lower_to)
copyE   partial_sort_copy(b, e, destB, destE, bOp=lower_to) // 若dest范围不够则取排序后的前面的元素
void    nth_element(b, m, e, bOp=lower_to)                  // 按m所指的元素作分界进行两边划分

bool    is_heap(b, e, bOp = lower_to)
sortE   is_heap_until(b, e, bOp = lower_to)                 // 返回已堆排序区间的尾后迭代器
void    make_heap(b, e, bOp=lower_to)
void    push_heap(b, e, bOp=lower_to)
void    pop_heap(b, e, bOp=lower_to)
void    sort_heap(b, e, bOp=lower_to)

bool    equal(b, e, cmpB, bOp = equal_to)
bool    lexicographical_compare(b1, e1, b2, e2, op=lower_to)// 比较两区间字典序
bool    is_permutation(b1, e1, b2, bOp=equal_to)            // 检测两个区间的所有元素是否为同一个集合，即不考虑顺序
bool    next_permutation(b, e, op=lower_to)                 // 当元素为完全升序时返回false
bool    prev_permutation(b, e, op=lower_to)                 // 当元素为完全降序时返回false

void    reverse(b, e)
destE   reverse_copy(b, e, destB)
b+e-m   rotate(b, m, e)                                     // 返回原本的begin现在的位置
destE   rotate_copy(b, m, e, destB)

void    shuffle(b, e, randomEngine)
destE   sample(b, e, destB, cnt, randomEngine)              // 随机取cnt个值到destB
```
<!-- entry end -->

<!-- entry begin: 集合算法 -->
### 集合算法
```cpp
// 集合算法均需提前排序
destE   merge(b1, e1, b2, e2, destB, bOp=lower_to)
void    inplace_merge(b, m, e, bOp=lower_to)                            // 将同一个集合中的两部分合并，两部分都有序
bool    includes(b1, e1, b2, e2, bOp=equal_to)                          // 区间`[b2, e2)`是否为区间`[b1, e1)`的**子序列**
destE   set_union(b1, e1, b2, e2, destB, bOp=lower_to)                  // 并集
destE   set_intersection(b1, e1, b2, e2, destB, bOp=lower_to)           // 交集
destE   set_symmetric_difference(b1, e1, b2, e2, destB, bOp=lower_to)   // 并集去交集
destE   set_difference(b1, e1, b2, e2, destB, bOp=lower_to)             // 前一个集合去交集
```
<!-- entry begin: 极值算法 -->
### 极值算法
```cpp
T       max(x, y)
T       max(il)
T       min(x, y)
T       min(il)
pair    minmax(x, y)                        // 返回`pair<min, max>`
pair    minmax(il)                          // 返回`pair<min, max>`
T       clamp(x, min, max)                  // 返回三者中的第二大者
T       min_element(b, e, bOp=lower_to)     // 返回第一个最小值
T       max_element(b, e, bOp=lower_to)     // 返回第一个最大值
T       minmax_element(b, e, bOp=lower_to)  // 返回第一个最小值和最后一个最大值
```
<!-- entry end -->

<!-- entry begin: cpp regex 正则表达式 -->
## 正则表达式库
```cpp
#include <regex>

namespace std::regex_contants
{
    // 即sflag，用于构造regex。默认为ECMAScript
    icase;              // 忽略大小写
    nosubs;             // 标记子表达式（(pat)）无效。同时影响regex::mark_contants()
    optimize;           // 提示优化匹配速度。同时可能增大构造开销
    collate;            // 形如 "[a-b]" 的字符范围将对本地环境敏感
    ECMAScript;         // C++改进版的ECMAScript正则文法
    // 即mflag，用于模式匹配。默认match_default
    match_not_bol       // 行首（^pat）无效
    match_not_eol       // 行尾（pat$）无效
    match_not_bow       // 行首（\bpat）无效
    match_not_eow       // 行尾（pat\b）无效
    match_not_null      // 不匹配空字符序列
    match_any           // 若多于一个匹配可行，则任何匹配都是可接受的结果
    match_continuous    // 仅匹配始于 first 的子串
    match_prev_avail    // --first 是合法的迭代位置。设置时导致 match_not_bol 和 match_not_bow 被忽略
    // 即rflag，用于模式替换。默认format_default
    format_default      // 使用ECMAScript规则进行模式替换
    format_sed          // 使用sed规则进行模式替换
    format_no_copy      // 不输出/返回不匹配的字符
    format_first_only   // 只替换收个匹配
}

class basic_regex<CharT>    // regex wregex
{
    // 构造函数
    basic_regex()
    basic_regex(cstr, sflag)
    basic_regex(cstr, len, sflag)
    basic_regex(str, sflag)
    basic_regex(b, e, sflag)
    basic_regex(il, sflag)
    // 成员函数
    unsigned    mark_count()        // 标记的子表达式数目
    flag        flags()
    locale      getloc()
    locale      imbue()             // 返回之前locale
};

class match_results<BidirIt>    // cmatch wcmatch smatch wsmatch
{
    // 状态
    bool        ready();
    // 容量
    bool        empty();
    size_t      size();
    size_t      max_size();
    // 访问
    sub_match   prefix();           // 返回当前匹配表达式相对整个目标的前缀的sub_match
    sub_match   suffix();           // 返回当前匹配表达式相对整个目标的后缀的sub_match
    sub_match   operator[](n);      // 返回第n个子表达式的sub_match
    itr         begin();
    itr         cbegin();
    itr         end();
    itr         cend();
    String      str(n = 0);         // 返回第n个子表达式的字符串。n=0表示整个表达式
    size_t      length(n = 0);      // 返回第n个子表达式的长度。n=0表示整个表达式
    size_t      position(n = 0);    // 返回第n个子表达式的首字符在目标字符串的位置。n=0表示整个表达式
    // 格式化替换
    outItr      format(outItr, fmtStr, rflag);
    outItr      format(outItr, fmrB, fmtE, rflag);
    string      format(fmtStr, rflag);
    string      format(fmrB, fmtE, rflag);
};

class sub_match<BidirIt>    // csub_match wcsub_match ssub_match wssub_match
{
    // 观察器
    BidirIt     first;      // 子表达式开始
    BidirIt     second;     // 子表达式尾后
    String      str();
    operator    String();
};

class regex_iterator<BidirIt>   // cregex_iterator wcregex_iterator sregex_iterator wsregex_iterator
{
    // 构造函数
    regex_iterator();                           // 默认构造为尾后迭代器
    regex_iterator(b, e, regex, mflag);         // 迭代器在每个匹配区间停留，每次从上次末尾开始匹配（不重合）

    match_results    operator*()                // 返回match_results
};

class regex_token_iterator<BidirIt> // cregex_token_iterator wcregex_token_iteratorsregex_token_iterator wsregex_token_iterator
{
    // 构造函数
    regex_token_iterator();                     // 默认构造为尾后迭代器
    regex_token_iterator(b, e, regex, il, mflag);// il指定关注的regex中的子表达式，0表示全部，-1表示模式取反

    sub_match       operator*()                 // 返回sub_match
};

bool    regex_match(str, [match&,] regex, mflag);
bool    regex_match(b, e, [match&,] regex, mflag);

bool    regex_search(str, [match&,] regex, mflag);
bool    regex_search(b, e, [match&,] regex, mflag);

outItr  regex_replace(outItr, str, regex, fmt, rflag);  // 利用fmt替换掉每个匹配的regex
outItr  regex_replace(outItr, b, e, regex, fmt, rflag);
string  regex_replace(str, regex, fmt, rflag);
string  regex_replace(b, e, regex, fmt, rflag);
// 替换格式化语法：
// $&       替换为regex中整个表达式的匹配
// $0       替换为regex中整个表达式的匹配
// $1, ...  替换为regex中第1个子表达式的匹配
// $'       替换为后缀
// $`       替换为前缀
// $$       转义$
```
<!-- entry end -->

## 输入输出库
### 总览
```cpp
#include <iosfwd>       // 输入输出库中所有类的前置声明
#include <streambuf>    // std::basic_streambuf 类模板
#include <ios>          // std::ios_base类、 std::basic_ios类模板与数个typedef
#include <istream>      // std::basic_istream 、std::basic_iostream 类模板与数个 typedef
#include <ostream>      // std::basic_ostream 类模板与数个 typedef
#include <iostream>     // 数个标准流全局对象。构造需要开销，若不使用则无需引入该头文件
#include <fstream>      // std::basic_fstream 、 std::basic_ifstream 、 std::basic_ofstream 类模板及数个 typedef
#include <sstream>      // std::basic_stringstream 、 std::basic_istringstream 、 std::basic_ostringstream 类模板与数个 typedef
#include <syncstream>   // std::basic_osyncstream 、 std::basic_syncbuf 及 typedef
#include <iomanip>      // 格式化输入与输出的辅助函数
#include <cstdio>       // C 风格输入输出函数

std::ios_base           // (定义了一些标志位)
streambuf               // (系统I/O并缓存数据, 提供位置信息)
locale                  // (包含facet将I/O进行进行本地格式化)
stream                  // (封装上述两者, 提供状态、格式化信息)
centry                  // (帮助stream每次I/O预处理与后处理)
操作符                  // (提供调整stream的便捷方法)
```

<!-- entry begin: iostream -->
### 基础操作
```cpp
class basic_ios<CharT>
{
    bool    good()              // 是否一切正常进行I/O操作
    bool    eof()               // 是否到达文件末尾
    bool    fail()              // 是否发生可恢复错误
    bool    bad()               // 是否发生不可恢复错误
    iostate rdstate()           // 读取状态码
    void    setstate(state)     // 置位状态码
    void    clear(state=goodbit)// 清空并设置状态码

    iostate exceptions()        // 返回触发异常的flag，无则返回ios::goodbit
    void    excptions(flag)     // 设定触发异常的flag

    ostrm*  tie()               // 返回当前关联的流的指针，无则返回nullptr
    ostrm*  tie(ostrm*)         // 关联目标流，返回之前关联流。关联后，任何在*this上的I/O之前，冲刷关联目标

    buf*    rdbuf()             // 返回当前流底层的缓冲区对象指针
    buf*    rdbuf(buf*)         // 设置新的缓冲区并清空状态
    strm&   copyfmt(strm&)      // 复制本地环境、格式化标志、tie()关联目标等信息到本地

    locale  getloc()
    locale  imbue(locale)
    char    narrow(CharT, def)  // 将CharT转换为char并返回，若失败则返回def
    CharT   widen(char)         // 将char转换为CharT
};
bool ios::sync_with_stdio(bool sync = true) // 是否开启多线程同步，以及与C缓冲区的互操
```
<!-- entry end -->

### 非格式输入输出
<!-- entry begin: 底层IO 底层I/O -->
```cpp
class basic_iostream<CharT>
{
    int     get()
    strm&   get(char&)
    strm&   get(char*, count, delim='\n')       // 读取 count - 1 个字符, 并自动添加'\0'在末尾。不包括'\n'
    strm&   get(ostrmbuf&, delim='\n')
    strm&   getline(char*, count, delim='\n')   // 其他同上, 但读取包括delim
    strm&   read(char*, count)                  // count代表指定读取的字符
    size    readsome(char*, count)              // 返回读取字符数, 只从缓冲区中读取已有字符，而不陷入系统调用
    size    gcount()                            // 返回上次无格式读取字符数
    strm&   ignore(count=1, delim=eof)
    int     peek()                              // 返回下个字符, 但不移动iterator
    strm&   unget()                             // 撤销上次读取的字符（回移iterator）
    strm&   putback(char)                       // 将指定字符置入流中
    strm&   sync()

    stm&    put(char)
    strm&   write(char*, count)
    strm&   flush()

    pos     tellp()
    pos     tellg()
    strm&   seekp(pos)
    strm&   seekg(pos)
    strm&   seekp(offset, dir)
    strm&   seekg(offset, dir)
    // dir包括beg、end、cur
};
```
<!-- entry end -->

### 预定义操作
```txt
整数：
     八进制：[0-7]+
     十进制：[0-9]+
     十六进制：(0x|0X)?[0-9a-fA-F]+

浮点型： ([0-9]+\.?[0-9]*|\.[0-9]+)(e[+-]?[0-9]+)

其他：bool, char, char*, void*, string, streambuf*, bitset, complex
```

<!-- entry begin: iomanip -->
### 操作符
```cpp
#include <iomanip>  // 操作符的原型为 strm& iomanip(strm&);
// 输入
ws                  // 立刻丢弃前导空白
noskipws            // 不忽略前导空白
skipws              // 忽略前导空白（默认）
// 输出
endl                // 输出`\n`并刷新缓冲区
ends                // 输出`\0`
flush               // 刷新缓冲区
nounitbuf           // 无需每次输出刷新缓冲区（默认）
unitbuf             // 每次输出都刷新缓冲区（cerr默认）
setfill(char)       // 用char填充setw制造的空白，默认空格
right               // 使用setw()后输出右对齐（默认）
left                // 使用setw()后输出左对齐
internal            // 正负号靠左，数值靠右
noboolalpha         // 布尔值输出0/1（默认）
boolalpha           // 布尔值输出true/false，与locale相关
noshowpos           // 正数无需正号（默认）
showpos             // 正数输出正号
nouppercase         // 数字输出中的字母小写
uppercase           // 数字输出中的字母大写
noshowpoint         // 小数部分为零的浮点数不用打印完整小数部分（默认）
showpoint           // 小数部分为零的浮点数也要打印完整小数部分
noshowbase          // 不用输出进制前缀（默认）
showbase            // 要输出进制前缀
setprecision(v)     // 设置输出浮点数的精度。使用fixed或scientific后精度的语义由“所有数字位数”变为“小数位数”
fixed               // 强制用定点表示法输出浮点数，并打印完整小数位数
scientific          // 强制用科学计数法输出浮点数，并打印完整小数位数
defaultfloat        // 恢复默认，即混合使用定点计数与科学计数
// 输入输出
dec                 // 十进制（默认）
oct                 // 八进制
hex                 // 十六进制
setw(n)             // 设定下次输出的栏宽，或下次输入的字符限制最多n-1个
```
<!-- entry end -->
<!-- entry begin: quoted iomanip -->
```cpp
#include <iomanip>
// 输出时，将str用delim引用起来，并用escape转义其中包含的delim
std::cout << std::quoted(str, delim='"', escape='\\');
// 输入时，将str两边的delim引用取消，并反转义其中用escape转义的delim
std::cin  >> std::quoted(str, delim='"', escape='\\');
```
<!-- entry end -->

### 文件流
<!-- entry begin: fstream -->
```cpp
#include <fstream>
// 类
fstream
ifstream
ofstream
wfstream
wifstream
wofstream

// 构造
basic_fstream(filename, oflag)
in              // 只读              （文件必须存在）
// out          // 清空然后涂写      （有必要则创建）
out|trunc       // 清空然后涂写      （有必要则创建）
out|app         // 追加              （有必要则创建）
// app          // 追加              （有必要则创建）
in|out          // 读写，初始位置为0 （文件必须存在）
in|out|trunc    // 清空然后读/写     （有必要则创建）
in|out|app      // 读写，追加        （有必要则创建）
// in|app       // 读写，追加        （有必要则创建）
binary          // 不将`\r\n`替换为`\n`

// 特有成员函数
bool    is_open()
void    open()
void    close()
```
<!-- entry end -->

<!-- entry begin: sstream stringstream -->
### 字符流
```cpp
#include <sstream>
// 类
stringstream
istringstream
ostringstream
wstringstream
wistringstream
wostringstream
// 构造
stringstream(str)
// 特有成员函数
string_view view()
string      str()
string      str(str)
```
<!-- entry end -->

<!-- entry begin: streambuf -->
### 流缓冲区
```cpp
#include <streambuf>
// 通过文件描述符构造
#include <stdio>
auto fd = fileno(FILE* file);
__gnu_cxx::stdio_filebuf<char> buf{fd, std::ios_base::in};
std::istream istrm{buf};

// 析构函数
// basic_iostream族类析构时不会销毁streambuf
// 其他流类析构时只不析构由rdbuf(buf*)得到的缓冲区

// 高效非格式化输入输出
// 利用streambuf_iterator避开构造sentry进行非格式化I/O
// 利用streambuf*预定义的输入输出操作进行直接I/O
```
<!-- entry end -->

<!-- entry begin: locale -->
### 本地环境
```cpp
#include <locale>
class locale
{
    // 构造函数
    locale()                // 默认构造为std::locale::classic。或为最近一次调用std::locale::global的参数locale
    locale(name)            // 指定语系与字符集。空字符串则设置为当前系统环境
    locale(loc, name, cate) // 复制loc，其中cate所指定的类别修正为与name相同
    // 成员函数
    string  name()          // 若无名则为"*"
    locale  combine()       // 构造 locale 对象，结果为 *this 的副本，除了 Facet 类型平面复制自 other 
  S locale  global(loc)     // 设置全局默认locale
  S locale  classic(loc)    // 返回"C"本地环境
};
```
<!-- entry end -->

## 并发库
### 线程启动
<!-- entry begin: async -->
> 头文件：`<future>`  
> 命名空间：`std::`
* `async(Func, Args...)`                        ：优先异步调用，不可行则延迟发射
* `async(std::launch::async, Func, Args...)`    ：异步调用，失败则抛出异常
* `async(std::launch::deferred, Func, Args...)` ：延迟发射

注释：
* 以下情况调用线程会阻塞直到对应`future`所对应的线程退出：
      * 最后一个`future`副本销毁
      * 对`future`调用`wait()`或`get()`
<!-- entry end -->

* * * * * * * * * *

<!-- entry begin: future shared_future -->
> 头文件：`<future>`  
> 命名空间：`std::`
* `future<ResultType>`
* `shared_future<ResultType>`
特种成员：
* `~future()`                   ：析构时令状态失效
* Move                          ：支持move操作，拒绝copy操作。
* `future()`                    ：构造为无效状态

成员函数：
* `.shared()`                   ：返回`shared_future`继承状态，并令本对象状态失效
* `.valid()`                    ：返回bool表示状态是否有效
* `.get()`                      ：返回对应线程返回值
* `.wait()`                     ：等待对应线程结束
* `.wait_for(duration)`         ：等待对应线程结束
* `.wait_until(time_point)`     ：等待对应线程结束

注释：
* `get()`可获取future的状态（线程的返回值或抛出的异常），只能获取一次然后失效
* `wait_for`与`wait_until`可能返回以下值
    * `std::future_status::deferred`：线程使用延迟发射策略且仍未启动
    * `std::future_status::timeout` ：等待超时
    * `std::future_status::ready`   ：线程已结束
* `shared_future`相对于`future`的区别：
    * 相对`future`，提供了特种成员copy，并取消了成员函数`.shared()`
    * `get()`可多次获取future的状态而不令其失效
<!-- entry end -->

### 线程控制
<!-- entry begin: this_thread thread -->
> 头文件：`<thread>`  
> 命名空间：`std::`
* `thread`
特种成员：
* Move                          ：将原对象设为nonjoinable
* `thread(Func, Args...)`       ：构造并启动线程
成员函数：
* `.joinable()`                 ：返回bool表示该线程是否joinable
* `.join()`                     ：阻塞直至线程结束并将该对象设为nonjoinable。注意销毁一个joinable的`thread`对象时会调用`terminate()`
* `.detach()`                   ：卸离线程
* `.get_id()`                   ：返回TID（真TID）

> 命名空间：`std::this_thread::`
* `get_id()`                    ：返回TID（假TID）
* `sleep_for(duration)`         ：休眠
* `sleep_until(time_point)`     ：休眠
* `yield()`                     ：建议该线程立即被调度
<!-- entry end -->

### 线程同步
<!-- entry begin: mutex 互斥锁 -->
> 头文件：`<mutex>`  
> 命名空间：`std::`
* `mutex`                               ：支持前3个操作
* `timed_mutex`                         ：支持前5个操作
* `recursive_mutex`                     ：支持多次上锁与解锁
* `recursive_timed_mutex`               ：支持多次上锁与解锁，且支持前5个操作
* `shared_mutex`                        ：支持除后2个之外的操作
* `shared_timed_mutex`                  ：支持所有操作

成员函数：
* `.lock()`                             ：获取锁（原子操作：读取-测试-上锁/阻塞）
* `.try_lock()`                         ：尝试获取锁，成功返回true
* `.unlock()`                           ：释放锁
* `.try_lock_for(duration)`             ：尝试获取锁，成功返回true
* `.try_lock_until(time_point)`         ：尝试获取锁，成功返回true
* `.lock_shared()`                      ：释放读锁
* `.unlock_shared()`                    ：释放读锁
* `.try_lock_shared()`                  ：尝试获取锁读锁
* `.try_lock_shared_for(duration)`      ：尝试获取锁读锁
* `.try_lock_shared_until(time_point)`  ：尝试获取锁读锁

全局函数：
* `lock(Mutex...)`                      ：阻塞直至获取所有锁，或解锁已获取的锁并抛出异常（死锁）
* `try_lock(Mutex...)`                  ：若全部获取则返回-1，否则解锁已获取的锁并返回第一个无法获取的锁的次序（加锁次序与实参次序相同且从0开始编号）
* `call_once(once_flag, Func, Args...)` ：根据`once_flag`来判断并只调用一次`func(args...)`

* * * * * * * * * *

* `lock_guard<Mutex>`
* `unique_lock<Mutex>`
* `shared_lock<Mutex>`
特种成员：
* `~Lock()`                     ：释放锁
* Move                          ：支持move操作，但不支持copy操作
* `Lock(Mutex)`                 ：获取锁
* `Lock(Mutex, std::adopt_lock)`：接管已上锁的锁
* `Lock(Mutex, std::defer_lock)`：不上锁
* `Lock(Mutex, std::try_lock)`  ：尝试上锁
* `Lock(Mutex, duration)`       ：尝试上锁
* `Lock(Mutex, time_point)`     ：尝试上锁

成员函数：
* `.lock()`
* `.try_lock()`
* `.unlock()`
* `.try_lock_for()`
* `.try_lock_until()`
* `.owns_lock()`
* `.operator bool()`
<!-- entry end -->

* * * * * * * * * *

<!-- entry begin: cv condition_variable 条件量  -->
> 头文件：`<condition_variable>`  
> 命名空间：`std::`
* `condition_variable`
特种成员：
* `~condition_variable()`
* `condition_variable()`

成员函数：
* `.wait(unique_lock, OP0 = Return_True)`
* `.wait_for(unique_lock, duration, OP0 = Return_True)`
* `.wait_until(unique_lock, time_point, OP0 = Return_True)`
* `.notify_one()`
* `.notify_all()`

全局函数：
* `notify_all_at_thread_exit(condition_variable, unique_lock)`

注释：
* wait时限系列成员函数的无OP0版本的返回值：
    * `std::cv_status::timeout`
    * `std::cv_status::no_timeout`
* 条件量的使用需要互斥锁提供临时保护区，创造条件的线程负责在保护区外调用notify系列函数
* 注意条件量与互斥锁的区别：
    > 需要强调的是，因互斥锁而阻塞的线程由互斥锁解锁时唤醒，而因条件量阻塞的线程需要调用notify系列函数唤醒
    * 互斥锁提供原子操作：读取-检测-上锁/阻塞
    * 条件量提供原子操作：解锁-阻塞
<!-- entry end -->

* * * * * * * * * *

<!-- entry begin: atomic -->
> 头文件：`<atomic>`  
> 命名空间：`std::`
* `atomic<BasicType>`

特种成员：
* `atomic()`                            ：构造时初始化lock

成员函数：
* `.compare_exchange_strong(exp, val)`  ：若`this->load() == exp`，则`this->store(val);return true;`，否则`exp = this->load();return false;`
* `.compare_exchange_weak(exp, val)`    ：同上，但可能假失败，也可能更高效
* `.load()`                             ：返回原值拷贝
* `.store(val)`                         ：赋值val
* `.exchange(val)`                      ：赋值val并返回旧值拷贝
* `.operator=(val)`                     ：赋值val并返回新值拷贝
* `++a, a++`
* `--a, a--`
* `a += val`
* `a -= val`
* `a &= val`
* `a |= val`
* `a ^= val`
<!-- entry end -->

### 并发实例
<!-- entry begin: 并发实例 -->
```cpp
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>

namespace
{
    std::mutex Mx{};
    std::condition_variable Cv{};
    std::queue<int> Que{};

    void consumer();
    void producer();
} // namespace

int main()
{
    auto f0 = std::async(consumer);
    auto f1 = std::async(consumer);
    auto f2 = std::async(consumer);
    auto f3 = std::async(consumer);

    auto f4 = std::async(producer);
    auto f5 = std::async(producer);
    auto f6 = std::async(producer);
    auto f7 = std::async(producer);

    return 0;
}

namespace
{
    void consumer()
    {
        { // synchronism
            std::unique_lock ul{Mx};
            Cv.wait(ul, [](){return Que.size();});
            std::cout << "consumer pop " << Que.front() << std::endl;
            Que.pop();
        }
    }

    void producer()
    {
        static int Cntr{};

        { // synchronism
            std::unique_lock ul{Mx};
            Que.push(++Cntr);
            std::cout << "producer push " << Cntr << std::endl;
        }
        Cv.notify_one();
    }
} // namespace
```
<!-- entry end -->

## 文件系统
### 文件信息
<!-- entry begin: filesystem fs status file_size hard_link_count last_write_time space -->
* status(path); symlink_status(path)：返回file_status
    * .type()
    * .permissions()
* file_size(path)
* hard_link_count(path)
* last_write_time(path)

* space()                           ：返回space_info
    * .capacity
    * .available
    * .free
<!-- entry end -->

<!-- entry begin: filesystem fs filetype ft -->
* 文件类型：
    * is_regular_file()
    * is_directory()
    * is_symlink()              ：符号链接除此之外还具有链接目标的文件类型
    * is_socket()
    * is_fifo()
    * is_block_file()
    * is_character_file()
<!-- entry end -->

<!-- entry begin: filesystem fs file_type ft -->
* file_type
    > 领域枚举
    * ::none
    * ::not_found
    * ::regular
    * ::directory
    * ::symlink
    * ::block
    * ::character
    * ::fifo
    * ::socket
    * ::unkown
<!-- entry end -->


### 目录
<!-- entry begin: filesystem fs directory_entry -->
* directory_entry `<filesystem>`
    > 目录项可能是目录下的任何类型的文件，文件名尾缀最好别带`/`
    * 构造与赋值
        * (path)                        ：string与path，path与direcoty_entry都可相互转换
    * 读取
        * .path()                       ：返回const path&（也可直接隐式转换为path）
<!-- entry end -->

<!-- entry begin: filesystem fs directory_iterator recursive_directory_iterator  -->
* directory_iterator `<filesystem>`
    * 构造：
        * (path)，默认构造为尾后迭代器
* recursive_directory_iterator  `<filesystem>`
    * .depth()                  ：返回当前递归深度
    * .pop()                    ：返回上级目录
    * .recursion_pending        ：返回当前目录是否禁用递归
    * .disable_recursion_pending：下次自增前禁用递归
<!-- entry end -->

<!-- entry begin: filesystem fs mkdir create_directory create_directories -->
* create_directory(path)            ：`mkdir`
* create_directories(path)          ：`mkdir -p`
<!-- entry end -->

### 符号链接
<!-- entry begin: read_symlink create_symlink -->
* read_symlink(path)    ：获取符号链接指向的文件path（可能为相对路径）
* create_symlink(target, link)
<!-- entry end -->

### 硬链接
<!-- entry begin: filesystem fs copy rename remove remove_all -->
* copy(source, target, copy_options)
    > * copy_options：领域枚举
    >     * ::none
    >     * ::skip_existing
    >     * ::overwrite_existing
    >     * ::update_existing
    >     * ::recursive
    >     * ::copy_symlinks
    >     * ::skip_symlinks
    >     * ::directories_only
    >     * ::create_symlinks
    >     * ::create_hard_links
* rename(old_path, new_path)        ：`mv`
* remove(path)                      ：`rm rmdir`
* remove_all(path)                  ：`rm -r`
<!-- entry end -->

### 权限
<!-- entry begin: filesystem fs file_perm permissions 权限 -->
* permissions(path, perms, perm_options)
    > * perm_options
    >     * ::replace
    >     * ::add
    >     * ::remove
    >     * ::nofollow（改变符号链接自身）
* file_perm
    > 领域枚举
    * ::none          0000
    * ::owner_read    0400
    * ::owner_write   0200
    * ::owner_exec    0100
    * ::owner_all     0700
    * ::group_read    0040
    * ::group_write   0020
    * ::group_exec    0010
    * ::group_all     0070
    * ::others_read   0004
    * ::others_write  0002
    * ::others_exec   0001
    * ::others_all    0007
    * ::all           0777
    * ::set_uid       4000
    * ::set_gid       2000
    * ::sticky_bit    1000
    * ::mask          7777
<!-- entry end -->

### 类
<!-- entry begin: filesystem fs path -->
* path `<filesystem>`
    * 读取
        * .c_str()                  ：返回char*
        * .native()                 ：返回string&
        * .begin()与.end()          ：若存在root_name则从root_name开始，否则从root_path开始，每个元素即是每层目录名（除了root_path外不加`/`或`\`）
        * .root_name()              ：` `或`C:`
        * .root_path()              ：`/`或`\`
        * .relative_path()          ：`tmp/fs.cpp`或`tmp\fs.cpp`
        * .parent_path()            ：`/tmp/`或`C:\tmp\`
        * .file_name()              ：`fs.cpp`
        * .stem()                   ：`fs`
        * .extension()              ：`.cpp`
    * 修改
        * `operator<<(strm, path)`    ：`/tmp/fs.cpp`或`C:\tmp\fs.cpp`
        * `operator>>(strm, path)`    ：`/tmp/fs.cpp`或`C:\tmp\fs.cpp`
        * `operator/()与operator/=()`
        * .remove_filename()
        * .replace_filename()
        * .replace_extension()
    * 判断
        * .empty()
        * .is_absolute()
        * .is_relative()
        * .has_root_name()
        * .has_root_path()
        * .has_relative_path()
        * .has_parent_path()
        * .has_file_name()
        * .has_stem()
        * .has_extension()
<!-- entry end -->

### 函数
<!-- entry begin: filesystem fs equivalent exists status_knows absolute canonical relative current_path temp_directory_path -->
* equivalent(path1, path2)  ：判断两路径是否为同一文件（包括链接）
* exists()
* absolute(path)        ：将path转换为绝对路径（可能带有`..`）
* canonical(path)       ：将path转换为绝对路径（不带有`..`）
* relative(path)        ：将path根据当前工作目录转换为相对路径
* current_path()        ：获取当前工作路径
* temp_directory_path() ：获取临时目录
<!-- entry end -->

# BOOST库
## 字符编码转换
```cpp
#include <boost/locale.hpp> // -lboost_locale
using namespace boost::locale::conv;
// from端的utf编码由Str的类型推知，to端编码由to_charset指出且只能为char类型
std::string                 from_utf(b, e, to_charset);
std::string                 from_utf(Str, to_charset);
// from端的编码由from_charset指出且类型为char ，to端的utf编码有CharT显示指出且同时也指定了类型
std::basic_string<CharT>    to_utf(b, e, from_charset);
std::basic_string<CharT>    to_utf(str, from_charset);
// from端的utf编码由Str的类型推知，to端的utf编码由CharT显示指定
std::basic_string<CharT>    utf8_to_utf(b, e);
std::basic_string<CharT>    utf8_to_utf(Str);
//
std::string                 between(b, e, to_charset, from_charset);
std::string                 between(str, to_charset, from_charset);
```
## 序列化
<!-- entry begin: serialization boost 序列化 -->
```cpp
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <fstream>
#include <ios>
#include <iostream>

class Test
{
    friend class boost::serialization::access; // Note!
public:
    Test() = default;

    Test(int i_a, double d_a, const std::string& s_a): i_m{i_a}, d_m{d_a}, s_m{s_a} {}

    void output()
    {
        std::cout << "i_m: " << i_m;
        std::cout << "\nd_m: " << d_m;
        std::cout << "\ns_m: " << s_m;
        std::cout << '\n';
    }
private:
    template <typename Archive>
    void serialize(Archive& ar, unsigned version) // Note!
    {
        ar & i_m;
        ar & d_m;
        ar & s_m;
    }

    int i_m;
    double d_m;
    std::string s_m;
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::fstream file{"test.bin", std::ios_base::in | std::ios_base::out | std::ios_base::trunc}; // Note

    boost::archive::binary_oarchive toa{file};
    Test t{1, 2.5, "string"};
    t.output();
    std::cout << "Writting..." << std::endl;
    toa << t;

    file.seekg(std::ios_base::beg); // Note
    boost::archive::binary_iarchive tia{file};
    std::cout << "Reading..." << std::endl;
    Test newT;
    tia >> newT;
    newT.output();

    return 0;
}
// 若使用指针，则指针所指类型必须具有`serilize`函数，所以指向内置类型不合法
```
<!-- entry end -->

# nlohmann-json
## 序列化
* 构造nlohmann::json
    * 变量转换：
        > 转换时注意使用`{}`还是`()`
        * bool  : bool
        * number: INT, FLOAT
        * string: string
        * list  : `il<non-pair>`, Seq, Set
        * object: `il<pair>`, Map
    * 支持STL容器接口
        > 将容器元素类型想象为`std::any`
        * Seq类接口创建List
        * Map类接口创建Object

* 修改nlohmann::json
    * .patch(jsonPatch)
        ```cpp
        auto jsonPatch = R"([
            { "op": "replace", "path": "/baz", "value": "boo" },
            { "op": "add", "path": "/hello", "value": ["world"] },
            { "op": "remove", "path": "/foo"}
        ])"_json;
        // jsonPatch is an array of object
        // 3 kinds of "op": replace, add(may override), remove
        // path is similar to filesystem path: /foo -> {"foo":"bar"}, /0 -> ["foo", "bar"]
        ```
    * .merge_patch(json)：合并或覆盖源json

* 序列化为JSON
    * 输出流：`ostream << setw(INDENT) << json;`
    * 字符解析：`.dump()与.dump(INDENT)`
        > 前者返回只一行字符串，后者可指定缩进且多行排版

## 反序列
* 构造nlohmann::json
    * 输入流：`istream >> json;`
    * 字符解析：`json::parse(strWithJson); json::parse(beg, end);`
    * 字面值：`R"({"json": "yes"})"_json`

* 反序列为cpp对象
    > 取决于当前json对象所存储的实际数据类型，类型转换失败会抛出异常（**就像std::any**）  
    > 弱类型系统与强类型系统的交互原理可参见[mysqlpp](#mysqlpplx)
    * `.get<cppType>(); .get_to(cppObj);`：支持的类型转换以及STL接口见上

## 自定义变量转换
为自己的类定义下列两个函数
* `void from_json(const json&, myClass&);`
* `void to_json(json&, const myClass&);`

# Mysql++
<!-- entry begin: mysqlpp mysql++ 异常 exception -->
## 异常
```
BadIndex        ：`row[idx]`中idx越界
BadFieldName    ：`row[fd_name]`中fd_name无效
BadConversion   ：SQL与C++数据类型之间的转换不合理（类型不匹配或窄化）
BadParamCount
TypeLookupFailed
```
* `mysqlpp::NoExceptions disableExceptions{con}`
    > 构造时禁用传递的mysqlpp::Connection的异常机制  
    > 销毁时解禁
<!-- entry end -->

<!-- entry begin: mysqlpp mysql++ Connection -->
## 连接
* mysqlpp::Connection
    * 构造：
        * Connection(bool=true)                         ：若为false则表示用false flag代替抛出异常，其他任何构造方式都会开启异常机制
        * Connection(db, server, user, password, port)  ：除了`db`外其余参数均有默认实参
            > 对于`server`：
            > * 0                   ：让数据库驱动选择通讯方式
            > * "."                 ：Windows named pipes
            > * "/path/to/socket"   ：Unix domain socket
            > * "host.or.ip:port"   ：TCP
    * 数据库服务连接：
        * .connect(db, host, user, password，port)  ：返回bool表示是否连接成功，其余同上述构造函数
        * .connected()                              ：返回bool表示是否连接成功
        * .disconnect()
        * .shutdown()
    * 数据库服务信息：
        * .client_version()
        * .server_version()
        * .server_status()
        * .ipc_info()
    * 数据库操作：
        * .select_db(db)
        * .create_db(db)
        * .drop_db(db)
    * 数据库查询：
        * .count_rows(tbl)                          ：返回table的行数
        * .query()                                  ：返回连接到该Connection的mysqlpp::Query
        * .query("SQL Statement")                   ：返回已初始化的mysqlpp::Query
    * 错误处理
        * .error()                                  ：返回上次发生错误时的信息
        * .ping()                                   ：返回bool表示是否可ping通
<!-- entry end -->

<!-- entry begin: mysqlpp mysql++ Query quote -->
## SQL语句执行
* mysqlpp::Query
    * 读取SQL语句
        * Query("SQL Statement")
        * `operator<<(Query, string)`
    * 执行SQL语句
        * .exec()                           ：只返回bool
        * .execute()                        ：返回mysqlpp::SimpleResult，存储提示信息
        * .store()                          ：返回mysqlpp::StoreQueryResult，存储数据信息
        * .use()                            ：返回mysqlpp::UseQueryReslt，存储数据信息（利用按需加载机制）
    * 错误处理
        * .error()                          ：返回错误消息

* mysqlpp::quote
    > 流操作符，作用类似std::quote
<!-- entry end -->

<!-- entry begin: mysqlpp mysql++ StroeQueryResult UseQueryResult SimpleQueryResult -->
* mysqlpp::SimpleQueryResult
    * .info()

* mysqlpp::StoreQueryResult
    * .begin()
    * .end()
    * .operator bool()
    * `.operator[]()`                       ：返回mysqlpp::Row
    * .num_rows()                           ：返回总行数

* mysqlpp::UseQueryReslt
    > 按需加载
    * .fetch_field()                        ：返回mysqlpp::Field
    * .fetch_row()                          ：返回mysqlpp::Row
<!-- entry end -->

<!-- entry begin: mysqlpp mysql++ Null type -->
<span id="mysqlpplx"></span>
MYSQL++中定义有类型映射到SQL类型，如：  
`mysqlpp::sql_tinyint_unsigned_null`表示SQL类型`TINYINT UNSIGNED`  
`mysqlpp::sql_tinyint_unsigned`表示SQL类型`TINYINT UNSIGNED NOT NULL`  
非NOT NULL的SQL类型可以接受`mysqlpp::null`的赋值，表示特殊值`TINYINT NULL`  
NULL类型的基础便是该类
* `mysqlpp::Null<Type, mysqlpp::NullIsZero或mysqlpp::NullIsNull>`

String 可以将 SQL 类型字符串转换为 C++ 数据类型  
STA 可以将 C++数据类型转换为 SQL 类型字符串  
两种字符串都使用了Copy-on-Write机制

<!-- entry end -->
