# C++

<!-- vim-markdown-toc GFM -->

- [C++](#c)
  - [C++标准库](#c标准库)
    - [错误处理](#错误处理)
      - [异常体系结构](#异常体系结构)
      - [异常对象使用](#异常对象使用)
    - [内存管理](#内存管理)
      - [new 与 delete 操作符](#new-与-delete-操作符)
      - [智能指针](#智能指针)
    - [工具库](#工具库)
      - [系统工具](#系统工具)
      - [基础设施](#基础设施)
      - [时间库](#时间库)
      - [对象载体](#对象载体)
    - [数值库](#数值库)
      - [数值极限](#数值极限)
      - [静态期分数](#静态期分数)
      - [数学库](#数学库)
      - [数值算法](#数值算法)
      - [随机数](#随机数)
    - [字符处理](#字符处理)
      - [字符分类](#字符分类)
      - [字符串](#字符串)
      - [字符串视图](#字符串视图)
      - [格式化](#格式化)
      - [正则表达式](#正则表达式)
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
      - [搜索算法](#搜索算法)
      - [集合算法](#集合算法)
      - [极值算法](#极值算法)
      - [变序算法](#变序算法)
      - [更易算法](#更易算法)
    - [输入输出库](#输入输出库)
      - [组件总览](#组件总览)
      - [基础操作](#基础操作)
      - [非格式输入输出](#非格式输入输出)
      - [预定义操作](#预定义操作)
      - [操作符](#操作符)
      - [文件流](#文件流)
      - [字符流](#字符流)
      - [流缓冲区](#流缓冲区)
      - [本地环境](#本地环境)
    - [线程支持库](#线程支持库)
      - [线程启动](#线程启动)
      - [线程同步](#线程同步)
      - [原子操作库](#原子操作库)
    - [文件系统库](#文件系统库)
      - [路径与文件表示](#路径与文件表示)
      - [文件属性](#文件属性)
      - [文件操作](#文件操作)
  - [BOOST 库](#boost-库)
    - [编码转换](#编码转换)
    - [异步与网络库](#异步与网络库)
  - [GOOGLE 库](#google-库)
    - [日志库](#日志库)
    - [测试库](#测试库)
  - [序列化库](#序列化库)
    - [YAS](#yas)
    - [BOOST](#boost)
    - [JSON](#json)

<!-- vim-markdown-toc -->

注：代码块中的示例为伪代码

## C++标准库

### 错误处理

<!-- entry begin: exception stdexcept 标准库异常 异常体系结构 -->

#### 异常体系结构

```txt
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

<!-- entry begin: logic_error runtime_error system_error error_code what code current_exception rethrow_exception 异常构造 异常成员 异常挂起 -->

#### 异常对象使用

```cpp
// 异常构造
class logic_error {
    logic_error(const string&);
    logic_error(const char*);
};
class runtime_error {
    runtime_error(const string&);
    runtime_error(const char*);
};
class system_error {
    system_error(error_code);
    system_error(error_code, const string&);
    system_error(error_code, const char*);
};
error_code make_error_code(errc);
// 通过errno构造System_error
std::system_error{errno, std::generic_category()};

// 异常成员
class exception {
    C char*     what();     // 返回的字符串指针保证在异常对象销毁前，或在调用异常对象的非静态成员函数前合法

    error_code& code();     // 适用于system_error及其派生类，与future_error
};

class error_code {
    void        clear()
    string      message()
    err_cat&    category()  // 返回std::error_category有成员函数`const char* name()`与`string message()`
    int         value()     // 在相同的category下才具有可比性

    // error_code是依赖平台的错误码，error_condition是可移植的错误码
    err_con     default_error_condition()   // 返回std::error_condition同样有上述四个成员函数

    // 重载了与错误码的比较
    operator<=>
};
#include <cerrno>
enum class errc;
#include <ios>
enum class io_errc;
#include <future>
enum class future_errc;

// 异常挂起
exception_ptr   current_exception()         // 挂起当前的异常
void            rethrow_exception(exceptr)  // 重抛挂起的异常
```

<!-- entry end -->

### 内存管理

<!-- entry begin: new delete get_new_handler set_new_handler -->

#### new 与 delete 操作符

```cpp
#include <new>
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

#### 智能指针

<!-- entry begin: memory unique_ptr 智能指针 -->

```cpp
#include <memory>
class unique_ptr<T, Deleter=default_delete<T> > {
    // 构造函数：支持move，拒绝copy
    unique_ptr(ptr);
    unique_ptr(ptr, del);
    // 修改器
    pointer     release();
    void        reset(ptr=nullptr);
    // 观察器
    pointer     get();
    Deleter&    get_deleter();
    // Operator
    operator bool
    operator*
    operator->
};
unique_ptr  make_unique<T>(args...);
```

<!-- entry begin:  -->
<!-- entry begin: memory shared_ptr 智能指针 -->

```cpp
class shared_ptr<T> {
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
    // Operator
    operator*
    operator->
};
shared_ptr  make_shared<T>(args...);
shared_ptr  static_pointer_cast<T>(sptr);
shared_ptr  const_pointer_cast<T>(sptr);
shared_ptr  dynamic_pointer_cast<T>(sptr);
shared_ptr  reinterpret_pointer_cast<T>(sptr);
shared_ptr  enable_shared_from_this<T>::shared_from_this();
weak_ptr    enable_shared_from_this<T>::weak_from_this();
```

<!-- entry end -->
<!-- entry begin: memory weak_ptr 智能指针 -->

```cpp
class weak_ptr<T> {
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

### 工具库

<!-- entry begin: cstdlib atof atoi strtol strtod -->

#### 系统工具

```cpp
#include <cstdlib>
// 进程终止
void    exit(int exit_code);                            // 正常终止进程，进行清理
void    quick_exit(int exit_code);                      // 正常终止进程，进行非完全清理
void    _Exit(int exit_code);                           // 正常终止进程，不进行清理
void    abort();                                        // 异常终止进程，不进行清理；可捕获但无法阻止程序终止
int     atexit(void(*func)());                          // 注册在调用exit()时被调用
int     at_quick_exit(void(*func)());                   // 注册在调用quick_exit()时被调用
// 系统环境
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

<!-- entry begin: functional hash function reference_wrapper ref cref invoke -->

#### 基础设施

```cpp
#include <functional>
struct hash<T> {
    // 特化包括有：
    // 整型、浮点型、指针、
    // 智能指针、string族、string_view族、bitset、vector<bool>、
    // error_code、error_condition、type_index、thread::id、optional、variant
};
class function<F(Args...)> {
    // 成员函数
    operator()
    operator bool
};
class reference_wrapper<T> {
    // 成员函数
    T& get();
    // Operator
    operator()  // 调用存储的函数
    operator T&
};
referece_wrapper<T>         ref(T& t);
referece_wrapper<const T>   cref(T& t);

Result  invoke(Func, args...);
// 对于成员函数指针，INVOKE(f, t1, t2, ..., tN) 等价于 (t1.*f)(t2, ..., tN)
// 对于成员数据指针，INVOKE(f, t1)              等价于 t1.*f
// 上述两者均对referece_wrapper有特化，将t1变为t1.get()

Functor bind(Callable, args...);
// bind(Callable, std::placeholders::_2, std::placeholders::_1)表示调用Functor(arg1, arg2)等于调用Callable(arg2, arg1)
```

<!-- entry end -->

<!-- entry begin: chrono 时间库 -->

#### 时间库

```cpp
#include <chrono>
using namespace chrono;

class Clock {   // 预定义有 system_clock 与 steady_clock 等
    // 成员类型
    typename    rep
    typename    period
    typename    duration    = duration<rep, period>
    typename    time_point  = time_point<Clock>
    // 成员函数
  S time_point  now();
  S time_t      to_time_t(time_point);
  S time_point  frome_time_t(time_t);
};

// 预定义有 nanoseconds microseconds milliseconds seconds minutes hours days weeks months years
class duration<Rep, Period=ratio<1> > {
    // 成员类型
    typename    rep
    typename    period
    // 成员函数
    rep         count();
  S duration    zero();
  S duration    min();
  S duration    max();
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
};
duration        duration_cast<D>(d);
duration        floor(d)
duration        ceil(d)
duration        round(d)
duration        abs(d)

class time_point<Clock, Duration = Clock::duration> {
    // 成员类型
    rep
    period
    duration
    clock
    // 成员函数
    duration    time_since_epoch();
  S duration    min();
  S duration    max();
    // Operator
    operator+
    operator-
};
time_point      time_point_cast<TP>(tp);
time_point      floor(tp);
time_point      ceil(tp);
time_point      round(tp);
```

<!-- entry end -->

#### 对象载体

<!-- entry begin: initializer_list -->

```cpp
#include <initializer_list>
class initializer_list<T> {
    // 构造函数
    initializer_list(); // 语言特性支持的列表初始化的默认类型
    // 成员函数
    size_t      size();
    const T*    begin();
    const T*    end();
};
```

<!-- entry end -->

<!-- entry begin: utility integer_sequence -->

```cpp
#include <utility>
struct integer_sequence<T, T... INTS> {
    // 成员函数
    static size_t size();
};
// 辅助模板
using index_sequence<INTS>        = integer_sequence<size_t, INTS>
using make_integer_sequence<T, N> = integer_sequence<T, 0..N-1>
using make_index_sequence<N>      = integer_sequence<size_t, 0..N-1>
```

<!-- entry end -->
<!-- entry begin: utility pair -->

```cpp
#include <utility>
class pair<T1, T2> {
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
};
pair    make_pair(x, y);            // 被C++17模板类的模板参数推断取代
T&      get<size_t>(p);
T&      get<T>(p);
size    tuple_size<pair>::value
typename tuple_element<size_t, pair>::type
```

<!-- entry end -->
<!-- entry begin: utility tuple -->

```cpp
#include <tuple>
class tuple<Types...> {
    // 构造函数
    tuple();                            // 默认构造
    tuple(args...);                     // 类聚合式构造
    tuple(tuple<UTypes...>);            // 成员模板构造
    tuple(p);                           // pair转换构造
};
tuple   make_tuple(args...);        // 被C++17模板类的模板参数推断取代
tuple   tie(args...);
tuple   tuple_cat(tuples...);
T&      get<size_t>(t);
T&      get<T>(t);
size    tuple_size<tuple>::value
typename tuple_element<size_t, tuple>::type
```

<!-- entry end -->

<!-- entry begin: any -->

```cpp
#include <any>
class any {
    // 构造函数
    any();                          // 默认构造。空对象
    any(value);                     // 类聚合式构造
    any(in_place, args... );        // 就地构造
    any(in_place, il, args...)      // in_place包括 std::in_place_type<T>
    // 修改器
    T&      emplace<T>(args...);
    T&      emplace<T>(il, args...);
    void    reset();
    // 观察器
    bool    has_value();
    typei&  type();                 // 返回 std::type_info 有成员函数 `const char* name()`
};
T       any_cast<T>(any&);
T*      any_cast<T>(any*);
```

<!-- entry end -->

<!-- entry begin: variant -->

```cpp
#include <variant>
class variant<Types...> {
    // 构造函数
    variant();                  // 默认构造第一个类型。可用std::monostate作占位符类型
    variant(t);                 // 匹配构造最佳类型
    variant(in_place, args...); // 就地构造
    variant(in_place, il, args...); // in_place包括 std::in_place_index<T> 与 std::in_place_index<T>
    // 修改器
    T&      emplace<T>(args...);
    T&      emplace<T>(il, args...);
    T&      emplace<size_t>(args...);
    T&      emplace<size_t>(il, args...);
    // 观察器
    size_t  index();
    bool    valueless_by_exception();   // 发生异常时，index()返回std::variant_npos
};
T&      get<size_t>(vrt);               // 转换失败抛出std::bad_variant_access
T&      get<T>(vrt);
T*      get_if<size_t>(vrt);            // 转换失败返回空指针
T*      get_if<T>(vrt);
R       visit(visitor, vrt);            // visitor内部可利用if constexpr
size    variant_size<variant>::value
typename variant_alternative<size_t, variant>::type
```

<!-- entry end -->

<!-- entry begin: optional -->

```cpp
#include <optional>
class optional<T> {
    // 构造函数
    optional();                         // 默认构造为std::nullopt
    optional(val);                      // 类聚合式构造
    optional(in_place, args...);        // 就地构造
    optional(in_place, il, args...);    // in_place包括std::in_place
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
    // Operator
    operator bool
};
```

<!-- entry end -->

<!-- entry begin: bitset -->

```cpp
#include <bitset>
class bitset<size_t> {  // std::vector<bool>特化可做动态版bitset
    // 构造函数
    bitset();
    bitset(ulong);
    bitset(str, pos=0, nbits=npos, zero='0', one='1');
    bitset(Cstr, nbits=npos, zero='0', one='1');
    // 元素访问
    bool        test(pos);
    bool        all();
    bool        any();
    bool        none();
    size_t      count();
    size_t      size();
    // 修改器
    bitset&     set();
    bitset&     set(pos, boolean=true);
    bitset&     reset();
    bitset&     reset(pos);
    bitset&     flip();
    bitset&     flip(pos);
    // 转换
    string      to_string(zero='0', one='1');
    u_long      to_ulong();     // 存储时低位在“左”，显示时低位在“右”（小端）
    // Operator
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

### 数值库

<!-- entry begin: limits 数值极限 -->

#### 数值极限

```cpp
#include <limits>
class numeric_limits<T> {
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

#### 静态期分数

```cpp
#include <ratio>
// 预定义有 pico nano micro centi deci deca hecto kilo mega giga tera peta exa
class ratio<Num, Den=1> {
    // 成员对象
  S intmax_t num;
  S intmax_t den;
};
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
```

<!-- entry end -->

<!-- entry begin: cmath 数学库 -->

#### 数学库

```cpp
#include <cmath>
// 三角函数
f   sin(x);             // 求 sin(x)
f   cos(x);             // 求 cos(x)
f   tan(x);             // 求 tan(x)
f   asin(x);            // 求 arcsin(x) 在[-π/2, π/2]
f   acos(x);            // 求 arccos(x) 在[0, π]
f   atan(x);            // 求 arctan(x) 在[-π/2, π/2]
f   atan2(y, x);        // 求 arctan(y/x)在[-π, π]
// 对数函数
f   log(x);             // 求 ln(x)
f   log1p(x);           // 求 ln(1+x)
f   log2(x);            // 求 log₂(x)
f   log10(x);           // 求 log₁₀(x)
// 指数函数
f   exp(x);             // 求 eˣ
f   expm1(x);           // 求 eˣ-1
f   exp2(x);            // 求 2ˣ
f   pow(x, y);          // 求 xʸ
// 幂函数
f   sqrt(x);            // 求 √x
f   cbrt(x);            // 求 ³√x
// 其他函数
f   erf(arg);           // 误差函数
f   erfc(arg);          // 补误差函数
f   tgamma(arg);        // gamma函数，传入正数相当于求阶乘arg!
f   lgamma(arg);        // gamma函数的自然对数
f   lerp(a, b, t);      // 求 a+t(b-a)
// 浮点运算
f   fmod(x, y);         // 求 x/y（向零取整）的余数
f   remainder(x, y);    // 求 x/y（四舍五入）的余数
f   fmax(x, y);         // 求 max(x, y) 若存在NAN则返回NAN
f   fmin(x, y);         // 求 min(x, y) 若存在NAN则返回NAN
f   dim(x, y);          // 求 max(x-y, 0)
f   fma(x, y, z);       // 求 x*y+z
f   hypot(x);           // 求 √(x²+y²)
// 浮点取整
f   ceil(f);            // 向上取整
f   floor(f);           // 向下取整
f   trunc(f);           // 向零取整
i   round(f);           // 四舍五入
// 符号
f   abs(f);             // 求绝对值
f   copysign(x, y);     // 求 x 的值与 y 的符号组成的浮点值
// 浮点编码
fl  nanf(Cstr);         // Cstr设置NAN的尾数
db  nan(Cstr);
ld  nanl(Cstr);
f   modf(f, iptr);      // 将 f 分解为整数与小数。整数存于iptr并返回小数
f   frexp(f, iptr);     // 将 f 分解为 n * 2ᵉˣᵖ，n ∈ [0.5, 1)。exp存于iptr并返回n
f   ldexp(f, i);        // 求 f * 2ᵉˣᵖ
f   logb(f);            // 求 log₂|f|
i   ilogb(f);           // 求 log₂|f|
f   nextafter(from, to);// 求 from 趋向 to 的下个可表示的浮点值
```

<!-- entry end -->

<!-- entry begin: midpoint lcm gcd transform_exclusive_scan transform_inclusive_scan exclusive_scan inclusive_scan partial_sum adjacent_difference inner_product transform_reduce transform_reduce reduce accumulate iota numeric 数值算法 -->

#### 数值算法

> bOp(e)二元操作符只有一个参数表示和

```cpp
#include <numeric>
void    iota(b, e, v);                                      // destEle=v++
T       accumulate(b, e, init, bOp=plus);                   // bOp(e)
T       reduce(b, e, init=0, bOp=plus);                     // bOp(e)。支持policy
T       transform_reduce(b1, e1, init, bOp, uOp);           // bOp(uOp(e))。支持policy
T       transform_reduce(b1, e1, b2, init,                  // bOp1(bOp2(e1, e2))。支持policy
                        bOp1=plus, bOp2=multiplies);
T       inner_product(b1, e1, b2, init,
                        bOp1=plus, bOp2=multiplies);        // bOp1(bOp2(e1, e2))
destE   adjacent_difference(b, e, destB, bOp=minus);        // destEle=e - em1
destE   partial_sum(b, e, destB, bOp=plus);                 // destEle=bOp(e)
destE   inclusive_scan(b, e, destB, bOp=plus);              // destEle=bOp(e)。支持policy
destE   exclusive_scan(b, e, destB, bOp=plus);              // destEle=bOp(em1)。支持policy
destE   transform_inclusive_scan(b, e, destB,
                        bOp, uOp, init=0);                  // destEle=bOp(uOp(e))。支持policy
destE   transform_exclusive_scan(b, e, destB,
                        bOp, uOp, init=0);                  // destEle=bOp(uOp(em1))。支持policy
T       gcd(m, n);                                          // 求最大公因数
T       lcm(m, n);                                          // 求最小公倍数
T       midpoint(a, b);                                     // 求中间值
```

<!-- entry end -->

<!-- entry begin: random 随机数 随即引擎 随机分布 -->

#### 随机数

```cpp
#include <random>
// 常用引擎：有成员函数`void seed(val)`作种
typename    minstd_rand             // 快
typename    mt19937_64              // 广
typename    ranlux48
typename    knuth_b
typename    default_random_engine
// 常用分布：有重载操作符`random operator()(engine)`返回符合分布的随机数
typename    uniform_int_distribution(min=0, max=INTMAX) // [min, max]的均匀整数分布
typename    uniform_real_distribution(min=0, max=1.0)   // [min, max)的均匀实数分布，可使用实现闭区间std::nextafter(b, std::numeric_limits<RealType>::max())
typename    bernoulli_distribution(p=0.5)               // 0-1分布，返回bool
typename    binomial_distribution(n=1, p=0.5)           // 二项分布
typename    normal_distribution(u=0, o=1)               // 正态分布
```

<!-- entry end -->

### 字符处理

<!-- entry begin: cctype -->

#### 字符分类

```cpp
#include <cctype>
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

#include <cwctype>
// 形如 bool iswalnum() 等等
```

<!-- entry end -->

<!-- entry begin: string -->

#### 字符串

```cpp
#include <string>
class String {
    // 目标：(str, pos=0, len=npos) (cstr, len=auto) (char) (n, char)
    // 构造函数
    string(目标)
    // 修改
    string& assign      (目标)
    string& append      (目标)  // 目标除开(char)
    string& operator=   (str) (cstr) (char)
    string& operator+=  (str) (cstr) (char)
    string& operator+   (str) (cstr) (char)
    string& insert      (pos, 目标)         // 目标除开(char)
    string& erase       (pos=0, len=npos)
    string& replace     (pos, len, 目标)
    // 创建
    string  substr(pos=0, len=npos);
    size_t  copy(cstr, len, pos=0);
    // 查找
    bool    starts_with (str) (cstr) (char)
    bool    end_with    (str) (cstr) (char)
    bool    contains    (str) (cstr) (char)
    size_t  find             (str, pos=0) (cstr, pos=0, len=auto) (char, pos=0) // 起始索引可越界
    size_t  rfind            (str, pos=0) (cstr, pos=0, len=auto) (char, pos=0)
    size_t  find_first_of    (str, pos=0) (cstr, pos=0, len=auto) (char, pos=0)
    size_t  find_first_not_of(str, pos=0) (cstr, pos=0, len=auto) (char, pos=0)
    size_t  find_last_of     (str, pos=0) (cstr, pos=0, len=auto) (char, pos=0)
    size_t  find_last_not_of (str, pos=0) (cstr, pos=0, len=auto) (char, pos=0)
    // 比较
    int     compare(pos, len, 目标);        // 目标除开(char) (n, char)
    // 容量
    bool    empty();
    size_t  size();
    size_t  length();
    void    resize(count, char='\0');
    size_t  capacity();
    void    shrink_to_fit();
    void    reserve(new_cap = 0);       // new_cap小于capacity()则无效
    // 转换
    C T*    data();
    C T*    c_str();
};
string  to_string(v);
wstring to_wstring(v);
int     stoi(str, size_t*=nullptr, base=10);
double  stod(str, size_t*=nullptr, base=10);
```

<!-- entry end -->

<!-- entry begin: string_view -->

#### 字符串视图

```cpp
#include <string_view>
class String_view {
    // 构造函数
    string_view(str);
    string_view(cstr, len=auto);
    string_view(beg, end);
    // 修改
    void    remove_prefix(n);
    void    remove_suffix(n);
    // 查找
    // 比较
    // 复制
    // 子串（大幅降低开销）
    // 容量
    // 转换
};
```

<!-- entry end -->

<!-- entry begin: format 格式化 -->

#### 格式化

> `"{arg_id:填充与对齐 符号 # 0 宽度 精度 L 类型}"`

- arg_id
  > 从 0 开始。要么全部默认按顺序，要么全部手动指定
- 填充与对齐
  > 填充与对齐只能一同出现，无填充则默认为空格
  - `<`：左对齐（非整数与非浮点数默认左对齐）
  - `>`：右对齐（整数与浮点数默认右对齐）
  - `^`：居中
- 符号
  - `+`：显示正负号
  - `-`：显示负号（默认）
  - `空`：非负数前导空格
- `#`
  - 对整数，showbase
  - 对浮点数，showpoint
- 0
  - 对整数与浮点数，用 0 填充前导空白，若与对齐符号一同使用则失效
- 宽度与精度
  - 宽度：`{:6}`
  - 精度：`{:.6}`
  - 宽度与精度：`{:6.6}`
- L
  - 对整数：插入合适数位分隔符
  - 对浮点数：插入合适数位分隔符与底分隔符
  - 对 bool：boolalpha
- 类型
  - 整数：
    - `b`与`B`：二进制
    - `o`：八进制
    - `x`与`X`：十六进制
  - 浮点数：
    - `a`与`A`：十六进制
    - `e`与`E`：科学计数法
    - `f`与`F`：定点表示法 \* `g`与`G`：智能表示

<!-- entry end -->

#### 正则表达式

<!-- entry begin: cpp regex regex_contants regex_flag 正则表达式 -->

```cpp
#include <regex>
namespace std::regex_contants {
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
    format_first_only   // 只替换首个匹配
}
```

<!-- entry end -->
<!-- entry begin: regex wregex basic_regex -->

```cpp
class basic_regex<CharT> {  // regex wregex
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
```

<!-- entry end -->
<!-- entry begin: regex match_results cmatch wcmatch smatch wsmatch csub_match wcsub_match ssub_match wssub_match regex_iterator regex_token_iterator -->

```cpp
class match_results<BidirIt> {  // cmatch wcmatch smatch wsmatch
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
class sub_match<BidirIt> {  // csub_match wcsub_match ssub_match wssub_match
    // 观察器
    BidirIt     first;      // 子表达式开始
    BidirIt     second;     // 子表达式尾后
    String      str();
    operator    String();
};
class regex_iterator<BidirIt> { // cregex_iterator wcregex_iterator sregex_iterator wsregex_iterator
    // 构造函数
    regex_iterator();                               // 默认构造为尾后迭代器
    regex_iterator(b, e, regex, mflag);             // 迭代器在每个匹配区间停留，每次从上次末尾开始匹配（不重合）。禁止regex右值

    match_results    operator*()                    // 返回match_results
};
class regex_token_iterator<BidirIt> {   // cregex_token_iterator wcregex_token_iteratorsregex_token_iterator wsregex_token_iterator
    // 构造函数
    regex_token_iterator();                         // 默认构造为尾后迭代器
    regex_token_iterator(b, e, regex, il, mflag);   // il指定关注的regex中的子表达式，0表示全部，-1表示模式取反。禁止regex右值。
    // 若有多个匹配组，则匹配组之间轮换

    sub_match       operator*()                     // 返回sub_match
};
```

<!-- entry end -->
<!-- entry begin: regex regex_match regex_search regex_replace -->

```cpp
// 没搜索到则match为空，其prefix余suffix也为空
bool    regex_match(str,  [match&,] regex, mflag);
bool    regex_match(b, e, [match&,] regex, mflag);

bool    regex_search(str,  [match&,] regex, mflag);
bool    regex_search(b, e, [match&,] regex, mflag);

outItr  regex_replace(outItr, str,  regex, fmt, rflag);  // 利用fmt替换掉每个匹配的regex
outItr  regex_replace(outItr, b, e, regex, fmt, rflag);
string  regex_replace(str,  regex, fmt, rflag);
string  regex_replace(b, e, regex, fmt, rflag);
// 替换格式化语法：
// $&       替换为regex中整个表达式的匹配
// $0       替换为regex中整个表达式的匹配
// $1, ...  替换为regex中第1个子表达式的匹配
// $`       替换为前缀
// $'       替换为后缀
// $$       转义$
```

<!-- entry end -->

### 容器库

- a : array
- s : string
- v : vector
- d : deque
- l : list
- A : Assoicated
- U : Unordered
- M : all-kinds-of-Map
  > c++20 新增 std::span，类似 std::string_view 之于 std::string

#### 构造

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

#### 赋值

```cpp
C&      operator=()             // ALL
C&      assign(il)              // ALL-a-A-U
C&      assign(beg, end)        // ALL-a-A-U
C&      assign(num, val)        // ALL-a-A-U
void    fill(v)                 // a
```

<!-- entry end -->

<!-- entry begin: 容器访问 -->

#### 访问

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
itr     rbegin()                // ALL-U
itr     crbegin()               // ALL-U
itr     rend()                  // ALL-U
itr     crend()                 // ALL-U
```

<!-- entry end -->

<!-- entry begin: 容器元素插入 -->

#### 插入

```cpp
1st_ins insert(pos, val)        // ALL-a
1st_ins insert(pos, il)         // s+v+d+l
1st_ins insert(pos, beg, end)   // s+v+d+l
1st_ins insert(pos, num, val)   // s+v+d+l
T&      emplace(pos, args...)   // v+d+l
T&      emplace_back(args...)   // v+d+l
T&      emplace_front(args...)  // d+l
void    push_back(v)            // s+v+d+l
void    push_front(v)           // d+l

ins     insert(hint, val)       // ALL-a
ins     insert(val)             // A+U（非multi返回pair<ins, bool>）
void    insert(il)              // A+U
void    insert(beg, end)        // A+U
ins     emplace(args...)        // A+U（非multi返回pair<ins, bool>）
ins     emplace_hint(pos,args..)// A+U
<ins,bool>  try_emplace(key, val)           // map+Umap
ins         try_emplace(hint, key, val)     // map+Umap
<ins,bool>  insert_or_assign(key, val)      // map+Umap
ins         insert_or_assign(hint, key, val)// map+Umap
```

<!-- entry end -->

<!-- entry begin: 容器元素删除 -->

#### 删除

```cpp
void    clear()                 // ALL
ers_num erase(v)                // A+U
ers+1   erase(pos)              // ALL
ers+1   erase(beg, end)         // ALL
void    pop_back()              // ALL-a-A-U
void    pop_front()             // d+l
```

<!-- entry end -->

#### 容量

<!-- entry begin: 容器容量 -->

```cpp
bool    empty()                 // ALL
size_t  size()                  // ALL
void    resize(num)             // ALL-a-A-U
void    resize(num, v)          // ALL-a-A-U
size_t  capacity()              // s+v  （clear()一般不减小capacity）
void    reserve(num)            // s+v+U（只能扩大）
void    shrink_to_fit()         // s+v+d
size_t  max_size()              // ALL
```

<!-- entry end -->

<!-- entry begin: U A 无序 关联 -->

#### 关联与无序容器

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

#### 无序容器

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
void    reserve(bnum)
```

<!-- entry end -->

<!-- entry begin: l 链表 -->

#### 链表容器

```cpp
void    sort(cmp = less)
void    unique(cmp = less)
void    merge(srcL, cmp = less)             // 需要先排序
void    reverse()
void    splice(pos, srcL, srcPos)
void    splice(pos, srcL, srcBeg, srcEnd)   // 注意pos不能在[srcBeg, srcEnd)中
ers_num remove(v)
ers_num remove_if(uOp)
```

<!-- entry end -->

### 迭代器库

<!-- entry begin: 迭代器辅助函数 -->

#### 辅助函数

```cpp
#include <iterator>
// 不改变原来迭代器
itr     next(itr, n=1)
itr     prev(itr, n=1)
itr     distance(itr1, itr2)
void    iter_swap(itr1, itr2)
```

<!-- entry end -->

<!-- entry begin: 流迭代器 istream_iterator ostream_iterator istreambuf_iterator ostreambuf_iterator -->

#### 流迭代器

```cpp
#include <iterator>
class istream_iterator<T, CharT = char> {
    // 构造函数
    istream_iterator()      // 默认构造尾后迭代器
    istream_iterator(istrm)
};
class ostream_iterator<T, CharT = char> {
    // 构造函数
    ostream_iterator(ostrm, const CharT* delim = "")
};

// 流缓冲区迭代器会自动冲刷缓冲区
class istreambuf_iterator<CharT> {
    // 构造函数
    istreambuf_iterator()       // 默认构造为尾后迭代器
    istreambuf_iterator(istrm)
    istreambuf_iterator(ibuf_ptr)
};
class ostreambuf_iterator<CharT> {
    // 构造函数
    ostreambuf_iterator(ostrm)
    ostreambuf_iterator(obuf_ptr)
};
```

<!-- entry end -->

<!-- entry begin: 反向迭代器 reverse_iterator -->

#### 反向迭代器

```cpp
#include <iterator>
class reverse_iterator<Iter> {
    // 构造函数
    reverse_iterator()
    reverse_iterator(itr)
    // 成员函数
    itr  base()
};
ritr make_reverse_iterator(itr)
ritr rbegin()
ritr crbegin()
ritr rend()
ritr crend()
```

<!-- entry end -->

<!-- entry begin: 移动迭代器 move_iterator -->

#### 移动迭代器

```cpp
#include <iterator>
class move_iterator<Iter> {
    // 构造函数
    move_iterator()
    move_iterator(itr)
};
mitr make_move_iterator(itr)
```

<!-- entry end -->

<!-- entry begin: 插入迭代器 insert_iterator front_insert_iterator back_insert_iterator -->

#### 插入迭代器

```cpp
#include <iterator>
class insert_iterator<Container> {
    // 构造函数
    insert_iterator()
    insert_iterator(Cont, itr)
};
iitr    inserter(Cont, pos)
class front_insert_iterator<Container> {
    // 构造函数
    front_insert_iterator()
    front_insert_iterator(Cont)
};
fiitr   front_inserter(Cont)
class back_insert_iterator<Container>
{
    // 构造函数
    back_insert_iterator()
    back_insert_iterator(Cont)
};
biitr   back_inserter(Cont)
```

<!-- entry end -->

### 算法库

<!-- entry begin: execution policy 执行策略 -->

#### 执行策略

```cpp
#include <execution>
// 执行策略常量
exe_policy std::execution::seq;
exe_policy std::execution::par;
exe_policy std::execution::unseq;
exe_policy std::execution::par_unseq;
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

<!-- entry begin: equal_range upper_bound lower_bound binary_search find_first_of adjacent_find find_end search_n search find_if_not find_if find 搜索算法 mismatch none_of any_of all_of count_if count for_each_n for_each-->

#### 搜索算法

```cpp
// 搜索单个元素
tagItr  find(b, e, v)
tagItr  find_if(b, e, uOp)
tagItr  find_if_not(b, e, uOp)

// 搜索子区间
tagItr  search(b, e, seqB, seqE, bOp=equal_to)          // 支持模板参数在functinal头文件中<boyer_noore_searcher>
tagItr  search_n(b, e, N, v, bOp=equal_to)              // bOp(elem, v)
tagItr  find_end(b, e, seqB, seqE, bOp=equal_to)
tagItr  adjacent_find(b, e, bOp=equal_to)               // 搜索一对连续相等的元素

// 搜索目标范围中的元素
tagItr  find_first_of(b, e, seqB, seqE, bOp=equal_to)

// 二分搜索，需要先排序
tagItr  binary_search(b, e, v, bOp=less)
tagItr  lower_bound(b, e, v, bOp=less)
tagItr  upper_bound(b, e, v, bOp=less)
p<b,e>  equal_range(b, e, v, bOp=less)

// 轮询搜索
uOp     for_each(b, e, uOp) // 返回uOp(已改动过的)拷贝
uOp     for_each_n(b, n, uOp)
size_t  count(b, e, v)
size_t  count_if(b, e, uOp)
bool    all_of(b, e, uOp)
bool    any_of(b, e, uOp)
bool    none_of(b, e, uOp)
```

<!-- entry end -->

#### 集合算法

```cpp
// 集合算法均需提前排序
p<i,ci> mismatch(b, e, cmpB, bOp=equal_to)      // 返回pair存储两个区间第一个不同点的迭代器
destE   merge(b1, e1, b2, e2, destB, bOp=less)
void    inplace_merge(b, m, e, bOp=less)                            // 将同一个集合中的两部分合并，两部分都有序
bool    includes(b1, e1, b2, e2, bOp=equal_to)                      // 区间`[b2, e2)`是否为区间`[b1, e1)`的**子序列**
destE   set_union(b1, e1, b2, e2, destB, bOp=less)                  // 并集
destE   set_intersection(b1, e1, b2, e2, destB, bOp=less)           // 交集
destE   set_symmetric_difference(b1, e1, b2, e2, destB, bOp=less)   // 并集去交集
destE   set_difference(b1, e1, b2, e2, destB, bOp=less)             // 前一个集合去交集
```

<!-- entry begin: 极值算法 -->

#### 极值算法

```cpp
T       max(x, y)
T       max(il)
T       min(x, y)
T       min(il)
pair    minmax(x, y)                    // 返回`pair<min, max>`
pair    minmax(il)                      // 返回`pair<min, max>`
T       clamp(x, min, max)              // 若x小于min则返回min，否则若x大于max则返回max，否则返回x
T       min_element(b, e, bOp=less)     // 返回第一个最小值
T       max_element(b, e, bOp=less)     // 返回第一个最大值
T       minmax_element(b, e, bOp=less)  // 返回第一个最小值和最后一个最大值
```

<!-- entry end -->

<!-- entry begin: partition_point partition_copy stable_partition partition is_partitioned 划分算法 sample shuffle rotate_copy rotate reverse_copy reverse prev_permutation next_permutation is_permutation lexicographical_compare equal sort_heap pop_heap push_heap make_heap is_heap_until is_heap nth_element partial_sort_copy partial_sort stable_sort sort is_sorted_until is_sorted 变序算法 -->

#### 变序算法

```cpp
bool    is_partitioned(b, e, uOp)
m       partition(b, e, uOp)                                // 返回划分的前半部分的尾后迭代器
m       stable_partition(b, e, uOp)                         // 返回划分的前半部分的尾后迭代器
p<t,f>  partition_copy(b, e, destTrueB, destFalseB, uOp)    // 返回pair分别保存两个目标区间的尾后迭代器
m       partition_point(b, e, uOp)                          // 返回满足uOp()为true的区间的尾后迭代器

bool    is_sorted(b, e, bOp=less)
sortE   is_sorted_until(b, e, bOp=less)                 // 返回已排序区间的尾后迭代器
void    sort(b, e, bOp=less)
void    stable_sort(b, e, bOp=less)
void    partial_sort(b, m, e, bOp=less)
copyE   partial_sort_copy(b, e, destB, destE, bOp=less) // 若dest范围不够则取排序后的前面的元素
void    nth_element(b, m, e, bOp=less)                  // 按m所指的元素作分界进行两边划分，左边元素小于或等于右边元素

bool    is_heap(b, e, bOp = less)
sortE   is_heap_until(b, e, bOp = less)                 // 返回已堆排序区间的尾后迭代器
void    make_heap(b, e, bOp=less)                       // 创建最大堆
void    push_heap(b, e, bOp=less)                       // 将e-1插入到[b, e-1)的最大堆中（上滤）
void    pop_heap(b, e, bOp=less)                        // 交换b与e-1的元素，并重新调整[b, e-1)的最大堆（下沉）
void    sort_heap(b, e, bOp=less)                       // 将最大堆进行序列的升序排序

bool    equal(b, e, cmpB, bOp = equal_to)
bool    lexicographical_compare(b1, e1, b2, e2, op=less)// 比较两区间字典序
bool    is_permutation(b1, e1, b2, bOp=equal_to)        // 检测两个区间的所有元素是否为同一个集合，即不考虑顺序
bool    next_permutation(b, e, op=less)                 // 当元素为完全降序时返回false
bool    prev_permutation(b, e, op=less)                 // 当元素为完全升序时返回false

void    reverse(b, e)
destE   reverse_copy(b, e, destB)
b+e-m   rotate(b, m, e)                                 // 返回原本的begin现在的位置
destE   rotate_copy(b, m, e, destB)

void    shuffle(b, e, randomEngine)
destE   sample(b, e, destB, cnt, randomEngine)          // 随机取cnt个值到destB
```

<!-- entry end -->

<!-- entry begin: swap_ranges swap transform transform generate_n generate fill_n fill replace_copy_if replace_copy replace_if replace shift_right shift_left unique_copy unique remove_copy_if remove_copy remove_if remove copy_n copy_if copy_backward copy move_backward move 更易算法 -->

#### 更易算法

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
rmCnt   erase(VecOrDeq, v)                      // 真正删除而非将元素后移
rmCnt   erase_if(VecOrDeq, v, uOp)              // 真正删除而非将元素后移
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

<!-- entry begin: mismatch minmax_element max_element min_element clamp minmax minmax min min max max set_difference set_symmetric_difference set_intersection set_union includes inplace_merge merge 集合算法 -->

### 输入输出库

#### 组件总览

<!-- entry begin: iostream iosfwd 组件总览 -->

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

typename std::ios_base  // (定义了一些标志位)
typename streambuf      // (系统I/O并缓存数据, 提供位置信息)
typename locale         // (包含facet将I/O进行进行本地格式化)
typename stream         // (封装上述两者, 提供状态、格式化信息)
typename centry         // (帮助stream每次I/O预处理与后处理)
操作符                  // (提供调整stream的便捷方法)
```

<!-- entry end -->

<!-- entry begin: iostream basic_ios sync_with_stdio 基础操作 -->

#### 基础操作

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
    buf*    rdbuf(buf*)         // 设置新的缓冲区并**清空状态**。注意缓冲区的生命周期
    strm&   copyfmt(strm&)      // 复制本地环境、格式化标志、tie()关联目标等信息到本地

    locale  getloc()
    locale  imbue(locale)
    char    narrow(CharT, def)  // 将CharT转换为char并返回，若失败则返回def
    CharT   widen(char)         // 将char转换为CharT
};
bool ios::sync_with_stdio(bool sync = true) // 是否开启多线程同步，以及与C缓冲区的互操
```

<!-- entry end -->

#### 非格式输入输出

<!-- entry begin: 非格式输入输出 非格式化IO 非格式化I/O 底层IO 底层I/O -->

```cpp
class basic_iostream<CharT>
{
    int     get()
    strm&   get(char&)
    strm&   get(char*, count, delim='\n')       // 最多读取 count-1 个字符，读到delim提前结束，并自动添加'\0'在末尾
    strm&   get(ostrmbuf&, delim='\n')
    strm&   getline(char*, count, delim='\n')   // 最多读取 count-1 个字符，读取到delim则丢弃之并将提前结束，并自动添加'\0'在末尾
    strm&   read(char*, count)                  // 最多读取 count 个字符
    size    readsome(char*, count)              // 返回读取字符数, 最多读取count个字符。只从缓冲区中读取已有字符，而不陷入系统调用
    size    gcount()                            // 返回上次无格式读取字符数
    strm&   ignore(count=1, delim=eof)          // 忽略包括delim
    int     peek()                              // 返回下个字符, 但不移动iterator
    strm&   unget()                             // iterator回移一个宽度
    strm&   putback(char)                       // 将指定字符置入流中
    strm&   sync()                              // 对输出流冲刷缓冲区，对输入流清空缓冲区并重新读取

    stm&    put(char)
    strm&   write(char*, count)
    strm&   flush()

    pos     tellp()
    pos     tellg()
    strm&   seekp(pos)
    strm&   seekg(pos)
    strm&   seekp(offset, dir)
    strm&   seekg(offset, dir)
    // dir包括std::ios_base::{beg, end, cur}
};
```

<!-- entry end -->

<!-- entry begin: iostream 预定义 -->

#### 预定义操作

```txt
整数：
     八进制：[0-7]+
     十进制：[0-9]+
     十六进制：(0x|0X)?[0-9a-fA-F]+

浮点型： ([0-9]+\.?[0-9]*|\.[0-9]+)(e[+-]?[0-9]+)

其他：bool, char, char*, void*, string, streambuf*, bitset, complex
```

<!-- entry end -->

<!-- entry begin: iomanip -->

#### 操作符

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

<!-- entry begin: fstream -->

#### 文件流

```cpp
#include <fstream>
class basic_iofstream { // 预定义有 fstream ifstream ofstream wfstream wifstream wofstream
    // 构造函数
    basic_iofstream(filename, oflag)
    /*
     * in               // 只读             （文件必须存在）
     * out              // 截断             （有必要则创建）
     * app              // 追加             （有必要则创建）
     * out|trunc        // 截断             （有必要则创建）
     * out|app          // 追加             （有必要则创建）
     * in|out           // 读写             （文件必须存在）
     * in|out|trunc     // 读写，截断       （有必要则创建）
     * in|out|app       // 读写，追加       （有必要则创建）
     * in|app           // 读写，追加       （有必要则创建）
     * binary           // 不将`\r\n`替换为`\n`
    */
    // 特有成员函数
    bool    is_open()
    void    open()
    void    close()
};
```

<!-- entry end -->

<!-- entry begin: sstream stringstream -->

#### 字符流

```cpp
#include <sstream>
class iostringstream {  // 预定义有stringstream istringstream ostringstream wstringstream wistringstream wostringstream
    // 构造函数
    stringstream(str)
    // 特有成员函数
    string_view view()
    string      str()       // 返回缓冲区的字符，与是否已被读取无关
    string      str(str)    // 该操作不清除流状态！！！
};
```

<!-- entry end -->

<!-- entry begin: streambuf -->

#### 流缓冲区

```cpp
#include <streambuf>

class basic_streambuf<CharT> {
    // 析构时
    // basic_iostream族类析构时不会销毁streambuf
    // 其他流类析构时只不析构由rdbuf(buf*)得到的缓冲区

    // 成员函数
    // 流与缓冲区独立，注意清除流的状态；还要注意以下4个函数只对stringstream_buf适用，因为它不冲刷缓冲区
    // 注意which需要适当给出，如istream不能接受out
    pos pubseekoff(off, dir, which=in|out);    // dir有std::ios_base::{beg, cur, end}
    pos pubseekpos(pos, which=in|out);         // which有std::ios_base::{in, out}
    int sgetc();        // 获取下个字符而不移动指针
    int sungetc();      // 回移指针
};
// 高效非格式化输入输出：
// 利用streambuf_iterator避开构造sentry进行非格式化I/O
// 利用streambuf*预定义的输入输出操作进行直接I/O

// 利用streambuf来通过文件描述符构造
#include <ext/stdio_filebuf.h>
#include <ios>
#include <cstdio>
/* ... */
auto fd = fileno(file);
__gnu_cxx::stdio_filebuf<char> buf{fd, std::ios_base::in};
std::istream istrm{&buf};
```

<!-- entry end -->

<!-- entry begin: locale -->

#### 本地环境

```cpp
#include <locale>
class locale {
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

### 线程支持库

#### 线程启动

<!-- entry begin: future shared_future async -->

```cpp
#include <future>
class future<T> {
    // 特种成员：支持move拒绝copy
    // 成员函数
    bool    valid();                // 查看状态是否有效（未被获取并销毁）
    T       get();                  // 获取并销毁状态（线程返回值或传递异常）。启动延迟线程
    void    wait();                 // 等待线程结束。启动延迟线程
    status  wait_for(duration);     // 等待线程结束
    status  wait_until(time_point); // 等待线程结束
    // status包括std::future_status::{deferred, timeout, ready}

    shared_future share();          // 返回shared_future，其提供了copy操作且可多次调用get()而不销毁状态
};
// 注意：future最后一个副本在销毁时会同步等待线程结束，故要想异步则必须nodiscard
future  async(function, args...);                   // 优先异步调用，不可行则延迟发射
future  async(launch_policy, function, args...);    // std::launch::async或std::launch::deferred
```

<!-- entry end -->

<!-- entry begin: this_thread thread -->

```cpp
#include <thread>
class thread {
    // 构造函数
    thread();
    thread(function, args...);      // 构造同时启动线程
    // 成员函数
    tid     get_id();               // 获取线程ID
    bool    joinable();             // 检查线程是否活跃
    void    join();                 // 等待线程结束并释放资源，调用后non-joinable
    void    detach();               // 卸离线程一旦退出自动释放资源，调用后non-joinable
    handle  native_handle()         // 返回实现定义的线程句柄
};
// 对joinable的线程必须调用其join()，否则程序异常终止
// 线程detach后便无必须将其join，故主线程终止后程序（所有线程）终止
// thread属于异步操作，防止function的传引用参数悬垂，thread会将args复制到内部。若用户可保证生命周期则可使用std::ref()

namespace std::this_thread {
    tid     get_id();               // 获取当前线程ID
    void    yield();                // 建议系统切换其他线程执行
    void    sleep_for(duration);    // 阻塞当前线程一段时间
    void    sleep_until(time_point);// 阻塞当前线程直到指定时间
}
```

<!-- entry end -->

#### 线程同步

> 并发会出现问题的地方
>
> - 访问可能会改变的共享对象
> - 阻塞以等待异步事件

<!-- entry begin: mutex lock try_lock call_once lock_guard scoped_lock unique_guard shared_guard 互斥锁管理器 -->

```cpp
#include <mutex>
#include <shared_mutex>
// 互斥锁：不可copy不可move
typename mutex                                       // 支持前3个操作
typename timed_mutex                                 // 支持前5个操作
typename recursive_mutex                             // 支持前3个操作，支持多次上锁与解锁
typename recursive_timed_mutex                       // 支持前5个操作，支持多次上锁与解锁
typename shared_mutex                                // 支持除后2个之外的操作
typename shared_timed_mutex                          // 支持所有操作
// 成员函数
class Mutex {
    void    lock()                              // 获取锁（原子操作：读取-测试-上锁/阻塞）
    void    unlock()                            // 释放锁
    bool    try_lock()                          // 尝试获取锁，成功返回true
    bool    try_lock_for(duration)              // 尝试获取锁，成功返回true
    bool    try_lock_until(time_point)          // 尝试获取锁，成功返回true
    void    lock_shared()                       // 释放读锁
    void    unlock_shared()                     // 释放读锁
    bool    try_lock_shared()                   // 尝试获取锁读锁
    bool    try_lock_shared_for(duration)       // 尝试获取锁读锁
    bool    try_lock_shared_until(time_point)   // 尝试获取锁读锁
};
// 辅助函数
void    lock(Mutex...)                      // 阻塞直至获取所有锁，或解锁已获取的锁并抛出异常（死锁）
int     try_lock(Mutex...)                  // 若全部获取则返回-1，否则解锁已获取的锁并返回第一个无法获取的锁的次序（加锁次序与实参次序相同且从0开始编号）
void    call_once(once_flag, Func, Args...) //根据once_flag（不可copy不可move）来判断并只调用一次`func(args...)`
// 互斥锁管理器
typename    lock_guard<Mutex>       // 支持第2、3种构造函数，且不支持下述成员函数
typename    scoped_lock<Mutex...>   // 支持第1个构造函数
typename    unique_guard<Mutex>     // 支持除第1个外全部构造函数
typename    shared_guard<Mutex>     // 支持除第1个外全部构造函数。上读锁
class Lock {
    // 特种成员：支持move拒绝copy
    Lock(Mutex...)                          // 免死锁上锁
    Lock(Mutex)                             // 获取锁
    Lock(Mutex, std::adopt_lock)            // 接管已上锁的锁
    Lock(Mutex, std::defer_lock)            // 不上锁
    Lock(Mutex, std::try_lock)              // 尝试上锁
    Lock(Mutex, duration)                   // 尝试上锁
    Lock(Mutex, time_point)                 // 尝试上锁
    ~Lock()                                 // 释放锁
    // 成员函数
    void    lock()
    void    unlock()
    bool    try_lock()
    bool    try_lock_for(duration)
    bool    try_lock_until(time_point)
    Mutex*  release()
    Mutex*  mutex()
    bool    owns_lock()
    operator bool()
};
```

<!-- entry end -->

<!-- entry begin: cv condition_variable 条件量  -->

```cpp
#include <condition_variable>
class condition_variable {
    // 构造函数：不可copy不可move

    // 等待：若条件不满足则原子性解锁-唤醒-阻塞
    void    wait(unique_lock)
    void    wait(unique_lock, Pred)
    cv_stat wait_for(unique_lock, duration)
    bool    wait_for(unique_lock, duration, Pred)
    cv_tsat wait_until(unique_lock, time_point)
    bool    wait_until(unique_lock, time_point, Pred)
    // cv_stat为std::cv_status::timeout或std::cv_status::no_timeout

    // 唤醒：由条件量等待阻塞的线程，只能由同一条件量唤醒
    void    notify_one()
    void    notify_all()
};
void    notify_all_at_thread_exit(condition_variable, unique_lock)
```

<!-- entry end -->

<!-- entry begin: atomic -->

#### 原子操作库

```cpp
#include <atomic>
struct atomic<T> {  // 内置类型均有预定义别名，C++20针对shared_ptr与weak_ptr有特化以支持原子访问其指向的数据
    // 构造
    atomic()        // 构造时初始化lock
    atomic(T)       // 非原子
    // 成员函数：
    operator T()
    operator=(val)
    T       load()                              // 返回原值拷贝
    void    store(val)                          // 赋值val
    T       exchange(val)                       // 赋值val并返回旧值拷贝
    bool    compare_exchange_strong(exp, val)   // 若*this==exp，则*this=val
    bool    compare_exchange_weak(exp, val)     // 同上，可能更高效但同时可能假失败
    // 仅支持内置类型
    T       fetch_add(t)
    T       fetch_sub(t)
    T       fetch_and(t)
    T       fetch_or(t)
    T       fetch_xor(t)
    operator--()
    operator++()
    operator--(int)
    operator++(int)
    operator-=(val)
    operator+=(val)
    operator&=(val)
    operator|=(val)
    operator^=(val)
};
struct atomic_ref<T> {
    operator T()
    operator=(val)
    bool    is_lock_free()
    T       load()
    void    store(t)
    T       exchange(t)
    bool    compare_exchange_strong(exp, val)
    bool    compare_exchange_weak(exp, val)
    void    wait(t)         // 等待直到被提醒且原子值被更改为不等于t
    void    notify_one()
    void    notify_all()
    // 仅支持内置类型
    T       fetch_add(t)
    T       fetch_sub(t)
    T       fetch_and(t)
    T       fetch_or(t)
    T       fetch_xor(t)
    operator--()
    operator++()
    operator--(int)
    operator++(int)
    operator-=(val)
    operator+=(val)
    operator&=(val)
    operator|=(val)
    operator^=(val)
};
```

<!-- entry end -->

### 文件系统库

<!-- entry begin: filesystem fs path directory_entry -->

#### 路径与文件表示

```cpp
#include <filesystem>
class path {
    // 构造函数
    path()
    path(str)
    path(b, e)
    // 修改器
    operator/=()
    operator+=()
    void    clear()
    path&   remove_filename()
    path&   replace_filename()
    path&   replace_extension()
    // 观察器
    CString c_str()
    String  native()
    operator String()
    itr     begin()
    itr     end()
    // 分解
    // Linux以`/tmp/fs.cpp`为例，Windows以`C:\tmp\fs.cpp`为例
    path    root_path()                 // `/`          或`C:\`
    path    root_name()                 // ` `          或`C:`
    path    root_directory()            // `/`          或`\`
    path    relative_path()             // `tmp/fs.cpp` 或`tmp\fs.cpp`
    path    parent_path()               // `/tmp/`      或`C:\tmp\`
    path    file_name()                 // `fs.cpp`
    path    stem()                      // `fs`
    path    extension()                 // `.cpp`
    // 查询
    bool    empty()
    bool    is_absolute()
    bool    is_relative()
    bool    has_root_path()
    bool    has_root_name()
    bool    has_root_directory()
    bool    has_relative_path()
    bool    has_parent_path()
    bool    has_file_name()
    bool    has_stem()
    bool    has_extension()
};
class directory_entry { // 存储一个path作为成员，并可能附带文件属性
    // 构造函数
    directory_entry()
    directory_entry(path)
    // 修改器
    void    replace_filename(path)      // 设置文件名，并更新缓存属性。仅更改path不更改文件系统
    void    refresh()                   // 更新缓存的文件属性
    // 观察器
    C path& path()
    operator C path&()
};
```

<!-- entry end -->

#### 文件属性

<!-- entry begin: permissions is_character_file is_block_file is_fifo is_socket is_symlink is_directory is_regular_file symlink_status status permissions file_status file_type perms -->

```cpp
#include <filesystem>
class file_status {
    // 成员函数
    ft      type();             // 返回文件类型
    void    type(ft);           // 设置文件类型
    perm    permissions();      // 返回文件权限
    void    permissions(perm);  // 设置文件权限
};
enum class file_type {
    none,
    not_found,
    regular,
    directory,
    symlink,
    block,
    character,
    fifo,
    socket,
    // 实现附加定义
    unkown,
};
enum class perms {
    none,         // 0000
    owner_read,   // 0400
    owner_write,  // 0200
    owner_exec,   // 0100
    owner_all,    // 0700
    group_read,   // 0040
    group_write,  // 0020
    group_exec,   // 0010
    group_all,    // 0070
    others_read,  // 0004
    others_write, // 0002
    others_exec,  // 0001
    others_all,   // 0007
    all,          // 0777
    set_uid,      // 04000
    set_gid,      // 02000
    sticky_bit,   // 01000
    mask,         // 07777
    unknown       // 0xFFFF
};
file_status status(path);
file_status symlink_status(path);
bool        is_regular_file()
bool        is_directory()
bool        is_symlink()                            // 符号链接除此之外还具有链接目标的文件类型
bool        is_socket()
bool        is_fifo()
bool        is_block_file()
bool        is_character_file()
void        permissions(path, perms, perm_options)  // 修改权限
// perm_options包括： std::filesystem::{replace, add, remove, nofollow}
```

<!-- entry end -->
<!-- entry begin: fs filesystem relative canonical absolute exists equivalent temp_directory_path current_path hard_link_count last_write_time resize_file file_size space  -->

```cpp
#include <filesystem>
struct space_info
{
    uintmax_t   capacity;   // 文件系统总字节大小
    uintmax_t   free;       // 文件系统空闲空间
    uintmax_t   available;  // 普通进程可用空闲空间
};

space_info  space(path);
size        file_size(path);
void        resize_file(path, new_size);
time_point  last_write_time(path);
time_point  last_write_time(path, new_time);
size_t      hard_link_count(path);

path        current_path()              // 获取当前工作路径
void        current_path(path)          // 设置当前工作路径
path        temp_directory_path()       // 获取临时目录
bool        equivalent(path1, path2)    // 判断两路径是否为同一文件
bool        exists()
path        absolute(path)              // 将path转换为绝对路径（可能带有`..`）
path        canonical(path)             // 将path转换为绝对路径（无点、双点元素或符号链接）
path        relative(path)              // 将path根据当前工作目录转换为相对路径
```

<!-- entry end -->

#### 文件操作

<!-- entry begin: copy rename remove_all remove create_hard_link create_directory_symlink create_symlink copy_symlink read_symlink create_directories create_directory disable_recursion_pending recursion_pending pop depth filesystem fs directory_iterator recursive_directory_iterator  -->

```cpp
// 文件系统库中函数发生错误时（如权限不足、文件不存在等等），
// 一般都提供一个版本的重载用于传递一个error_code&来关闭该次调用的异常机制
class directory_iterator {
    directory_iterator()                // 默认构造尾后迭代器
    directory_iterator(path , dir_opt)  // 构造指定路径的目录项迭代器（不递归子目录）
};
class recursive_directory_iterator {
    recursive_directory_iterator()                // 默认构造尾后迭代器
    recursive_directory_iterator(path , dir_opt)  // 构造指定路径的目录项迭代器（递归子目录）

    int     depth()                     // 返回当前递归深度（目录深度），从0开始
    void    pop()                       // 返回上级目录
    bool    recursion_pending()         // 返回当前目录是否禁用递归
    void    disable_recursion_pending() // 下次自增前禁用递归（用于跳过某些目录）
};
enum class directory_options {
    none,                               // （默认）跳过符号链接、权限拒绝是错误
    follow_directory_symlink,           // 跟随符号链接
    skip_permission_denied              // 跳过不具权限的项
};

bool    create_directory(path)          // 创建目录文件，其路径必须存在
bool    create_directory(path, epath)   // 从已存在的文件复制属性
bool    create_directories(path)        // 创建目录文件，其路径不存在则创建

path    read_symlink(path)              // 获取符号链接指向的文件path（可能为相对路径）
void    copy_symlink(target, link)      // 创建符号链接（不跟随符号链接）
void    create_symlink(target, link)    // 创建符号链接（不跟随符号链接）
void    create_directory_symlink(tar, link) // 有些系统单独区分目录的符号链接
void    create_hard_link(target, link)  // 创建硬链接

void    remove(path)                    // 删除目标文件
void    remove_all(path)                // 递归删除整个目录下文件
void    rename(old_p, new_p)            // 移动或更名
void    copy(from, to, copy_option)

enum class copy_options {
    none,                   // 默认
    recursive,              // 目录递归复制
    skip_existing,          // 跳过已存在文件
    overwrite_existing,     // 覆盖已存在文件
    update_existing,        // “更新”已存在文件
    copy_symlinks,          // 复制符号链接本身而不（默认）跟随之
    skip_symlinks,          // 忽略符号链接
    directories_only,       // 仅复制目录结构，忽略非目录文件
    create_symlinks,        // 创建符号链接
    create_hard_links,      // 创建硬链接
};
```

<!-- entry end -->

## BOOST 库

<!-- entry begin: boost locale codecvt 字符编码转换 字符转换 -->

### 编码转换

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
std::basic_string<CharT>    utf_to_utf(b, e);
std::basic_string<CharT>    utf_to_utf(Str);
// from端与to端的编码均需显示指出，且类型为char
std::string                 between(b, e, to_charset, from_charset);
std::string                 between(str, to_charset, from_charset);
```

<!-- entry end -->

### 异步与网络库

核心概念（Proactor 设计模式）

![Proactor](images/proactor.png)

<https://think-async.com/Asio/asio-1.22.2/doc/asio/overview/core/async.html>

服务器一般存在三类状态机：

- io_context 为其一，负责运转其他状态机
- acceptor 为其二，负责创建一个新状态机
- 每个连接为其三，负责程序主要工作

&emsp;一般设计：

- Server 类 ：管理[io_context/ssl::context]与 acceptor
- Session 类：管理 socket 与 buffer。
  > 利用`bind(&Session::handler, shared_from_this())`
  > 或 lambda 捕获 shared_from_this()来保证异步操作过程中 buffer 一直有效

<!-- entry begin: boost asio io_context 异步 -->

```cpp
#include <boost/asio.hpp>   // 集成于boost库中的asio，命名空间boost::asio
#include <asio.hpp>         // 独立于boost库的asio  ，命名空间asio::
class io_context {
    // 构造函数
    io_context()
    io_context(int concurrency_hint);// 若传入1则会跳过所有锁机制
    // 成员函数

    // 等待事件完成-分发执行句柄-调用回调函数-重回等待状态。handler只在调用下列函数的线程中被调用
    count       run();              // 返回完成了多少异步操作，下同
    count       run_for(du);
    count       run_until(tp);
    count       run_one();
    count       run_one_for(du);
    count       run_one_until(tp);
    // 仅执行已完成的异步操作的回调函数，不等待全部异步操作结束
    count       poll();
    count       poll_one();

    void        stop();
    bool        stopped();
    void        restart();
    executor&   get_executor();
};
?       post(ex, handler);          // 将handler加入完成事件队列，保证在该函数返回前不会调用handler
?       dispath(ex, handler);       // 将handler加入完成事件队列，保证在该函数返回前开始调用handler
wfunc   bind_executor(ex, func);    // 返回包装后的func，调用wfunc()相当于调用dispath(ex, func)

my_execution_context.notify_fork(execution_context::fork_prepare);
if (fork() == 0) {
  my_execution_context.notify_fork(execution_context::fork_child);
} else {
  my_execution_context.notify_fork(execution_context::fork_parent);
}

// 构造I/O object一般需要一个io_context做第一个参数；
// 注册回调函数时，默认使用对应的io_context的Executor；

// 可能出错而抛出异常的操作一般都提供一个版本的重载用于传递一个error_code&来关闭该次调用的异常机制
// 异步版本的低速操作的handler的参数一般为 error_code + 同步版本返回值
// 异步操作的协程版本的返回值与同步版本的返回值一样
```

<!-- entry end -->

<!-- entry begin: 网络地址 asio::ip::address network endpoint resolver -->

```cpp
class ip::address { // 可以是ipv4也可是ipv6
    // 成员函数
    bool        is_loopback();
    bool        is_multicast();
    bool        is_unspecified();
    bool        is_v4();
    bool        is_v6();
    address_v4  to_v4();
    address_v6  to_v6();
    str         to_string();
    operator<<
};
address     ip::make_address(str);      // 返回IP地址，点分十进制或十六进制计数法
address_v4  ip::address_v4::loopback(); // 返回IPv4表示本地回环127.0.0.1
address_v4  ip::address_v4::any();      // 返回IPv4表示任意地址0.0.0.0
address_v4  ip::address_v4::broadcast();// 返回IPv4表示绝对广播255.255.255.255
string      ip::host_name();            // 返回本地主机名

class ip::network_v4 {  // ip::network_v6
    // 一般用 ip::make_network_v4("192.168.0.1/24") 构造
    // 成员函数
    address_v4  address();      // 192.168.0.1
    ushort      prefix_length();// 24
    address_v4  netmask();      // 255.255.255.0
    network_v4  canonical();    // 192.168.0.0/24
    address_v4  network();      // 192.168.0.0
    address_v4  broadcast();    // 192.168.255.255
    bool        is_subnet_of(network_v4);
    range       hosts();        // 返回ip::address_v4_range
    str         to_string();
    operator<<
};
network_v4  ip::make_network_v4(str);   // 返回带掩码的IP地址，一般用于本机判断网段

class ip::tcp::endpoint {       // ip::udp::endpoint、local::stream_protocol::endpoint
    // 构造函数
    endpoint(address, portNum);
    endpoint(protocol, protNum);        // protocol一般为ip::tcp::v4()从而构造关于0.0.0.0的endpoint
    /* endpoint(sockfile); */           // UNIX域socket的endpoint接受字符串指定文件名
    // 成员函数
    address     address();
    void        address(address);
    ushort      port();
    void        port(portNum);
};

class ip::tcp::resolver {
    // 构造函数
    resolver(ex);
    // 成员函数
    // handle：void(const asio::error_code&, resolver::resilts_type&) 返回的查询结果是一个range
    ?       async_resolve(host, service, [flag,] handle);
    /* flag 位于 class ip::resolver_base 中
     * address_configured   根据系统是否设置有non-loopback地址而只返回IPv4或IPv6
     * all_matching         若同时指定了v4_mapped，则返回所有匹配到的IPv4与IPv6
     * v4_mapped            若指定查询IPv6但会找到IPv6地址，则返回IPv4映射的IPv6地址
     * numeric_host         强制指定host为数字表示而非域名表示。默认二者皆可
     * numeric_service      强制指定service为数字表示而非服务名表示。默认二者皆可
     * passive              指示返回的endpoint用于绑定本地，此时host应指定为""
    */
    void        cancel();
};
```

<!-- entry end -->

<!-- entry begin: 网络 asio buffer socket -->

```cpp
// 无需保证size()足够，缓冲区自动扩张最大为max_size
buffer    dynamic_buffer(vector, max_size=UMAX);
buffer    dynamic_buffer(string, max_size=UMAX);
// 需要保证size()足够，缓冲区大小限制为min(C.size(), max_size)
buffer    buffer(void*, size_t);
buffer    buffer(array[, max_size]);
buffer    buffer(vector[,max_size]);
buffer    buffer(string[,max_size]);

class ip::tcp::acceptor {
    // 构造函数
    acceptor(ex, [endpoint]);
    // 成员函数
    void        bind(endpoint);     // 意味着只接受来自绑定的IF的数据包
    endpoint    local_endpoint();

    // handle：void(const asio::error_code&, ip::tcp::socket&)
    ?           async_accept(handler);
    // handle：void(const asio::error_code&)
    ?           async_accept(peer_sock, handler);

    void        cancel();
    void        close();
    void        open([protocol]);
    bool        is_open();
};

class ip::tcp::socket {
    // 构造函数
    socket(ex);
    socket(ex, [endpoint]);

    // 成员函数
    void        bind(endpoint);
    endpoint    local_endpoint();
    endpoint    remote_endpoint();
    size        available();                            // 返回socket缓冲区已接收字节数（可无阻塞读取）

    // void(const asio::error_code&)
    ?           async_connect(endpoint, handler);
    // flag   ：ip::socket_type::socket::{message_peek, message_out_of_band}
    // handler：void(const asio::error_code&, size)
    ?           async_read_some(buffer,[flag,] handler);    // 读到EOF抛出异常!
    ?           async_write_some(buffer,[flag,] handler);
    // 以下两成员函数为ip::udp::socket特有
    /* ?        async_receive_from(buffer, endpoint,[flag,] handler); */
    /* ?        async_send_to(buffer, endpoint,[flag,] handler);      */

    void        cancel();
    void        shutdown(what);                         // ip::tcp::socket::{shutdown_send, shutdown_receive, shutdown_both}
    void        close();                                // 关闭底层socket
    void        open([protocol ]);                      // 打开底层socket。ip::tcp::{v4(), v6()}
    bool        is_open();
};
// condition：bool(const asio::error_code&, endpoint next)          // condition在每次连接尝试之前调用，返回false则跳过该ep
// handler  ：void(const asio::error_code&, iterator)               // iterator为当前连接成功的迭代器
?       async_connect(socket, begin, [end,] [condition], handler);  // resolver得到的endpoint的ranger可用于此处begin

// completion：size_t(const asio::error_code&, size)                // 返回size_t表示下次读取多少个字符，一直重复读取到其返回0
// handler   ：void(const asio::error_ code&, size)
?       async_read    (stream, buffer [, completion] ,handler)
?       async_write   (stream, buffer [, completion] ,handler)
?       async_read_at (stream, offset, buffer [, completion], handler)
?       async_write_at(stream, offset, buffer [, completion], handler)

// completion：pair<end,bool>(begin, end)                           // 返回的end表示下次调用completion的参数begin
// handler   ：void(const asio::error_ code&, size)
?       async_read_until(stream, buffer, delim, handler)            // delim为string或regex
?       async_read_until(stream, buffer, completion, handler)

// asio提供的completion快捷函数对象有
transfer_all()          // 默认completion
transfer_at_least(n)
transfer_exactly(n)
```

<!-- entry end -->

<!-- entry begin: 网络 asio ssl -->

```cpp
#include <asio/ssl.hpp>
class ssl::context {
    // 构造函数
    context(method);                // method一般为ssl::sslv23

    void    set_options(opts);      // ssl::context::default_workarounds
    void    clear_options(opts);

    // 被验证方：
    // callback：string(size_t max_len, int password_purpose); 第二参数ssl::context::{for_reading, for_writing}
    void    set_password_callback(callback);
    // format为ssl::context::{pem, asn1}
    void    use_certificate(buffer, format);
    void    use_private_key(buffer, format);
    void    use_certificate_file(file, format);
    void    use_private_key_file(file, format);

    // 验证方：
    // callback为bool(bool preverified, ssl::verify_context&);  其中利用可OpenSSL API来获取证书信息
    void    set_verify_callback(callback);
    void    set_default_verify_paths();
    void    add_verify_path(path);
    void    add_certificate_authority(buffer);
    void    load_verify_file(file);

    // 设置验证方式
    void    set_verify_mode(verify_mode);
    // verify_mode包括ssl::{
    //  verify_none,                 // 不验证对方证书
    //  verify_peer,                 // 验证对方证书
    //  verify_fail_if_no_peer_cert, // 若对方无证书则失败，C验证S时无则默认失败，S验证C时无则默认继续
    //  verify_client_once           // 仅验证一次客户端的证书
    // }
};

class ssl::stream<Socket> {
    // 构造函数
    stream(socket, ssl::context);
    stream(ex, ssl::context);
    // 成员函数
    socket& lowest_layer();         // 返回socket&用于connect、shutdown
    // handshake_type：ssl::stream_base::{server, client}
    // handler       ：void(const asio::error_code&);
    ?       async_handshake(handshake_type, handler)
    ?       async_shutdown(handshake_type, handler)
    // handler       ：void(const asio::error_code&, size)
    ?       async_read_some(buffer, handler)
    ?       async_write_some(buffer, handler)
};
// 可使用async_read()等free function
```

<!-- entry end -->

<!-- entry begin: asio co_spawn awaitable use_await 协程 -->

```cpp
#include <asio/co_spawn.hpp>
asio::awaitable<void> test_asio_with_coroutine()
{
    // 异步获取当前协程的executor，通过某处调用co_spawn(executor, awaitable, token);实现
    auto executor = co_await asio::this_coro::executor;

    tcp::acceptor acceptor{ex, tcp::endpoint{tcp::v4(), 50001}};

    // 异步调用的协程版本，handler用asio::use_awaitable代替，返回值为同步版本返回值
    // 可以定义宏ASIO_ENABLE_HANDLER_TRACKING来使用编译器预定义宏来讲源码位置传给use_awaitable
    auto sock = co_await acceptor.async_aceept(asio::use_awaitable);
}
// ex为executor或者io_context，用于调度协程的执行（实质就是调度回调函数，而回调函数负责恢复相关协程）
// awaitable为调用协程的（第一次）返回值，内含协程句柄可用于恢复协程等操作
// void handler(std::exception_ptr, T); T为协程co_return结束返回的值的类型（void则无），若无则可使用asio::detached
?   co_spawn(ex, awaitable, handler);
?   co_spawn(ex, ret_awaitable_func, handler);  // co_spawn保证传入的函数对象生命周期不短于协程生命周期
```

<!-- entry end -->

<!-- entry begin: asio signal_set -->

```cpp
class signal_set {
    // 构造函数
    signal_set(ex [,signal1] [,signal2]);
    // 成员函数
    void    add(signal);
    void    remove(signal);
    void    clear();
    ?       async_wait(handler);    // void(const asio::error_code&, int signal);
    void    cancel();
};
```

<!-- entry end -->

<!-- entry begin: asio steady_timer system_timer -->

```cpp
class basic_waitable_timer {    // 预定义有steady_timer、system_timer等
    // 构造函数
    basic_waitable_timer(ex, duration); // ex可以是executor或者是io_context（提供其自身的executor）
    basic_waitable_timer(ex, time_point);
    // 成员函数
    ?           async_wait(WaitHandle); // void(const asio::error_code&)
    time_point  expiry();
    ?           expires_at(tp);
    ?           expires_after(du);
    size_t      cancel();               // 返回取消的异步操作数目。取消后调用回调函数（以参数asio::error::operation_aborted）
    size_t      cancel_one();           // 返回取消的异步操作数目（0或1），按FIFO顺序取消，并调用回调函数
    executor    get_executor();
};
```

<!-- entry end -->

## GOOGLE 库

<!-- entry begin: 日志库 glog -->

### 日志库

```cpp
#include <glog/logging.h>
int main(int argc, char* argv[]) {
    // 初始化glog
    google::InitGoogleLogging(argv[0]);
    // 使用函数func代替发生FATAL日志或CHECK失败时的终止函数，一般在其内调用exit(1)
    google::InstallFailureFunction(void(*func)());
    // 日志清理
    google::EnableLogCleaner(ndays);    // 日志有效期为n天，每次冲刷日志时检测
    google::DisableLogCleaner();        // 关闭自动清理
    google::FlushLogFiles(google::INFO) // 冲刷指定等级的日志
    // 日志等级包括 INFO、WARNING、ERROR、FATAL。
    // 高等级日志同时会写入低等级的日志，ERROR与FATAL会写入stderr，FATAL还会终止程序
    LOG(severity)                         << "Something goes wrong!";
    LOG_IF(severity, cond)                << "Something goes wrong!";
    LOG_EVERY_N(severity, times)          << "Something goes wrong!" << google::COUNTER; // 原理是每次宏展开就从零开始，再内部记录状态
    LOG_IF_EVERY_N(severity, cond, times) << "Something goes wrong!" << google::COUNTER;
    LOG_FIRST_N(severity, times)          << "Something goes wrong!" << google::COUNTER;
    // 用于调试的日志，未定义宏NDEBUG时有效
    DLOG(severity)                        << "Something goes wrong!";
    // 用户自定义日志，用数字表示等级，其等级均属于INFO
    VLOG(int)                             << "Something goes wrong!";
    // 额外输出errno状态
    PLOG(severity)                        << "Something goes wrong!";
    // 额外输出至系统日志
    SYSLOG(severity)                      << "Something goes wrong!";
    // 检查失败则相当于FATAL报错
    CHECK(cond)                           << "Something goes wrong!";
    CHECK_EQ(cond1, cond2)                << "Something goes wrong!";
    CHECK_LT(cond1, cond2)                << "Something goes wrong!";
    CHECK_LE(cond1, cond2)                << "Something goes wrong!";
    CHECK_GT(cond1, cond2)                << "Something goes wrong!";
    CHECK_GE(cond1, cond2)                << "Something goes wrong!";
    // 可检查C-string与std::string，C-string支持空指针（NULL!=non-NULL，NULL==NULL）
    CHECK_STREQ(str1, str2)               << "Something goes wrong!";
    CHECK_STRNE(str1, str2)               << "Something goes wrong!";
    CHECK_STRCASEEQ(str1, str2)           << "Something goes wrong!";
    CHECK_STRCASENE(str1, str2)           << "Something goes wrong!";
    // 双精度相等性检测，误差不能超过e(-15)（或pre）
    CHECK_DOUBLE_EQ(val1, val2);
    CHECK_NEAR(val1, val2, pre);
    // 检查指针是否为非空，返回ptr以继续正常代码
    ptr CHECK_NOTNULL(ptr)
}
```

- 设置 glog 行为 > 可通过更改运行程序的“环境变量”，前缀`GLOG_` > 或者通过在代码中更改”全局变量“，前缀`FLAGS_`
- `logtostderr=0`：是否输出到 stderr 代替输出到 tmpfile
- `stderrthreshold=2`：高于该等级的日志额外输出到 stderr。
  INFO、WARNING、ERROR、FATAL 分别为 0、1、2、3
- `minloglevel=0`：设置最低报告等级。数字同上
- `v=0`：设置用户自定义 VLOG 的日志记录等级，只记录低于或等于设置值的日志
- `log_dir=""`：设置日志目录。需要在初始化前设置 > 下述为完整宏名
- `GOOGLE_STRIP_LOG`：宏值表示删除低于该等级的日志字符串
<!-- entry end -->

### 测试库

<!-- entry begin: 单元测试 测试库 gtest -->

```cpp
#include <gtets/gtest.h>
TEST(TestSuiteName, TestName) { // 注册一个单元测试，名字不能含下划线'_'
    // 输出字符类型可不必为char，输出时自动转换为utf8
    EXPECT_*(exp)   << "Something goes wrong!";  // EXPECT系列，失败则继续执行
    ASSERT_*(exp)   << "Something goes wrong!";  // ASSERT系列，失败则直接退出函数（小心资源泄露），一般用于会导致后续测试无意义的失败
    SUCCEED()       // 直接返回成功
    FAIL()          // ASSERT失败
    ADD_FAILURE()   // EXPECT失败
    // 以下只列出EXPECT版本
    EXPECT_PRED1(pred1, arg1)               EXPECT_PRED2(pred2, arg1, arg2)     // 可打印参数值
    EXPECT_TRUE(cond)   EXPECT_FALSE(cond)  EXPECT_EQ(v1, v2)   EXPECT_NE(v1, v2)
    EXPECT_LT(v1, v2)   EXPECT_LE(v1, v2)   EXPECT_GT(v1, v2)   EXPECT_GE(v1, v2)
    EXPECT_FLOAT_EQ(f1, f2)                 EXPECT_DOUBLE_EQ(d1, d2)
    EXPECT_NEAR(v1, v2, pre)
    EXPECT_STREQ(s1, s2)                    EXPECT_STRNE(s1, s2)
    EXPECT_STRCASEEQ(s1, s2)                EXPECT_STRCASENE(s1, s2)
    EXPECT_THROW(exp, ExceptType)           EXPECT_ANY_THEOW(exp)
    EXPECT_NO_THROW(exp)
    // 死亡测试，TestSuitName必须以DeathTest为后缀来提前运行。
    // pred包括testing::ExitedWithCode(exit_code)与testing::KilledBySignal(sig_num)
    EXPECT_DEATH(exp, regex)                EXPECT_EXIT(exp, pred, regex)
}
class TestClass: public testing::Test {    // 一、public继承testing::Test
    protected:  // 二、所有成员为protected
    /* 三、定义用于多个测试复用成员对象 */
    /* 四、设计默认构造函数或`void SetUp() override`用于初始化 */
    /* 五、设计析构函数或`void TearDown() override`用于释放资源 */
};
TEST_F(TestClass, TestName) { // 注册前需定义Fixture类TestClass
    // 内部可直接使用FixtureTestClass中定义并初始化的成员对象
    // 每个测试中使用的对象会在测试前构造并调用SetUp()，然后在测试结束后调用TearDown并析构
}
int main(int argc, char* argv[]) {  // 或者直接链接libgtest_main.so而避免手动定义main函数
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

| gtest 命令行参数         | 说明                                                |
| ------------------------ | --------------------------------------------------- |
| --gtest_list_tests       | 列出所有测试而不运行                                |
| --gtest_filter           | 通配符过滤，一次指定一个。通配符包括`-`,`?`,`*`,`:` |
| --gtest_repeat           | 指定重复运行次数，-1 表示无限                       |
| --gtest_break_on_failure | 失败便停止                                          |
| --gtest_throw_on_failure | 失败时以异常形式抛出                                |

<!-- entry end -->
<!-- entry begin: 单元测试 测试库 gmock -->

```cpp
#include <gmock/gmock.h>
// 一、修改接口源码，将感兴趣的方法声明为纯虚函数（可继承且无需定义），
// 特别是析构函数必须是虚函数且必须提供定义）
struct MockClass: public Class { // 二、继承自需要模仿的类
    // 三、在public区域定义方法
    MOCK_METHOD(RetType, Method, (T1 arg1, T2 arg2), (const, override));
};
TEST(ClassTest, TestName) {
    MockClass mc{};
    // 模拟指定方法的指定重载版本。当传入的各个参数与各个位置上的matcher适配时，执行对应的action
    // matcher可以直接省略参数列表（包括括号），仅用于无重载的方法
    // 默认情况下，期望的匹配顺序是按注册的顺序从后往前（覆盖）的
    // 并且默认情况下，已经饱和（达到次数上限）了的期望仍会参与匹配，从而导致错误
    EXPECT_CALL(mc, Method(matcher1, matcher2))
        // 额外的匹配器，对各参数之间的关系再进行限定
        // 比如，.Witch(Args<0,1>(Lt()))表示期望参数0小于参数1
        .With(multi_argument_matcher)
        // 可忽略Times而由gmock推测：
        // 若无WillOnce且无WillRepeatedly则为1，
        // 若n>=1次WillOnce且无WillRepeatedly则为n，
        // 若n>=0次WillOnce且有WillRepeatedly则为AtLeast(n)
        .Times(cardinality)
        .WillOnce(action)
        .WillRepeatedly(action)
        // 指明该整条期望满足后便失效，以便实施多重期望时已饱和的期望又参与匹配。
        // [deprecated](https://stackoverflow.com/questions/52765902/gmock-insequence-vs-retiresonsaturation)
        .RetiresOnSaturation();
    {
        testing::InSequence seq{};
        // 在该块作用域内，顺序是强制从前往后依序测试，若matcher不匹配则直接报错
        // 期望满足后会自动退出
    }
    // 注意：mock对象析构时会检测条件是否满足，故对mock类方法的调用应在此函数中出现
}
namespace testing {
    DefaultValue<T>::Set(value);        // 设置T类方法的默认action，T必须可copy
    DefaultValue<T>::SetFactory(MakeT); // 设置T类方法的默认action，T必须可move
    DefaultValue<T>::Clear();           // 重置T类方法的默认action
    // 匹配器matcher包括：（m表示匹配器）
    // 通配符匹配
    _               A<Type>()           An<Type>()
    // 值匹配：下述value通常会被copy，若其不能copy则使用std::ref(value)
    value           Eq(value)           Ge(value)       Gt(value)
    Le(value)       Lt(value)           Ne(value)
    IsFalse()       IsTrue()            IsNull()        NotNull()
    Optional(m)     VariantWith<T>(m)   Ref(variable)   TypeEq<Type>(value)
    // 浮点数匹配：前两者视两NAN为不相等，随后两者视其作相等
    DoubleEq(d)                         FloatEq(f)      IsNan()
    NanSensitiveDoubleEq(d)             NanSensitiveFloatEq(f)
    DoubleNear(d, pre)                  FloatNear(f, pre)
    NanSensitiveDoubleNear(d, pre)      NanSensitiveFloatNear(f, pre)
    // 字符串匹配
    ContainsRegex(string)               MatchsRegex(string)
    StartsWith(prefix)                  EndsWith(suffix)    HasSubstr(string)
    StrEq(string)                       StrNe(string)
    StrCaseEq(string)                   StrCaseNe(string)
    // 容器匹配
    SizeIs(m)       IsEmpty()           Contains(m)     Each(m)
    ElementsAre(m0,...,mn)              UnorderedElementsAre(m0,...,mn)
    WhenSorted(m)                       WhenSortedBy(comp, m)
    ContainerEq(cont)
    // 成员匹配
    Key(m)          Pair(m1, m2)
    Field(&Type::Mem, m)                FieldsAre(m,...)
    // func(argument)返回值匹配m
    ResultOf(func, m)
    // 指针匹配
    Address(m)      Pointee(m)          Pointer(m)      WhenDynamicCastTo<T>(m)
    // 复合匹配
    Not(m)          AllOf(m0,...,mn)    AnyOf(m0,...,mn)
    // 多参匹配
    Eq()    Ne()    Ge()    Gt()        Le()    Lt()
    AllArgs(m)      Args<N1,...,Nn>(m)

    // 行为action包括：（a表示action）
    // 返回值
    Return()        Return(value)       ReturnArg<N>()  ReturnNew<T>(args...)
    Return(Null)                        ReturnPointee(ptr2Value)
    ReturnRef(variable)                 ReturnRefOfCopy(value)
    ReturnRoundRobin({a0,a1,...,an})
    // 副作用
    Assign(&variable, value)            DeleteArg<N>()
    SaveArg<N>(ptr)                     SaveArgPointee<N>(ptr)
    SetArgReferee<N>(value)             SetArgPointee<N>(value)
    SetArrayArgument<N>(beg, end)
    Throw(exception)                    SetErrnoAndReturn(error, value)
    // 调用
    func            Invoke(f)           Invoke(obj_ptr, mem_ptr)
    InvokeWithoutArgs(f)                InvokeWithoutArgs(obj_ptr, mem_ptr)
    InvokeArgument<N>(arg1,...,argn)
    // 复合
    DoAll(a1,...,an)                    IgnoreResult(a)
    WithArg<N>(a)                       WithArgs<N1,...,Nn>(a)
    WithoutArgs(a)

    // 计数cardinalities包括：
    AnyNumber()     AtLeast(n)          AtMost(n)       Between(m, n)
}
```

<!-- entry end -->

## 序列化库

<!-- entry begin: 序列化 json yas -->

### YAS

```cpp
// 手动定义类模板成员
template<typename Archive>
void serialize(Archive &ar) {
    auto o = YAS_OBJECT("type", m);
    ar & o;
}
// 自定义序列化类
YAS_DEFINE_STRUCT_SERIALIZE(oname, m)                       // 前两者定义在类内
YAS_DEFINE_STRUCT_SERIALIZE_NVP(oname, ("m", m))
YAS_DEFINE_INTRUSIVE_SERIALIZE(oname, type, m)              // 后两者定义在类外，type类型名不用字符串
YAS_DEFINE_INTRUSIVE_SERIALIZE_NVP(oname, type, ("m", m))

// 创建中间对象待读写，其作为实际对象的引用而不存储实体
// 当序列化为yas::json时，对象名可做键值
YAS_OBJECT(oname, v)                            // 创建v的中间对象（名为oname），v名即为"v"
YAS_OBJECT_NVP(oname, ("value", v))             // 创建v的中间对象（名为oname），v名指定为"value"
YAS_OBJECT_STRUCT(oname, sname, m)              // 创建sname.m的中间对象（名为oname），m名即为"m"
YAS_OBJECT_STRUCT_NVP(oname, sname, ("mem", m)) // 创建sname.m的中间对象（名为oname），m名指定为"mem"

// yas::Format包括yas::bin、yas::json、yas::text
yas::save<yas::mem  | yas::Format>(yas_object)          // 返回buffer{shared_ptr data; size_t size;};
yas::load<yas::mem  | yas::Format>(yas_buf,  yas_object)
yas::save<yas::file | yas::Format>(filename, yas_object)
yas::load<yas::file | yas::Format>(filename, yas_object)
```

<!-- entry end -->

### BOOST

<!-- entry begin: serialization boost 序列化 -->

```cpp
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <fstream>
#include <iostream>

class Test
{
    friend class boost::serialization::access; // Note!
public:
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

### JSON

为自己的类定义下列两个函数

- `void from_json(const json&, myClass&);`
- `void to_json(json&, const myClass&);`

---

**序列化：**

- 构造 nlohmann::json

  - 变量转换：
    > 转换时注意使用`{}`还是`()`
    - bool : bool
    - number: INT, FLOAT
    - string: string
    - list : `il<non-pair>`, Seq, Set
    - object: `il<pair>`, Map
  - 支持 STL 容器接口
    > 将容器元素类型想象为`std::any`
    - Seq 类接口创建 List
    - Map 类接口创建 Object

- 修改 nlohmann::json

  - .patch(jsonPatch)

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

  - .merge_patch(json)：合并或覆盖源 json

- 序列化为 JSON
  - 输出流：`ostream << setw(INDENT) << json;`
  - 字符解析：`.dump()与.dump(INDENT)`
    > 前者返回只一行字符串，后者可指定缩进且多行排版

---

**反序列化：**

- 构造 nlohmann::json

  - 输入流：`istream >> json;`
  - 字符解析：`json::parse(strWithJson); json::parse(beg, end);`
  - 字面值：`R"({"json": "yes"})"_json`

- 反序列为 cpp 对象
  > 取决于当前 json 对象所存储的实际数据类型，类型转换失败会抛出异常（**就像 std::any**）  
  > 弱类型系统与强类型系统的交互原理可参见[mysqlpp](#mysqlpplx)
  - `.get<cppType>(); .get_to(cppObj);`：支持的类型转换以及 STL 接口见上
