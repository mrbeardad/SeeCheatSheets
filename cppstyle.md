# 目录
<!-- vim-markdown-toc GFM -->

- [命名](#命名)
- [注释](#注释)
- [顺序](#顺序)
- [空白符](#空白符)

<!-- vim-markdown-toc -->

风格部分参考自[HosseinYousefi/CompetitiveCPPManifesto](https://github.com/HosseinYousefi/CompetitiveCPPManifesto)

# 命名
| 内容                         | 规范            | 举例                           |
|------------------------------|-----------------|--------------------------------|
| 命名空间                     | 全部小写+下划线 | `example_name`                 |
| 全局函数                     | 全部小写+下划线 | `::example_name`               |
| 全局变量                     | 大驼峰拼写法    | `::ExampleName`                |
| 宏、常量、模板非类型参数     | 全部大写+下划线 | `EXAMPLE_NAME`                 |
| 类名、类型别名、模板类型参数 | 大驼峰拼写法    | `ExampleName`                  |
| 静态变量                     | 大驼峰拼写法    | `ExampleName`                  |
| 动态变量                     | 小驼峰拼写法    | `exampleName`                  |
| 类的数据成员                 | 带`_`后缀       | `exampleName_`与`ExampleName_` |
| 类的成员函数                 | 带`_`后缀       | `exampleName`与`ExampleName`   |

可用于命名标识符的一些通用前后缀：
* 位置：`prev`，`next`，`lhs`，`rhs`，`head`，`tail`，`mid`，`begin`，`end`，`last`，`this`
* 时间：`new`，`old`，`orig`，`before`，`after`，`cur`
* 循环：`pos`，`idx`
* 计数：`size`，`len`，`width`，`height`，`depth`，`num`，`nr`，`cnter`
* 序数：`1st`，`2nd`，`3rd`，`last`
* bool：`is`，`not`，`and`，`or`，`any`，`all`，`none`，`has`
* 介词：`in`，`on`，`at`，`of`，`2`，`4`
* 类型：`int`，`char`，`str`，`strm`，`p`
* 用途：`ret`，`ans`，`val`，`need`，`tmp`，`deal`，`src`，`dst`


# 注释
* 块注释`/* comment */`：
    * 源文件标题
    * 注释大片代码
* 单行注释`// comment `：
    * 尾后注释：解释该行代码
    * 行前注释：解释接下来一段代码


# 顺序
* 命名空间：
    1. 类型别名与类的声明/定义
    2. 变量声明/定义
    3. 函数声明/定义
```cpp
class Demo {
    /* 成员类型 */
    /* 成员对象 */
    /* 静态成员对象 */

public:
    /* 普通构造函数 */
    /* copy & move */
    /* 析构函数 */
    /* 成员函数 */
    /* 静态成员函数 */
    /* operator */

private:
    /* public段有需要private的移到此处 */
};
```


# 空白符
* 预处理分支指令层级缩进时如下
```c
#ifndef TEST
#define TEST
#   ifndef NDEBUG
/* ... */
#   endif // NDEBUG
#endif  // TEST
```
* 命名空间内成员无需行首缩进，尾部加上注释`// namespace std`

* 尖括号`<>`只与右边间隔空格，如`template<typename T>`
* 嵌套模板参数的右括号`>`间隔空格，如`vector<vector<int> >`
* 模板参数包`...`只与右边隔空格，如`<typename... T>`与`(T&&... args)`

* 函数签名过长或有关键字template、static、inline、constexpr应该分行写，尽量让返回类型与参数列表同行
* 函数默认实参的等号两边都不隔空格`void f(int i=1)`

* 控制流语句关键字与括号隔空格，控制流语句的括号需内部空格，如`if ( get_bool() )`
* else(catch)跟在if(try)语句块末端花括号后面
* 标签与访问控制后面的冒号只与右边隔空格，其他冒号两边都隔空格
* lambda函数体语句与控制流语句内容超过1句时应该分行

* 指针与引用声明中，`*`与`&`应该与类型放在一起，且应该单独声明，如`int& ref{val}; double* ptr{};`

* 一元运算符只隔一边空格，二元两边空格，三元全隔，`*x`、`x + y`、`x < y ? x : y`
* 逗号与分号只与右边隔空格，`func(a, b, c)` `for ( init; condition; iterate )` `return 0;`

* 若本该在一行的内容过长，要有节奏的断行。
* 断行时二元运算符（除了逗号）断到次行开端。

* 函数体、类作用域的头尾不留空行，命名空间首尾留一空行
* 各变量、函数、类的声明/定义之间隔两行
