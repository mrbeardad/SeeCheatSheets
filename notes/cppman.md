***注意，该文档主要用于通过本仓库的see命令进行终端查询，故未太注重渲染后的排版与美观***
# 目录
<!-- vim-markdown-toc GFM -->

- [C++标准库](#c标准库)
  - [规范解释](#规范解释)
  - [标准库异常](#标准库异常)
  - [C库](#c库)
  - [通用工具](#通用工具)
  - [STL](#stl)
  - [其它容器](#其它容器)
  - [流与格式化](#流与格式化)
  - [并发](#并发)
- [BOOST](#boost)
  - [序列化](#序列化)
- [Mysql++](#mysql)

<!-- vim-markdown-toc -->

# C++标准库
## 规范解释
* C++程序库规范解释
    * 类聚合式构造    ：即是对每个数据成员进行copy/move构造
    * 逐块式构造      ：利用tuple传递每个数据成员的构造函数的实参
    * 成员模板构造    ：比如pair<int, char*>可以赋值给pair<int, string>，
        尽管类型不一样，但提供了模板构造函数用于接收不同实例。

    * 所有需要使用`foo<T>::type`与`bar<T>::value`，都提供了模板类型别名`foo_t<T>`与变量模板`bar_v<T>`代替
    * **访问**表示可以读取也可以写入
    * 一些参数内容很明显的函数不再指出形参列表，如`.operator()`
    * 函数参数列表如`(x, y = 0)`，不一定表示y有默认实参，
        也可能表示有两个重载函数，第一个为`(x)`，第二个为`(x, y)`，只不过若第二个中y=0则与第一个函数作用一样（实现细节不一样）

## 标准库异常
<!-- entry begin: c++ cpp 标准库异常 -->
* 异常体系结构
```cpp
exception                 `<exception>`  
├─── bad_cast             `<typeinfo>`      ：多态引用的转换失败  
├─── bad_typeid           `<typeinfo>`      ：在typeid()中解引用多态类型的空指针
├─── bad_weak_ptr         `<memory>`        ：构造weak_ptr失败  
├─── bad_function_call    `<functional>`    ：调用无目标的function类  
├─── bad_alloc            `<new>`           ：内存申请失败  
├─── bad_array_new_length `<new>`           ：传给new的size不在有效范围  
│  
├─── logic_error          `<stdexcept>`  
│   │  
│   ├─── domain_error                       ：数学库, 传入值域错误  
│   ├─── invalid_argument                   ：bitset构造参数无效  
│   ├─── length_error                       ：容器size超出限制  
│   ├─── out_of_range                       ：容器的无效索引  
│   └─── future_error     `<future>`        ：异步系统调用  
│  
└─── runtime_error        `<stdexcept>`  
    │  
    ├─── range_error                        ：wide string与byte string转换出错  
    ├─── overflow_error                     ：bitset转换为整型时溢出  
    ├─── underflow_error                    ：算术下溢  
    └─── system_error     `<system_error>`  ：系统调用出错  
        │  
        └─── ios::failure `<ios>`           ：stream出错  
```
<!-- entry end -->

<!-- entry begin: 异常成员 -->
* 异常成员
    * .what() ：返回`const char*`用于打印
        > 根部基类**exception**的虚函数
        > 异常类销毁后该C-string也不复存在  
    * .code() ：返回error_code类对象
        > error_code 与 error_condition 区别：**可移植性**  
        > 前者由编译器定义(OS相关), 后者为默认标准
        * error_code成员：
            * .message()
            * .category().name()
            * .value()
            * .default_error_condition().message()
            * .default_error_condition().category().name()
            * .default_error_condition().value()
        * 比较：
            > 重载了与**领域枚举值**的比较运算符
            * errc::mem        ：`<cerrno>`
            * io_errc::mem     ：`<ios>`
            * future_errc::mem ：`<future>`
<!-- entry end -->

<!-- entry begin: 异常构造 -->
* 异常构造参数
    * logic_error与runtime_error
        * (const string&)
        * (const char*)
    * system_error
        > 提供`make_error_code(errc)`构造error_code
        * (error_code)
        * (error_code, const string&)
        * (error_code, const char*)
<!-- entry end -->

<!-- entry begin: 异常挂起 -->
* 异常挂起
    * current_exception()       ：返回exception_ptr对象
    * rethrow_exception(exceptr)：重新抛出exception_ptr对象
<!-- entry end -->

## C库
<!-- entry begin: 调试 cassert -->
* 调试：`<cassert>`
    * assert(expr)                  ：运行时断言, false则执行
        > #define NDEGUG  
        > 可以取消**宏函数**assert()
    * static_assert(constexpr, message)  ：编译期断言, 可以自定义打印消息
        > #define NDEBUG  
        > 并不会取消**关键字static_assert()**
    * 编译器预处理宏：
        * `__func__`
        * `__LINE__`
        * `__FILE__`
        * `__TIME__`
        * `__DATE__`
<!-- entry end -->

<!-- entry begin: cstdlib -->
* `<cstdlib>`
    * EXIT_SUCCESS
    * EXIT_FAILURE
    * exit(status)
    * atexit(void (*func)())
    * quick_exit(status)
    * at_quick_exit(void (*func)())
<!-- entry end -->

<!-- entry begin: cctype -->
* `<cctype>`
    * `isalnum(c)`     ：字母+数字
    * `isalpha(c)`     ：字母
    * `islower(c)`     ：小写字母
    * `isupper(c)`     ：大写字母
    * `isdigit(c)`     ：数字
    * `isxdigit(c)`    ：数字+`xabcdefXABCDEF`
    * `ispunct(c)`     ：符号
    * `isblank(c)`     ：`空格`+`\t`
    * `isspace(c)`     ：`空格`+`\t`+`\n`+`\r`+`\v`+`\f`
    * `iscntrl(c)`     ：0x00-0x1F + 0x7F
    * `isgraph(c)`     ：字母+数字+符号
    * `isprint(c)`     ：字母+数字+符号+空格
    * `toupper(c)`
    * `tolower(c)`
<!-- entry end -->

<!-- entry begin: cmath 数学 -->
* 数学库：`<cmath>`
    > 几乎所有函数的参数都对`float` `double` `long double` 和整数 有重载，故一般省略形参类型  
    > 若只能为浮点数（float或double）会指明`double`

* 三角函数
    * cos(T)
    * sin(T)
    * tan(T)
    * acos(T)
    * asin(T)
    * atan(T)

* 对数与幂
    * log(N)                    ：log_e(N)
    * log1p(N)                  ：log_e(N + 1)（计算更精准）
    * log2(N)                   ：log_2(N)
    * log10(N)                  ：log_10(N)
<!--  -->
    * exp(x)                    ：e ^ x
    * expm1(x)                  ：exp(x) - 1（计算更精准）
    * exp2(x)                   ：2 ^ x
    * exp10(x)                  ：10 ^ x
<!--  -->
    * pow(x, y)                 ：x ^ y
    * sqrt(double x)            ：x的平方根
    * cbrt(double x)            ：x的立方根
    * hypot(x, y, z = 0)        ：`sqrt(x * x, y * y, z * z)`

* 浮点数取整
    * ceil(double x)            ：向上取整
    * floor(double d)           ：向下取整
    * trunc(double x)           ：丢弃小数
    * round(double x)           ：四舍五入

* 其它
    * frexp(double x, int* exp) ：x = n * (2 ^ exp)，返回n值域为[0.5, 1.0)，并返回exp到指针所指的int
    * ldexp(double x, int exp)  ：n = x * (2 ^ exp)，返回n，x的值域为[0.5, 1.0)
    * fmod(double x, double y)  ：浮点数的x % y
    * modf(double x, int* n)    ：将x分解为整数与小数部分，整数存于n，返回小数
<!--  -->
    * abs(x)                    ：x的绝对值
    * fdim(x, y)                ：如果x > y则返回x - y，否则返回0
    * fma(x, y, z)              ：返回x * y + z
    * div(x, y)                 ：x除以y的商(div_t.quot)和余数(div_t.rem)   `<cstdlib>`
<!--  -->
    * gcd(x, y)                 ：`<numeric>`
    * lcm(x, y)                 ：`<numeric>`
<!-- entry end -->

<!-- entry begin: getopt -->
* `int getopt(int argc, char* const argv[], const char* optstring)`  `<unistd.h>`
    > 命令行参数，即`argv`字符串数组中的各个字符串的集合，每个字符串为一个命令行参数。有如下几种情况：
    > * 执行命令      ：即`argv[0]`
    > * 选项          ：即以`-`开头的命令行参数。选项分为三种类型，`-o`单选项、`-opt`多选项（`o`与`p`选项必须为无参选项）、`-tfile`选项`t`及其参数`file`
    > * 选项参数      ：若某选项必有或可能有参数，则跟在该选项后面的同一命令行参数的字符，或下个命令行参数即为该选项的参数，见上
    > * 命令参数      ：不属于上面三种情况的命令行参数，作为该命令本身的主要参数。**getopt会将所有命令参数保持顺序的移动到`argv`数组的尾部**
    * 参数argc与argv：
        * 来自`int main(int argc, char* argv[])`
    * 参数optstring：
        * `:o`      ：开头`:`表示开启silent-mode，默认为print-mode
        * `o`       ：代表选项`o`没有参数  
        * `o:`      ：代表选项`o`必有参数, 紧跟`-oarg`或间隔`-o arg`中的`arg`都被视为`-o`的参数  
        * `o::`     ：代表选项`o`可选参数, 只识别紧跟`-oarg`
    * 全局变量：
        * optarg    ：类型为`char*`，指向当前选项的参数，无则为NULL
        * optind    ：类型为`size_t`，作为下次调用getopt()将要处理的argv数组中元素的索引
    * print-mode
        > getopts()函数自动打印错误消息
        * 返回int表示当前选项字符
            * `?`表示无效选项。无效选项即选项字符不在于`optstring`中或本该需要参数的选项却没有参数
            * `-1`表示解析结束，剩余的都是命令参数
    * silent-mode
        > 不自动打印错误消息
            * `?`表示未知选项，该选项未在`optstring`中指定
            * `:`表示错误选项，该选项必有参数却为提供参数（即该选项作为最后一个命令行参数）
            * `-1`表示解析结束，剩余的都是命令参数
<!-- entry end -->

<!-- entry begin: getopt_long getopt_long_only -->
* `int getopt_long(argc, argv, optstring, const struct option longopts[], int* longindex)`   `<getopt.h>`
    > 基本规则同`getopt()`，增加了对长选项的解析：
    > “长选项的紧跟”为`--option=arg`, 而且长选项若无歧义可不用完整输入
    * 参数longopts：
        > struct option的数组, 最后一个option必须全0以作为数组结束标志
        ```c
        struct option {
            const char* name;    // 选项名称
            int has_arg;         // no_argrument|required_argrument|optional_argrument
            int* flag;           // 等于NULL则函数返回val，否则匹配时*flag=val且函数返回0
            int val;             // 指定匹配到该选项时返回的int值
        };
        ```
    * 参数longindex：
        > 若不等于NULL，存储当前处理的长选项在`longopts`中的索引
* `getopt_long_only(argc, argv, optstring, option*, int*)`
    > 注：规则同上, 但是`-opt`会优先解析为长选项, 不符合再为短
<!-- entry end -->

<!-- entry begin: simd immintrin.h -->
* SIMD：`<immintrin.h>`
    > C++17中SIMD指令可以通过给STL算法执行策略而应用到程序中
    * 需要利用`alignas(32)`对齐数组
    * 向量寄存器抽象类型：
        * `__m256`
        * `__m256d`
        * `__m256i`
    * 加载到向量寄存器：
        ```c
        _mm256_load_ps( float* )
        _mm256_load_pd( double* )
        _mm256_load_epi256( __m256i* )
        ```
    * SIMD运算：
        ```c
        _mm256_OP_ps( __m256, __m256 )
        _mm256_OP_pd( __m256d, __m256d )
        _mm256_OP_epi32( __m256i, __m256i )
        _mm256_OP_epi64( __m256i, __m256i )
        ```
    * 存储回内存：
        ```c
        _mm256_store_ps( float* , __m256)
        _mm256_store_pd( double* , __m256d )
        _mm256_store_epi256( int* , __m256i )
        ```
<!-- entry end -->

## 通用工具
<!-- entry begin: initializer_list -->
* initializer_list：`<initializer_list>`
    > 语言支持库，支持聚合初始化
    > * 使用列表初始化时，会优先调用参数为initializer_list的构造函数
    > * 在range-based-for中，可以直接在冒号右边用列表初始化构造initializer_list作为容器
<!-- entry end -->

<!-- entry begin: integer_sequence utility -->
* integer_sequence：`<utility>`
    > 与initializer_list的区别在于，integer_sequence可以用于编译期计算
    * 构造
        * `integer_sequence<typename T, T... INTS>{}`
        * `index_sequence<size_t... SIZETS>{}`
        > 以下构造1 ~ N-1的T类型的整数序列
        * `make_integer_sequence<typename T, T N>{}`
        * `make_index_sequence<size_t N>{}`
    * 读取
        * ::size()                                  ：获取整数个数
        * function(integer_sequece<T, INTS...> t)   ：利用模板参数解包与折叠表达式处理INTS
<!-- entry end -->

<!-- entry begin: pair utility -->
* pair：`<utility>`
    * 构造
        * 类聚合式构造    ：支持移动语义
        * 逐块式构造      ：参数(std::piecewise_constructor, make_tuple(args1), make_tuple(args2))
        * 成员模板构造
    * 访问
        * .first
        * .second
    * 比较：
        > 字典比较
<!-- entry end -->

<!-- entry begin: tuple -->
* tuple：`<tuple>`
    * 构造
        * 类聚合式构造    ：支持移动语义
        * 成员模板构造
        * 支持由pair赋值
    * 访问
        * `get<T>(t)` 与 `get<N>(t)`
            > 返回引用
    * 读取
        * `tuple_size<TT>::value`
        * `tuple_element<N, TT>::type`
        * `tuple_cat(tuple1, tuple2, ...)`
    * 比较：
        > 字典比较
<!-- entry end -->

<!-- entry begin: any -->
* any：`<any>`
    * 构造：
        * 默认构造      ：构造为nullptr
        * 类聚合式构造  ：支持移动语义
        * 就地构造      ：参数`(std::in_place_type<Type>, args...)`
    * 访问：
        * `any_cast<T&>(any)`
    * 读取：
        * `.has_value()`
        * `.type().name()`
            > 利用关键字type_id()比较
    * 修改：
        * `.operator=()`
        * `.emplace<T>()`
        * `.reset()`
<!-- entry end -->

<!-- entry begin: variant -->
* variant：`<variant>`
    * 构造：
        * 默认构造      ：默认构造第一个类型
            > std::monostate类作占位符避免无默认构造函数
        * 类聚合式构造  ：支持移动语义
            > 匹配最佳的类型，但注意char*匹配数值类型比匹配string更佳
        * 就地构造：
            * 参数`(std::in_place_type<Type>, args...)`
            * 参数`(std::in_place_index<Type>, args...)`
    * 读取：
        * .index()
    * 修改：
        * .operator=()
        * `.emplace<T>()`
        * `.emplace<N>()`
    * 访问：
        * `get<T>(vrt)`
        * `get<N>(vrt)`
        * `get_if<T>(vrt*)`
        * `get_if<N>(vrt*)`
        > get<>错误匹配类型会抛出异常，get_if<>错误匹配类型返回空指针
        * visit(func, vrt)
            > func为能接受所有vrt模板参数类型的重载可调用类型
    * 比较：
        > 字典序
<!-- entry end -->

<!-- entry begin: optional -->
* optional：`<optional>`
    * 构造：
        * 默认构造      ：构造为std::nullopt
        * 类聚合式构造  ：支持移动构造
        * 就地构造      ：参数(std::in_place, args...)
    * 访问
        * .operator*()
        * .operator->()
    * 读取
        * .value()            ：nullopt则抛出异常
        * .value_or(type_val) ：nullopt则返回type_val
        * .operator bool()
<!-- entry end -->

<!-- entry begin: shaerd_ptr memory -->
* shared_ptr：`<memory>`
    * 构造：
        * 拷贝/移动构造         ：更新引用计数
        * `shared_ptr<T>(unique_ptr)`
        * `shared_ptr<T>(new_ptr, deleter)`
        * `make_shared<T>()`
            > 对象内存与引用计数器一次分配  
            > 同时避免new表达式与用shared_ptr管理new获取的指针这两步之间发生异常，而导致内存泄漏（make_unique同）
    * 访问：
        * .operator*()
        * .operator->()
        * .get()
    * 读取：
        * .use_count()
    * 修改：
        * .reset()
        * .reset(ptr)
        * .reset(ptr, del)
    * 类型转换：
        * static_pointer_cast<>(sp)
        * dynamic_pointer_cast<>(sp)
        * const_pointer_cast<>(sp)
        * .operator bool()
    * 比较：
        > 比较存储的指针
    * 支持成员函数返回this的唯一的shaerd_ptr
        * 继承`std::enable_shared_from_this<T>`
        * 返回`shared_from_this()`
    * 错误问题：
        * 循环依赖
        * 多组指向
<!-- entry end -->

<!-- entry begin: weak_ptr memory -->
* weak_ptr：`<memory>`
    * 构造：
        * `weak_ptr<T>(shared_ptr)`
    * 访问
        * .lock()
            > 返回shared_ptr
    * 读取
        * .expired()
            > 返回是否为空
<!-- entry end -->

<!-- entry begin: unique_ptr memory -->
* unique_ptr：`<memory>`
    * 构造：
        * unique_ptr<T, Del>(new_ptr, del)
            > del默认为delete表达式
        * make_unique<>()
        * 支持move, 拒绝copy
    * 访问：
        * .operator*()
        * .operator->()
        * .get()
    * 修改：
        * .reset()
        * .reset(ptr)
<!-- entry end -->

<!-- entry begin: numeric_limits limits -->
* numeric_limits：`<limits>`
    * ::lowest()        ：负数最小值
    * ::min()           ：正数最小值
    * ::max()           ：最大值
    * ::digits          ：二进制数字位数
    * ::digits10        ：能保证准确表示的十进制数字位数
    * ::max_digits10    ：能表示的最大的十进制数字位数
    * ::max_exponent10  ：浮点数最大正指数
    * ::min_exponent10  ：浮点数最小负指数
    * ::infinity()      ：正无穷
    * ::quiet_NaN()     ：NAN
<!-- entry end -->

<!-- entry begin: type_traits -->
* `<type_traits>`
    > 元编程可以利用该库进行模板类型限制
    * 类型判断式
    * 类型关系检验
    * 类型修饰符
    * 类型计算
    * 使用：
        * ::value   ：返回std::true_type或std::false_type
        * ::type    ：返回修饰后的类型
<!-- entry end -->

<!-- entry begin: reference_wrapper functional -->
* reference_wrapper：`<functional>`
    * 构造：
        * ref(obj)
        * cref(obj)
    * 转换：提供到目标引用的转换`from & to`
    * 访问
        * .get()    ：返回目标引用，如此才能调用其成员函数
<!-- entry end -->

<!-- entry begin: hash functional -->
* hash：`<functional>`
    > 预定义：整型、浮点型、指针、智能指针、string、bitset<>、vector<bool>
<!-- entry end -->

<!-- entry begin: ratio -->
* ratio：`<ratio>`
    * 构造：预定义ratio类型
    * 读取：
        > 模板非类型参数作分子与分母
        * ::num    ：分子
        * ::den    ：分母
    * 运算：
        > 编译期运算、比较、化简、报错
        * 算术运算：ratio_OP<ratio1, ratio2>::type
        * 关系运算：ratio_OP<ratio1, ratio2>::value
<!-- entry end -->

<!-- entry begin: chrono ctime get_time put_time -->
* 时间库：`<chrono>`
    * duration
        * 构造：
            * time_point相减
            * 字面值构造
        * 读取：
            * .count()
            * ::rep     ：整数的类型
            * ::period  ：分数的类型
        * 算术运算：会隐式转换为更高精度
        * 类型转换：转为粗精度直接截断数值
            * duration_cast<>()
    * Clock
        * 构造：
            * 预定义system_clock、steady_clock等
        * 读取：
            * ::now()
            * ::duration
            * ::time_point
            * system_clock还提供
                * ::from_time_t()
                * ::to_time_t()
    * time_point
        > 由Clock提供Epoch，
        > duration可相对为负值
        * 构造：
            * 默认构造为epoch
            * Clock::now()获取
            * time_point与duration运算
        * 算术运算、关系运算、类型转换

* `<ctime>`：
    * time(time_t*)                         ：获取当前时间并存储到time_t*指向的位置
    * localtime(time_t*); gmtime(time_t*)   ：传入上述的time_t*，返回`tm*`

* `<iomanip>`：
    > `fmt`格式见linux中date命令的格式
    * get_time(tm*, fmt)
    * put_time(tm*, fmt)
<!-- entry end -->

## STL
> * STL组件
>     * 容器：序列、关联、无序
>         * 异常发生：容器reallocate, 元素的copy与move等
>         * 异常处理：容器保证reallocate异常安全；对于元素增删时产生的异常, 随机访问容器无法恢复, 节点式容器保证安全
>     * 迭代器：输出、输入、单向、双向、随机
>     * 泛型算法：搜索比较、更替复制、涂写删除
> * 解释：
>     * a : array
>     * s : string
>     * v : vector
>     * d : deque
>     * l : list
>     * fl: forward-list
>     * A : Assoicated
>     * U : Unordered
>     * M : all-kinds-of-Map
> * STL算法
>     * 为了简化，将重载函数的形参当作同一函数的默认实参来处理
>     * op1表示接收一个实参的操作数，op2接收两个
>     * b表示begin，e表示end
>     * partB表示是和b是同一个容器的且非end的迭代器
>     * destB表示是作为算法的输出区间
<!-- entry end -->

<!-- entry begin: 容器构造 -->
* 容器构造：
    * 默认               ：all-a
    * (initializer-list) ：all-a
    * (beg, end)         ：all-a
    * 聚合初始化         ：a
    * 拷贝               ：all
    * 移动               ：all
    * (num)              ：v, d, l, fl
    * (num, value)       ：s, v, d, l, fl
    > array为聚合类  
    > 前三条对于A与U都可加额外参数(..., cmpPred)与(..., bnum, hasher, eqPred)
<!-- entry end -->

<!-- entry begin: 容器赋值 -->
* 容器赋值：
    * .operator=()                ：all
    * .fill(v)                    ：a
    * .assign(initializer-list)   ：all-a-A-U
    * .assign(beg, end)           ：all-a-A-U
    * .assign(num, value)         ：all-a-A-U
<!-- entry end -->

<!-- entry begin: 容器访问 -->
* 容器访问：
    * .at(idx)                                ：a, s, v, d
    * .at(key)                                ：Map
    * .operator[](idx)                        ：a, s, v, d
    * .operator[](key)                        ：Map(自动创建key)
    * .front()                                ：a, v, d, l, fl
    * .back()                                 ：a, v, d, l
    * .data()                                 ：a, s, v
    * .begin(), .cbegin(), .end(), cend()     ：all
    * .rbegin(), .crbegin(), .rend(), crend() ：all-U-fl
<!-- entry end -->

<!-- entry begin: 元素插入 -->
* 元素插入：
    * .insert(pos, value)            ：all
    * .insert(pos, num, val)         ：s, v, d, l
    * .insert(pos, initializer-list) ：s, v, d, l
    * .insert(pos, beg, end)         ：s, v, d, l
    * .insert(value)                 ：A, U(非multi返回pair<iter, bool>)
    * .insert(initializer-list)      ：A, U
    * .insert(beg, end)              ：A, U
    * .emplace(pos, args...)         ：v, d, l
    * .emplace(args...)              ：A, U(非multi返回pair<iter, bool>)
    * .emplace_hint(pos, args...)    ：A, U
    * .emplace_back(v)               ：v, d, l
    * .emplace_front(v)              ：d, l, fl
    * .push_back(v)                  ：s, v, d, l
    * .push_front(v)                 ：d, l, fl
<!-- entry end -->

<!-- entry begin: 元素删除 -->
* 元素删除：
    * .erase(v)          ：A, U(返回删除个数)
    * .erase(pos)        ：all-fl
    * .erase(beg, end)   ：all-fl
    * .pop_back()        ：s, v, d, l
    * .pop_front()       ：d, l, fl
    * .clear()           ：all
<!-- entry end -->

<!-- entry begin: 容器大小 -->
* 容器大小：
    * .empty()                   ：all
    * .size()                    ：all-fl
    * .max_size()                ：all
    * .resize(num)               ：s, v, d, l, fl
    * .resize(num, v)            ：s, v, d, l, fl
    * .capacity()                ：s, v
    * .reserve(num)              ：s, v, U(v不能缩小)
    * .shrink_to_fit()           ：s, v, d
<!-- entry end -->

<!-- entry begin: 容器比较 -->
* 容器比较：
    * 相等比较：U
    * 非相等比较：all-U
<!-- entry end -->

<!-- entry begin: U A 无序 关联 A特有 -->
* U与A特有：
    * .count(v)         ：A, U
    * .find(v)          ：A, U
    * .lower_bound(v)   ：A
    * .upper_bound(v)   ：A
    * .equal_range(v)   ：A
    * .merge(C )        ：A, U
    * .extract(iter)    ：A, U
    * .extract(key)     ：A, U
        > .extract()的作用是利用move语义获取元素handle
<!-- entry end -->

<!-- entry begin: U 无序 U特有 -->
* U特有：bucket接口
    * .bucket_count()
    * .max_bucket_count()
    * .load_factor()
    * .max_load_factor()
    * .max_load_factor(float)
    * .rehash(bnum)
    * .bucket(val)
    * .bucket_size(bucktidx)
    * .begin(bidx)
    * .cbegin(bidx)
    * .end(bidx)
    * .cend(bidx)
<!-- entry end -->

<!-- entry begin: l特有 list -->
* l与fl特有
    * .remove(v)
    * .remove_if(op)
    * .sort()
    * .sort(op)
    * .unique()
    * .unique(op)
    * .splice(pos, sourceList, sourcePos)
    * .splice(pos, sourceList, sourceBeg, sourceEnd)
    * .merge(source)
    * .merge(source, cmpPred)
    * .reverse()
<!-- entry end -->

<!-- entry begin: 迭代器辅助函数 iterator -->
* 迭代器辅助函数： `<iterator>`
    * next(iter, n=1)
    * prev(iter, n=1)
    * distance(iter1, iter2)
    * iter_swap(iter1, iter2)
<!-- entry end -->

<!-- entry begin: 反向迭代器 -->
* 反向迭代器
    * 获取：容器的成员函数
        * .rbegin()
        * .rend()
        * .crbegin()
        * .crend()
    * .base()：反向迭代器的成员，转换为正常迭代器(+1)
<!-- entry end -->

<!-- entry begin: 流迭代器 -->
* 流迭代器
    * 获取：类模板构造
        * `istream_iterator<T>(istream)`              ：默认构造为end
        * `ostream_iterator<T>(ostream, delim="")`    ：`delim`为C-Style-String
    > 注： 只是通过I/O操作符实现, 而非底层I/O, 迭代器保存上次读取的值
<!-- entry end -->

<!-- entry begin: 缓冲区迭代器 -->
* 流缓冲区迭代器
    * 获取：类模板构造
        * `istreambuf_iteratot<char>`
            * ()    ：默认构造为end
            * (istrm)
            * (ibuf_ptr)
        * `ostreambuf_iteratot<char>`
            * (ostrm)
            * (obuf_ptr)
<!-- entry end -->

<!-- entry begin: 移动迭代器 -->
* 移动迭代器
    * 获取：泛型函数获取
        * make_move_iterator(iter)
    * 作算法源区间, 需要保证元素只能处理一次
<!-- entry end -->

<!-- entry begin: 插入迭代器 -->
* 插入迭代器
    * 获取：通过泛型函数
        * back_inserter(C)
        * front_inserter(C)
        * inserter(C, pos)
<!-- entry end -->

> * 泛型算法：`<algorithm>, <numeric>, <execution>`
> * 默认by value传递pred, 算法并不保证在类内保存状态的pred能正确运作(重新构造pred可能导致重置状态)
> * 获取谓词状态：
>     * 谓词指向外部状态
>     * 显式指定模板实参为reference
>     * 利用for_each()算法的返回值
> * 执行策略：做第一个参数
>     * std::seq        ：顺序执行（默认）
>     * std::par        ：多线程并行
>     * std::unseq      ：使用SIMD
>     * std::par_unseq  ：并行或SIMD
> * 规范：
>     * b, e代表源区间的begin与end
>     * op1, op2代表单参函数与双参函数
<!-- entry end -->

<!-- entry begin: 非更易算法 -->
* 非更易算法
    * for_each(b, e, op1)                   ：返回op1(已改动过的)拷贝
    * for_each_n(b, n, op1)
    * count(b, e, v)
    * count_if(b, e, op1)
<!-- entry end -->

<!-- entry begin: 最值比较算法 非更易 -->
* 最值比较
    * max(x, y)
    * max(initializer_list)
    * min(x, y)
    * min(initializer_list)
    * minmax(x, y)                          ：返回pair<min, max>
    * minmax(initializer_list)              ：返回pair<min, max>
    * clamp(x, min, max)                    ：返回三者中的第二大者
    * min_element(b, e, op2=lower_to)       ：返回第一个最小值
    * max_element(b, e, op2=lower_to)       ：返回第一个最大值
    * minmax_element(b, e, op2=lower_to)    ：返回第一个最小值和最后一个最大值
<!-- entry end -->

<!-- entry begin: 搜索算法 非更易 -->
* 搜索算法：返回搜索结果的第一个位置
    > 搜索单个元素
    * find(b, e, v)
    * find_if(b, e, op1)
    * find_if_not(b, e, op1)
    > 以下四个为二分搜索，需要先排序
    * binary_search(b, e, v, op2=lower_to)
    * lower_bound(b, e, v, op2=lower_to)
    * upper_bound(b, e, v, op2=lower_to)
    * equal_range(b, e, v, op2=lower_to)

    > 搜索子区间
    * search(b, e, searchB, searchE, op2=equal_to)
    * search_n(b, e, n, v, op2=equal_to)                    ：op2(elem, v)
    * find_end(b, e, searchB, searchE, op2=equal_to)
    * adjacent_find(b, e, op2=equal_to)                     ：搜索一对连续相等的元素, 返回第一个位置

    > 搜索目标范围中的元素
    * find_first_of(b, e, searchB, searchE, op2=equal_to)   ：搜索
<!-- entry end -->

<!-- entry begin: 更易算法 -->
* 更易算法：
    > 存在destB的算法返回dest区间的尾后迭代器
    * move(b, e, destB)                         ：支持子区间左移
    * move_backward(b, e, destE)                ：支持子区间右移
    * copy(b, e, destB)                         ：支持子区间左移
    * copy_backward(b, e, destE)                ：支持子区间右移
    * copy_if(b, e, destB, op1)
    * copy_n(b, n, destB)

    > 删除算法只是将需要删除的元素移到容器后面，若无destB则返回删除后新区间的尾后迭代器
    * remove(b, e, v)
    * remove_if(b, e, op1)
    * remove_copy(b, e, destB, v)
    * remove_copy_if(b, e, destB, op1)
    > unique算法删除相邻重复元素，应该先排序
    * unique(b, e, op2=equal_to)
    * unique_copy(b, e, destB, op2=equal_to)

    * replace(b, e, oldV, newV)                 ：返回void
    * replace_if(b, e, op1, newV)               ：返回void
    * replace_copy(b, e, destB, oldV, newV)
    * replace_copy_if(b, e, destB, op1, newV)
    * transform(b, e, destB, op1)               ：用`[b, e)`区间的元素调用op1()，并将返回结果写入destB
    * transform(b1, e1, b2, destB, op2)         ：用`[b1, e1)`与`[b2, e2)`的元素调用将op2()，并将返回结果写入destB
    * fill(b, e, v)                             ：返回void
    * fill_n(b, n, v)                           ：返回void
    * generate(b, e, op0)                       ：返回void
    * generate_n(b, n, op0)                     ：返回void
    * swap(x, y)                                ：返回void
    * swap_ranges(b, e, destB)
<!-- entry end -->

<!-- entry begin: 变序算法 -->
* 变序算法
    > 存在destB的算法返回dest区间的尾后迭代器，
    > 存在partB或partE的算法返回part
    * sort(b, e, op2=lower_to)                      ：返回void
    * stable_sort(b, e, op2=lower_to)               ：返回void
    * partition_sort(b, partE, e, op=lower_to)      ：返回void
    * partition_sort_copy(b, partE, e, op2=lower_to)
    * nth_element(b, nth, e, op2=lower_to)          ：返回void
    * make_heap(b, e, op2=lower_to)                 ：返回void
    * push_heap(b, e, op2=lower_to)                 ：返回void
    * pop_heap(b, e, op2=lower_to)                  ：返回void
    * sort_heap(b, e, op2=lower_to)                 ：返回void
    * next_permutation(b, e, op=lower_to)           ：当元素为完全升序时返回false
    * prev_permutation(b, e, op=lower_to)           ：当元素为完全降序时返回false
    * reverse(b, e)                                 ：返回void
    * reverse_copy(b, e, destB)
    * rotate(b, partB, e)                           ：返回原本的begin现在的位置
    * rotate_copy(b, partB, e, destB)
    * shuffle(b, e, randomEngine)                   ：返回void
    * sample(b, e, destB, cnt, randomEngine)        ：随机取cnt个值到destB
    * shift_left(b, e, cnt)                         ：返回左移后区间的尾后迭代器
    * shift_right(b, e, cnt)                        ：返回右移后区间的尾后迭代器
    * partition(b, e, op1)                          ：返回划分的前半部分的尾后迭代器
    * stable_partition(b, e, op1)                   ：返回划分的前半部分的尾后迭代器
    * partition_copy(b, e, destTrueB, destFalseB, op1)
<!-- entry end -->

<!-- entry begin: 区间检验算法 -->
* 区间检验与比较：一般返回boolean
    * equal(b, e, cmpB, op2 = equal_to)
    * mismatch(b, e, cmpB, op2 = equal_to)                  ：查找第一个不相同的元素, 返回pair存储两个区间的不同点的迭代器
    * lexicographical_compare(b1, e1, b2, e2, op = lower_to)：比较两区间字典序
    * is_sorted(b, e, op2 = lower_to)
    * is_sorted_until(b, e, op2 = lower_to)                 ：返回已排序区间的尾后迭代器
    * is_heap(b, e, op2 = lower_to)
    * is_heap_until(b, e, op2 = lower_to)                   ：返回已堆排序区间的尾后迭代器
    * is_partitioned(b, e, op1)
    * partition_point(b, e, op1)                            ：返回满足op1()为true的区间的尾后迭代器
    * includes(b1, e1, b2, e2, op2 = equal_to)              ：区间`[b2, e2)`是否为区间`[b1, e1)`的**子序列**
    * is_permutation(b1, e1, b2, op2 = equal_to)            ：检测两个区间的所有元素是否为同一个集合，即不考虑顺序
    * all_of(b, e, op1)
    * any_of(b, e, op1)
    * none_of(b, e, op1)
<!-- entry end -->

<!-- entry begin: 集合算法 -->
* 集合算法
    > 需要先排序
    * merge(b1, e1, b2, e2, destB, op2=lower_to)
    * inplace_merge(b1, partB, e2 ,op=lower_to)                         ：将同一个集合中的两部分合并, 两部分都有序
    * set_union(b1, e1, b2, e2, destB, op2=lower_to)                    ：并集
    * set_intersection(b1, e1, b2, e2, destB, op2=lower_to)             ：交集
    * set_difference(b1, e1, b2, e2, destB, op2=lower_to)               ：前一个集合去交集
    * set_symmetric_difference(b1, e1, b2, e2, destB, op2=lower_to)     ：并集去交集
<!-- entry end -->

<!-- entry begin: 数值算法 -->
* 数值算法：`<numeric>`
    * iota(b, e, v)                                                     ：依序赋值 V, V+1, V+2, ...
    * accumulate(b, e, initV, op2=plus)                                 ：求和
    * reduce(b, e, initV=0, op2=plus)                                   ：允许使用执行策略的求和
    * inner_product(b1, e1, b2, e2, initV, op2=plus, op2 = multiply)    ：内积
    * partial_sum(b, e, destB, op2=plus)                                ：a1, a1+a2, a1+a2+a3,
    * adjacent_difference(b, e, destB, op2=reduce)                      ：a1, a2-a1, a3-a2,
<!-- entry end -->

## 其它容器

<!-- entry begin: bitset -->
* bitset：`<bitset>`
    * 方便访问指定位
    * 构造：() (ulong) (string) (cstring)
    * 操作：
        * .any()
        * .all()
        * .none()
        * .count()
        * .size()
        * .set()
        * .set(pos, v=true)
        * .reset()
        * .reset(pos)
        * .flip()
        * .flip(pos)
        * .operator[](idx).flip()
    * 转换：
        * .to_ulong()
        * .to_ullong()
        * .to_string(zero, one)
<!-- entry end -->

<!-- entry begin: string -->
* string：`<string>`
    > 范围：(i, l)、(b, e)  
    > 目标：(s)、(s, i)、(s, i, l)、(c)、(c, l)、(char)、(n, char)
    * 修改
        * string() .assign() .append()      ：目标
        * operator= operator+ operator+=    ：(s)、(c)、(char)
        * .insert()                         ：pos + 目标（除了(char)）
        * .replace()                        ：范围+ 目标
    * 搜索
        > 参数：(s) (s,i) (c) (c,i) (c,i,l) (char) (char,i)
        * .find()
        * .rfind()
        * .find_first_of()
        * .find_first_not_of()
        * .find_last_of()
        * .find_last_not_of()
    * 比较
        * .compare()                        ：范围+目标（除了(char)、(n, char)）
        * .operator<=>()                    ：(s)、(c)
    * 转换
        * stoi() stol() stoul() stof() stod()：(str, idx=nullptr, base=10)
        * to_string(val)
    * 其它
        * .substr()                         ：范围
        * .copy(c, length, idx)             ：不包含`\0`
        * getline(istrm, string)
<!-- entry end -->

<!-- entry begin: string_view -->
* string_view：`<string_view>`
    > 原理：只是string或C-string的引用, 没有数据的拥有权, 只含有元数据  
    > 目的：高效的提供string接口的拷贝操作, 尤其.substr(), 当需要const string时改用string_view  
    > 注意：所有拷贝共享一个底层数据, 所以.substr().data()会导致错误（因为'\0'只在最后才有）
    * 构造：(string) (string_view) (cstring) (cstring, len)
    * 额外提供：.remove_prefix和.remove_suffix缩减视图范围
<!-- entry end -->

<!-- entry begin: cpp regex -->
* 正则表达式：`<regex>`
* 组件：
    * regex
    * sregex_iterator
    * sregex_token_iterator
    * smatch
    * ssub_match
    * regex_search()
    * regex_match()
    * regex_replace()
    * regex_contants： 标志用于控制regex、match、replace行为

* regex
    > flag 主要就有 regex_constants::icase
    * (string, flag)
    * (char*, flag)
    * (char*, len, flag)
    * (b, e, flag)
* sregex_iterator
    > 自增自减移动模式匹配到的子串  
    > 解引用得到smatch
    * 构造：(b, e, regex)，默认初始化为end
* sregex_token_iterator
    > 保留不匹配间的子字符串
    * (b, e, r, -1)

* smatch
    > 存放ssub_match的容器  
    > 0索引存放整个模式匹配到的子串
    * .begin() .cbegin() .end() .cend()
    * .size()
    * .empty()
    * .operator[](idx)
    * .prefix()
    * .suffix()
    * .length(n)
    * .position(n)  ：返回difference_type数字
    * .str(n)
    * .format(dest, fmt, flag)
    * .format(fmt, flag)
* ssub_match： 指向表达式匹配到的子表达式
    * .operator basic_string<charT>()

* regex_replace((str|b, e), regex, repl, flag)
    * 替换语法：
    ```
    $0
    $1, $2, $3, ...
    $&：全部
    $'：后缀
    $`：前缀
    $$：转义$
    ```
<!-- entry end -->

## 流与格式化
<!-- entry begin: iostream 状态 异常 -->
* iostream：`<iostream>`

* 状态与异常
    * .good()
    * .eof()
    * .faile()
    * .bad()
    * .rdstate()
    * .clear()
    * .clear(state)
    * .setstate(state)
    * .excptions(flags) ：设定触发异常的flag
    * .exceptions()     ：返回触发异常的flag, 无则返回ios::goodbit
<!-- entry end -->

<!-- entry begin: 底层IO -->
* 底层I/O：
    * .get()
    * .get(char&)
    * .get(char*, count, delim='\n')     ：读取 count - 1 个字符, 并自动添加'\0'在末尾
    * .getline(char*, count, delim='\n') ：其他同上, 但读取包括delim
    * .read(char*, count)                ：count代表指定读取的字符
    * .readsome(char*, count)            ：返回读取字符数, 只从缓冲区中读取, 而不陷入系统调用
    * .gcount()                          ：返回上次读取字符数
    * .ignore(count=1)
    * .ignore(count, delim)
    * .peek()                            ：返回下个字符, 但不移动iterator
    * .unget()                           ：把上次读取的字符放回（回移iterator）
    * .putback(char)                     ：放回指定字符
    * .put(char)
    * .write(char*, count)
    * .flush()
<!-- entry end -->

<!-- entry begin: 随机访问 -->
* 随机访问：
    * .tellg()
    * .tellp()
    * .seekg(pos) .seekp(pos)
    * .seekg(offset, rpos) .seekp(offset, rpos)：rpos可以是ios::beg ios::end ios::cur
<!-- entry end -->

<!-- entry begin: IO运算符 -->
* 预定义I/O运算符：
```
    * 整型：  
        [0-7]*  
        [0-9]*
        (0x|0X)?[0-9a-fA-F]*
    * 浮点型：  
        ([0-9]+\.?[0-9]*|\.[0-9]+)(e[+-]?[0-9]+)
    * 其他：bool, char, char*, void*, string, streambuf*, bitset, complex
```
<!-- entry end -->

<!-- entry begin: 关联 stream -->
* 关联stream：
    * 以.tie()和.tie(ostream&)关联, 在I/O该stream时冲刷关联的ostream
    * 以.rdbuf()和.rdbuf(streambuf*)关联, 对同一缓冲区建立多个stream对象
    * 以.copyfmt()传递所有格式信息
<!-- entry end -->

<!-- entry begin: iostream性能 -->
* 关于性能
    * ios::sync_with_stdio(false)：关闭C-stream同步与多线程同步机制
    * cin.tie(nullptr)：关闭cin与cout的关联
<!-- entry end -->

<!-- entry begin: iostream 国际化 -->
* 国际化
    * .imbue(locale)
    * .getloc()
    * .widen(char)
    * .narrow(c, default)
<!-- entry end -->

<!-- entry begin: iomanip -->
* 操作符：`<iomanip>`
    > 后面加`!`代表默认
    > IStream
    * ws                            ：立刻丢弃前导空白
    * noskipws | skipws!            ：是否需要输入时忽略前导空白
    > OStream
    * endl                          ：输出`\n`并刷新缓冲区
    * flush                         ：刷新缓冲区
    * ends                          ：输出`\0`
    * nounitbuf | unitbuf           ：是否每次都刷新缓冲区
    * setfill(char)                 ：用char填充setw()制造的空白，默认空格
    * left                          ：使用setw()后输出左对齐
    * right!                        ：使用setw()后输出右对齐
    * internal                      ：正负号靠左，数值靠右（无`no`版本）
    * noboolalpha! | boolalpha      ：是否字符化输出boolean，如`true`和`false`
    * noshowpos! | showpos          ：是否正数输出正号
    * nouppercase! | uppercase      ：是否对数值输出中的字母强制大写或强制小写
    * noshowpoint! | showpoint      ：是否小数部分为零的浮点数也打印小数部分
    * noshowbase! | showbase        ：是否对二/八/十六/进制的数字输出进制前缀
    * setprecision(v)               ：设置输出浮点数的精度
        > 使用以下两个操作符后, 精度的语义由“所有数字位数”变为“小数位数”
        * fixed                     ：强制用定点表示法输出浮点数
        * scientific                ：强制用科学计数法输出浮点数
    > IOStream
    * oct | dec! | hex              ：设置输入或输出数值时的进制
    * setw(n)                       ：设定下次输出的栏宽，或输入的字符限制最多n-1个
<!-- entry end -->

<!-- entry begin: quote -->
* quoted：`<iomanip>`
    > 将字符串引用转义  
    > 输出(`<<`)时quoted()的参数作为引用转义的输入对象  
    > 输入(`>>`)时quoted()的参数作为引用转义的输出对象
    * 签名：
        `quoted(char* s, delim='"', escape='\\')`  
        `quoted(string& s, delim='"', escape='\\')`  
```cpp
string in{"hello \"world\""}, out;
stringstream ss;
ss << quoted(in);   // 输出时进行引用。ss.str() == "\"hello \\\"world\\\""，即输出"hello \"world\""
ss >> quoted(out);  // 输入是取消引用。将ss中被引用包围后的字符还原，即out输出为hello "world"
```
<!-- entry end -->

<!-- entry begin: fstream -->
* fstream：`<fstream>`
    * 构造：(filename, flag=)
    * 成员：
        * .open(filename, flag=)
        * .is_open()
        * .close()
    * flags：`ios_base::`
        > 若未指出**文件必须存在**，则表示**不存在则自动创建**
        * in            ：只读，文件必须存在
        * out           ：只写，覆盖
        * app           ：只写，追加
        * in|out        ：读写，覆盖，文件必须存在，(fstream默认)
        * in|out|trunc  ：读写，覆盖
        * in|app        ：读写，追加
        * binary        ：不要将`\r\n`替换为`\n`
<!-- entry end -->

<!-- entry begin: sstream stringstream -->
* stringstream：`<sstream>`
    * 构造：(string)
    * 成员：
        * .str()
        * .str(string)
<!-- entry end -->

<!-- entry begin: streambuf -->
* streambuf：`<streambuf>`
    * 销毁问题：basic_i/ostream析构时不会销毁, 其他stream析构时只是不销毁.rdbuf()得到的
    * 高效非格式化I/O：
        * streambuf_iterator    ：不通过stream对象直接I/O缓冲区
        * `streambuf*`          ：利用stream.rdbuf()获取后直接调用I/O运算符与另一个流缓冲区对接, 注意输入时需要std::noskipws
<!-- entry end -->

<!-- entry begin: locale -->
* locale：`<locale>`
    * locale：封装了多个facet用于多方面信息本地化
    * facet：数值、货币、时间、编码
    * locale的构造：
        * 默认："C"
        * 智能：""
        * 自定义：`zh_CN.UTF-8[@modifier]`
    * 提供
        * .name()
        * ::global(locale)
<!-- entry end -->

<!-- entry begin: iofwd stream库 -->
* Stream库总览：`<iofwd>`
* 组件：
    * streambuf(系统I/O并缓存数据, 提供位置信息)
    * locale(包含facet将I/O进行进行本地格式化)
    * stream(封装上述两者, 提供状态、格式化信息)
    * centry(帮助stream每次I/O预处理与后处理)
    * 操作符(提供调整stream的便捷方法)
    * std::ios(定义了一些标志位)
<!-- entry end -->

<!-- entry begin: 字符转换 字符处理 -->
* 字符转换与处理：
    > `<codecvt>`   ：字符编码转换器
    > `<locale>`    ：转换宽字符需要此头文件
    例：
    * 转换stream_buffer
    ```cpp
    wbuffer_convert<codecvt_utf8<wchar_t> > utf8_to_wchar_t(cin.rdbuf())
    wistream get_wstring_from_multibytes_stream(&utf8_to_wchar_t)
    get_wstring_from_multibytes_stream >> wstring;

    wbuffer_convert<codecvt_utf8<wchar_t> > wchar_t_to_utf8(cout.rdbuf())
    wostream put_wstring_to_multibytes_stream(&wchar_t_to_utf8)
    put_wstring_to_multibytes_stream << wstring; // 注意结束时必须冲刷该缓冲区，不然会有字符留在里面未输出
    // 注意cout与put_wstring_to_multibytes_stream的缓冲区并不一样，
    ```
    * 转换string
    ```cpp
    wstring_convert<codecvt_utf8<wchar_t>> convertor
    convertor.to_bytes(wstring&)
    convertor.from_bytes(string)
    /* 参数：
     * (char byte);
     * (const char* ptr);
     * (const byte_string& str);
     * (const char* first, const char* last);
     */

    ```
<!-- entry end -->

<!-- entry begin: format 格式化 -->
* 格式化：`<format>`
    > `"{arg_id:填充与对齐 符号 # 0 宽度 精度 L 类型}"`
    * 填充与对齐
        > 对齐符号前可选添加填充符
        * `<`left，非整数与非浮点数默认左对齐
        * `>`right，整数与浮点数默认右对齐
        * `^`center，居中
    * 符号
        * `+`showpos
        * `<space>`非负数前导空格
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
    > 本地化
        * 对整数：插入合适数位分隔符
        * 对浮点数：插入合适数位分隔符与底分隔符
        * 对bool：boolalpha
    * 类型
    > 类型再编译期便已知，故不其它需要则无需指出
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

<!-- entry begin: random 随机数 -->
* 随机数生成器：`<random>`
* 引擎：
    * default_random_engine
    * .seed()
    * .seed(result_type)
* 分布：
    > 使用：先构造分布对象，再用引擎作参数调用其.operator()(re)
    * uniform_int_distribution di(min=0, max=INTMAX)    ：min-max的均匀整数分布
    * uniform_real_distribution dr(min=0, max=1.0)      ：min-max的均匀实数分布
    * bernoulli_distribution db(p=0.5)                  ：0-1分布，返回bool
    * binomial_distribution dbi(n=1, p=0.5)             ：二项分布
    * normal_distribution dn(u=0, o=1)                  ：正态分布
<!-- entry end -->

## 并发
<!-- entry begin: future async -->
* 任务发射：`<future>`
    * `async(func, args...)`与`async(launch::async | launch::deferred, func, args...)`
        > 发射策略：
        * launch::async         ：异步调用
        * launch::deferred      ：延迟发射
    * `future<>`与`shared_future<>`
        > 对于由以异步执行策略运行的`async()`所创造的`future`对象，若它是最后一个指涉其共享状态的`future`对象，则当它析构时会阻塞当前线程直到它所代表的线程执行完毕。  
        > 这也造成了：如果忽略`async()`函数的返回值（返回的future立即析构），因为阻塞的原因则相当于同步调用而非异步并行
        * .valid()                  ：是否有效，即是否已调用过`.get()`或`.wait()`
        * .share()                  ：返回`shared_future`
        * .get()                    ：若线程被延迟则同步启动，并获取返回值或异常，同时取消其有效状态
        * .wait()                   ：若线程被延迟则同步启动，不获取返回值或异常，但不取消其有效状态
        * .wait_for(duration)与.wait_until(time_point)
            > 不会启动延迟的线程  
            > 返回值包括：
            * future_status::deferred   ：线程被延迟启动，且此前未调用`.get()`与`.wait()`让其强制启动
            * future_status::timeout    ：线程已启动，但是已超时，即还未执行完成
            * future_status::ready      ：线程已启动，且执行完成
<!-- entry end -->

<!-- entry begin: this_thread -->
* this_thread：`<thread>`
    * ::get_id()
    * ::sleep_for(dur)
    * ::sleep_until(tp)
    * ::yield()
<!-- entry end -->

> * 并发问题
>     * 访问共享数据： 数据竞争、数据销毁
>     * 编译器优化： 访存优化、顺序优化

<!-- entry begin: mutex 互斥锁 -->
* 互斥锁：`<mutex>`
    * mutex及其变种：
        * 用法：在全局中声明

        | 操作                | mutex | recursive_mutex | shared_mutex | timed_mutex | recursive_timed_mutex | shared_timed_mutex |
        |---------------------+-------+-----------------+--------------+-------------+-----------------------+--------------------|
        | .lock()             |                                   捕获mutex，失败则阻塞                                           |
        | .try_lock()         |                               捕获mutex，失败则返回false                                          |
        | .unlock()           |                                   解除锁定的mutex                                                 |
        | .try_lock_for(dur)  |   -   |        -        |      -       |                限制时间内尝试捕获                        |
        | .try_lock_until(tp) |   -   |        -        |      -       |                限制期限前尝试捕获                        |
        | 多个lock            |   -   |        🉑️️       |      -       |      -      |          🉑️️           |         -          |
    * 辅助函数：
        * lock(mutex...)      ：避免死锁
        * try_lock(mutex...)  ：保证加锁次序
        * once_flag类型
        * call_once(once_flag, func, args...)
    * mutex托管：
        > 用法：mutex做模板类型, mutex对象做构造参数(可能还有其他参数), 在块作用域中初始化
        * lock_guard<>额外参数：adopt_lock(已锁)
        * unique_lock<>额外参数：
            * adopt_lock(已锁)
            * defer_lock(不锁)
            * try_lock(试锁)
            * duration
            * time_point
        * shared_lock<>
    * 提供原子性操作原理：
        * 读取mutex - 判断mutex - 上锁或阻塞
        * 解锁-唤醒
<!-- entry end -->

<!-- entry begin: cv condition_variable 条件量 -->
* 条件量：`<condition_variable>`
    > 依赖于unique_lock`<>`提供保护区, 在全局中声明，  
    > 注意由mutex锁住的线程由mutex唤醒，被condition_variable锁住的线程由condition_variable唤醒，
    > 前者可由lock_guard自动触发唤醒机制，而condition_variable需要手动
    * 成员：
        * .wait(u_l, pred)
        * .wait_for(u_l, dur, func)
        * .wait_until(u_l, tp, func)
            > 返回等待状态：
            > * cv_status::time_out
            > * cv_status::no_timeout
        > 解锁后再notify
        * .notify_one()
        * .notify_all()
        * notify_all_at_thread_exit(cv, ul)
    * 提供原子性操作：
        * 解锁-阻塞
        * 唤醒
<!-- entry end -->

## 文件系统
<!-- entry begin: cpp filesystem fs path -->
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
        * operator<<(strm, path)    ：`/tmp/fs.cpp`或`C:\tmp\fs.cpp`
        * operator>>(strm, path)    ：`/tmp/fs.cpp`或`C:\tmp\fs.cpp`
        * operator/()与operator/=()
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

<!-- entry begin: cpp filesystem fs dir directory_entry directory_iterator -->
* directory_entry `<filesystem>`
    > 目录项可能是目录下的任何类型的文件
    * 读取
        * .path()                       ：返回path&，也可隐式转换为path
        * .status()与.symlink_status()  ：返回file_status
        * .hard_link_count()
        * .last_write_time()
        * .file_size()
    * 判断
        * .exists()
        * .is_regular_file()
        * .is_directory()
        * .is_symlink()
        * .is_socket()
        * .is_fifo()
        * .is_blovk_file()
        * .is_character_file()
* directory_iterator `<filesystem>`
    * range-based-for：
        ```cpp
        for ( auto& entry : directory_iterator{"/tmp"} ) {
            /* ... */
        }
        ```
<!-- entry end -->

<!-- entry begin: cpp filesystem fs file_status file_type file_perm -->
* file_status
    * .type()
    * .permissions()
* file_type：领域枚举
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
* file_perm：领域枚举
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

# BOOST
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

    std::fstream file{"test.bin", std::ios_base::in | std::ios_base::out | std::ios_base::trunc | std::ios_base::binary}; // Note

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

# Mysql++
<!-- entry begin: mysqlpp mysql++ 异常 exception -->
```cpp
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
* mysqlpp::Query
    * 读取SQL语句
        * Query("SQL Statement")
        * operator<<(Query, string)
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
    * .operator[]()                         ：返回mysqlpp::Row
    * .num_rows()                           ：返回总行数

* mysqlpp::UseQueryReslt
    > 按需加载
    * .fetch_field()                        ：返回mysqlpp::Field
    * .fetch_row()                          ：返回mysqlpp::Row
<!-- entry end -->

<!-- entry begin: mysqlpp mysql++ Null type -->
MYSQL++中定义有类型映射到SQL类型，如：  
`mysqlpp::sql_tinyint_unsigned_null`表示SQL类型`TINYINT UNSIGNED`  
`mysqlpp::sql_tinyint_unsigned`表示SQL类型`TINYINT UNSIGNED NOT NULL`  
非NOT NULL的SQL类型可以接受`mysqlpp::null`的赋值，表示特殊值`TINYINT NULL`  
NULL类型的基础便是该类
* mysqlpp::Null<Type, mysqlpp::NullIsZero或mysqlpp::NullIsNull>

String 可以将 SQL 类型字符串转换为 C++ 数据类型  
STA 可以将 C++数据类型转换为 SQL 类型字符串  
两种字符串都使用了Copy-on-Write机制

<!-- entry end -->

